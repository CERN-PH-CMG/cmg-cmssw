void compute_Zpt_SF(){

  static const int nsamples=7;

  TFile*finZptDATA = new TFile("/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_5/src/CMGTools/WMass/analysis/JobOutputs/test_ptreweighting1_RochCorr_EffSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root");
  TH1D*hDATA=(TH1D*)finZptDATA->Get("hZ_pt_Sig_eta0p6_80419");
  hDATA->Sumw2();
  hDATA->Rebin();
  hDATA->Scale(1/hDATA->Integral());
  hDATA->Print();
  
  // TString     sample[nsamples]  = { "WJets", "DYJets",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "QCD"};
  // TString fWana_str[nsamples] = {
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJets_10wGen_ZanaAndCounts_TightAlone/WJets/VertexAnalyzer/pileup.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJets_10wGen_ZanaAndCounts_TightAlone/DYJets/VertexAnalyzer/pileup.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TTJets_10wGen_ZanaAndCounts_TightAlone/TTJets/VertexAnalyzer/pileup.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen_ZanaAndCounts_TightAlone/ZZJetsTo2L2Nu/VertexAnalyzer/pileup.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen_ZanaAndCounts_TightAlone/WWJetsTo2L2Nu/VertexAnalyzer/pileup.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen_ZanaAndCounts_TightAlone/WZJetsTo3LNu/VertexAnalyzer/pileup.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_QCD_10wGenCounts/QCD15/VertexAnalyzer/pileup.root"
  // };  
  TString fWana_str = "";

  // for(int i=0; i<nsamples; i++){
  
    // TFile*finZptMC = new TFile(Form("%s",fWana_str[i].Data()));
    TFile*finZptMC = new TFile("/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_5/src/CMGTools/WMass/analysis/JobOutputs/test_ptreweighting1_RochCorr_EffSFCorr_PileupSFCorr/output_DYJetsSig/ZanalysisOnDATA.root");
    finZptMC->Print();
    TH1D*hMC=(TH1D*)finZptMC->Get("hZ_pt_Sig_eta0p6_80419");
    hMC->Sumw2();
    hMC->Rebin();
    hMC->Scale(1/hMC->Integral());
    hMC->Print();
    
    TH1D hratio = (*hDATA)/(*hMC);
  
    TFile*foutZptMC = new TFile(Form("Zpt_reweighting.root"),"RECREATE");
    foutZptMC->cd();
    hratio.SetName("hZ_pt_Sig_eta0p6");
    hratio.SetTitle("hZ_pt_Sig_eta0p6");
    hratio.Write();
    foutZptMC->Write();
    foutZptMC->Close();
  // }

}
