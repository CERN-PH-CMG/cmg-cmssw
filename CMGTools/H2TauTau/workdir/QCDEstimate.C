
//                Anti-Iso              SSx1.11
// -------------------------------------------------
// njet==0
// QCD Region B 7190.1 +- 115.0
// QCD Region C 6859.0 +- 115.9
// QCD Region D 7756.0 +-  67.2
// QCD Avg:     7461.2 +-  51.9      7271.3 +- 124.6
// njet=1
// QCD Region B 1589.6 +-  59.7
// QCD Region C 1511.4 +-  51.7
// QCD Region D 1778.4 +-  29.8 
// QCD Avg:     1692.7 +-  23.7      1638.8 +-  70.6
// njet=2
// QCD Region B  375.1 +-  32.5
// QCD Region C  316.4 +-  26.5
// QCD Region D  428.0 +-  13.6
// QCD Avg:      401.1 +-  11.3       301.7 +-  31.8
// Boosted
// QCD Region B   11.4 +-   6.7
// QCD Region C   17.4 +-   9.1
// QCD Region D   11.3 +-   1.7
// QCD Avg:       11.5 +-   1.6
// VBF
// QCD Region B   17.5 +-   4.6
// QCD Region C   21.9 +-   3.8
// QCD Region D   24.7 +-   2.3
// QCD Avg:       22.9 +-   1.8 

// Variations between Regions:
// njet=0 : 602/7460 = 0.08
// njet=1 : 181/1690 = 0.11
// njet=2 :  84/400  = 0.20
// Variations between SS
// njet=0 : 200/7270 = 0.03
// njet=1 : 60/1650  = 0.04
// njet=2 : 100/350  = 0.28 


void MakeQCDShape(){

  //Shapes studied in the SS samples

  gROOT->ProcessLine(".L ./tauMuConfig.C");
  TauMuPlotter *analysis =tauMuConfig("analysis");
  analysis->plotvar_="svfitmass";
  analysis->nbins_=30;
  analysis->xmin_=0.;
  analysis->xmax_=300.;

  long njet=2;

  //Anti-Iso QCD 
  analysis->Isocat_=2;
  analysis->MTcat_=1;
  analysis->SMcat_=-1;
  analysis->extrasel_=TString("(njet==")+njet+"&&muiso>0.1&&tauisodisc<2)";
  if(!analysis->scaleSamplesLumi())return 0;
  if(!analysis->correctSamplesInc())return 0;
  TH1F*hDataAIso=analysis->getTotalDataSS();
  hDataAIso->SetName("hDataAIso");
  TH1F*hMCAIso=analysis->getTotalMCSS();
  hMCAIso->SetName("hMCAIso");
  hDataAIso->Add(hMCAIso,-1);
  
  //Anti-Iso QCD with Ratio
  analysis->Isocat_=1;
  analysis->MTcat_=1;
  analysis->SMcat_=-1;
  analysis->extrasel_=TString("(njet==")+njet+")";
  if(!analysis->scaleSamplesLumi())return 0;
  if(!analysis->correctSamplesInc())return 0;
  TH1F*hQCDAIsoR=analysis->getQCDIsoSMSS();
  hQCDAIsoR->SetName("hQCDAIsoR");

  //Iso QCD
  analysis->Isocat_=1;
  analysis->MTcat_=1;
  analysis->SMcat_=-1;
  analysis->extrasel_=TString("(njet==")+njet+")";
  if(!analysis->scaleSamplesLumi())return 0;
  if(!analysis->correctSamplesInc())return 0;
  TH1F*hDataIso=analysis->getTotalDataSS();
  hDataIso->SetName("hDataIso");
  TH1F*hDMCIso=analysis->getTotalMCSS();
  hDMCIso->SetName("hDMCIso");
  hDataIso->Add(hDMCIso,-1);


  TFile F("QCDMassShape.root","recreate");
  hDataAIso->SetName("hQCDAIso");
  hDataAIso->Write();
  hDataIso->SetName("hQCDIso");
  hDataIso->Write();
  hQCDAIsoR->Write();
  F.ls();
  F.Close();
}

void QCDMassShape(){

  TCanvas C;
  C.Print("QCDMassShape.ps[");


  ///njet=0
  
  
  TFile RootFile("QCDMassShape.root","read");
  TH1F* HQCDIso=(TH1F*)RootFile.Get("hQCDIso");
  TH1F* HQCDAIso=(TH1F*)RootFile.Get("hQCDAIso");
  TH1F* HQCDAIsoR=(TH1F*)RootFile.Get("hQCDAIsoR");

  C.Clear();
  HQCDIso->GetYaxis()->SetRangeUser(0,1.5*HQCDIso->GetMaximum());
  HQCDIso->Draw("histpe");
  HQCDAIsoR->SetLineColor(2);
  HQCDAIsoR->Scale(HQCDIso->Integral()/HQCDAIsoR->Integral());
  HQCDAIsoR->Draw("histsame");
  HQCDAIso->Scale(HQCDIso->Integral()/HQCDAIso->Integral());
  HQCDAIso->Draw("histsame");
  C.Print("QCDMassShape.ps");


//   TFile FBoostedRpT("/data/benitezj/Samples/Mar19MuJet/FQCDIsoSM_svfitmass_TauIso-1_MT1_SM1.root","read");
//   TFile FBoostedScale("/data/benitezj/Samples/Mar19MuJet/DiffFileInc_svfitmass1_TauIso2_MT1_SM1.root","read");
//   TH1F* HBoostedRpt=(TH1F*)FBoostedRpT.Get("hQCD");
//   TH1F* HBoostedScale=(TH1F*)FBoostedScale.Get("hDiff");

//   C.Clear();
//   HBoostedRpt->Draw("histpe");
//   HBoostedScale->Scale(HBoostedRpt->Integral()/HBoostedScale->Integral());
//   HBoostedScale->Draw("histsame");
//   C.Print("QCDMassShape.ps");


//   TFile FVBFRpT("/data/benitezj/Samples/Mar19MuJet/FQCDIsoSM_svfitmass_TauIso-1_MT1_SM2.root","read");
//   TFile FVBFScale("/data/benitezj/Samples/Mar19MuJet/DiffFileInc_svfitmass1_TauIso2_MT1_SM2.root","read");
//   TH1F* HVBFRpt=(TH1F*)FVBFRpT.Get("hQCD");
//   TH1F* HVBFScale=(TH1F*)FVBFScale.Get("hDiff");

//   C.Clear();
//   HVBFRpt->Draw("histpe");
//   HVBFScale->Scale(HVBFRpt->Integral()/HVBFScale->Integral());
//   HVBFScale->Draw("histsame");
//   C.Print("QCDMassShape.ps");


  C.Print("QCDMassShape.ps]");  
}
