{
  
  TauMuAnalysis analysis("analysis");
  //analysis.setTruncateEvents(100);
  analysis.setPrintFreq(500);
  analysis.setInputTag("cmgTauMuSelClean");

  float MCEffCorrFactor = 0.968 * 0.92; 


  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/Test"; 
  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/Veto";
  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/NotRecoilCorrected";
  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/Final";
  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/Json";

  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/SVFit";
  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/SVFitErr";

  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/TauRate";
  
  TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/massT";
  //TString path="/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/prod/massTRecoilCorr";

  analysis.setOutputPath(path);

  /////////////////////////
  //define samples    
  /////////////////////////
  Sample WJetsToLNu("WJetsToLNu","/data/benitezj/RootFiles/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/cmgTauMu");
  WJetsToLNu.setDataType("MC");
  WJetsToLNu.setApplyRecoilCorr(1);
  WJetsToLNu.setColor(kRed+2);
  WJetsToLNu.setCrossection(31314);
  WJetsToLNu.setSampleGenEvents(53227112); //  53655290 = PFAOD integrity, 81352581 = "DBS"
  WJetsToLNu.addTrigPath("HLT_IsoMu12_v1");
  WJetsToLNu.setEffCorrFactor(MCEffCorrFactor);
  WJetsToLNu.setApplyTauRateWeight(0);
  analysis.addSample(&WJetsToLNu);  
  
  Sample TTJets("TTJets","/data/benitezj/RootFiles/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/cmgTauMu");
  TTJets.setDataType("MC");
  TTJets.setColor(kBlue-8);
  TTJets.setCrossection(157.5);
  TTJets.setSampleGenEvents(3542770); // 3701947=Enriques PFAOD integrity, 3701947="DBS"
  TTJets.addTrigPath("HLT_IsoMu12_v1");
  TTJets.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&TTJets);
 
  Sample DYJetsToLL("DYJetsToLL","/data/benitezj/RootFiles/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/cmgTauMu");
  DYJetsToLL.setDataType("MC");
  DYJetsToLL.setApplyRecoilCorr(1);
  DYJetsToLL.setColor(kOrange-4);
  DYJetsToLL.setCrossection(3048);
  DYJetsToLL.setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
  DYJetsToLL.addTrigPath("HLT_IsoMu12_v1");
  DYJetsToLL.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&DYJetsToLL);

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
  
  //   Sample Higgs("Higgs","/data/benitezj/RootFiles/CMGTauMuClean")+tag+"/"));
  //   Higgs.setDataType("Signal");
  //   Higgs.setColor(0);
  //   Higgs.setLineStyle(2);
  //   Higgs.setCrossection(18.12*0.0765);
  //   Higgs.setSampleGenEvents(196002);
  //   Higgs.addTrigPath("HLT_IsoMu12_v1");
  //   Higgs.setEffCorrFactor(MCEffCorrFactor);
  //   analysis.addSample(&Higgs);    

//   Sample SingleMuMay("SingleMuMay","/data/benitezj/RootFiles/SingleMu/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_3_0/cmgTauMu");
//   SingleMuMay.setDataType("Data");
//   //SingleMuMay.setSampleLumi(44318268.311/1000000.);
//   SingleMuMay.setSampleLumi(47.022);//47.022 new Cert files 
//   SingleMuMay.setRunRange(160000,163261);
//   SingleMuMay.addTrigPath("HLT_IsoMu12_v1");
//   analysis.addSample(&SingleMuMay);

  Sample TauPlusXMay("TauPlusXMay","/data/benitezj/RootFiles/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_3_0/TauMu");
  TauPlusXMay.setDataType("Data");
  TauPlusXMay.setSampleLumi(168.597);//168.597
  TauPlusXMay.setRunRange(163262,170000);
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v1");
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v2");
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v4");
  analysis.addSample(&TauPlusXMay);

  Sample TauPlusXv4("TauPlusXv4","/data/benitezj/RootFiles/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_3_0/TauMu");
  TauPlusXv4.setDataType("Data");
  TauPlusXv4.setSampleLumi(929.748);//929.748
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v2");//include just in case ??
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v4");//include just in case ??
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v5");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");
  analysis.addSample(&TauPlusXv4);

  Sample TauPlusXAug("TauPlusXAug","/data/benitezj/RootFiles/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2/PAT_CMG_V2_3_0/TauMu");
  TauPlusXAug.setDataType("Data");
  TauPlusXAug.setSampleLumi(373.349);//373.349
  TauPlusXAug.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");//include just in case ??
  TauPlusXAug.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8");
  analysis.addSample(&TauPlusXAug);

  Sample TauPlusXv6("TauPlusXv6","/data/benitezj/RootFiles/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_3_0/TauMu");
  TauPlusXv6.setDataType("Data");
  TauPlusXv6.setRunRange(170000,172802);
  TauPlusXv6.setSampleLumi(124.683);//124.683
  TauPlusXv6.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");//include just in case ??
  TauPlusXv6.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8");
  analysis.addSample(&TauPlusXv6);

  //////////////////////////////////////////////////////
  /////////////Same Sign samples////////////////////////
  //////////////////////////////////////////////////////


  Sample WJetsToLNu_SS("WJetsToLNu_SS",(const char*)(TString(WJetsToLNu.GetTitle())+"_SS"));
  WJetsToLNu_SS.setDataType("MC_SS");
  WJetsToLNu_SS.setApplyRecoilCorr(WJetsToLNu.getApplyRecoilCorr());
  WJetsToLNu_SS.setColor(WJetsToLNu.getColor());
  WJetsToLNu_SS.setCrossection(WJetsToLNu.getCrossection());
  WJetsToLNu_SS.setSampleGenEvents(WJetsToLNu.getSampleGenEvents());
  WJetsToLNu_SS.addTrigPaths(WJetsToLNu.getTrigPaths());
  WJetsToLNu_SS.setEffCorrFactor(MCEffCorrFactor);
  WJetsToLNu_SS.setApplyTauRateWeight(WJetsToLNu.getApplyTauRateWeight());
  analysis.addSample(&WJetsToLNu_SS);
  
  Sample TTJets_SS("TTJets_SS",(const char*)(TString(TTJets.GetTitle())+"_SS"));
  TTJets_SS.setDataType("MC_SS");
  TTJets_SS.setColor(TTJets.getColor());
  TTJets_SS.setCrossection(TTJets.getCrossection());
  TTJets_SS.setSampleGenEvents(TTJets.getSampleGenEvents());
  TTJets_SS.addTrigPaths(TTJets.getTrigPaths());
  TTJets_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&TTJets_SS);

  Sample DYJetsToLL_SS("DYJetsToLL_SS",(const char*)(TString(DYJetsToLL.GetTitle())+"_SS"));
  DYJetsToLL_SS.setDataType("MC_SS");
  DYJetsToLL_SS.setApplyRecoilCorr(DYJetsToLL.getApplyRecoilCorr());
  DYJetsToLL_SS.setColor(DYJetsToLL.getColor());
  DYJetsToLL_SS.setCrossection(DYJetsToLL.getCrossection());
  DYJetsToLL_SS.setSampleGenEvents(DYJetsToLL.getSampleGenEvents());
  DYJetsToLL_SS.addTrigPaths(DYJetsToLL.getTrigPaths());
  DYJetsToLL_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&DYJetsToLL_SS);
    
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
  
  
//   //  /////////////////create histos
//   //   //   analysis.createHistos("Higgs");
//   analysis.createHistos("DYJetsToLL");
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
  
//   analysis.createHistos("DYJetsToLL_SS");
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
  
  
  //////////Make plots
  //   //0 index reserved for QCD 
  TTJets.setPlotOrder(1);       TTJets_SS.setPlotOrder(1);
  WJetsToLNu.setPlotOrder(2);   WJetsToLNu_SS.setPlotOrder(2);
  DYJetsToLL.setPlotOrder(3);   DYJetsToLL_SS.setPlotOrder(3);
  //dont plot Di-Bosons, too small
  


  
  //can only do one plot at a time for now 
  //  analysis.plotVisMass();
  
//   Float_t c[4]={0,0,0,0};
//   Float_t r[4]={160000,175000,0,400};
//   analysis.plotDistribution("runNumberHisto",0,"","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={-0.5,24.5,0,4000};
//   analysis.plotDistribution("nVertexHisto",1," # of reco. vertices ","",c,r);

//     Float_t c[4]={.5,.75,.42,.81};
//     Float_t r[4]={0,100,0,1500};
//     analysis.plotDistribution("muPtHisto",1," #mu p_{T}   (GeV)","Events / 1 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,0.5,0,6000};
//   analysis.plotDistribution("muIsoHisto_muiso",1," muon isolation  ","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,100,0,1800};
//   analysis.plotDistribution("tauPtHisto",1," #tau p_{T}   (GeV)","Events / 1 GeV",c,r);

  //   Float_t c[4]={.5,.75,.42,.81};
  //   analysis.plotDistribution("tauIsoHisto",2," tau isolation  ","",c,0);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,150,0,2000};
//   analysis.plotDistribution("metHisto",2," MET   (GeV)","Events / 2 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,150,0,3000};
//   analysis.plotDistribution("metHisto_massT",2," MET   (GeV)","Events / 2 GeV",c,r);
  
//    Float_t c[4]={.15,.45,.42,.81};
//    Float_t r[4]={-140,40,0,6000};
//    analysis.plotDistribution("pZetaHisto_pzeta",6,"P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)","Events / 6 GeV",c,r);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,0,2000};
//  analysis.plotDistribution("transverseMassHisto_massT",2,"m_{T}   (GeV)","Events / 2 GeV",c,r);
  

  Float_t c[4]={.5,.75,.42,.81};
  Float_t r[4]={0,300,0,4000};
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,350,5,1e7};
  analysis.plotDistribution("diTauMassHisto",5,"m_{vis}   (GeV)","Events / 5 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,300,0,1400};
//   analysis.plotDistribution("diTauMassSVFitHisto",5," mass   (GeV)","Events / 5 GeV",c,r);
  

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

