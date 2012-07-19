void WJetsMethodSystematic(){
 
//                   m_T>60            AntiIso-Tau 
//    ---------------------------------------------
//    njet0:     4568.7 +-  95.0    4848.5 +-  95.5  
//    njet1:     1411.3 +-  54.7    1522.8 +-  32.3  
//    njet2:      455.1 +-  37.2     385.6 +-  16.8 
//    SM0:       5964.4 +- 109.4    6395.5 +- 102.7
//    SM1:         52.1 +-  10.3      57.2 +-   5.2
//    SM2:         13.3 +-   4.9      17.2 +-   2.8


// New highMT normalization:
//                   m_T>60            AntiIso-Tau 
//    ---------------------------------------------
//    njet0:      4574.0 +-  96.2     4848.5 +-  95.5  
//    njet1:      1410.0 +-  54.5     1522.8 +-  32.3  
//    njet2:       403.8 +-  31.8      385.6 +-  16.8 
//    SM0:        5973.2 +- 110.6     6395.5 +- 102.7
//    SM1:          50.1 +-  10.0       57.2 +-   5.2
//    SM2:          24.7 +-   9.5       17.2 +-   2.8


//Simple Iso/Anti-Iso ratio:
//SM1 : 232.7 +- 7.2
//SM2 :  38.9 +- 3.4

  //yields vs njet
  Float_t YieldmT60[3];
  Float_t YieldErrmT60[3];
  YieldmT60[0]=4574.0;  YieldErrmT60[0]=96.2;
  YieldmT60[1]=1410.0;  YieldErrmT60[1]=54.5;
  YieldmT60[2]=403.8;   YieldErrmT60[2]=31.8;

  Float_t YieldRatio[3];
  Float_t YieldErrRatio[3];
  YieldRatio[0]=4848;  YieldErrRatio[0]=105;
  YieldRatio[1]=1522;  YieldErrRatio[1]=32;
  YieldRatio[2]=385;  YieldErrRatio[2]=16;

  TH1F hmT60("hmT60","",3,-0.5,2.5);
  hmT60.GetXaxis()->SetTitle("# of jets");
  TH1F hRatio("hRatio","",3,-0.5,2.5);
  hRatio.GetXaxis()->SetTitle("# of jets");
  for(Int_t b=1;b<=3;b++){
    hmT60.SetBinContent(b,YieldmT60[b-1]);
    hmT60.SetBinError(b,YieldErrmT60[b-1]);
    hRatio.SetBinContent(b,YieldRatio[b-1]);
    hRatio.SetBinError(b,YieldErrRatio[b-1]);
  }

  TCanvas C;

  C.Clear();
  hRatio.SetStats(0);
  hRatio.Divide(&hmT60);
  hRatio.GetYaxis()->SetRangeUser(0.5,1.5);
  hRatio.GetYaxis()->SetTitle("Yield R(pT) / Yield mT>60");
  hRatio.Draw("histpe");
  TLine line;
  line.DrawLine(hRatio.GetXaxis()->GetXmin(),1,hRatio.GetXaxis()->GetXmax(),1);
  C.Print("WJetsMethodSystematic.ps");
  
}


void WJetsMCShape(){
  TString ratio="((20<taujetpt&&taujetpt<=25)*15.2+(25<taujetpt&&taujetpt<=30)*1.53+(30<taujetpt&&taujetpt<=35)*0.492+(35<taujetpt&&taujetpt<=40)*0.237+(40<taujetpt&&taujetpt<=45)*0.124+(45<taujetpt&&taujetpt<=50)*0.0864+(50<taujetpt&&taujetpt<=55)*0.0613+(55<taujetpt&&taujetpt<=60)*0.0465+(60<taujetpt&&taujetpt<=65)*0.0405+(65<taujetpt&&taujetpt<=70)*0.0342+(70<taujetpt&&taujetpt<=75)*0.0302+(75<taujetpt&&taujetpt<=80)*0.0253+(80<taujetpt&&taujetpt<=85)*0.0208+(85<taujetpt&&taujetpt<=90)*0.0228+(90<taujetpt&&taujetpt<=95)*0.0157+(95<taujetpt&&taujetpt<=100)*0.0174)";

  Sample s("WJetsToLNu","/data/benitezj/Samples/Mar19MuJet");
  
//   TString sel="*1";
  TString sel="*(njet==1)";
//   TString sel="*(categorySM==1)";
//   TString sel="*(categorySM==2)";

//   TString variable="ditaumass";
//   Int_t nbins=30;
//   float xmin=0;
//   float xmax=300;

//   TString variable="taupt";
//   Int_t nbins=10;
//   float xmin=0;
//   float xmax=100;

  TString variable="svfitmass";
  Int_t nbins=20;
  float xmin=0;
  float xmax=400;

  
  TH1F*hi=s.getHistoNtpFile(variable,nbins,xmin,xmax,"(categoryCh==1&&categoryMT==1&&muiso<0.1&&tauisodisc>=2)"+sel);
  hi->SetName("HVisMassIso");hi->SetTitle("HVisMassIso");
  TH1F*ha=s.getHistoNtpFile(variable,nbins,xmin,xmax,"(categoryCh==1&&categoryMT==1&&muiso<0.1&&tauisodisc<2)"+sel);
  ha->SetName("HVisMassAntiIso");ha->SetTitle("HVisMassAntiIso");
  TH1F*hiw=s.getHistoNtpFile(variable,nbins,xmin,xmax,"eventweight*(categoryCh==1&&categoryMT==1&&muiso<0.1&&tauisodisc>=2)"+sel);
  hiw->SetName("HVisMassIsoWeight");hiw->SetTitle("HVisMassIsoWeight");
  TH1F*haw=s.getHistoNtpFile(variable,nbins,xmin,xmax,"eventweight*(categoryCh==1&&categoryMT==1&&muiso<0.1&&tauisodisc<2)"+sel);
  haw->SetName("HVisMassAntiIsoWeight");haw->SetTitle("HVisMassAntiIsoWeight");
  TH1F*hawr=s.getHistoNtpFile(variable,nbins,xmin,xmax,TString("eventweight*(categoryCh==1&&categoryMT==1&&muiso<0.1&&tauisodisc<2)")+"*"+ratio+sel);
  hawr->SetName("HVisMassAntiIsoWeightTimesRatio");hawr->SetTitle("HVisMassAntiIsoWeightTimesRatio");


  TFile F("WJetsMCShape.root","recreate");
  hi->Write();
  ha->Write();
  hiw->Write();
  haw->Write();
  hawr->Write();
  F.ls();
  F.Close();


  TCanvas C("WJetsMCShape");
  C.Print("WJetsMCShape.ps[");
  
  C.Clear();
  hi->Draw();
  C.Print("WJetsMCShape.ps");
  
  C.Clear();
  ha->Draw();
  C.Print("WJetsMCShape.ps");

  //with jet pT dependent ratio
  C.Clear();
  hi->GetYaxis()->SetRangeUser(0,1.3*hi->GetMaximum());
  hi->Draw();
  hawr->SetLineColor(2);
  hawr->Draw("histsame");
  C.Print("WJetsMCShape.ps");

  //With simple fake rate ratio
  C.Clear();
  hi->Draw();
  hawr->Draw("histsame");
  haw->Scale(hi->Integral()/haw->Integral());
  haw->Draw("histsame");
  C.Print("WJetsMCShape.ps");

  
  C.Print("WJetsMCShape.ps]");




}


void WJetsTotalSystematic(){


  float stat[2];
  float method[2];
  float mTShape[2];
  float total[2];

  //Boosted:
  stat[0]=0.09;
  method[0]=0.1;
  mtShape[0]=0.06;
  
  //VBF:
  stat[1]=0.16;
  method[1]=0.1;
  mtShape[1]=0.06;

  
  total[0]=sqrt(stat[0]*stat[0] + method[0]*method[0] + mtShape[0]*mtShape[0]);
  cout<<0<<" "<<total[0]<<endl;

  total[1]=sqrt(stat[1]*stat[1] + method[1]*method[1] + mtShape[1]*mtShape[1]);
  cout<<1<<" "<<total[1]<<endl;


}


//////////Check shape of WJets Shape as a function of tau isolation
void plotWJetsShape(){
  Sample s("WJetsToLNu","/data/benitezj/Samples/TauMuV541June2_TrigEff");
  
  TString sel="eventweight*(categoryCh==1&&muiso<0.1)";
  
  TH1F*hmtP075=s.getHistoNtpFile("transversemass",15,0,150,sel+"*(tauisomva>0.75)");
  hmtP075->SetName("hmtP075");hmtP075->SetTitle("");

  TH1F*hmtP050=s.getHistoNtpFile("transversemass",15,0,150,sel+"*(tauisomva>0.5)");
  hmtP050->SetName("hmtP050");hmtP050->SetTitle("");
  hmtP050->Scale(hmtP075->Integral(8,15)/hmtP050->Integral(8,15));

  TH1F*hmtM000=s.getHistoNtpFile("transversemass",15,0,150,sel+"*(tauisomva>0.0)");
  hmtM000->SetName("hmtM000");hmtM000->SetTitle("");
  hmtM000->Scale(hmtP075->Integral(8,15)/hmtM000->Integral(8,15));

  TH1F*hmtM050=s.getHistoNtpFile("transversemass",15,0,150,sel+"*(tauisomva>-0.50)");
  hmtM050->SetName("hmtM050");hmtM050->SetTitle("");
  hmtM050->Scale(hmtP075->Integral(8,15)/hmtM050->Integral(8,15));

  TH1F*hmtM075=s.getHistoNtpFile("transversemass",15,0,150,sel+"*(tauisomva>-0.75)");
  hmtM075->SetName("hmtM075");hmtM075->SetTitle("");
  hmtM075->Scale(hmtP075->Integral(8,15)/hmtM075->Integral(8,15));

  TH1F*hmtM100=s.getHistoNtpFile("transversemass",15,0,150,sel+"*(tauisomva>-1.0)");
  hmtM100->SetName("hmtM100");hmtM100->SetTitle("");
  hmtM100->Scale(hmtP075->Integral(8,15)/hmtM100->Integral(8,15));



  TCanvas C("plotWJetsMTShape");
  C.Print("plotWJetsMTShape.ps[");
  C.Clear();
  hmtP075->GetYaxis()->SetRangeUser(0,1.2*hmtP075->GetMaximum());
  hmtP075->Draw("histpe");
  hmtP050->Draw("histsame");
  hmtM000->Draw("histsame");
  hmtM050->Draw("histsame");
  hmtM075->Draw("histsame");
  hmtM100->Draw("histsame");
  C.Print("plotWJetsMTShape.ps");

  //plot ratios
  hmtP050->Divide(hmtP075);
  hmtM000->Divide(hmtP075);
  hmtM050->Divide(hmtP075);
  hmtM075->Divide(hmtP075);
  hmtM100->Divide(hmtP075);
  C.Clear();
  hmtP050->GetYaxis()->SetRangeUser(0.5,1.5);
  hmtP050->GetYaxis()->SetTitle("ratio");
  hmtP050->Draw("hist");
  hmtM000->Draw("histsame");
  hmtM050->Draw("histsame");
  hmtM075->Draw("histsame");
  hmtM100->Draw("histsame");
  TLine line;
  line.DrawLine(0,1,150,1);
  C.Print("plotWJetsMTShape.ps");





  //////changes on the svfitmass
  TH1F*hmsvP075=s.getHistoNtpFile("svfitmass",30,0,300,sel+"*(tauisomva>0.75)");
  hmsvP075->SetName("hmsvP075");hmsvP075->SetTitle("");

  TH1F*hmsvP050=s.getHistoNtpFile("svfitmass",30,0,300,sel+"*(tauisomva>0.5)");
  hmsvP050->SetName("hmsvP050");hmsvP050->SetTitle("");
  hmsvP050->Scale(hmsvP075->Integral()/hmsvP050->Integral());

  TH1F*hmsvM000=s.getHistoNtpFile("svfitmass",30,0,300,sel+"*(tauisomva>0.0)");
  hmsvM000->SetName("hmsvM000");hmsvM000->SetTitle("");
  hmsvM000->Scale(hmsvP075->Integral()/hmsvM000->Integral());

  TH1F*hmsvM050=s.getHistoNtpFile("svfitmass",30,0,300,sel+"*(tauisomva>-0.50)");
  hmsvM050->SetName("hmsvM050");hmsvM050->SetTitle("");
  hmsvM050->Scale(hmsvP075->Integral()/hmsvM050->Integral());

  TH1F*hmsvM075=s.getHistoNtpFile("svfitmass",30,0,300,sel+"*(tauisomva>-0.75)");
  hmsvM075->SetName("hmsvM075");hmsvM075->SetTitle("");
  hmsvM075->Scale(hmsvP075->Integral()/hmsvM075->Integral());

  TH1F*hmsvM100=s.getHistoNtpFile("svfitmass",30,0,300,sel+"*(tauisomva>-1.0)");
  hmsvM100->SetName("hmsvM100");hmsvM100->SetTitle("");
  hmsvM100->Scale(hmsvP075->Integral()/hmsvM100->Integral());


  C.Clear();
  hmsvP075->GetYaxis()->SetRangeUser(0,1.2*hmsvP075->GetMaximum());
  hmsvP075->Draw("histpe");
  hmsvP050->Draw("histsame");
  hmsvM000->Draw("histsame");
  hmsvM050->Draw("histsame");
  hmsvM075->Draw("histsame");
  hmsvM100->Draw("histsame");
  C.Print("plotWJetsMTShape.ps");

  //plot ratios
  hmsvP050->Divide(hmsvP075);
  hmsvM000->Divide(hmsvP075);
  hmsvM050->Divide(hmsvP075);
  hmsvM075->Divide(hmsvP075);
  hmsvM100->Divide(hmsvP075);
  C.Clear();
  hmsvP050->GetYaxis()->SetRangeUser(0.5,1.5);
  hmsvP050->GetYaxis()->SetTitle("ratio");
  hmsvP050->Draw("hist");
  hmsvM000->Draw("histsame");
  hmsvM050->Draw("histsame");
  hmsvM075->Draw("histsame");
  hmsvM100->Draw("histsame");
  TLine line;
  line.DrawLine(0,1,150,1);
  C.Print("plotWJetsMTShape.ps");



  C.Print("plotWJetsMTShape.ps]");
  

}


//////////compute the pile-up reweighting for 2011 WJetsMC
void computePileReweight2011To2012(){
  Sample s2011("WJetsToLNu","/data/benitezj/Samples/TauMuV541June2_TrigEff");
  Sample s2012("ZToTauTau","/data/benitezj/Samples/TauMu2012V5_4_0_NewType1MET4");
  
  TString sel="eventweight*(mupt>20)";

  TH1F*H2012=s2012.getHistoNtpFile("nvtx",30,-.5,29.5,sel);
  H2012->SetName("H2012");H2012->SetTitle("");
  
  TH1F*H2011=s2011.getHistoNtpFile("nvtx",30,-.5,29.5,sel);
  H2011->SetName("H2011");H2011->SetTitle("");
  H2011->Scale(H2012->Integral()/H2011->Integral());

  TCanvas C;
  C.Print("computePileReweight2011To2012.ps[");

  C.Clear();
  H2011->Draw("hist");
  H2012->Draw("histpesame");
  C.Print("computePileReweight2011To2012.ps");

  TH1F*HRatio = (TH1F*) H2012->Clone("HRatio");
  HRatio->Divide(H2011);
  C.Clear();
  HRatio->Draw("histpe");
  C.Print("computePileReweight2011To2012.ps");

  cout<<"(";
  for(Int_t b=1;b<=HRatio->GetNbinsX();b++)
    //cout<<b<<" "<<HRatio->GetBinContent(b)<<endl;
    cout<<"(nvtx=="<<(b-1)<<")*"<<HRatio->GetBinContent(b)<<"+";
  cout<<")"<<endl;

  C.Print("computePileReweight2011To2012.ps]");

}
