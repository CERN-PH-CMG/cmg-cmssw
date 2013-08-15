
/*
 Auxiliary macro for sobWeightedCombineAll.C macro, see instructions there.

Authors: Jose Benitez, Lorenzo Bianchini 
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
  double bkgerr=FErr.Integral(xlow,xhigh);

  return sig/bkg; //purity
  //return sig/sqrt(sig+bkg);//statistical significance
  //return sig/sqrt( bkgerr*bkgerr + sig + bkg );//stat+syst. significance
}

int countInputs(TString * Input){
  int n=0;
  for(Int_t j=0;j<NMAXINPUT;j++){
    if(Input[j])
      if(Input[j].CompareTo("")!=0)n++;
  }
  cout<< " Input has "<<n<<" Files"<<endl;
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


void sobWeightedCombine(TString *Input, TString outName, int weight=1, float  MuValue=1.0){
  
  cout<<"Warning: using fitted mu-value = "<<MuValue<<" , make sure its up to date"<<endl;
  

  int n=countInputs(Input);
  Int_t Rebin[NMAXINPUT];
  findRebin(Input,n,Rebin);
  
  /////Determine the weight for each category
  float weights[100];
  double weightsum=0.;
  for(Int_t f=0;f<n;f++){
    TFile F((Input[f]+".root").Data(),"READ");
    gROOT->cd();

    TH1F* ggH=(TH1F*)(F.Get("ggH")->Clone("ggH"));
    TH1F* Ztt=(TH1F*)(F.Get("Ztt")->Clone("Ztt"));

    weights[f]=getSoB(ggH,Ztt);
    weightsum+=weights[f];
  }
  if(weightsum<=0.){
    cout<<" weightsum is bad :"<<weightsum<<endl;
    return;
  }
  for(Int_t f=0;f<n;f++){//normalize to 1
    weights[f]*= 1./weightsum;
  }
  //////////////print out the list of ordered weights
  cout<<" Ordered Weights"<<endl;
  float maxw=100000.0;
  float TotalweightSum=0.;
  for(Int_t i=0;i<n;i++){
    int maxindex=-1;
    float initmax=0.;
    for(Int_t j=0;j<n;j++)
      if(weights[j]>initmax  &&  weights[j] <maxw){
	maxindex=j;
	initmax=weights[j];
      }
    if(maxindex==-1){
      cout<<"Max weight not found "<<endl;
      break;
    } 
    maxw=weights[maxindex];
    printf("%.4f   %s\n",maxw,Input[maxindex].Data());
    TotalweightSum+=maxw;
  }
  cout<<" sum of weights = "<<TotalweightSum<<endl;



  //////////////////////////////////////////////////////////////////////////////
  //modify the weightsum so that the integral of the Fitted signal is equal to the event yield from the fit
  float signalintegral=0.;
  float weightedsignalintegral=0.;
  for(Int_t f=0;f<n;f++){
    TFile File((Input[f]+".root").Data(),"READ");
    gROOT->cd();

    TH1F* ggH=(TH1F*)File.Get("ggH");
    TH1F* Ztt=(TH1F*)File.Get("Ztt");
    if(!ggH || !Ztt){ cout<<" ggH or Ztt not found in "<<Input[f].Data()<<endl;}

    TH1F* signal=(TH1F*)ggH->Clone("signal");
    signal->Add(Ztt,-1);
    signal->Scale(SignalScale*MuValue);

    //need to undo the bin density to get proper integrals
    //as different categories have different binning
    for(Int_t b=1;b<=signal->GetNbinsX();b++)
      signal->SetBinContent(b,signal->GetBinContent(b)*signal->GetBinWidth(b));
    
    signalintegral += signal->Integral();
    if(weight==1) signal->Scale(weights[f]);
    weightedsignalintegral += signal->Integral();
    delete signal;
  }
  cout<<" signal yield "<<signalintegral<<endl;
  cout<<" weighted signal yield "<<weightedsignalintegral<<endl;
  cout<<" signal yield scale factor "<<signalintegral/weightedsignalintegral<<endl;
  for(Int_t f=0;f<n;f++){//normalize signal yield to original yield
    weights[f] *= signalintegral/weightedsignalintegral;
  }
  /////////////////////////////////////////////////////////////


  TFile F1((Input[0]+".root").Data(),"READ");
  gROOT->cd();
  TH1F* Ztt1=(TH1F*)F1.Get("Ztt");
  TH1F* ggH1=(TH1F*)F1.Get("ggH");

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
  
  F1.Close();

  //printf("%.4f %s\n",weights[0],Input[0].Data());
  for(Int_t f=1;f<n;f++){
    TFile F2((Input[f]+".root").Data(),"READ");
    gROOT->cd();

    TH1F* ggH2=(TH1F*)(F2.Get("ggH")->Clone("ggH2"));
    TH1F* Ztt2=(TH1F*)(F2.Get("Ztt")->Clone("Ztt2"));

    TH1F* signal2=(TH1F*)ggH2->Clone("signal2");
    signal2->Add(Ztt2,-1);
    signal2->Scale(SignalScale);

    TH1F* data_obs2=(TH1F*)(F2.Get("data_obs")->Clone("data_obs2"));
    TH1F* TT2=(TH1F*)(F2.Get("ttbar")->Clone("ttbar2"));    
    TH1F* EWK2=(TH1F*)(F2.Get("EWK")->Clone("EWK2"));    
    TH1F* Fakes2=(TH1F*)(F2.Get("Fakes")->Clone("Fakes2"));


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


    F2.Close();

    delete ggH2;
    delete signal2;
    delete data_obs2;
    delete Ztt2;
    delete TT2;
    delete EWK2;
    delete Fakes2;

  }




  TString outname=TString("Plot_")+outName+".root";

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

void CMSPrelim(const char* dataset, const char* channel,const char* cat)
{

  double lowX=0.19;
  double lowY=0.835;

  int color=1; int font = 62;
  
  TPaveText* cmsprel  = new TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC");
  cmsprel->SetBorderSize(   0 );
  cmsprel->SetFillStyle(    0 );
  cmsprel->SetTextAlign(   12 );
  cmsprel->SetTextColor( color );
  cmsprel->SetTextFont ( font );

  //cmsprel->SetTextSize ( 0.035 );
  //cmsprel->SetTextSize ( 0.027 );
  cmsprel->SetTextSize ( 0.030 );
  cmsprel->AddText(dataset);

  cmsprel->Draw();

 
  TPaveText* chan     = new TPaveText(lowX+0.05, lowY-0.002, lowX+0.45, lowY+0.028, "NDC");
  chan->SetBorderSize(   0 );
  chan->SetFillStyle(    0 );
  chan->SetTextAlign(   12 );
  chan->SetTextSize ( 0.035 );
  chan->SetTextColor( color );
  chan->SetTextFont ( font );
  chan->AddText(channel);
  chan->Draw();

  TPaveText* category     = new TPaveText(lowX+0.05, lowY-0.002-0.06, lowX+0.45, lowY+0.028-0.06, "NDC");
  category->SetBorderSize(   0 );
  category->SetFillStyle(    0 );
  category->SetTextAlign(   12 );
  category->SetTextSize ( 0.035 );
  category->SetTextColor( color );
  category->SetTextFont ( font );
  category->AddText(cat);
  category->Draw();
}



TH1F* diffPlot(TH1F*h1,TH1F*h2,int opt){
  
  TH1F* h=(TH1F*)(h1->Clone(TString(h1->GetName())+h2->GetName()+"_diff"));
  for(Int_t b=1;b<=h1->GetNbinsX();b++){
    h->SetBinContent(b,h1->GetBinContent(b)-h2->GetBinContent(b));
    h->SetBinError(b,0.);
    if(opt==1)h->SetBinError(b,h1->GetBinError(b));
    if(opt==2)h->SetBinError(b,sqrt(h1->GetBinError(b)*h1->GetBinError(b)+h2->GetBinError(b)*h2->GetBinError(b)));
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


void sobWeightedPlot(TString filename,const char* dataset , const char* channel,const char* cat){

  TFile F((TString("Plot_")+filename+".root").Data(),"READ");
  gROOT->cd();
  TH1F* data=(TH1F*)F.Get("data_obs");
  TH1F* Ztt=(TH1F*)F.Get("Ztt");
  TH1F* ggH=(TH1F*)F.Get("ggH");
  TH1F* sig=(TH1F*)F.Get("signal");
  TH1F* tt=(TH1F*)F.Get("ttbar");
  TH1F* ewk=(TH1F*)F.Get("EWK");
  TH1F* fakes=(TH1F*)F.Get("Fakes");
  if(!sig){cout<<"No input histograms in file: "<<filename.Data()<<endl; return;}


  float xmininset=60; float xmaxinset=180;
  //float xmininset=0; float xmaxinset=340;//full range

  ////Format the histograms

  Ztt->GetYaxis()->SetRangeUser(0.,1.3*findMaxY(data,0));
  Ztt->GetXaxis()->SetTitle("#bf{m_{#tau#tau}  [GeV]}");
  Ztt->GetYaxis()->SetTitle("#bf{S/B Weighted dN/dm_{#tau#tau} [1/GeV]}");
  Ztt->SetNdivisions(505);

  for(Int_t b=0;b<=sig->GetNbinsX()+1;b++){
    //remove red line on top of y axis in plot
    if(sig->GetBinCenter(b)<xmininset||xmaxinset<sig->GetBinCenter(b)){
      sig->SetBinContent(b,0);
      sig->SetBinError(b,0);
    }
  }

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


  TLegend legend;
  
  TString higgslabel="H(125 GeV)#rightarrow #tau#tau";
  legend.SetFillStyle(0);
  legend.SetFillColor(0);
  legend.SetBorderSize(0);
  legend.AddEntry(ggH,higgslabel,"F");
  legend.AddEntry(data,"observed","LP");  
  legend.AddEntry(Ztt,"Z#rightarrow#tau#tau","F");
  legend.AddEntry(tt,"t#bar{t}","F");
  legend.AddEntry(ewk,"electroweak","F");
  legend.AddEntry(fakes,"QCD","F");

  legend.SetX1NDC(0.57);
  legend.SetX2NDC(1.05);
  legend.SetY1NDC(0.27);
  legend.SetY2NDC(0.48);
  legend.SetTextSize(.028);
  legend.SetTextAlign(   12 );


  //TH1F* dataDiff=diffPlot(data,Ztt,2);
  TH1F* dataDiff=diffPlot(data,Ztt,1);

 
  TH1F* errBand=getErrorBand(Ztt);
  errBand->SetFillStyle(3013);//1001=solid , 3004,3005=diagonal, 3013=hatched official for H->tau tau
  errBand->SetFillColor(1);
  errBand->SetLineStyle(1);
  errBand->SetLineColor(1);
  

  TH1F errBandFrame("errBandFrame","",(xmaxinset-xmininset)/dataDiff->GetBinWidth(1),xmininset,xmaxinset);
  errBandFrame.GetYaxis()->SetTitle("");
  errBandFrame.GetYaxis()->SetRangeUser(-1.1*findMinY(dataDiff,0,xmininset,xmaxinset),2.0*findMaxY(dataDiff,0,xmininset,xmaxinset));
  errBandFrame.GetYaxis()->SetTitle("");
  errBandFrame.GetYaxis()->SetNdivisions(5);
  errBandFrame.GetYaxis()->SetLabelSize(0.06);
  errBandFrame.GetXaxis()->SetTitle(TString(Ztt->GetXaxis()->GetTitle())+"       ");
  errBandFrame.GetXaxis()->SetTitleSize(0.07);
  errBandFrame.GetXaxis()->SetTitleOffset(0.95);
  errBandFrame.GetXaxis()->SetLabelSize(0.06);
  errBandFrame.SetNdivisions(505);

  TLegend legendDiff;
  legendDiff.SetFillStyle(0);
  legendDiff.SetFillColor(0);
  legendDiff.SetBorderSize(0);
  legendDiff.AddEntry(sig,higgslabel,"F");
  legendDiff.AddEntry(dataDiff,"Data - Background","LP");  
  legendDiff.AddEntry(errBand,"Bkg. Uncertainty","F");
  legendDiff.SetX1NDC(0.45);
  legendDiff.SetX2NDC(0.88);
  legendDiff.SetY1NDC(0.67);
  legendDiff.SetY2NDC(0.88);
  legendDiff.SetTextSize(.045);
  legendDiff.SetTextAlign(12);


  TCanvas C(filename,"",600,600);

  TPad padBack("padBack","padBack",0.57,0.58,0.975,0.956);//TPad must be created after TCanvas otherwise ROOT crashes
  padBack.SetFillColor(0);

  TPad pad("diff","diff",0.45,0.5,0.9765,0.957);//TPad must be created after TCanvas otherwise ROOT crashes
  pad.cd();
  pad.SetFillColor(0);
  pad.SetFillStyle(0);
  errBandFrame.Draw();
  errBand->Draw("e2same");
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
  errorBand->Draw("e2same");

  tt->Draw("histsame");
  ewk->Draw("histsame");
  fakes->Draw("histsame");
  data->Draw("pesame");
  legend.Draw();
  C.RedrawAxis();
  padBack.Draw();//clear the background axes
  pad.Draw();

  CMSPrelim(dataset,channel,cat);
  C.Print(TString("Plot_")+filename+".eps");
  C.Print(TString("Plot_")+filename+".png");
  C.Print(TString("Plot_")+filename+".pdf");
  
  delete errorBand;
  delete dataDiff;
  delete errBand;

}

