{
  // for MetDyn studies 
  gROOT->Macro("initTree.C");

  Chain RD("Events", "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/JetMETTau/Run2010A-Jul26thReReco-v1/RECO/CorrectJetPtSelection/HBHENoiseFilter/NewFramework/MoreMET/*tree*root", "RD");
  
  initCMG("ANA", &RD);

  float intLumi = 0.066;

  Chain *chain;
  Chain *ria;

  TTree* QCDZ250to80 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 
  TTree* QCDZ280to120 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 
  TTree* QCDZ2120to170 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 
  TTree* QCDZ2170to300 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 
  TTree* QCDZ2300to470 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 

  Plot::SetWriteIdInfo( false );

  Samples samz2; 
  samz2.SetLegendCorners(0.65,0.7,0.88,0.88);
  samz2.AddTree(&RD, "DATA"); 
  samz2.AddTree(QCDZ280to120, "QCDZ2"); 
  samz2.AddTree(QCDZ2120to170, "QCDZ2"); 
  samz2.AddTree(QCDZ2170to300, "QCDZ2"); 
  samz2.AddTree(QCDZ2300to470, "QCDZ2"); 
  
  Styles styles;
  samz2.SetStyle( "DATA", false, styles.sdata1 );
  samz2.SetStyle( "QCDZ2", true, styles.spred );
//   samz2.AddTree(QCDZ250to80, "QCDZ2-50to80"); 
//   samz2.SetStyle( "QCDZ2-50to80", true, styles.spblue );


  TTree* QCDT180to120 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_80to120_Tune1_7TeV_pythia8/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 
  TTree* QCDT1120to170 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_120to170_Tune1_7TeV_pythia8/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 
  TTree* QCDT1170to300 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_170to300_Tune1_7TeV_pythia8/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 
  TTree* QCDT1300to470 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_300to470_Tune1_7TeV_pythia8/Fall10-START38_V12-v1/AODSIM/NewFramework/MoreMET", intLumi); 

  Plot::SetWriteIdInfo( false );

  Samples samt1; 
  samt1.SetLegendCorners(0.65,0.7,0.88,0.88);
  samt1.AddTree(&RD, "DATA"); 
  samt1.AddTree(QCDT180to120, "QCDT1"); 
  samt1.AddTree(QCDT1120to170, "QCDT1"); 
  samt1.AddTree(QCDT1170to300, "QCDT1"); 
  samt1.AddTree(QCDT1300to470, "QCDT1"); 
  

  //COLIN : protect this function. 
  samt1.SetStyle( "DATA", false, styles.sdata1 );
  samt1.SetStyle( "QCDT1", true, styles.spred );


}
