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
  TString sel="*(njet==0)";
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
