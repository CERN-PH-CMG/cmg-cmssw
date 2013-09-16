

void computePZetaFraction(){

  TFile FWJets("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/Final/WJetsToLNu_Sample_Histograms.root");
  TH1F* HWJets=(TH1F*)FWJets.Get("WJetsToLNu_pZetaHisto_pzeta");
    
  TFile FWJetsSS("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/Final/WJetsToLNu_SS_Sample_Histograms.root");
  TH1F* HWJetsSS=(TH1F*)FWJetsSS.Get("WJetsToLNu_SS_pZetaHisto_pzeta");
    

  TCanvas C;
  C.Print("computePZetaFraction.ps[");
  C.Clear();
  HWJets->Draw();
  C.Print("computePZetaFraction.ps");
  cout<<"Fraction: "<<HWJets->Integral(1,100)<<" "<<HWJets->Integral(120,180)<<" "<<(float)(HWJets->Integral(120,180))/HWJets->Integral(1,100)<<endl;


  C.Clear();
  HWJetsSS->Draw();
  C.Print("computePZetaFraction.ps");
  cout<<"FractionSS: "<<HWJetsSS->Integral(1,100)<<" "<<HWJetsSS->Integral(120,180)<<" "<<(float)(HWJetsSS->Integral(120,180))/HWJetsSS->Integral(1,100)<<endl;

  C.Print("computePZetaFraction.ps]");
}
