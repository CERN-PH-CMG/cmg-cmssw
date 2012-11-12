
/*************************************************
 * Functions used to weight the input root files
 ************************************************
*/
#define SignalScale 1

int findFirstBin(TH1F* h,double frac=0.1){
  double max=h->GetMaximum();
  int bin=0;
  for(Int_t b=1;b<=h->GetNbinsX();b++){
    //cout<<b<<" "<<h->GetBinContent(b)<<endl;
    if(h->GetBinContent(b)>frac*max){ bin=b; break;}
  }
  //cout<<"First: "<<max<<" "<<bin<<endl;
  return bin;
}

int findLastBin(TH1F* h,double frac=0.1){
  double max=h->GetMaximum();
  int bin=0;
  for(Int_t b=h->GetNbinsX();b>=1;b--){
    //cout<<b<<" "<<h->GetBinContent(b)<<endl;
    if(h->GetBinContent(b)>frac*max){ bin=b; break;}
  }
  //cout<<"Last "<<max<<" "<<bin<<endl;
  return bin;
}

void findPlusMinus1Sigma(TH1F*hsig){
  

}

double getSoB(TH1F*hsig,TH1F*hbkg){
  if(!hsig||!hbkg){
    cout<<"getSoB : null histos"<<endl;
  }

  //remove the bkg from the signal hist and scale to 1x SM higgs
  TH1F*HS=(TH1F*)hsig->Clone("HS");
  TH1F*HB=(TH1F*)hbkg->Clone("HB");
  HS->Add(HB,-1);
  HS->Scale(SignalScale);

//   //need to scale bin contents by bin width to properly compute the significance
//   for(Int_t b=1;b<=HS->GetNbinsX();b++){
//     HS->SetBinContent(b,HS->GetBinContent(b)*HS->GetBinWidth(b));
//     HS->SetBinError(b,HS->GetBinError(b)*HS->GetBinWidth(b));
//     HB->SetBinContent(b,HB->GetBinContent(b)*HB->GetBinWidth(b));
//     HB->SetBinError(b,HB->GetBinError(b)*HB->GetBinWidth(b));
//   }

//   double sig=HS->Integral(findFirstBin(HS),findLastBin(HS));
//   double bkg=HB->Integral(findFirstBin(HS),findLastBin(HS));

//   double sig=HS->Integral();
//   double bkg=HB->Integral();


  //put the histos into a TF1 for easy access to integrals
  TString SigString="";
  TString BkgString="";
  float xmin=HS->GetXaxis()->GetXmin();
  float xmax=HS->GetXaxis()->GetXmax();
  for(Int_t b=1;b<=HS->GetNbinsX();b++){
    char str[100];
    sprintf(str,"%.6f*(%.2f<x&&x<%.2f)",HS->GetBinContent(b),HS->GetBinLowEdge(b),HS->GetBinLowEdge(b)+HS->GetBinWidth(b));
    SigString+=str;
    if(b<HS->GetNbinsX())SigString+="+";

    char strBkg[100];
    sprintf(strBkg,"%.6f*(%.2f<x&&x<%.2f)",HB->GetBinContent(b),HB->GetBinLowEdge(b),HB->GetBinLowEdge(b)+HB->GetBinWidth(b));
    BkgString+=strBkg;
    if(b<HB->GetNbinsX())BkgString+="+";

  }

  //cout<<SigString<<endl;
  //cout<<BkgString<<endl;
  TF1 FSig("FSig",SigString,xmin,xmax);
  TF1 FBkg("FBkg",BkgString,xmin,xmax);
  
  //find the -/+ 1 sigma x values
  int npoints=10000;
  float total=FSig.Integral(xmin,xmax);
  float dx=(xmax-xmin)/npoints;
  float xlow=xmin;
  float lowint=0.;
  float xhigh=xmax;
  float highint=0.;
  for(Int_t i=1;i<=npoints;i++){
    float xl=xmin+i*dx;
    lowint+=FSig.Eval(xl)*dx;
    if(lowint<total*0.158)xlow=xl;

    float xh=xmax-(i-1)*dx;
    highint+=FSig.Eval(xh)*dx;
    if(highint<total*0.158)xhigh=xh;
  }
  //cout<<xlow<<" "<<xhigh<<endl;

  double sig=FSig.Integral(xlow,xhigh);
  double bkg=FBkg.Integral(xlow,xhigh);

  delete HS;
  delete HB;

  //return sig/bkg;
  return sig/sqrt(sig+bkg);
}

void weightedCombine(TString *filename, int n, int rebin, TString outName, int weight=1){

  //weight=0;

  TFile F1((filename[0]+".root").Data(),"READ");
  gROOT->cd();

  //first compute the sum of the weights
  double weightsum=0.;
  for(Int_t f=0;f<n;f++){
    TFile File((filename[f]+".root").Data(),"READ");
    gROOT->cd();

    TH1F* ggH=(TH1F*)File.Get("ggH");
    TH1F* Ztt=(TH1F*)File.Get("Ztt");
    if(!ggH || !Ztt){ cout<<" ggH or Ztt not found in "<<filename[f].Data()<<endl;}

    weightsum+=getSoB(ggH,Ztt);
  }
  //weightsum=1.0;

  TH1F* Ztt1=(TH1F*)F1.Get("Ztt");
  TH1F* ggH1=(TH1F*)F1.Get("ggH");

  //Compute the S/B weight
  double sob1=getSoB(ggH1,Ztt1)/weightsum;


  ///Signal
  TH1F* signal=(TH1F*)ggH1->Clone("signal");
  signal->Add(Ztt1,-1);
  signal->Scale(SignalScale);//SM Higgs is x5 in input histos
  if(weight==1) signal->Scale(sob1);
  if(rebin>0) signal->Rebin(2);

  //Background
  TH1F* Ztt=(TH1F*)Ztt1->Clone("Ztt");
  if(weight==1)Ztt->Scale(sob1);
  if(rebin>0) Ztt->Rebin(2);

  //Data
  TH1F* data_obs=((TH1F*)F1.Get("data_obs")->Clone("data_obs"));
  if(weight==1)data_obs->Scale(sob1);
  if(rebin>0) data_obs->Rebin(2);

  //Other backgrounds for plot
  TH1F* TT=(TH1F*)(F1.Get("ttbar")->Clone("ttbar"));
  if(weight==1)TT->Scale(sob1);
  if(rebin>0) TT->Rebin(2);

  TH1F* EWK=(TH1F*)(F1.Get("EWK")->Clone("EWK"));
  if(weight==1)EWK->Scale(sob1);
  if(rebin>0) EWK->Rebin(2);

  TH1F* Fakes=(TH1F*)(F1.Get("Fakes")->Clone("Fakes"));
  if(weight==1)Fakes->Scale(sob1);
  if(rebin>0) Fakes->Rebin(2);
  

  printf("%.4f %s\n",sob1,filename[0].Data());
  for(Int_t f=1;f<n;f++){
    TFile F2((filename[f]+".root").Data(),"READ");
    gROOT->cd();

    TH1F* ggH2=(TH1F*)F2.Get("ggH");
    TH1F* Ztt2=(TH1F*)F2.Get("Ztt");

    double sob2=getSoB(ggH2,Ztt2)/weightsum;
    printf("%.4f %s\n",sob2,filename[f].Data());

    TH1F* signal2=(TH1F*)ggH2->Clone("signal2");
    signal2->Add(Ztt2,-1);
    signal2->Scale(SignalScale);

    TH1F* data_obs2=(TH1F*)F2.Get("data_obs");
    TH1F* TT2=(TH1F*)F2.Get("ttbar");    
    TH1F* EWK2=(TH1F*)F2.Get("EWK");    
    TH1F* Fakes2=(TH1F*)F2.Get("Fakes");


    //rescale by S/B before adding
    signal2->Scale(weight==1 ? sob2 : 1);
    data_obs2->Scale(weight==1 ? sob2 : 1);
    Ztt2->Scale(weight==1 ? sob2 : 1);
    TT2->Scale(weight==1 ? sob2 : 1);
    EWK2->Scale(weight==1 ? sob2 : 1);
    Fakes2->Scale(weight==1 ? sob2 : 1);

    ///Rebin
    if(rebin>0 && f<rebin) signal2->Rebin(2);
    if(rebin>0 && f<rebin) Ztt2->Rebin(2);
    if(rebin>0 && f<rebin) data_obs2->Rebin(2);
    if(rebin>0 && f<rebin) TT2->Rebin(2);
    if(rebin>0 && f<rebin) EWK2->Rebin(2);
    if(rebin>0 && f<rebin) Fakes2->Rebin(2);


    //Combine
    signal->Add(signal2);
    data_obs->Add(data_obs2);
    Ztt->Add(Ztt2);
    TT->Add(TT2);
    EWK->Add(EWK2);
    Fakes->Add(Fakes2);

    delete signal2;
  }


  TString outname=outName;
  if(weight==1)outname+="_Weighted.root";
  if(weight==0)outname+="_UnWeighted.root";

  TFile outFile(outname.Data(),"recreate");
  signal->Write();
  data_obs->Write();
  Ztt->Write();
  TT->Write();
  EWK->Write();
  Fakes->Write();
  outFile.ls();
  outFile.Close();

}



/************************************************
 * Functions used to produce the weighted plot 
 ************************************************
*/
//#include "/afs/cern.ch/user/b/benitezj/scratch1/V5_8_0/CMGTools/CMSSW_5_3_3_patch3/src/HiggsAnalysis/HiggsToTauTau/interface/HttStyles.h"

void CMSPrelim(const char* dataset, const char* channel, double lowX, double lowY)
{
  TPaveText* cmsprel  = new TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC");
  cmsprel->SetBorderSize(   0 );
  cmsprel->SetFillStyle(    0 );
  cmsprel->SetTextAlign(   12 );
  cmsprel->SetTextSize ( 0.04 );
  cmsprel->SetTextColor(    1 );
  cmsprel->SetTextFont (   62 );
  cmsprel->AddText("CMS Prelim, ");
  cmsprel->Draw();

  TPaveText* lumi     = new TPaveText(lowX+0.24, lowY+0.061, lowX+0.61, lowY+0.161, "NDC");
  lumi->SetBorderSize(   0 );
  lumi->SetFillStyle(    0 );
  lumi->SetTextAlign(   12 );
  lumi->SetTextSize ( 0.04 );
  lumi->SetTextColor(    1 );
  lumi->SetTextFont (   62 );
  lumi->AddText(dataset);
  lumi->Draw();

  TPaveText* chan     = new TPaveText(lowX+0.05, lowY-0.002, lowX+0.45, lowY+0.028, "NDC");
  chan->SetBorderSize(   0 );
  chan->SetFillStyle(    0 );
  chan->SetTextAlign(   12 );
  chan->SetTextSize ( 0.04 );
  chan->SetTextColor(    1 );
  chan->SetTextFont (   62 );
  chan->AddText(channel);
  chan->Draw();
}



TH1F* diffPlot(TH1F*h1,TH1F*h2,int opt){
  TH1F* h=h1->Clone("hdiff");
  h->Reset();
  h->Clear();
  for(Int_t b=1;b<=h1->GetNbinsX();b++){
    h->SetBinContent(b,h1->GetBinContent(b)-h2->GetBinContent(b));
    h->SetBinError(b,0.);
    if(opt==1)h->SetBinError(b,h1->GetBinError(b));
    if(opt==2)h->SetBinError(b,sqrt(pow(h1->GetBinError(b)*h1->GetBinError(b),2)+pow(h2->GetBinError(b)*h2->GetBinError(b),2)));
  }
  h->SetBinContent(0,0);
  h->SetBinError(0,0.);
  h->SetBinContent(h1->GetNbinsX()+1,0);
  h->SetBinError(h1->GetNbinsX()+1,0.);

  return h;
}

TH1F* errorBand(TH1F*h1){
  TH1F* h=h1->Clone("herrorBand");
  h->Reset();
  h->Clear();
  for(Int_t b=1;b<=h1->GetNbinsX();b++){
    h->SetBinContent(b,0.);
    h->SetBinError(b,h1->GetBinError(b));
  }
  h->SetBinContent(0,0);
  h->SetBinError(0,0.);
  h->SetBinContent(h1->GetNbinsX()+1,0);
  h->SetBinError(h1->GetNbinsX()+1,0.);


  return h;
}

float findMaxY(TH1F* h,int opt){
  float max=0.;

  for(int b=1;b<=h->GetNbinsX();b++){
    if(h->GetBinContent(b)>0.){
      if(opt==0)      if(fabs(h->GetBinContent(b))+fabs(h->GetBinError(b)) > max) max = fabs(h->GetBinContent(b))+fabs(h->GetBinError(b));
      if(opt==1)      if(fabs(h->GetBinContent(b)) > max)                         max = fabs(h->GetBinContent(b));
    }
  }

  //cout<<max<<endl;
  return max;
}

float findMinY(TH1F* h,int opt){
  float max=0.;

  for(int b=1;b<=h->GetNbinsX();b++){
    if(h->GetBinContent(b)<0.){
      if(opt==0)      if(fabs(h->GetBinContent(b))+fabs(h->GetBinError(b)) > max) max = fabs(h->GetBinContent(b))+fabs(h->GetBinError(b));
      if(opt==1)      if(fabs(h->GetBinContent(b)) > max)                         max = fabs(h->GetBinContent(b));
    }
  }

  //cout<<max<<endl;
  return max;
}

void weightedPlot(TString filename="combineAll_Weighted"){

  TFile F((filename+".root").Data(),"READ");
  TH1F* data=(TH1F*)F.Get("data_obs");
  TH1F* bkg=(TH1F*)F.Get("Ztt");
  TH1F* sig=(TH1F*)F.Get("signal");
  TH1F* tt=(TH1F*)F.Get("ttbar");
  TH1F* ewk=(TH1F*)F.Get("EWK");
  TH1F* fakes=(TH1F*)F.Get("Fakes");
  if(!sig){cout<<"No input histograms in file: "<<filename.Data()<<endl; return;}

  TString outname=TString("Plot_")+filename;

  TCanvas C(outname);
  C.Print(outname+".ps[");

  
  sig->SetBinContent(0,0);//remove red line on top of y axis in plot
  sig->SetBinContent(sig->GetNbinsX()+1,0);
  sig->SetBinError(0,0);
  sig->SetBinError(sig->GetNbinsX()+1,0);
  sig->SetName("sig");
  sig->SetFillStyle(3353);//1001=solid , 3004,3005=diagonal
  sig->SetFillColor(2);
  sig->SetLineColor(2);
  sig->SetLineStyle(1);
  sig->SetLineWidth(0.);


  //Data and Background
  C.Clear();
  bkg->GetYaxis()->SetRangeUser(0.,1.2*findMaxY(bkg,0));
  bkg->GetXaxis()->SetTitle("#bf{m_{#tau#tau} [GeV]}");
  bkg->GetYaxis()->SetTitle("#bf{Weighted dN/dm_{#tau#tau} [1/GeV]}");//  dN/dm(#tau#tau)
  bkg->SetNdivisions(505);
  bkg->Draw("hist");

  tt->Draw("histsame");
  ewk->Draw("histsame");
  fakes->Draw("histsame");

  data->Draw("pesame");

  TLegend legend;//(0.59, 0.65, 0.99, 0.90);
  
  legend.SetFillStyle(0);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  legend.AddEntry(data,"Observed","LP");  
  legend.AddEntry(bkg,"Z#rightarrow#tau#tau","F");
  legend.AddEntry(tt,"t#bar{t}","F");
  legend.AddEntry(ewk,"electroweak","F");
  legend.AddEntry(fakes,"QCD","F");
  legend.SetTextSize(.04);
  legend.SetTextAlign(   12 );
  legend.SetX1NDC(0.60);
  legend.SetX2NDC(0.95);
  legend.SetY1NDC(0.55);
  legend.SetY2NDC(0.85);
  legend.Draw();

  const char* dataset = "#sqrt{s} = 7 + 8 TeV ,   L = 17.0 fb^{-1}";
  const char* channel = "#tau_{e}#tau_{#mu},#tau_{e}#tau_{h},#tau_{#mu}#tau_{h},#tau_{h}#tau_{h}";
  CMSPrelim(dataset,channel , 0.18, 0.835);

  C.RedrawAxis();
  C.Print(outname+".ps");

  //Data - Background and Signal
  C.Clear();
  TH1F* dataDiff=diffPlot(data,bkg,1);
  dataDiff->SetName("dataDiff");

  TH1F* errBand=errorBand(bkg);
  errBand->SetFillStyle(1001);//1001=solid , 3004,3005=diagonal
  errBand->SetFillColor(16);
  errBand->SetLineStyle(1);
  errBand->SetLineColor(16);
  errBand->GetYaxis()->SetRangeUser(-1.5*findMinY(dataDiff,0),2.0*findMaxY(dataDiff,0));
  errBand->GetYaxis()->SetTitle(bkg->GetYaxis()->GetTitle());
  errBand->GetXaxis()->SetTitle(bkg->GetXaxis()->GetTitle());
  errBand->SetNdivisions(505);
  errBand->Draw("histE2");

  sig->Draw("histsame");

  //draw line to cover the red from the signal
  TLine line;
  line.DrawLine(errBand->GetXaxis()->GetXmin(),0,errBand->GetXaxis()->GetXmax(),0);

  //Draw the Data
  dataDiff->Draw("pesame");

  //Draw the Key
  TLegend legendDiff;
  legendDiff.SetFillStyle(0);
  legendDiff.SetFillColor(0);
  legendDiff.SetBorderSize(0);
  legendDiff.AddEntry(dataDiff,"Data - Background","LP");  
  legendDiff.AddEntry(errBand,"Bkg. Uncertainty","F");
  legendDiff.AddEntry(sig,"SM Higgs (125 GeV)","F");
  legendDiff.SetX1NDC(0.55);
  legendDiff.SetX2NDC(0.84);
  legendDiff.SetY1NDC(0.67);
  legendDiff.SetY2NDC(0.87);
  legendDiff.SetTextSize(.035);
  legendDiff.SetTextAlign(   12 );
  legendDiff.Draw();

  CMSPrelim(dataset,channel , 0.18, 0.835);

  C.RedrawAxis();
  C.Print(outname+".ps");



  C.Print(outname+".ps]");

  cout<<"SM Higgs Integral :"<<sig->Integral()<<endl;
  cout<<"Data - Bkg Integral :"<<dataDiff->Integral()<<endl;
  cout<<"Data-Bkg/Higgs : "<<dataDiff->Integral()/sig->Integral()<<endl;

  delete dataDiff;
  delete errBand;


  gROOT->ProcessLine(".q");
}
