{  
  TauMuAnalysis analysis("analysis");
  //analysis.setPupWeightName("vertexWeight2invfb");
  analysis.setPupWeightName("vertexWeight2011AB");
  //analysis.setPupWeightName("vertexWeight2011B");
  analysis.setInputTag("cmgTauMuCorSVFitFullSel");
  //analysis.setInputTag("cmgTauMuCorSVFitPreSel");
  analysis.setdiLeptonVetoListName("cmgMuonSel");
  analysis.setQCDOStoSSRatio(1.11);//value from AN-11-390 v4
  analysis.setTransverseMassSignalMax(40);
  analysis.setTransverseMassSideBandMin(60);
  analysis.setVerbosity(0);

  //analysis.setTruncateEvents(1000); 
  analysis.makeAllHistos(1);
  analysis.setPrintFreq(1000);

  //analysis.calcSVFit(1);
  float MCEffCorrFactor = 1.0;// 0.968;// * 0.92; 
     
  TString outpath="/afs/cern.ch/user/b/benitezj/public/V240AB";
  
  analysis.setOutputPath(outpath);

  TString datapath="/data/benitezj/Samples";
  TString tag="/PAT_CMG_V2_4_0/H2TAUTAU_Dec2"; 
  
  /////////////////////////
  //define samples    
  /////////////////////////
  
  //   Sample TestSample("TestSample",(const char*)("../prod"));
  //   TestSample.setDataType("MC");
  //   TestSample.setRecoilCorr("../../Common/data/metRecoilCorrection/recoilfit_wjets_njet.root");
  //   analysis.addSample(&TestSample);  
  
  
  Sample WJetsToLNu("WJetsToLNu",(const char*)(datapath+"/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
  WJetsToLNu.setDataType("MC");
  WJetsToLNu.setRecoilCorr("../data/recoilfit_wjets_njet.root"); 
  WJetsToLNu.setCrossection(31314);
  WJetsToLNu.setSampleGenEvents(53227112); //  53655290 = PFAOD integrity, 81352581 = "DBS"
  //WJetsToLNu.addTrigPath("HLT_IsoMu12_v1");
  WJetsToLNu.setEffCorrFactor(MCEffCorrFactor);
  WJetsToLNu.setApplyTauRateWeight(0);
  analysis.addSample(&WJetsToLNu);  
  

  Sample TTJets("TTJets",(const char*)(datapath+"/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
  TTJets.setDataType("MC");
  TTJets.setCrossection(165.8);//157.5=NLO theory, 165.8=CMS TOP-11-024
  TTJets.setSampleGenEvents(3542770); // 3701947=Enriques PFAOD integrity, 3701947="DBS"
  //TTJets.addTrigPath("HLT_IsoMu12_v1");
  TTJets.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&TTJets);
 
  Sample ZToMuMu("ZToMuMu",(const char*)(datapath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
  ZToMuMu.setDataType("MC");
  ZToMuMu.setTruthEventType(3);
  ZToMuMu.setRecoilCorr("../data/recoilfit_zmm42X_njet.root");
  ZToMuMu.setCrossection(3048);
  ZToMuMu.setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
  //ZToMuMu.addTrigPath("HLT_IsoMu12_v1");
  ZToMuMu.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToMuMu);

  Sample ZToTauTau("ZToTauTau",(const char*)(datapath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
  ZToTauTau.setDataType("MC");
  ZToTauTau.setTruthEventType(5);
  ZToTauTau.setRecoilCorr("../data/recoilfit_zjets_ltau_njet.root");
  ZToTauTau.setCrossection(3048);
  ZToTauTau.setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
  //ZToTauTau.addTrigPath("HLT_IsoMu12_v1");
  ZToTauTau.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToTauTau);

  Sample ZToLJet("ZToLJet",(const char*)(datapath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
  ZToLJet.setDataType("MC");
  ZToLJet.setTruthEventType(6);
  ZToLJet.setRecoilCorr("../data/recoilfit_wjets_njet.root");
  ZToLJet.setCrossection(3048);
  ZToLJet.setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
  //ZToLJet.addTrigPath("HLT_IsoMu12_v1");
  ZToLJet.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToLJet);

  Sample WW("WW",(const char*)(datapath+"/WW_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
  WW.setDataType("MC");
  WW.setCrossection(27.83);
  WW.setSampleGenEvents(4052865);
  //WW.addTrigPath("HLT_IsoMu12_v1");
  WW.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WW);

  Sample WZ("WZ",(const char*)(datapath+"/WZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
  WZ.setDataType("MC");
  WZ.setCrossection(10.47);
  WZ.setSampleGenEvents(4169751);
  //WZ.addTrigPath("HLT_IsoMu12_v1");
  WZ.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WZ);

  Sample ZZ("ZZ",(const char*)(datapath+"/ZZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
  ZZ.setDataType("MC");
  ZZ.setCrossection(4.287);
  ZZ.setSampleGenEvents(4112445);
  //ZZ.addTrigPath("HLT_IsoMu12_v1");
  ZZ.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZZ);

  ///Crossections taken from here:
  ///https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt7TeV#gluon_gluon_Fusion_Process 
  ///Higgs --> tau tau branching ratios taken from http://arxiv.org/abs/1101.0593 v3
  Float_t HiggsGGcross[8]={21.78,19.84,18.13,16.63,15.31,14.12,13.08,12.13};
  Float_t HiggsVBFcross[8]={1.472,1.398,1.332,1.269,1.211,1.154,1.100,1.052};
  Float_t HiggsTauTauBF[8]={8.25e-2,8.03e-2,7.65e-2,7.11e-2,6.37e-2,5.49e-2,4.52e-2,3.54e-2};
  Sample * HiggsGG[8];
  Sample * HiggsVBF[8];
  for(Int_t i=0;i<7;i++){
    HiggsGG[i]=new Sample((const char*)(TString("HiggsGG")+(long)(105+i*5)),(const char*)(datapath+"/GluGluToHToTauTau_M-"+(long)(105+i*5)+"_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
    HiggsGG[i]->setDataType("Signal");
    HiggsGG[i]->setCrossection(HiggsGGcross[i]*HiggsTauTauBF[i]);
    HiggsGG[i]->setSampleGenEvents(196002);
    HiggsGG[i]->setEffCorrFactor(MCEffCorrFactor);
    analysis.addSample(HiggsGG[i]);    

    HiggsVBF[i]=new Sample((const char*)(TString("HiggsVBF")+(long)(105+i*5)),(const char*)(datapath+"/VBF_HToTauTau_M-"+(long)(105+i*5)+"_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag));
    HiggsVBF[i]->setDataType("Signal");
    HiggsVBF[i]->setCrossection(HiggsVBFcross[i]*HiggsTauTauBF[i]);
    HiggsVBF[i]->setSampleGenEvents(196002);
    HiggsVBF[i]->setEffCorrFactor(MCEffCorrFactor);
    analysis.addSample(HiggsVBF[i]);    
  }


  Sample TauPlusXMay("TauPlusXMay",(const char*)(datapath+"/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2"+tag));
  TauPlusXMay.setDataType("Data");
  TauPlusXMay.setSampleLumi(168.597);
  TauPlusXMay.setRunRange(163262,170000);
  TauPlusXMay.addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v4");
  analysis.addSample(&TauPlusXMay);

  Sample TauPlusXv4("TauPlusXv4",(const char*)(datapath+"/TauPlusX/Run2011A-PromptReco-v4/AOD/V2"+tag));
  TauPlusXv4.setDataType("Data");
  TauPlusXv4.setSampleLumi(929.748);
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v2");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v4");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v5");
  TauPlusXv4.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6");
  analysis.addSample(&TauPlusXv4);

  Sample TauPlusXAug("TauPlusXAug",(const char*)(datapath+"/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2"+tag));
  TauPlusXAug.setDataType("Data");
  TauPlusXAug.setSampleLumi(373.349);
  TauPlusXAug.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8");
  analysis.addSample(&TauPlusXAug);

  Sample TauPlusXOct3("TauPlusXOct3",(const char*)(datapath+"/TauPlusX/Run2011A-03Oct2011-v1/AOD/V2"+tag));
  TauPlusXOct3.setDataType("Data");
  TauPlusXOct3.setSampleLumi(658.886);
  TauPlusXOct3.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8");
  TauPlusXOct3.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9");
  analysis.addSample(&TauPlusXOct3);

  Sample TauPlusX2011B("TauPlusX2011B",(const char*)(datapath+"/TauPlusX/Run2011B-PromptReco-v1/AOD/V2"+tag));
  TauPlusX2011B.setDataType("Data");
  TauPlusX2011B.setSampleLumi(2511);
  // HLT_IsoMu15_LooseIsoPFTau15_v9             173236 - 178380         1945    L1_SingleMu10    (Note! this trigger got prescaled in 2011B)
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5      178420 - 179889         706.7   L1_SingleMu14_Eta2p1    ET(tau)>20 GeV, |eta(mu)|<2.1
  // HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6      179959 - 180252         120.7   L1_SingleMu14_Eta2p1    end of 2011 run 
  TauPlusX2011B.addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9");//to recover ~100pb-1 
  TauPlusX2011B.addTrigPath("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1");//used when HLT_IsoMu15_LooseIsoPFTau15_v9 got prescaled 
  TauPlusX2011B.addTrigPath("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5");
  TauPlusX2011B.addTrigPath("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6");
  analysis.addSample(&TauPlusX2011B);


  Sample EmbeddedMay("EmbeddedMay",(const char*)(datapath+"/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag));
  EmbeddedMay.setDataType("Embedded");
  analysis.addSample(&EmbeddedMay);

  Sample EmbeddedV4("EmbeddedV4",(const char*)(datapath+"/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag));
  EmbeddedV4.setDataType("Embedded");
  analysis.addSample(&EmbeddedV4);

  Sample EmbeddedAug("EmbeddedAug",(const char*)(datapath+"/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag));
  EmbeddedAug.setDataType("Embedded");
  analysis.addSample(&EmbeddedAug);

  Sample EmbeddedOct("EmbeddedOct",(const char*)(datapath+"/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag));
  EmbeddedOct.setDataType("Embedded");
  analysis.addSample(&EmbeddedOct);

  Sample Embedded2011B("Embedded2011B",(const char*)(datapath+"/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag));
  Embedded2011B.setDataType("Embedded");
  analysis.addSample(&Embedded2011B);



  //////////////////////////////////////////////////////
  /////////////Same Sign samples////////////////////////
  //////////////////////////////////////////////////////


  Sample WJetsToLNu_SS("WJetsToLNu_SS",WJetsToLNu.GetTitle());
  WJetsToLNu_SS.setDataType("MC_SS");
  WJetsToLNu_SS.setRecoilCorr(WJetsToLNu.getRecoilCorrProcessFile());
  WJetsToLNu_SS.setCrossection(WJetsToLNu.getCrossection());
  WJetsToLNu_SS.setSampleGenEvents(WJetsToLNu.getSampleGenEvents());
  WJetsToLNu_SS.addTrigPaths(WJetsToLNu.getTrigPaths());
  WJetsToLNu_SS.setEffCorrFactor(MCEffCorrFactor);
  WJetsToLNu_SS.setApplyTauRateWeight(WJetsToLNu.getApplyTauRateWeight());
  analysis.addSample(&WJetsToLNu_SS);
  
  Sample TTJets_SS("TTJets_SS",TTJets.GetTitle());
  TTJets_SS.setDataType("MC_SS");
  TTJets_SS.setCrossection(TTJets.getCrossection());
  TTJets_SS.setSampleGenEvents(TTJets.getSampleGenEvents());
  TTJets_SS.addTrigPaths(TTJets.getTrigPaths());
  TTJets_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&TTJets_SS);

  Sample ZToMuMu_SS("ZToMuMu_SS",ZToMuMu.GetTitle());
  ZToMuMu_SS.setDataType("MC_SS");
  ZToMuMu_SS.setTruthEventType(ZToMuMu.getTruthEventType());
  ZToMuMu_SS.setRecoilCorr(ZToMuMu.getRecoilCorrProcessFile());
  ZToMuMu_SS.setCrossection(ZToMuMu.getCrossection());
  ZToMuMu_SS.setSampleGenEvents(ZToMuMu.getSampleGenEvents());
  ZToMuMu_SS.addTrigPaths(ZToMuMu.getTrigPaths());
  ZToMuMu_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToMuMu_SS);

  Sample ZToTauTau_SS("ZToTauTau_SS",ZToTauTau.GetTitle());
  ZToTauTau_SS.setDataType("MC_SS");
  ZToTauTau_SS.setTruthEventType(ZToTauTau.getTruthEventType());
  ZToTauTau_SS.setRecoilCorr(ZToTauTau.getRecoilCorrProcessFile());
  ZToTauTau_SS.setCrossection(ZToTauTau.getCrossection());
  ZToTauTau_SS.setSampleGenEvents(ZToTauTau.getSampleGenEvents());
  ZToTauTau_SS.addTrigPaths(ZToTauTau.getTrigPaths());
  ZToTauTau_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToTauTau_SS);

  Sample ZToLJet_SS("ZToLJet_SS",ZToLJet.GetTitle());
  ZToLJet_SS.setDataType("MC_SS");
  ZToLJet_SS.setTruthEventType(ZToLJet.getTruthEventType());
  ZToLJet_SS.setRecoilCorr(ZToLJet.getRecoilCorrProcessFile());
  ZToLJet_SS.setCrossection(ZToLJet.getCrossection());
  ZToLJet_SS.setSampleGenEvents(ZToLJet.getSampleGenEvents());
  ZToLJet_SS.addTrigPaths(ZToLJet.getTrigPaths());
  ZToLJet_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZToLJet_SS);

    
  Sample WW_SS("WW_SS",WW.GetTitle());
  WW_SS.setDataType("MC_SS");
  WW_SS.setCrossection(WW.getCrossection());
  WW_SS.setSampleGenEvents(WW.getSampleGenEvents());
  WW_SS.addTrigPaths(WW.getTrigPaths());    
  WW_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WW_SS);

  Sample WZ_SS("WZ_SS",WZ.GetTitle());
  WZ_SS.setDataType("MC_SS");
  WZ_SS.setCrossection(WZ.getCrossection());
  WZ_SS.setSampleGenEvents(WZ.getSampleGenEvents());
  WZ_SS.addTrigPaths(WZ.getTrigPaths());
  WZ_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&WZ_SS);

  Sample ZZ_SS("ZZ_SS",ZZ.GetTitle());
  ZZ_SS.setDataType("MC_SS");
  ZZ_SS.setCrossection(ZZ.getCrossection());
  ZZ_SS.setSampleGenEvents(ZZ.getSampleGenEvents());
  ZZ_SS.addTrigPaths(ZZ.getTrigPaths());
  ZZ_SS.setEffCorrFactor(MCEffCorrFactor);
  analysis.addSample(&ZZ_SS);

  Sample TauPlusXMay_SS("TauPlusXMay_SS",TauPlusXMay.GetTitle());
  TauPlusXMay_SS.setDataType("Data_SS");
  TauPlusXMay_SS.setSampleLumi(TauPlusXMay.getLumi());
  TauPlusXMay_SS.setRunRange(TauPlusXMay.getFirstRun(),TauPlusXMay.getLastRun());
  TauPlusXMay_SS.addTrigPaths(TauPlusXMay.getTrigPaths());
  analysis.addSample(&TauPlusXMay_SS);

  Sample TauPlusXv4_SS("TauPlusXv4_SS",TauPlusXv4.GetTitle());
  TauPlusXv4_SS.setDataType("Data_SS");
  TauPlusXv4_SS.setSampleLumi(TauPlusXv4.getLumi());
  TauPlusXv4_SS.addTrigPaths(TauPlusXv4.getTrigPaths());
  analysis.addSample(&TauPlusXv4_SS);

  Sample TauPlusXAug_SS("TauPlusXAug_SS",TauPlusXAug.GetTitle());
  TauPlusXAug_SS.setDataType("Data_SS");
  TauPlusXAug_SS.setSampleLumi(TauPlusXAug.getLumi());
  TauPlusXAug_SS.addTrigPaths(TauPlusXAug.getTrigPaths());
  analysis.addSample(&TauPlusXAug_SS);

  Sample TauPlusXOct3_SS("TauPlusXOct3_SS",TauPlusXOct3.GetTitle());
  TauPlusXOct3_SS.setDataType("Data_SS");
  TauPlusXOct3_SS.setSampleLumi(TauPlusXOct3.getLumi());
  TauPlusXOct3_SS.addTrigPaths(TauPlusXOct3.getTrigPaths());
  analysis.addSample(&TauPlusXOct3_SS);

  Sample TauPlusX2011B_SS("TauPlusX2011B_SS",TauPlusX2011B.GetTitle());
  TauPlusX2011B_SS.setDataType("Data_SS");
  TauPlusX2011B_SS.setSampleLumi(TauPlusX2011B.getLumi());
  TauPlusX2011B_SS.addTrigPaths(TauPlusX2011B.getTrigPaths());
  analysis.addSample(&TauPlusX2011B_SS);


  Sample EmbeddedMay_SS("EmbeddedMay_SS",EmbeddedMay.GetTitle());
  EmbeddedMay_SS.setDataType("Embedded_SS");
  analysis.addSample(&EmbeddedMay_SS);

  Sample EmbeddedV4_SS("EmbeddedV4_SS",EmbeddedV4.GetTitle());
  EmbeddedV4_SS.setDataType("Embedded_SS");
  analysis.addSample(&EmbeddedV4_SS);

  Sample EmbeddedAug_SS("EmbeddedAug_SS",EmbeddedAug.GetTitle());
  EmbeddedAug_SS.setDataType("Embedded_SS");
  analysis.addSample(&EmbeddedAug_SS);

  Sample EmbeddedOct_SS("EmbeddedOct_SS",EmbeddedOct.GetTitle());
  EmbeddedOct_SS.setDataType("Embedded_SS");
  analysis.addSample(&EmbeddedOct_SS);

  Sample Embedded2011B_SS("Embedded2011B_SS",Embedded2011B.GetTitle());
  Embedded2011B_SS.setDataType("Embedded_SS");
  analysis.addSample(&Embedded2011B_SS);
}


  ///Crossections taken from here:
  ///https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt7TeV#gluon_gluon_Fusion_Process 
  ///Gluon-Gluon
  // 105.0 	21.78 	20.8 	-15.5 	
  // 110.0 	19.84 	20.4 	-15.3 	
  // 115.0 	18.13 	20.0 	-15.3 	
  // 120.0 	16.63 	19.7 	-15.1 	
  // 125.0 	15.31 	19.5 	-15.1 	
  // 130.0 	14.12 	19.2 	-15.1 	
  // 135.0 	13.08 	18.9 	-15.0 	
  // 140.0 	12.13 	18.8 	-14.9 
  ///VBF  
  // 105.0 	1.472 	2.5 	-2.4 	
  // 110.0 	1.398 	2.8 	-2.3 	
  // 115.0 	1.332 	2.5 	-2.3 	
  // 120.0 	1.269 	2.8 	-2.5 	
  // 125.0 	1.211 	2.7 	-2.4 	
  // 130.0 	1.154 	2.8 	-2.3 	
  // 135.0 	1.100 	3.0 	-2.2 	
  // 140.0 	1.052 	2.8 	-2.2 	

  ///Higgs --> tau tau branching ratios taken from http://arxiv.org/abs/1101.0593 v3
  // 105  8.25e-2 
  // 110  8.03e-2 
  // 115  7.65e-2 
  // 120  7.11e-2 
  // 125  6.37e-2 
  // 130  5.49e-2 
  // 135  4.52e-2 
  // 140  3.54e-2 
