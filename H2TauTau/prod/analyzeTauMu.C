{
  
  TauMuAnalysis analysis("analysis");
  //analysis.setTruncateEvents(10);
  analysis.setPrintFreq(500);
  analysis.setInputTag("cmgTauMuSelClean");
  analysis.calcSVFit(0);
  analysis.setQCDOStoSSRatio(1.06);

  float MCEffCorrFactor = 0.968 * 0.92; 

  TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/21fbCat";

  analysis.setOutputPath(path);

  /////////////////////////
  //define samples    
  /////////////////////////
  Sample WJetsToLNu("WJetsToLNu","/data/benitezj/RootFiles/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/cmgTauMu");
  WJetsToLNu.setDataType("MC");
  WJetsToLNu.setRecoilCorr("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfits/recoilfit_wjets_njet.root");
  WJetsToLNu.setColor(kRed+2);
  WJetsToLNu.setLineColor(WJetsToLNu.getColor());
  WJetsToLNu.setPlotLabel("W");
  WJetsToLNu.setLegendOption("f");
  WJetsToLNu.setCrossection(31314);
  WJetsToLNu.setSampleGenEvents(53227112); //  53655290 = PFAOD integrity, 81352581 = "DBS"
  WJetsToLNu.addTrigPath("HLT_IsoMu12_v1");
  WJetsToLNu.setEffCorrFactor(MCEffCorrFactor);
  WJetsToLNu.setApplyTauRateWeight(0);
  analysis.addSample(&WJetsToLNu);  
  

  Sample TTJets("TTJets","/data/benitezj/RootFiles/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/cmgTauMu");
  TTJets.setDataType("MC");
  TTJets.setColor(kBlue-8);
  TTJets.setLineColor(TTJets.getColor());
  TTJets.setPlotLabel("t#bar{t}");
  TTJets.setLegendOption("f");
  TTJets.setCrossection(157.5);
  TTJets.setSampleGenEvents(3542770); // 3701947=Enriques PFAOD integrity, 3701947="DBS"
  TTJets.addTrigPath("HLT_IsoMu12_v1");
  TTJets.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&TTJets);
 
  Sample ZToMuMu("ZToMuMu","/data/benitezj/RootFiles/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/cmgTauMu");
  ZToMuMu.setDataType("MC");
  ZToMuMu.setTruthEventType(3);
  ZToMuMu.setRecoilCorr("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfits/recoilfit_zmm42X_njet.root");
  ZToMuMu.setColor(kRed);//kRed+2);
  ZToMuMu.setLineColor(ZToMuMu.getColor());
  ZToMuMu.setPlotLabel("Z#rightarrow#mu^{+}#mu^{-}");
  ZToMuMu.setLegendOption("f");
  ZToMuMu.setCrossection(3048);
  ZToMuMu.setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
  ZToMuMu.addTrigPath("HLT_IsoMu12_v1");
  ZToMuMu.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToMuMu);

  Sample ZToTauTau("ZToTauTau","/data/benitezj/RootFiles/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/cmgTauMu");
  ZToTauTau.setDataType("MC");
  ZToTauTau.setTruthEventType(5);
  ZToTauTau.setRecoilCorr("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfits/recoilfit_zjets_ltau_njet.root");
  ZToTauTau.setColor(kOrange-4);
  ZToTauTau.setLineColor(ZToTauTau.getColor());
  ZToTauTau.setPlotLabel("Z#rightarrow#tau^{+}#tau^{-}");
  ZToTauTau.setLegendOption("f");
  ZToTauTau.setCrossection(3048);
  ZToTauTau.setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
  ZToTauTau.addTrigPath("HLT_IsoMu12_v1");
  ZToTauTau.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToTauTau);

  Sample ZToLJet("ZToLJet","/data/benitezj/RootFiles/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/cmgTauMu");
  ZToLJet.setDataType("MC");
  ZToLJet.setTruthEventType(6);
  ZToLJet.setRecoilCorr("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfits/recoilfit_wjets_njet.root");
  ZToLJet.setColor(kRed+2);
  ZToLJet.setLineColor(ZToLJet.getColor());
  ZToLJet.setPlotLabel("Z(l+jet)");
  ZToLJet.setLegendOption("f");
  ZToLJet.setCrossection(3048);
  ZToLJet.setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
  ZToLJet.addTrigPath("HLT_IsoMu12_v1");
  ZToLJet.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToLJet);



  //   Sample WW("WW","/data/benitezj/RootFiles/CMGTauMuClean")+tag+"/"));
  //   WW.setDataType("MC");
  //   WW.setColor(kMagenta);
  //   WW.setCrossection(27.83);
  //   WW.setSampleGenEvents(4052865);
  //   WW.addTrigPath("HLT_IsoMu12_v1");
  //   WW.setEffCorrFactor(MCEffCorrFactor);
  //   analysis.addSample(&WW);

  //   Sample WZ("WZ","/data/benitezj/RootFiles/CMGTauMuClean")+tag+"/"));
  //   WZ.setDataType("MC");
  //   WZ.setCrossection(10.47);
  //   WZ.setColor(kMagenta);
  //   WZ.setSampleGenEvents(4169751);
  //   WZ.addTrigPath("HLT_IsoMu12_v1");
  //   WZ.setEffCorrFactor(MCEffCorrFactor);
  //   analysis.addSample(&WZ);

  //   Sample ZZ("ZZ","/data/benitezj/RootFiles/CMGTauMuClean")+tag+"/"));
  //   ZZ.setDataType("MC");
  //   ZZ.setColor(kMagenta);
  //   ZZ.setCrossection(4.287);
  //   ZZ.setSampleGenEvents(4112445);
  //   ZZ.addTrigPath("HLT_IsoMu12_v1");
  //   ZZ.setEffCorrFactor(MCEffCorrFactor);
  //   analysis.addSample(&ZZ);
  
  Sample Higgs("Higgs","/data/benitezj/RootFiles/GluGluToHToTauTau_M-115_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/TauMu");
  Higgs.setDataType("MC");
  Higgs.setColor(0);
  Higgs.setLineColor(1);
  Higgs.setLineStyle(2);
  Higgs.setPlotLabel("5 x SM Higgs(115)");
  Higgs.setLegendOption("L");
  Higgs.setCrossection(18.12*0.0765);
  Higgs.setRecoilCorr("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfits/recoilfit_zjets_ltau_njet.root");
  Higgs.setSampleGenEvents(196002);
  Higgs.addTrigPath("HLT_IsoMu12_v1");
  Higgs.setEffCorrFactor(MCEffCorrFactor*5);
  analysis.addSample(&Higgs);    

//   Sample Higgs4("Higgs4","/data/benitezj/RootFiles/GluGluToHToTauTau_M-115_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2_/PAT_CMG_V2_4_0/TauMu");
//   Higgs4.setDataType("MC");
//   Higgs4.setColor(0);
//   Higgs4.setLineStyle(2);
//   Higgs4.setPlotLabel("Higgs(115)");
//   Higgs4.setCrossection(18.12*0.0765);
//   Higgs4.setSampleGenEvents(196002);
//   Higgs4.addTrigPath("HLT_IsoMu12_v1");
//   Higgs4.setEffCorrFactor(MCEffCorrFactor);
//   analysis.addSample(&Higgs4);    

//   Sample SingleMuMay("SingleMuMay","/data/benitezj/RootFiles/SingleMu/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_3_0/cmgTauMu");
//   SingleMuMay.setDataType("Data");
//   //SingleMuMay.setSampleLumi(44318268.311/1000000.);
//   SingleMuMay.setSampleLumi(47.022);//47.022 new Cert files 
//   SingleMuMay.setRunRange(160000,163261);
//   SingleMuMay.addTrigPath("HLT_IsoMu12_v1");
//   analysis.addSample(&SingleMuMay);

  Sample TauPlusXMay("TauPlusXMay","/data/benitezj/RootFiles/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_3_0/TauMu");
  TauPlusXMay.setDataType("Data");
  TauPlusXMay.setSampleLumi(168.597);
  TauPlusXMay.setRunRange(163262,170000);
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v4");
  analysis.addSample(&TauPlusXMay);

  Sample TauPlusXv4("TauPlusXv4","/data/benitezj/RootFiles/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_3_0/TauMu");
  TauPlusXv4.setDataType("Data");
  TauPlusXv4.setSampleLumi(929.748);
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v2");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v4");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v5");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");
  analysis.addSample(&TauPlusXv4);

  Sample TauPlusXAug("TauPlusXAug","/data/benitezj/RootFiles/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2/PAT_CMG_V2_3_0/TauMu");
  TauPlusXAug.setDataType("Data");
  TauPlusXAug.setSampleLumi(373.349);
  TauPlusXAug.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8");
  analysis.addSample(&TauPlusXAug);

  Sample TauPlusXv6("TauPlusXv6","/data/benitezj/RootFiles/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_3_0/TauMu");
  TauPlusXv6.setDataType("Data");
  //TauPlusXv6.setRunRange(170000,172802);
  TauPlusXv6.setSampleLumi(658.886); //124.683);
  TauPlusXv6.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8");
  TauPlusXv6.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9");
  analysis.addSample(&TauPlusXv6);

  //////////////////////////////////////////////////////
  /////////////Same Sign samples////////////////////////
  //////////////////////////////////////////////////////


  Sample WJetsToLNu_SS("WJetsToLNu_SS",(const char*)(TString(WJetsToLNu.GetTitle())+"_SS"));
  WJetsToLNu_SS.setDataType("MC_SS");
  WJetsToLNu_SS.setRecoilCorr(WJetsToLNu.getRecoilCorrProcessFile());
  WJetsToLNu_SS.setColor(WJetsToLNu.getColor());
  WJetsToLNu_SS.setLineColor(WJetsToLNu.getColor());
  WJetsToLNu_SS.setCrossection(WJetsToLNu.getCrossection());
  WJetsToLNu_SS.setSampleGenEvents(WJetsToLNu.getSampleGenEvents());
  WJetsToLNu_SS.addTrigPaths(WJetsToLNu.getTrigPaths());
  WJetsToLNu_SS.setEffCorrFactor(MCEffCorrFactor);
  WJetsToLNu_SS.setApplyTauRateWeight(WJetsToLNu.getApplyTauRateWeight());
  analysis.addSample(&WJetsToLNu_SS);
  
  Sample TTJets_SS("TTJets_SS",(const char*)(TString(TTJets.GetTitle())+"_SS"));
  TTJets_SS.setDataType("MC_SS");
  TTJets_SS.setColor(TTJets.getColor());
  TTJets_SS.setLineColor(TTJets.getColor());
  TTJets_SS.setLegendOption(TTJets.getLegendOption());
  TTJets_SS.setCrossection(TTJets.getCrossection());
  TTJets_SS.setSampleGenEvents(TTJets.getSampleGenEvents());
  TTJets_SS.addTrigPaths(TTJets.getTrigPaths());
  TTJets_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&TTJets_SS);

  Sample ZToMuMu_SS("ZToMuMu_SS",(const char*)(TString(ZToMuMu.GetTitle())+"_SS"));
  ZToMuMu_SS.setDataType("MC_SS");
  ZToMuMu_SS.setTruthEventType(ZToMuMu.getTruthEventType());
  ZToMuMu_SS.setRecoilCorr(ZToMuMu.getRecoilCorrProcessFile());
  ZToMuMu_SS.setColor(ZToMuMu.getColor());
  ZToMuMu_SS.setLineColor(ZToMuMu.getColor());
  ZToMuMu_SS.setLegendOption(ZToMuMu.getLegendOption());
  ZToMuMu_SS.setCrossection(ZToMuMu.getCrossection());
  ZToMuMu_SS.setSampleGenEvents(ZToMuMu.getSampleGenEvents());
  ZToMuMu_SS.addTrigPaths(ZToMuMu.getTrigPaths());
  ZToMuMu_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToMuMu_SS);

  Sample ZToTauTau_SS("ZToTauTau_SS",(const char*)(TString(ZToTauTau.GetTitle())+"_SS"));
  ZToTauTau_SS.setDataType("MC_SS");
  ZToTauTau_SS.setTruthEventType(ZToTauTau.getTruthEventType());
  ZToTauTau_SS.setRecoilCorr(ZToTauTau.getRecoilCorrProcessFile());
  ZToTauTau_SS.setColor(ZToTauTau.getColor());
  ZToTauTau_SS.setLineColor(ZToTauTau.getColor());
  ZToTauTau_SS.setLegendOption(ZToTauTau.getLegendOption());
  ZToTauTau_SS.setCrossection(ZToTauTau.getCrossection());
  ZToTauTau_SS.setSampleGenEvents(ZToTauTau.getSampleGenEvents());
  ZToTauTau_SS.addTrigPaths(ZToTauTau.getTrigPaths());
  ZToTauTau_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToTauTau_SS);

  Sample ZToLJet_SS("ZToLJet_SS",(const char*)(TString(ZToLJet.GetTitle())+"_SS"));
  ZToLJet_SS.setDataType("MC_SS");
  ZToLJet_SS.setTruthEventType(ZToLJet.getTruthEventType());
  ZToLJet_SS.setRecoilCorr(ZToLJet.getRecoilCorrProcessFile());
  ZToLJet_SS.setColor(ZToLJet.getColor());
  ZToLJet_SS.setLineColor(ZToLJet.getColor());
  ZToLJet_SS.setLegendOption(ZToLJet.getLegendOption());
  ZToLJet_SS.setCrossection(ZToLJet.getCrossection());
  ZToLJet_SS.setSampleGenEvents(ZToLJet.getSampleGenEvents());
  ZToLJet_SS.addTrigPaths(ZToLJet.getTrigPaths());
  ZToLJet_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToLJet_SS);

    
  //   Sample WW_SS("WW_SS","/data/benitezj/RootFiles/CMGTauMuClean")+tag+"/"));
  //   WW_SS.setDataType("MC_SS"));
  //   WW_SS.setColor(kMagenta);
  //   WW_SS.setCrossection(27.83);
  //   WW_SS.setSampleGenEvents(4052865);
  //   WW_SS.addTrigPath("HLT_IsoMu12_v1");
  //   WW_SS.setEffCorrFactor(MCEffCorrFactor);
  //   analysis.addSample(&WW_SS);

  //   Sample WZ_SS("WZ_SS","/data/benitezj/RootFiles/CMGTauMuClean")+tag+"/"));
  //   WZ_SS.setDataType("MC_SS");
  //   WZ_SS.setColor(kMagenta);
  //   WZ_SS.setCrossection(10.47);
  //   WZ_SS.setSampleGenEvents(4169751);
  //   WZ_SS.addTrigPath("HLT_IsoMu12_v1");
  //   WZ_SS.setEffCorrFactor(MCEffCorrFactor);
  //   analysis.addSample(&WZ_SS);

  //   Sample ZZ_SS("ZZ_SS","/data/benitezj/RootFiles/CMGTauMuClean")+tag+"/"));
  //   ZZ_SS.setDataType("MC_SS");
  //   ZZ_SS.setColor(kMagenta);
  //   ZZ_SS.setCrossection(4.287);
  //   ZZ_SS.setSampleGenEvents(4112445);
  //   ZZ_SS.addTrigPath("HLT_IsoMu12_v1");
  //   ZZ_SS.setEffCorrFactor(MCEffCorrFactor);
  //   analysis.addSample(&ZZ_SS);
  

  int QCDColor=kMagenta-10;

//   Sample SingleMuMay_SS("SingleMuMay_SS",(const char*)(TString(SingleMuMay.GetTitle())+"_SS"));
//   SingleMuMay_SS.setDataType("Data_SS");
//   SingleMuMay_SS.setColor(QCDColor);//kViolet-5
//   SingleMuMay_SS.setSampleLumi(SingleMuMay.getLumi());
//   SingleMuMay_SS.setRunRange(SingleMuMay.getFirstRun(),SingleMuMay.getLastRun());
//   SingleMuMay_SS.addTrigPaths(SingleMuMay.getTrigPaths());
//   analysis.addSample(&SingleMuMay_SS);

  Sample TauPlusXMay_SS("TauPlusXMay_SS",(const char*)(TString(TauPlusXMay.GetTitle())+"_SS"));
  TauPlusXMay_SS.setDataType("Data_SS");
  TauPlusXMay_SS.setColor(QCDColor);
  TauPlusXMay_SS.setSampleLumi(TauPlusXMay.getLumi());
  TauPlusXMay_SS.setRunRange(TauPlusXMay.getFirstRun(),TauPlusXMay.getLastRun());
  TauPlusXMay_SS.addTrigPaths(TauPlusXMay.getTrigPaths());
  analysis.addSample(&TauPlusXMay_SS);

  Sample TauPlusXv4_SS("TauPlusXv4_SS",(const char*)(TString(TauPlusXv4.GetTitle())+"_SS"));
  TauPlusXv4_SS.setDataType("Data_SS");
  TauPlusXv4_SS.setColor(QCDColor);
  TauPlusXv4_SS.setSampleLumi(TauPlusXv4.getLumi());
  TauPlusXv4_SS.addTrigPaths(TauPlusXv4.getTrigPaths());
  analysis.addSample(&TauPlusXv4_SS);

  Sample TauPlusXAug_SS("TauPlusXAug_SS",(const char*)(TString(TauPlusXAug.GetTitle())+"_SS"));
  TauPlusXAug_SS.setDataType("Data_SS");
  TauPlusXAug_SS.setColor(QCDColor);
  TauPlusXAug_SS.setSampleLumi(TauPlusXAug.getLumi());
  TauPlusXAug_SS.addTrigPaths(TauPlusXAug.getTrigPaths());
  analysis.addSample(&TauPlusXAug_SS);

  Sample TauPlusXv6_SS("TauPlusXv6_SS",(const char*)(TString(TauPlusXv6.GetTitle())+"_SS"));
  TauPlusXv6_SS.setDataType("Data_SS");
  TauPlusXv6_SS.setColor(QCDColor);
  TauPlusXv6_SS.setRunRange(TauPlusXv6.getFirstRun(),TauPlusXv6.getLastRun());
  TauPlusXv6_SS.setSampleLumi(TauPlusXv6.getLumi());
  TauPlusXv6_SS.addTrigPaths(TauPlusXv6.getTrigPaths());
  analysis.addSample(&TauPlusXv6_SS);

  ///////////initialize
  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}
  
  
//   /////////////////create histos
// //  analysis.createHistos("Higgs4");
//  analysis.createHistos("Higgs");
//   analysis.createHistos("ZToMuMu");
//   analysis.createHistos("ZToTauTau");
//   analysis.createHistos("ZToLJet");
//   analysis.createHistos("WJetsToLNu");
//   analysis.createHistos("TTJets");
//   //   analysis.createHistos("WW");
//   //   analysis.createHistos("WZ");
//   //   analysis.createHistos("ZZ");
//   //   analysis.createHistos("SingleMuMay");
//   analysis.createHistos("TauPlusXMay");
//   analysis.createHistos("TauPlusXv4");
//   analysis.createHistos("TauPlusXAug");
//   analysis.createHistos("TauPlusXv6");
  
//   analysis.createHistos("ZToMuMu_SS");
//   analysis.createHistos("ZToTauTau_SS");
//   analysis.createHistos("ZToLJet_SS");
//   analysis.createHistos("WJetsToLNu_SS");
//   analysis.createHistos("TTJets_SS");
//   //   analysis.createHistos("WW_SS");
//   //   analysis.createHistos("WZ_SS");
//   //   analysis.createHistos("ZZ_SS");
//   //   analysis.createHistos("SingleMuMay_SS");
//   analysis.createHistos("TauPlusXMay_SS");
//   analysis.createHistos("TauPlusXv4_SS");
//   analysis.createHistos("TauPlusXAug_SS");
//   analysis.createHistos("TauPlusXv6_SS");
  
  
  
  //can only do one plot at a time for now 
  WJetsToLNu.setPlotOrder(1);   WJetsToLNu_SS.setPlotOrder(1);
  ZToLJet.setPlotOrder(2);   ZToLJet_SS.setPlotOrder(2);
  TTJets.setPlotOrder(3);       TTJets_SS.setPlotOrder(3);
  ZToMuMu.setPlotOrder(4);   ZToMuMu_SS.setPlotOrder(4);
  ZToTauTau.setPlotOrder(5);   ZToTauTau_SS.setPlotOrder(5);
  Higgs.setPlotOrder(6);  

  


//   Float_t c[4]={0,0,0,0};
//   Float_t r[4]={160000,175000,0,400};
//   analysis.plot("runNumberHisto",0,"","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={-0.5,24.5,0,4000};
//   analysis.plot("nVertexHisto",1," # of reco. vertices ","",c,r);

//     Float_t c[4]={.5,.75,.42,.81};
//     Float_t r[4]={0,100,0,1500};
//     analysis.plot("muPtHisto",1," #mu p_{T}   (GeV)","Events / 1 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,0.5,0,6000};
//   analysis.plot("muIsoHisto_muiso",1," muon isolation  ","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,100,0,1800};
//   analysis.plot("tauPtHisto",1," #tau p_{T}   (GeV)","Events / 1 GeV",c,r);

  //   Float_t c[4]={.5,.75,.42,.81};
  //   analysis.plot("tauIsoHisto",2," tau isolation  ","",c,0);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,150,0,5000};
//   analysis.plot("metHisto",4," MET   (GeV)","Events / 4 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,150,0,5000};
//   analysis.plot("metHisto_massT",4," MET   (GeV)","Events / 4 GeV",c,r);
  
//   Float_t c[4]={.5,.75,.48,.83};
//   Float_t r[4]={-3.2,3.2,0,1600};
//   analysis.plot("metphiHisto_massT",1," MET #phi ","",c,r);

//    Float_t c[4]={.15,.45,.42,.81};
//    Float_t r[4]={-140,40,0,6000};
//    analysis.plot("pZetaHisto_pzeta",6,"P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)","Events / 6 GeV",c,r);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,0,2500};
//   analysis.plot("transverseMassHisto_massT",2,"m_{T}   (GeV)","Events / 2 GeV",c,r);


//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={-0.5,5.5,0,35000};
//   analysis.plot("njetHisto_massT",1,"# of jets","",c,r);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,0,3500};
// //   Float_t c[4]={.5,.75,.42,.81};
// //   Float_t r[4]={0,350,5,1e7};
//   analysis.plot("diTauMassHisto",4,"m_{vis}   (GeV)","Events / 4 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,300,0,4000};
//   analysis.plot("diTauMassSVFitHisto",8," mass   (GeV)","Events / 8 GeV",c,r);

//    Float_t c[4]={.5,.75,.42,.81};
//    Float_t r[4]={0,400,0,3000};
//    analysis.plot("svFitCov00Histo",10,"matrix element 00","Events",c,r);


  Float_t c[4]={.5,.75,.42,.81};
  Float_t r[4]={0,200,0,30};
  analysis.plot("diTauMassVBFHisto",20,"m_{vis}   (GeV)","Events / 20 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={300,1000,0,30};
//   analysis.plot("diJetMassVBFHisto",100,"m_{jj} (GeV)","Events / 100 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={3,10,0,60};
//   analysis.plot("diJetDeltaEtaVBFHisto",10,"|#Delta#eta_{jj}|","Events / 1",c,r,0);

    



  //  return 1;
  gROOT->ProcessLine(".q");
}


  ///triggers run on Higgs MC
// HLT_IsoMu12_LooseIsoPFTau10_v2  0
// HLT_IsoMu12_v1  1
// HLT_IsoMu15_v5  1
// HLT_IsoMu17_CentralJet40_BTagIP_v2      0
// HLT_IsoMu17_v5  1
// HLT_IsoMu24_v1  1
// HLT_IsoMu30_v1  1

