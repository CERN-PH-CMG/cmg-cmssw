{
  // for MetDyn studies 
  gROOT->Macro("initTree.C");

  Chain RD("Events", "Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/*tree*.root", "RD");
  
  initCMG("ANA", &RD);
//   RD.SetAlias("cmgPFBaseJets", "cmgPFJets_pfJetSelector__PRUNE" );

//   initPAT("PAT", &RD);

  TCut pt("cmgPFMET.obj.sumEt()>300 && cmgPFBaseJets.obj[0].pt()>50 && cmgPFBaseJets.obj[1].pt()>50 && cmgPFBaseJets.obj[2].pt()>50");
  // warning - for PU / no PU comparison
  // TCut pt("cmgPFBaseJets.obj[0].pt()>200 && cmgPFBaseJets.obj[1].pt()>50 && cmgPFBaseJets.obj[2].pt()>50");
    
  TCut dphi0("abs(cmgPFBaseJets.obj[0].phi()-cmgPFMET.obj[0].phi())>0.3");
  TCut dphi1("abs(cmgPFBaseJets.obj[1].phi()-cmgPFMET.obj[0].phi())>0.3");
  TCut dphi2("abs(cmgPFBaseJets.obj[2].phi()-cmgPFMET.obj[0].phi())>0.3");

  TCut dphi(dphi0 && dphi1 && dphi2 );
  
  TCut RA2( pt && dphi ); 

  float intLumi = 24;

  Chain *chain;
  Chain *ria;

  TTree* QCD50to80 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi,
			       "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_50to80_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim"); 

  TTree* QCD80to120 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi, 
				"/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim"); 

  TTree* QCD120to170 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi,
				 "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim"); 

  TTree* QCD170to300 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi,
				 "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/PatTuple/NoMETSkim"); 
  
  TTree* QCD300to470 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/PatTuple/NoMETSkim/MHT", intLumi,
				 "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/PatTuple/NoMETSkim"); 

  TTree* QCD470to600 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi,
				 "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim");
 
  TTree* QCD600to800 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi,
				 "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim"); 

  TTree* QCD800to1000 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi,
				  "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim"); 


//   TTree* Zinvisible = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/NoMETSkim", intLumi, ""); 

  TTree* TT = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/NoMETSkim", intLumi, "");  
  // override the wrong internal quantities, and inject the NLO x-section
  TT->SetWeight(157.5/100000*intLumi,"global");


  // these guys don't include neutrinos!
//   TTree* Z3J_100to300 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/NoMETSkim", intLumi, "");  

//   TTree* Z3J_300to800 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/Z3Jets_Pt300to800-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/NoMETSkim", intLumi, "");  

//   TTree* Z3J_800to1600 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/Z3Jets_Pt800to1600-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/NoMETSkim", intLumi, "");  


  TTree* W3J_100to300 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/NoMETSkim", intLumi, "");  

  TTree* W3J_300to800 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/W3Jets_Pt300to800-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/NoMETSkim", intLumi, "");  

  TTree* W3J_800to1600 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/W3Jets_Pt800to1600-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/NoMETSkim", intLumi, "");  



  TTree* LM0 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi,
			 "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim");
 
  TTree* LM1 = InitChain("/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim/MHT", intLumi,
			 "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/NoMETSkim"); 


  Plot::SetWriteIdInfo( false );

  bool addData = True;

  Samples sam; 
  sam.SetLegendCorners(0.65,0.7,0.88,0.88);
  if( addData )
    sam.AddTree(&RD, "DATA"); 

  sam.AddTree(QCD50to80, "QCD"); 
  sam.AddTree(QCD80to120, "QCD"); 
  sam.AddTree(QCD120to170, "QCD"); 
  sam.AddTree(QCD170to300, "QCD"); 
  sam.AddTree(QCD300to470, "QCD"); 
  sam.AddTree(QCD470to600, "QCD"); 
  sam.AddTree(QCD600to800, "QCD"); 
  sam.AddTree(QCD800to1000, "QCD"); 
//   sam.AddTree(Zinvisible, "ZToNuNu"); 
  sam.AddTree(TT, "TT"); 

//   sam.AddTree(Z3J_100to300, "Z3J"); 
//   sam.AddTree(Z3J_300to800, "Z3J"); 
//   sam.AddTree(Z3J_800to1600, "Z3J"); 

  sam.AddTree(W3J_100to300, "W3J"); 
  sam.AddTree(W3J_300to800, "W3J"); 
  sam.AddTree(W3J_800to1600, "W3J"); 

//   sam.AddTree(LM0, "LM0"); 
//   sam.AddTree(LM1, "LM1"); 
  
  Styles styles;
  if( addData )
    sam.SetStyle( "DATA", false, styles.sdata1 );
//   sam.SetStyle( "QCD2", true, styles.spred );
  sam.SetStyle( "QCD", true, styles.spyellow );
  sam.SetStyle( "TT", true, styles.spgray,"hist");
//   sam.SetStyle( "Z3J", true, styles.spblue,"hist");
  sam.SetStyle( "W3J", true, styles.spred,"hist");
//   sam.SetStyle( "LM0", false, styles.spblue, "hist");
//   sam.SetStyle( "LM1", false, styles.spred , "hist");

}
