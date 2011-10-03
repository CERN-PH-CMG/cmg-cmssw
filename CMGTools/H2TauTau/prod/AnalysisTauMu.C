// #include "../interface/BaseAnalysis.h"
// #include "../interface/DiTauAnalysis.h"
// #include "../interface/Sample.h"

//int Analysis()
{
  
  TauMuAnalysis analysis("analysis");
  //analysis.setTruncateEvents(1000);
  analysis.setPrintFreq(100);

  float MCEffCorrFactor = 0.968 * 0.92; 
  
  //TString tag="_NoPZeta";
  TString tag="";
  
  //define samples    
  Sample WJetsToLNu("WJetsToLNu",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/WJetsToLNu_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1"));
  WJetsToLNu.setDataType("MC");
  WJetsToLNu.setColor(kBlue);
  WJetsToLNu.setCrossection(31314);
  WJetsToLNu.setSampleGenEvents(50822395);
  WJetsToLNu.addTrigPath("HLT_IsoMu12_v1");
  WJetsToLNu.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WJetsToLNu);
  
  Sample TTJets("TTJets",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/TTJets_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1"));
  TTJets.setDataType("MC");
  TTJets.setColor(kGreen);
  TTJets.setCrossection(157.5);
  TTJets.setSampleGenEvents(3466251);
  TTJets.addTrigPath("HLT_IsoMu12_v1");
  TTJets.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&TTJets);
 
  Sample DYJetsToLL("DYJetsToLL",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/DYJetsToLL_TuneZ2_M_50_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1"));
  DYJetsToLL.setDataType("MC");
  DYJetsToLL.setColor(kOrange);
  DYJetsToLL.setCrossection(3048);
  DYJetsToLL.setSampleGenEvents(34508918);
  DYJetsToLL.addTrigPath("HLT_IsoMu12_v1");
  DYJetsToLL.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&DYJetsToLL);

  Sample WW("WW",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/WW_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1"));
  WW.setDataType("MC");
  WW.setColor(kMagenta);
  WW.setCrossection(27.83);
  WW.setSampleGenEvents(4052865);
  WW.addTrigPath("HLT_IsoMu12_v1");
  WW.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WW);

  Sample WZ("WZ",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/WZ_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1"));
  WZ.setDataType("MC");
  WZ.setCrossection(10.47);
  WZ.setColor(kMagenta);
  WZ.setSampleGenEvents(4169751);
  WZ.addTrigPath("HLT_IsoMu12_v1");
  WZ.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WZ);

  Sample ZZ("ZZ",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/ZZ_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1"));
  ZZ.setDataType("MC");
  ZZ.setColor(kMagenta);
  ZZ.setCrossection(4.287);
  ZZ.setSampleGenEvents(4112445);
  ZZ.addTrigPath("HLT_IsoMu12_v1");
  ZZ.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZZ);
  
  Sample Higgs("Higgs",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/GluGluToHToTauTau_M_115_7TeV_powheg_pythia6_Summer11_PU_S4_START42_V11_v1"));
  Higgs.setDataType("Signal");
  Higgs.setColor(0);
  Higgs.setLineStyle(2);
  Higgs.setCrossection(18.12*0.0765);
  Higgs.setSampleGenEvents(196002);
  Higgs.addTrigPath("HLT_IsoMu12_v1");
  Higgs.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&Higgs);    

  Sample SingleMuMay("SingleMuMay",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/SingleMu_Run2011A_May10ReReco_v1"));
  SingleMuMay.setDataType("Data");
  SingleMuMay.setSampleLumi(44318268.311/1000000.);
  SingleMuMay.setRunRange(160000,163261);
  SingleMuMay.addTrigPath("HLT_IsoMu12_v1");
  analysis.addSample(&SingleMuMay);

  Sample TauPlusXMay("TauPlusXMay",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/TauPlusX_Run2011A_May10ReReco_v1"));
  TauPlusXMay.setDataType("Data");
  TauPlusXMay.setSampleLumi(155886779.011/1000000.);
  TauPlusXMay.setRunRange(163262,170000);
  //  TauPlusXMay.setSampleLumi(200205047);//
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v1");
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v4");
  analysis.addSample(&TauPlusXMay);

  Sample TauPlusXv4("TauPlusXv4",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/TauPlusX_Run2011A_PromptReco_v4"));
  TauPlusXv4.setDataType("Data");
  TauPlusXv4.setSampleLumi(886790147.468/1000000.);//933101709.896 
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v2");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v4");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v5");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");
  analysis.addSample(&TauPlusXv4);


  //////////////////////////////////////////////////////
  /////////////Same Sign samples////////////////////////
  //////////////////////////////////////////////////////

  Sample WJetsToLNu_SS("WJetsToLNu_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/WJetsToLNu_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1_SS"));
  WJetsToLNu_SS.setDataType("MC_SS");
  WJetsToLNu_SS.setColor(kBlue);
  WJetsToLNu_SS.setCrossection(31314);
  WJetsToLNu_SS.setSampleGenEvents(50822395);
  WJetsToLNu_SS.addTrigPath("HLT_IsoMu12_v1");
  WJetsToLNu_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WJetsToLNu_SS);
  
  Sample TTJets_SS("TTJets_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/TTJets_TuneZ2_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1_SS"));
  TTJets_SS.setDataType("MC_SS");
  TTJets_SS.setColor(kGreen);
  TTJets_SS.setCrossection(157.5);
  TTJets_SS.setSampleGenEvents(3466251);
  TTJets_SS.addTrigPath("HLT_IsoMu12_v1");
  TTJets_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&TTJets_SS);

  Sample DYJetsToLL_SS("DYJetsToLL_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/DYJetsToLL_TuneZ2_M_50_7TeV_madgraph_tauola_Summer11_PU_S4_START42_V11_v1_SS"));
  DYJetsToLL_SS.setDataType("MC_SS");
  DYJetsToLL_SS.setColor(kOrange);
  DYJetsToLL_SS.setCrossection(3048);
  DYJetsToLL_SS.setSampleGenEvents(34508918);
  DYJetsToLL_SS.addTrigPath("HLT_IsoMu12_v1");
  DYJetsToLL_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&DYJetsToLL_SS);
    
  Sample WW_SS("WW_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/WW_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1_SS"));
  WW_SS.setDataType("MC_SS");
  WW_SS.setColor(kMagenta);
  WW_SS.setCrossection(27.83);
  WW_SS.setSampleGenEvents(4052865);
  WW_SS.addTrigPath("HLT_IsoMu12_v1");
  WW_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WW_SS);

  Sample WZ_SS("WZ_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/WZ_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1_SS"));
  WZ_SS.setDataType("MC_SS");
  WZ_SS.setColor(kMagenta);
  WZ_SS.setCrossection(10.47);
  WZ_SS.setSampleGenEvents(4169751);
  WZ_SS.addTrigPath("HLT_IsoMu12_v1");
  WZ_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WZ_SS);

  Sample ZZ_SS("ZZ_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/ZZ_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S4_START42_V11_v1_SS"));
  ZZ_SS.setDataType("MC_SS");
  ZZ_SS.setColor(kMagenta);
  ZZ_SS.setCrossection(4.287);
  ZZ_SS.setSampleGenEvents(4112445);
  ZZ_SS.addTrigPath("HLT_IsoMu12_v1");
  ZZ_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZZ_SS);
  

  Sample SingleMuMay_SS("SingleMuMay_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/SingleMu_Run2011A_May10ReReco_v1_SS"));
  SingleMuMay_SS.setDataType("Data_SS");
  SingleMuMay_SS.setColor(kViolet-5);
  SingleMuMay_SS.setSampleLumi(44318268.311/1000000.);
  SingleMuMay_SS.setRunRange(160000,163261);
  SingleMuMay_SS.addTrigPath("HLT_IsoMu12_v1");
  analysis.addSample(&SingleMuMay_SS);

  Sample TauPlusXMay_SS("TauPlusXMay_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/TauPlusX_Run2011A_May10ReReco_v1_SS"));
  TauPlusXMay_SS.setDataType("Data_SS");
  TauPlusXMay_SS.setColor(kViolet-5);
  TauPlusXMay_SS.setSampleLumi(155886779.011/1000000.);
  TauPlusXMay_SS.setRunRange(163262,170000);
  //TauPlusXMay_SS.setSampleLumi(200205047);
  TauPlusXMay_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v1");
  TauPlusXMay_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  TauPlusXMay_SS.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v4");
  analysis.addSample(&TauPlusXMay_SS);

  Sample TauPlusXv4_SS("TauPlusXv4_SS",(const char*)(TString("tuples/CMGTauMuClean")+tag+"/TauPlusX_Run2011A_PromptReco_v4_SS"));
  TauPlusXv4_SS.setDataType("Data_SS");
  TauPlusXv4_SS.setColor(kViolet-5);
  TauPlusXv4_SS.setSampleLumi(886790147.468/1000000.);
  TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v2");
  TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v4");
  TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v5");
  TauPlusXv4_SS.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");
  analysis.addSample(&TauPlusXv4_SS);



  ///////////initialize
  //analysis.setInputTag("cmgTauMuSel");
  analysis.setInputTag("cmgTauMuSelClean");
  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}
  
//   /////////////////create histos
//   analysis.createHistos("Higgs");
//   analysis.createHistos("DYJetsToLL");
//   analysis.createHistos("WJetsToLNu");
//   analysis.createHistos("TTJets");
//   analysis.createHistos("WW");
//   analysis.createHistos("WZ");
//   analysis.createHistos("ZZ");
//   analysis.createHistos("SingleMuMay");
//   analysis.createHistos("TauPlusXMay");
//   analysis.createHistos("TauPlusXv4");
  
//   analysis.createHistos("DYJetsToLL_SS");
//   analysis.createHistos("WJetsToLNu_SS");
//   analysis.createHistos("TTJets_SS");
//   analysis.createHistos("WW_SS");
//   analysis.createHistos("WZ_SS");
//   analysis.createHistos("ZZ_SS");
//   analysis.createHistos("SingleMuMay_SS");
//   analysis.createHistos("TauPlusXMay_SS");
//   analysis.createHistos("TauPlusXv4_SS");

  
  ////Make plots
  //0 index reserved for QCD 
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
  //    if(!analysis.plotVisMass()){cout<<" could not createTotalMass"<<endl;return 0;}  

  
//  Float_t c[4]={.5,.75,.42,.81};
//  analysis.plotDistribution("nVertexHisto",0," # of reco. vertices ","",c);
//   analysis.plotDistribution("nVertexPUPHisto",1," # of reco. vertices ","",c);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,5,100000};
//   analysis.plotDistribution("muPtPUPHisto",5," #mu p_{T}   (GeV)","Events / 5 GeV",c,r,1);

//    Float_t c[4]={.5,.75,.42,.81};
//    Float_t r[4]={0,200,5,100000};
//    analysis.plotDistribution("tauPtPUPHisto",5," #tau p_{T}   (GeV)","Events / 5 GeV",c,r,1);

//   Float_t c[4]={.5,.75,.42,.81};
//   analysis.plotDistribution("tauIsoPUPHisto",2," I^{#tau}_{abs}(#rho)   (GeV)","Events / 0.1 GeV",c,0);

  //Float_t c[4]={.5,.75,.42,.81};
  //analysis.plotDistribution("metPUPHisto",2," MET   (GeV)","Events / 2 GeV",c);
  
//   Float_t c[4]={.15,.45,.42,.81};
//   Float_t r[4]={-140,40,0,4000};
//   analysis.plotDistribution("pZetaPUPHisto",6,"P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)","Events / 6 GeV",c,r);
  
    Float_t c[4]={.5,.75,.42,.81};
    Float_t r[4]={0,400,0,5500};
    analysis.plotDistribution("diTauMassPUPHisto",10,"m_{vis}   (GeV)","Events / 10 GeV",c,r);

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

