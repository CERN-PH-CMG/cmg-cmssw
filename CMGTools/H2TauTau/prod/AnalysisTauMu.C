// #include "../interface/BaseAnalysis.h"
// #include "../interface/DiTauAnalysis.h"
// #include "../interface/Sample.h"

//int Analysis()
{
  
  TauMuAnalysis analysis("analysis");
  //analysis.setTruncateEvents(1000);
  analysis.setPrintFreq(100);


  //define samples
  Sample DYJetsToLL("DYJetsToLL","tuples/CMGTauMuClean/DYJetsToLL_TuneZ2_M_50_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1");
  DYJetsToLL.setDataType("MC");
  DYJetsToLL.setColor(kOrange);
  DYJetsToLL.setCrossection(2475.0);
  DYJetsToLL.setSampleGenEvents(34471278);
  //  DYJetsToLL.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&DYJetsToLL);
    
  Sample WJetsToLNu("WJetsToLNu","tuples/CMGTauMuClean/WJetsToLNu_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1");
  WJetsToLNu.setDataType("MC");
  WJetsToLNu.setColor(kBlue);
  WJetsToLNu.setCrossection(27770.0);
  WJetsToLNu.setSampleGenEvents(50740770);
  //  WJetsToLNu.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&WJetsToLNu);
  
  Sample TTJets("TTJets","tuples/CMGTauMuClean/TTJets_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1");
  TTJets.setDataType("MC");
  TTJets.setColor(kGreen);
  TTJets.setCrossection(94.76);
  TTJets.setSampleGenEvents(3466238);
  //  TTJets.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&TTJets);
 
  Sample WW("WW","tuples/CMGTauMuClean/WW_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1");
  WW.setDataType("MC");
  WW.setColor(kMagenta);
  WW.setCrossection(27.83);
  WW.setSampleGenEvents(4052865);
  //  WW.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&WW);

  Sample WZ("WZ","tuples/CMGTauMuClean/WZ_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1");
  WZ.setDataType("MC");
  WZ.setCrossection(10.47);
  WZ.setColor(kMagenta);
  WZ.setSampleGenEvents(4169751);
  //  WZ.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&WZ);

  Sample ZZ("ZZ","tuples/CMGTauMuClean/ZZ_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1");
  ZZ.setDataType("MC");
  ZZ.setColor(kMagenta);
  ZZ.setCrossection(4.287);
  ZZ.setSampleGenEvents(4112445);
  //  ZZ.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&ZZ);
  
  Sample Higgs("Higgs","tuples/CMGTauMuClean/GluGluToHToTauTau_M_115_7TeV_powheg_pythia6_Summer11_PU_S4_START42_V11_v1");
  Higgs.setDataType("Signal");
  Higgs.setColor(0);
  Higgs.setLineStyle(2);
  Higgs.setCrossection(18.12*0.0765);
  Higgs.setSampleGenEvents(195988);
  //  Higgs.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&Higgs);    

//   Sample SingleMuMay("SingleMuMay","tuples/CMGTauMuClean/SingleMu_May10ReReco_v1");
//   SingleMuMay.setDataType("Data");
//   SingleMuMay.setSampleLumi(6.6+40.8);
//   SingleMuMay.setRunRange(160000,163261);
//   SingleMuMay.addTrigPath("HLT_IsoMu12");
//   analysis.addSample(&SingleMuMay);

  Sample TauPlusXMay("TauPlusXMay","tuples/CMGTauMuClean/TauPlusX_Run2011A_May10ReReco_v1");
  TauPlusXMay.setDataType("Data");
  //TauPlusXMay.setSampleLumi(167.8);
  TauPlusXMay.setSampleLumi(204.0);
//   TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v1");
//   TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
//   TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v4");
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v*");
  analysis.addSample(&TauPlusXMay);

  Sample TauPlusXv4("TauPlusXv4","tuples/CMGTauMuClean/TauPlusX_Run2011A_PromptReco_v4");
  TauPlusXv4.setDataType("Data");
  TauPlusXv4.setSampleLumi(933101709.896/1000000.);
//   TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v2");
//   TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v4");
//   TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v5");
//   TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v*");
  analysis.addSample(&TauPlusXv4);

//   Sample TauPlusXv5("TauPlusXv5","tuples/CMGTauMuClean/TauPlusX_Run2011A_PromptReco_v5");
//   TauPlusXv5.setDataType("Data");
//   TauPlusXv5.setSampleLumi(54766729.565/1000000.);
//   TauPlusXv5.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8");
//   analysis.addSample(&TauPlusXv5);

  /////////////Same Sign samples////////////////////////
  Sample DYJetsToLL_SS("DYJetsToLL_SS","tuples/CMGTauMuClean/DYJetsToLL_TuneZ2_M_50_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1_SS");
  DYJetsToLL_SS.setDataType("MC_SS");
  DYJetsToLL_SS.setCrossection(2475.0);
  DYJetsToLL_SS.setSampleGenEvents(34471278);
  //  DYJetsToLL_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&DYJetsToLL_SS);
    
  Sample WJetsToLNu_SS("WJetsToLNu_SS","tuples/CMGTauMuClean/WJetsToLNu_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1_SS");
  WJetsToLNu_SS.setDataType("MC_SS");
  WJetsToLNu_SS.setCrossection(27770.0);
  WJetsToLNu_SS.setSampleGenEvents(50740770);
  //  WJetsToLNu_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&WJetsToLNu_SS);
  
  Sample TTJets_SS("TTJets_SS","tuples/CMGTauMuClean/TTJets_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1_SS");
  TTJets_SS.setDataType("MC_SS");
  TTJets_SS.setCrossection(94.76);
  TTJets_SS.setSampleGenEvents(3466238);
  //  TTJets_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&TTJets_SS);

  Sample WW_SS("WW_SS","tuples/CMGTauMuClean/WW_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1_SS");
  WW_SS.setDataType("MC_SS");
  WW_SS.setCrossection(27.83);
  WW_SS.setSampleGenEvents(4052865);
  //  WW_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&WW_SS);

  Sample WZ_SS("WZ_SS","tuples/CMGTauMuClean/WZ_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1_SS");
  WZ_SS.setDataType("MC_SS");
  WZ_SS.setCrossection(10.47);
  WZ_SS.setSampleGenEvents(4169751);
  //  WZ_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&WZ_SS);

  Sample ZZ_SS("ZZ_SS","tuples/CMGTauMuClean/ZZ_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1_SS");
  ZZ_SS.setDataType("MC_SS");
  ZZ_SS.setCrossection(4.287);
  ZZ_SS.setSampleGenEvents(4112445);
  //  ZZ_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  analysis.addSample(&ZZ_SS);
  
  Sample TauPlusXMay_SS("TauPlusXMay_SS","tuples/CMGTauMuClean/TauPlusX_Run2011A_May10ReReco_v1_SS");
  TauPlusXMay_SS.setDataType("Data_SS");
  TauPlusXMay_SS.setColor(kViolet-5);
  TauPlusXMay_SS.setSampleLumi(204.0);
//   TauPlusXMay_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v1");
//   TauPlusXMay_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
//   TauPlusXMay_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v4");
  TauPlusXMay_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v*");
  analysis.addSample(&TauPlusXMay_SS);

  Sample TauPlusXv4_SS("TauPlusXv4_SS","tuples/CMGTauMuClean/TauPlusX_Run2011A_PromptReco_v4_SS");
  TauPlusXv4_SS.setDataType("Data_SS");
  TauPlusXv4_SS.setColor(kViolet-5);
  TauPlusXv4_SS.setSampleLumi(933101709.896/1000000.);
//   TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v2");
//   TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v4");
//   TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v5");
//   TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");
  TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v*");
  analysis.addSample(&TauPlusXv4_SS);

//   Sample TauPlusXv5_SS("TauPlusXv5_SS","tuples/CMGTauMuClean/TauPlusX_Run2011A_PromptReco_v5_SS");
//   TauPlusXv5_SS.setDataType("Data_SS");
//   TauPlusXv5_SS.setColor(kViolet-5);
//   TauPlusXv5_SS.setSampleLumi(54766729.565/1000000.);
//   TauPlusXv5_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8");
//  analysis.addSample(&TauPlusXv5_SS);


  ///////////initialize
  //analysis.setInputTag("cmgTauMuSel");
  analysis.setInputTag("cmgTauMuSelClean");
  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}
  
// //   /////////////////create histos
   analysis.createHistos("Higgs");
  analysis.createHistos("DYJetsToLL");
  analysis.createHistos("WJetsToLNu");
  analysis.createHistos("TTJets");
  analysis.createHistos("WW");
  analysis.createHistos("WZ");
  analysis.createHistos("ZZ");
  analysis.createHistos("TauPlusXMay");
  analysis.createHistos("TauPlusXv4");
  //  analysis.createHistos("TauPlusXv5");
  
  analysis.createHistos("DYJetsToLL_SS");
  analysis.createHistos("WJetsToLNu_SS");
  analysis.createHistos("TTJets_SS");
  analysis.createHistos("WW_SS");
  analysis.createHistos("WZ_SS");
  analysis.createHistos("ZZ_SS");
  analysis.createHistos("TauPlusXMay_SS");
  analysis.createHistos("TauPlusXv4_SS");
  //  analysis.createHistos("TauPlusXv5_SS");

  
  ////Make plots
  WJetsToLNu.setPlotOrder(1);
  TTJets.setPlotOrder(2);
  WW.setPlotOrder(3);
  WZ.setPlotOrder(4);
  ZZ.setPlotOrder(5);
  DYJetsToLL.setPlotOrder(6);

  WJetsToLNu_SS.setPlotOrder(1);
  TTJets_SS.setPlotOrder(2);
  WW_SS.setPlotOrder(3);
  WZ_SS.setPlotOrder(4);
  ZZ_SS.setPlotOrder(5);
  DYJetsToLL_SS.setPlotOrder(6);
  
  //can only do one plot at a time for now 
//  if(!analysis.plotVisMass()){cout<<" could not createTotalMass"<<endl;return 0;}  
//  if(!analysis.plotDistribution("metPUPHisto",2)){cout<<" could not plot"<<endl;return 0;}
//  if(!analysis.plotDistribution("pZetaPUPHisto",6)){cout<<" could not plot"<<endl;return 0;}
  Float_t c[4]={.5,.75,.42,.81};
  //  if(!analysis.plotDistribution("diTauMassPUPHisto",10,"m_{vis}   (GeV)","Events / 10 GeV",c)){cout<<" could not plot"<<endl;return 0;}

  //  return 1;
}


  //for mu-tau
  //  triggerPaths_.push_back("HLT_Mu15_v2");
  //  triggerPaths_.push_back("HLT_Mu15_LooseIsoPFTau20_v2");
  //triggerPaths_.push_back("HLT_IsoMu12_v1");
  //triggerPaths_.push_back("HLT_IsoMu12_LooseIsoPFTau10_v2");
  //triggerPaths_.push_back("HLT_IsoMu15");//for run < 163261
  //triggerPaths_.push_back("HLT_IsoMu15_LooseIsoPFTau15_v8");
  //triggerPaths_.push_back("HLT_IsoMu15_LooseIsoPFTau20_v6");
  //  triggerPaths_.push_back("HLT_IsoMu15_TightIsoPFTau20_v6");

  ///triggers run on Higgs MC
// HLT_IsoMu12_LooseIsoPFTau10_v2  0
// HLT_IsoMu12_v1  1
// HLT_IsoMu15_v5  1
// HLT_IsoMu17_CentralJet40_BTagIP_v2      0
// HLT_IsoMu17_v5  1
// HLT_IsoMu24_v1  1
// HLT_IsoMu30_v1  1

//   triggerPaths_.push_back("HLT_IsoMu12_LooseIsoPFTau10_*");
//   triggerPaths_.push_back("HLT_IsoMu15_LooseIsoPFTau15_*");
//   triggerPaths_.push_back("HLT_IsoMu15_LooseIsoPFTau20_*");
//   //  triggerPaths_.push_back("HLT_IsoMu15_LooseIsoPFTau15_v8");


//   //for e-tau                            
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v6");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v2");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v8");
//   triggerPaths_.push_back("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v9");
//   triggerPaths_.push_back("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v1");
//   triggerPaths_.push_back("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2");

  //for e-mu
//   triggerPaths_.push_back("HLT_Mu8_Ele17_CaloIdL_v1");
//   triggerPaths_.push_back("HLT_Mu17_Ele8_CaloIdL_v1");
//   triggerPaths_.push_back("HLT_Mu8_Ele17_CaloIdL_v2");
//   triggerPaths_.push_back("HLT_Mu17_Ele8_CaloIdL_v2");

