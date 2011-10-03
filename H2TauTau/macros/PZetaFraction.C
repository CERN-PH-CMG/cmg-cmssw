

void computePZetaFraction(){

  TFile FWJets("tuples/CMGTauMuClean_NoPZeta/WJetsToLNu_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1/Sample_Histograms.root");
  TH1F* HWJets=(TH1F*)FWJets.Get("WJetsToLNu_pZetaPUPHisto");
    
  TFile FWJetsSS("tuples/CMGTauMuClean_NoPZeta/WJetsToLNu_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1_SS/Sample_Histograms.root");
  TH1F* HWJetsSS=(TH1F*)FWJetsSS.Get("WJetsToLNu_SS_pZetaPUPHisto");
    

  TCanvas C;
  C.Print("computePZetaFraction.ps[");
  C.Clear();
  HWJets->Draw();
  C.Print("computePZetaFraction.ps");
  cout<<HWJets->Integral(1,100)<<" "<<HWJets->Integral(120,180)<<" "<<(float)(HWJets->Integral(120,180))/HWJets->Integral(1,100)<<endl;


  C.Clear();
  HWJetsSS->Draw();
  C.Print("computePZetaFraction.ps");
  cout<<HWJetsSS->Integral(1,100)<<" "<<HWJetsSS->Integral(120,180)<<" "<<(float)(HWJetsSS->Integral(120,180))/HWJetsSS->Integral(1,100)<<endl;

  C.Print("computePZetaFraction.ps]");
}
