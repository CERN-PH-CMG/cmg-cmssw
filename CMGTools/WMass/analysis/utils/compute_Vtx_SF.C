void compute_Vtx_SF(){

  static const int nsamples=7;

  TFile*finVtxDATA = new TFile("/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/data_Run2011AB_Nov8Nov19ReReco_v1_ZanaAndCounts_TightAlone/data_Run2011AB_Nov8Nov19ReReco_v1/VertexAnalyzer/pileup.root");
  TH1D*hDATA=(TH1D*)finVtxDATA->Get("pileup");
  hDATA->Scale(1/hDATA->Integral());
  hDATA->Print();
  
  TString     sample[nsamples]  = { "WJets", "DYJets",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "QCD"};
  TString fWana_str[nsamples] = {
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJets_10wGen_ZanaAndCounts_TightAlone/WJets/VertexAnalyzer/pileup.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJets_10wGen_ZanaAndCounts_TightAlone/DYJets/VertexAnalyzer/pileup.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TTJets_10wGen_ZanaAndCounts_TightAlone/TTJets/VertexAnalyzer/pileup.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen_ZanaAndCounts_TightAlone/ZZJetsTo2L2Nu/VertexAnalyzer/pileup.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen_ZanaAndCounts_TightAlone/WWJetsTo2L2Nu/VertexAnalyzer/pileup.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen_ZanaAndCounts_TightAlone/WZJetsTo3LNu/VertexAnalyzer/pileup.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_QCD_10wGenCounts/QCD15/VertexAnalyzer/pileup.root"
  };  

  for(int i=0; i<nsamples; i++){
  
    TFile*finVtxMC = new TFile(Form("%s",fWana_str[i].Data()));
    finVtxMC->Print();
    TH1D*hMC=(TH1D*)finVtxMC->Get("pileup");
    hMC->Scale(1/hMC->Integral());
    hMC->Print();
    
    TH1D hratio = (*hDATA)/(*hMC);
  
    TFile*foutVtxMC = new TFile(Form("pileup_reweighting_%s.root",sample[i].Data()),"RECREATE");
    foutVtxMC->cd();
    hratio.Write();
    foutVtxMC->Write();
    foutVtxMC->Close();
  }

}
