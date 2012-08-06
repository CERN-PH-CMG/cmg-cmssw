
void drawHisto(TString Group1, TH1F * H1,TString Group2, TH1F* H2, TString channel,TString category,TString sample){
  //H1->SetTitle("");
  H1->SetTitle(channel+"  "+category+"  "+sample);
  H1->GetXaxis()->SetTitle("mass");
  H1->GetYaxis()->SetRangeUser(0,(H1->GetMaximum()>H2->GetMaximum() ? H1->GetMaximum() : H2->GetMaximum())*1.2);
  H1->SetLineWidth(3);
  H1->SetLineColor(1);
  H1->SetFillColor(0);
  H1->Draw("hist");
  H2->SetLineWidth(3);
  H2->SetLineColor(2);
  H2->SetFillColor(0);
  H2->Draw("histsame");

  TText TXmine;
  TXmine.SetTextColor(1);
  TXmine.SetTextSize(.04);
  TText TXother;
  TXother.SetTextColor(2);
  TXother.SetTextSize(.04);
  TXmine.DrawTextNDC(.55,.845,Group1+" : "+(long)(H1->Integral(0,H1->GetNbinsX()+1)));
  TXother.DrawTextNDC(.55,.81,Group2+" : "+(long)(H2->Integral(0,H2->GetNbinsX()+1)));
 
}


void drawCategory(TCanvas* C,TString Group1, TFile* F1,TString Group2, TFile* F2,TString channel, TString category){

  TH1F* HSignal1 = (TH1F*) F1->Get(channel+"_"+category+"/qqH125");
  TH1F* HSignal2 = (TH1F*) F2->Get(channel+"_"+category+"/qqH125");
  C->Clear();
  drawHisto(Group1,HSignal1,Group2,HSignal2,channel,category,"qqH125");
  C->Print(TString(C->GetName())+".ps");

  TH1F* HZTT1 = (TH1F*) F1->Get(channel+"_"+category+"/ZTT");
  TH1F* HZTT2 = (TH1F*) F2->Get(channel+"_"+category+"/ZTT");
  C->Clear();
  drawHisto(Group1,HZTT1,Group2,HZTT2,channel,category,"ZTT");
  C->Print(TString(C->GetName())+".ps");

  TH1F* HW1 = (TH1F*) F1->Get(channel+"_"+category+"/W");
  TH1F* HW2 = (TH1F*) F2->Get(channel+"_"+category+"/W");
  C->Clear();
  drawHisto(Group1,HW1,Group2,HW2,channel,category,"W");
  C->Print(TString(C->GetName())+".ps");

  TH1F* HQCD1 = (TH1F*) F1->Get(channel+"_"+category+"/QCD");
  TH1F* HQCD2 = (TH1F*) F2->Get(channel+"_"+category+"/QCD");
  C->Clear();
  drawHisto(Group1,HQCD1,Group2,HQCD2,channel,category,"QCD");
  C->Print(TString(C->GetName())+".ps");

  TH1F* HTT1 = (TH1F*) F1->Get(channel+"_"+category+"/TT");
  TH1F* HTT2 = (TH1F*) F2->Get(channel+"_"+category+"/TT");
  C->Clear();
  drawHisto(Group1,HTT1,Group2,HTT2,channel,category,"TT");
  C->Print(TString(C->GetName())+".ps");

  TH1F* HData1 = (TH1F*) F1->Get(channel+"_"+category+"/data_obs");
  TH1F* HData2 = (TH1F*) F2->Get(channel+"_"+category+"/data_obs");
  C->Clear();
  drawHisto(Group1,HData1,Group2,HData2,channel,category,"data_obs");
  C->Print(TString(C->GetName())+".ps");
  

}


void compareDataCard(){

  TString channel="muTau";
  TString year="2011";

  TString Group1="CERN";
  TString Path1="/afs/cern.ch/user/b/benitezj/public/datacards/2011/Aug4";
  TString File1="muTauSM_svfitmass";
  TString Group2="Saclay";
  TString Path2="/afs/cern.ch/user/b/bianchi/public/Roger/datacards2011v3";
  TString File2="muTauSM";

  TFile F1(Path1+"/"+File1+".root","read");
  TFile F2(Path2+"/"+File2+".root","read");
  
  F1.ls();
  F2.ls();

  TString fname=channel+year+"_"+Group1+"_"+Group2+"_DataCardDiff";
  TCanvas C(fname);
  
  C.Print(TString(C.GetName())+".ps[");
  
  drawCategory(&C,Group1,&F1,Group2,&F2,channel,"0jet_low");
  //C.Clear(); C.Print(TString(C->GetName())+".ps");
  drawCategory(&C,Group1,&F1,Group2,&F2,channel,"0jet_high");
  drawCategory(&C,Group1,&F1,Group2,&F2,channel,"boost_low");
  drawCategory(&C,Group1,&F1,Group2,&F2,channel,"boost_high");
  drawCategory(&C,Group1,&F1,Group2,&F2,channel,"vbf");

  C.Print(TString(C.GetName())+".ps]");
  
  gROOT->ProcessLine(".q");
}

