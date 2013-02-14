
/*************************************************
 * Functions used to weight the input root files
 ************************************************
*/
#include <TROOT.h>
#include <TFile.h>
#include <TString.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TF1.h>
#include <iostream>
#include <iomanip>

using namespace std;

#define SignalScale 1
#define NMAXINPUT 30

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

TH1F* getErrorBand(TH1F*h1){
  TH1F* h=(TH1F*)(h1->Clone(TString(h1->GetName())+"herrorBand"));
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


double getSoB(TH1F*hsig,TH1F*hbkg){
  if(!hsig||!hbkg){
    cout<<"getSoB : null histos"<<endl;
  }

  //remove the bkg from the signal hist and scale to 1x SM higgs
  TH1F*HS=(TH1F*)hsig->Clone("HS");
  TH1F*HB=(TH1F*)hbkg->Clone("HB");
  HS->Add(HB,-1);
  HS->Scale(SignalScale);

  ///get the error band for  the systematic uncertainty
  //TH1F* HBErr=getErrorBand(HB);

  //put the histos into a TF1 for easy access to integrals
  TString SigString="";
  TString BkgString="";
  TString ErrString="";
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

    char strErr[100];
    //sprintf(strErr,"%.6f*(%.2f<x&&x<%.2f)",HBErr->GetBinError(b),HBErr->GetBinLowEdge(b),HBErr->GetBinLowEdge(b)+HBErr->GetBinWidth(b));
    sprintf(strErr,"%.6f*(%.2f<x&&x<%.2f)",HB->GetBinError(b),HB->GetBinLowEdge(b),HB->GetBinLowEdge(b)+HB->GetBinWidth(b));
    ErrString+=strErr;
    if(b<HB->GetNbinsX())ErrString+="+";

  }


  delete HS;
  delete HB;
  //delete HBErr;

  //cout<<SigString<<endl;
  //cout<<BkgString<<endl;
  TF1 FSig("FSig",SigString,xmin,xmax);
  TF1 FBkg("FBkg",BkgString,xmin,xmax);
  TF1 FErr("FErr",ErrString,xmin,xmax);
  
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
  //double bkgerr=FErr.Integral(xlow,xhigh);


  return sig/bkg;//purity
  //return sig/sqrt(sig+bkg);//statistical significance
  //return sig/sqrt( bkgerr*bkgerr + sig + bkg );//stat+syst. significance
}

int countInputs(TString * Input){
  int n=0;
  for(Int_t j=0;j<NMAXINPUT;j++){
    ////while(Input[n].CompareTo("")!=0 && n<100){
    //cout<<j<<" "<<Input[j]<<endl;
    if(Input[j])
      if(Input[j].CompareTo("")!=0)n++;
  }
  return n;
}

void findRebin(TString * Input,Int_t N, Int_t * Rebin){
  
  float binw=0.;
  //determine the max bin width
  for(Int_t i=0;i<N;i++){
    TFile File((Input[i]+".root").Data(),"READ");
    TH1F* ggH=(TH1F*)File.Get("ggH");
    if(ggH->GetBinWidth(1)>binw) 
      binw=ggH->GetBinWidth(1);
  }
  cout<<"Max bin width "<<binw<<endl;


  for(Int_t i=0;i<N;i++){
    Rebin[i]=0;

    TFile File((Input[i]+".root").Data(),"READ");
    TH1F* ggH=(TH1F*)File.Get("ggH");
    if(ggH->GetBinWidth(1)!=binw){
      //cout<<i<<" "<<Input[i]<<" "<<ggH->GetBinWidth(1)<<endl;
      Rebin[i]=1;
    }
  }
  

}


void weightedCombine(TString *Input, TString outName, int weight=1){

  int n=countInputs(Input); cout<< " Input has "<<n<<" Files"<<endl;
  Int_t Rebin[NMAXINPUT];
  findRebin(Input,n,Rebin);
  

  TFile F1((Input[0]+".root").Data(),"READ");
  gROOT->cd();

  //array of the weights for ordering later
  float weights[100];


  //first compute the sum of the weights
  double weightsum=0.;
  for(Int_t f=0;f<n;f++){
    TFile File((Input[f]+".root").Data(),"READ");
    gROOT->cd();

    TH1F* ggH=(TH1F*)File.Get("ggH");
    TH1F* Ztt=(TH1F*)File.Get("Ztt");
    if(!ggH || !Ztt){ cout<<" ggH or Ztt not found in "<<Input[f].Data()<<endl;}

    weightsum+=getSoB(ggH,Ztt);
  }

  TH1F* Ztt1=(TH1F*)F1.Get("Ztt");
  TH1F* ggH1=(TH1F*)F1.Get("ggH");

  //Compute the S/B weight
  weights[0]=getSoB(ggH1,Ztt1)/weightsum;

  ///Signal
  TH1F* ggH=(TH1F*)ggH1->Clone("ggH");
  if(weight==1) ggH->Scale(weights[0]);
  if(Rebin[0]) ggH->Rebin(2);

  TH1F* signal=(TH1F*)ggH1->Clone("signal");
  signal->Add(Ztt1,-1);
  signal->Scale(SignalScale);
  if(weight==1) signal->Scale(weights[0]);
  if(Rebin[0]) signal->Rebin(2);

  //Background
  TH1F* Ztt=(TH1F*)Ztt1->Clone("Ztt");
  if(weight==1)Ztt->Scale(weights[0]);
  if(Rebin[0]) Ztt->Rebin(2);

  //Data
  TH1F* data_obs=((TH1F*)F1.Get("data_obs")->Clone("data_obs"));
  if(weight==1)data_obs->Scale(weights[0]);
  if(Rebin[0]) data_obs->Rebin(2);

  //Other backgrounds for plot
  TH1F* TT=(TH1F*)(F1.Get("ttbar")->Clone("ttbar"));
  if(weight==1)TT->Scale(weights[0]);
  if(Rebin[0]) TT->Rebin(2);

  TH1F* EWK=(TH1F*)(F1.Get("EWK")->Clone("EWK"));
  if(weight==1)EWK->Scale(weights[0]);
  if(Rebin[0]) EWK->Rebin(2);

  TH1F* Fakes=(TH1F*)(F1.Get("Fakes")->Clone("Fakes"));
  if(weight==1)Fakes->Scale(weights[0]);
  if(Rebin[0]) Fakes->Rebin(2);
  


  //printf("%.4f %s\n",weights[0],Input[0].Data());
  for(Int_t f=1;f<n;f++){
    TFile F2((Input[f]+".root").Data(),"READ");
    gROOT->cd();

    TH1F* ggH2=(TH1F*)F2.Get("ggH");
    TH1F* Ztt2=(TH1F*)F2.Get("Ztt");

    weights[f]=getSoB(ggH2,Ztt2)/weightsum;
    //printf("%.4f   %s\n",weights[f],Input[f].Data());

    TH1F* signal2=(TH1F*)ggH2->Clone("signal2");
    signal2->Add(Ztt2,-1);
    signal2->Scale(SignalScale);

    TH1F* data_obs2=(TH1F*)F2.Get("data_obs");
    TH1F* TT2=(TH1F*)F2.Get("ttbar");    
    TH1F* EWK2=(TH1F*)F2.Get("EWK");    
    TH1F* Fakes2=(TH1F*)F2.Get("Fakes");


    //rescale by S/B before adding
    ggH2->Scale(weight==1 ? weights[f] : 1);
    signal2->Scale(weight==1 ? weights[f] : 1);
    data_obs2->Scale(weight==1 ? weights[f] : 1);
    Ztt2->Scale(weight==1 ? weights[f] : 1);
    TT2->Scale(weight==1 ? weights[f] : 1);
    EWK2->Scale(weight==1 ? weights[f] : 1);
    Fakes2->Scale(weight==1 ? weights[f] : 1);

    ///Rebin
    if(Rebin[f])ggH2->Rebin(2);
    if(Rebin[f])signal2->Rebin(2);
    if(Rebin[f])Ztt2->Rebin(2);
    if(Rebin[f])data_obs2->Rebin(2);
    if(Rebin[f])TT2->Rebin(2);
    if(Rebin[f])EWK2->Rebin(2);
    if(Rebin[f])Fakes2->Rebin(2);


    //Combine
    ggH->Add(ggH2);
    signal->Add(signal2);
    data_obs->Add(data_obs2);
    Ztt->Add(Ztt2);
    TT->Add(TT2);
    EWK->Add(EWK2);
    Fakes->Add(Fakes2);

    delete signal2;
  }



  //print out ordered weights
  cout<<" Ordered Weights"<<endl;
  float maxw=1.0;
  float TotalweightSum=0.;
  for(Int_t i=0;i<n;i++){
    int maxindex=-1;
    float initmax=0.;
    for(Int_t j=0;j<n;j++){
      if(weights[j]>initmax  &&  weights[j] <maxw){
	maxindex=j;
	initmax=weights[j];
      }
    }
    
    maxw=weights[maxindex];
      
    if(maxindex==-1) cout<<"Max not found "<<endl;
    else printf("%.4f   %s\n",maxw,Input[maxindex].Data());
    
    TotalweightSum+=maxw;
  }
  cout<<" sum of weights = "<<TotalweightSum<<endl;


  TString outname=outName;
  if(weight==1)outname+="_Weighted.root";
  if(weight==0)outname+="_UnWeighted.root";

  TFile outFile(outname.Data(),"recreate");
  ggH->Write();
  signal->Write();
  data_obs->Write();
  Ztt->Write();
  TT->Write();
  EWK->Write();
  Fakes->Write();
  outFile.ls();
  outFile.Close();


  delete ggH;
  delete signal;
  delete data_obs;
  delete Ztt;
  delete TT;
  delete EWK;
  delete Fakes;

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
  cmsprel->AddText("CMS Prelim., ");
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
  TH1F* h=(TH1F*)h1->Clone("hdiff");
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



float findMaxY(TH1F* h,int opt=0,float lowx=0.,float highx=0.){
  float max=0.;

  if(lowx==0.)lowx=h->GetXaxis()->GetXmin();
  if(highx==0.)highx=h->GetXaxis()->GetXmax();

  for(int b=1;b<=h->GetNbinsX();b++){
    if(h->GetBinContent(b)>0.&&lowx<h->GetBinCenter(b)&&h->GetBinCenter(b)<highx){
      if(opt==0)      if(fabs(h->GetBinContent(b))+fabs(h->GetBinError(b)) > max) max = fabs(h->GetBinContent(b))+fabs(h->GetBinError(b));
      if(opt==1)      if(fabs(h->GetBinContent(b)) > max)                         max = fabs(h->GetBinContent(b));
    }
  }

  //cout<<max<<endl;
  return max;
}

float findMinY(TH1F* h,int opt=0,float lowx=0.,float highx=0.){
  float max=0.;

  if(lowx==0.)lowx=h->GetXaxis()->GetXmin();
  if(highx==0.)highx=h->GetXaxis()->GetXmax();

  for(int b=1;b<=h->GetNbinsX();b++){
    if(h->GetBinContent(b)<0.&&lowx<h->GetBinCenter(b)&&h->GetBinCenter(b)<highx){
      if(opt==0)      if(fabs(h->GetBinContent(b))+fabs(h->GetBinError(b)) > max) max = fabs(h->GetBinContent(b))+fabs(h->GetBinError(b));
      if(opt==1)      if(fabs(h->GetBinContent(b)) > max)                         max = fabs(h->GetBinContent(b));
    }
  }

  //cout<<max<<endl;
  return max;
}

void weightedPlot(TString filename,const char* dataset , const char* channel ){

  TFile F((filename+".root").Data(),"READ");
  TH1F* data=(TH1F*)F.Get("data_obs");
  TH1F* Ztt=(TH1F*)F.Get("Ztt");
  TH1F* ggH=(TH1F*)F.Get("ggH");
  TH1F* sig=(TH1F*)F.Get("signal");
  TH1F* tt=(TH1F*)F.Get("ttbar");
  TH1F* ewk=(TH1F*)F.Get("EWK");
  TH1F* fakes=(TH1F*)F.Get("Fakes");
  if(!sig){cout<<"No input histograms in file: "<<filename.Data()<<endl; return;}


  ////Format the histograms
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

  ggH->SetBinContent(0,0);//remove red line on top of y axis in plot
  ggH->SetBinContent(ggH->GetNbinsX()+1,0);
  ggH->SetBinError(0,0);
  ggH->SetBinError(ggH->GetNbinsX()+1,0);
  ggH->SetName("ggH");
  ggH->SetFillStyle(3353);//1001=solid , 3004,3005=diagonal
  ggH->SetFillColor(2);
  ggH->SetLineColor(2);
  ggH->SetLineStyle(1);
  ggH->SetLineWidth(0.);

  TH1F* errorBand = (TH1F*)Ztt->Clone();
  errorBand  ->SetMarkerSize(0);
  errorBand  ->SetFillColor(1);
  errorBand  ->SetFillStyle(3013);
  errorBand  ->SetLineWidth(1);
  errorBand->Draw("e2same");


  TLegend legend;//(0.59, 0.65, 0.99, 0.90);
  
  legend.SetFillStyle(0);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  legend.AddEntry(data,"Observed","LP");  
  legend.AddEntry(ggH,"SM Higgs (125 GeV)","F");
  legend.AddEntry(Ztt,"Z#rightarrow#tau#tau","F");
  legend.AddEntry(tt,"t#bar{t}","F");
  legend.AddEntry(ewk,"electroweak","F");
  legend.AddEntry(fakes,"QCD","F");

  legend.SetX1NDC(0.57);
  legend.SetX2NDC(0.86);
  legend.SetY1NDC(0.60);
  legend.SetY2NDC(0.87);
  legend.SetTextSize(.035);
  legend.SetTextAlign(   12 );


  TString outname=TString("Plot_")+filename;

  TCanvas C(outname);
  C.Print(outname+".ps[");


  //Data and Background Plot
  C.Clear();
  Ztt->GetYaxis()->SetRangeUser(0.,1.2*findMaxY(Ztt,0));
  Ztt->GetXaxis()->SetTitle("#bf{m_{#tau#tau} [GeV]}");
  Ztt->GetYaxis()->SetTitle("#bf{Weighted dN/dm_{#tau#tau} [1/GeV]}");//  dN/dm(#tau#tau)
  Ztt->SetNdivisions(505);
  Ztt->Draw("hist");

  ggH->Draw("histsame");
  Ztt->Draw("histsame");
  tt->Draw("histsame");
  ewk->Draw("histsame");
  fakes->Draw("histsame");
  data->Draw("pesame");
  legend.Draw();

  CMSPrelim(dataset,channel , 0.18, 0.835);

  C.RedrawAxis();
  C.Print(outname+".ps");
  C.Print(outname+".png");

  delete errorBand;



  ///////////Data - Background and Signal Plot

  C.Clear();
  TH1F* dataDiff=diffPlot(data,Ztt,1);
  dataDiff->SetName("dataDiff");

  TH1F* errBand=getErrorBand(Ztt);
  errBand->SetFillStyle(1001);//1001=solid , 3004,3005=diagonal
  errBand->SetFillColor(16);
  errBand->SetLineStyle(1);
  errBand->SetLineColor(16);
  errBand->GetYaxis()->SetRangeUser(-1.5*findMinY(dataDiff,0),2.0*findMaxY(dataDiff,0));
  errBand->GetYaxis()->SetTitle(Ztt->GetYaxis()->GetTitle());
  errBand->GetXaxis()->SetTitle(Ztt->GetXaxis()->GetTitle());
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
  C.Print(outname+"_diff.png");



  C.Print(outname+".ps]");

  cout<<"SM Higgs Integral :"<<sig->Integral()<<endl;
  cout<<"Data - Bkg Integral :"<<dataDiff->Integral()<<endl;
  cout<<"Data-Bkg/Higgs : "<<dataDiff->Integral()/sig->Integral()<<endl;

  delete dataDiff;
  delete errBand;


  //gROOT->ProcessLine(".q");
}



void weightedPlotInset(TString filename,const char* dataset , const char* channel ){

  TFile F((filename+".root").Data(),"READ");
  gROOT->cd();
  TH1F* data=(TH1F*)F.Get("data_obs");
  TH1F* Ztt=(TH1F*)F.Get("Ztt");
  TH1F* ggH=(TH1F*)F.Get("ggH");
  TH1F* sig=(TH1F*)F.Get("signal");
  TH1F* tt=(TH1F*)F.Get("ttbar");
  TH1F* ewk=(TH1F*)F.Get("EWK");
  TH1F* fakes=(TH1F*)F.Get("Fakes");
  if(!sig){cout<<"No input histograms in file: "<<filename.Data()<<endl; return;}


  ////Format the histograms

  Ztt->GetYaxis()->SetRangeUser(0.,1.3*findMaxY(Ztt,0));
  Ztt->GetXaxis()->SetTitle("#bf{m_{#tau#tau}    (GeV)}              ");
  Ztt->GetYaxis()->SetTitle("#bf{S/B Weighted dN/dm_{#tau#tau} [1/GeV]}");
  Ztt->SetNdivisions(505);

  TH1F*Bkg=(TH1F*)Ztt->Clone("Bkg");
  Bkg->SetFillStyle(3004);//1001=solid , 3004,3005=diagonal
  Bkg->SetFillColor(1);
  Bkg->SetMarkerSize(0.);

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

  ggH->SetBinContent(0,0);//remove red line on top of y axis in plot
  ggH->SetBinContent(ggH->GetNbinsX()+1,0);
  ggH->SetBinError(0,0);
  ggH->SetBinError(ggH->GetNbinsX()+1,0);
  ggH->SetName("ggH");
  ggH->SetFillStyle(3353);//1001=solid , 3004,3005=diagonal
  ggH->SetFillColor(2);
  ggH->SetLineColor(2);
  ggH->SetLineStyle(1);
  ggH->SetLineWidth(0.);

  TH1F* errorBand = (TH1F*)Ztt->Clone("errorBand");
  errorBand->SetMarkerSize(0);
  errorBand->SetFillColor(1);
  errorBand->SetFillStyle(3013);
  errorBand->SetLineWidth(1);
  errorBand->Draw("e2same");


  TLegend legend;
  
  legend.SetFillStyle(0);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  legend.AddEntry(data,"Observed","LP");  
  //legend.AddEntry(ggH,"SM Higgs (125 GeV)","F");
  legend.AddEntry(Ztt,"Z#rightarrow#tau#tau","F");
  legend.AddEntry(tt,"t#bar{t}","F");
  legend.AddEntry(ewk,"electroweak","F");
  legend.AddEntry(fakes,"QCD","F");

//   legend.SetX1NDC(0.57);
//   legend.SetX2NDC(0.86);
//   legend.SetY1NDC(0.25);
//   legend.SetY2NDC(0.5);

  legend.SetX1NDC(0.72);
  legend.SetX2NDC(0.92);
  legend.SetY1NDC(0.23);
  legend.SetY2NDC(0.48);

  legend.SetTextSize(.028);
  legend.SetTextAlign(   12 );


  TH1F* dataDiff=diffPlot(data,Ztt,1);
  dataDiff->SetName("dataDiff");

  float xmininset=60; float xmaxinset=180;
  TH1F* errBand=getErrorBand(Ztt);
  errBand->SetFillStyle(1001);//1001=solid , 3004,3005=diagonal
  errBand->SetFillColor(16);
  errBand->SetLineStyle(1);
  errBand->SetLineColor(16);
//   errBand->GetYaxis()->SetRangeUser(-1.5*findMinY(dataDiff,0),2.0*findMaxY(dataDiff,0));
//   errBand->GetYaxis()->SetTitle("");
//   errBand->GetYaxis()->SetNdivisions(5);
//   errBand->GetYaxis()->SetLabelSize(0.06);
//   errBand->GetXaxis()->SetTitle(Ztt->GetXaxis()->GetTitle());
//   errBand->GetXaxis()->SetTitleSize(0.07);
//   errBand->GetXaxis()->SetTitleOffset(0.95);
//   errBand->GetXaxis()->SetLabelSize(0.06);
//   errBand->SetNdivisions(505);
//   errBand->GetXaxis()->SetRangeUser(xmininset,xmaxinset);

  TH1F errBandFrame("errBandFrame","",(xmaxinset-xmininset)/dataDiff->GetBinWidth(1),xmininset,xmaxinset);
  errBandFrame.GetYaxis()->SetTitle("");
  errBandFrame.GetYaxis()->SetRangeUser(-1.1*findMinY(dataDiff,0,xmininset,xmaxinset),2.0*findMaxY(dataDiff,0,xmininset,xmaxinset));
  errBandFrame.GetYaxis()->SetTitle("");
  errBandFrame.GetYaxis()->SetNdivisions(5);
  errBandFrame.GetYaxis()->SetLabelSize(0.06);
  errBandFrame.GetXaxis()->SetTitle(Ztt->GetXaxis()->GetTitle());
  errBandFrame.GetXaxis()->SetTitleSize(0.07);
  errBandFrame.GetXaxis()->SetTitleOffset(0.95);
  errBandFrame.GetXaxis()->SetLabelSize(0.06);
  errBandFrame.SetNdivisions(505);

  TLegend legendDiff;
  legendDiff.SetFillStyle(0);
  legendDiff.SetFillColor(0);
  legendDiff.SetBorderSize(0);
  legendDiff.AddEntry(dataDiff,"Data - Background","LP");  
  legendDiff.AddEntry(errBand,"Bkg. Uncertainty","F");
  legendDiff.AddEntry(sig,"SM Higgs (125 GeV)","F");
  legendDiff.SetX1NDC(0.45);
  legendDiff.SetX2NDC(0.88);
  legendDiff.SetY1NDC(0.67);
  legendDiff.SetY2NDC(0.88);
  legendDiff.SetTextSize(.045);
  legendDiff.SetTextAlign(   12 );



  TString outname=TString("Plot_")+filename;
  TCanvas C(outname);

  TPad pad("diff","diff",0.45,0.5,0.975,0.956);//TPad must be created after TCanvas otherwise ROOT crashes
  pad.cd();
  pad.SetFillColor(0);
  pad.SetFillStyle(0);
  errBandFrame.Draw();
  errBand->Draw("E2same");
  sig->Draw("histsame");
  TLine line;
  line.DrawLine(xmininset,0,xmaxinset,0);
  dataDiff->Draw("pesame");
  legendDiff.Draw();
  pad.RedrawAxis();

  C.cd();
  Ztt->Draw("hist");
  ggH->Draw("histsame");
  Ztt->Draw("histsame");
  Bkg->Draw("e2same");

  tt->Draw("histsame");
  ewk->Draw("histsame");
  fakes->Draw("histsame");
  data->Draw("pesame");
  legend.Draw();
  C.RedrawAxis();
  pad.Draw();

  CMSPrelim(dataset,channel , 0.18, 0.835);
  C.Print(outname+".eps");
  C.Print(outname+".png");
  
  delete Bkg;
  delete dataDiff;
  delete errBand;
  delete errorBand;

  F.Close();

  //gROOT->ls();
  //gROOT->Reset();

}



void weightedPlotGammaGamma(TString filename,const char* dataset , const char* channel ){

  TFile F((filename+".root").Data(),"READ");
  TH1F* data=(TH1F*)F.Get("data_obs");
  TH1F* Ztt=(TH1F*)F.Get("Ztt");
  TH1F* ggH=(TH1F*)F.Get("ggH");
  if(!data){cout<<"No input histograms in file: "<<filename.Data()<<endl; return;}



  ggH->SetBinContent(0,0);//remove red line on top of y axis in plot
  ggH->SetBinContent(ggH->GetNbinsX()+1,0);
  ggH->SetBinError(0,0);
  ggH->SetBinError(ggH->GetNbinsX()+1,0);
  ggH->SetName("ggH");
  ggH->SetFillColor(0);
  ggH->SetLineColor(2);
  ggH->SetLineStyle(1);

  TH1F* errorBand = (TH1F*)Ztt->Clone();
  errorBand->SetMarkerSize(0);
  errorBand->SetFillColor(1);
  errorBand->SetFillStyle(3013);
  errorBand->SetLineWidth(1);
  errorBand->Draw("e2same");


  TLegend legend;
  
  legend.SetFillStyle(0);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  legend.AddEntry(data,"Observed","LP");  
  legend.AddEntry(ggH,"SM Higgs (125 GeV)","F");
  legend.AddEntry(Ztt,"Z#rightarrow#tau#tau","F");

  legend.SetX1NDC(0.57);
  legend.SetX2NDC(0.86);
  legend.SetY1NDC(0.60);
  legend.SetY2NDC(0.87);
  legend.SetTextSize(.035);
  legend.SetTextAlign(   12 );


  TString outname=TString("Plot_")+filename;

  TCanvas C(outname);
  C.Print(outname+".ps[");


  //Data and Background Plot
  C.Clear();
  Ztt->GetYaxis()->SetRangeUser(0.,1.2*findMaxY(Ztt,0));
  Ztt->GetXaxis()->SetTitle("#bf{m_{#tau#tau} [GeV]}");
  Ztt->GetYaxis()->SetTitle("#bf{Weighted dN/dm_{#tau#tau} [1/GeV]}");//  dN/dm(#tau#tau)
  Ztt->SetNdivisions(505);
  Ztt->SetFillColor(0);
  Ztt->SetLineColor(4);
  Ztt->Draw("hist");

  ggH->Draw("histsame");
  Ztt->Draw("histsame");
  data->Draw("pesame");
  legend.Draw();

  CMSPrelim(dataset,channel , 0.18, 0.835);

  C.RedrawAxis();
  C.Print(outname+".ps");
  C.Print(outname+".png");

  delete errorBand;


  C.Print(outname+".ps]");

}
