{
  // for MetDyn studies 
  //   gROOT->Macro("initTree.C");
  
  Chain RD("Summary/tree", "Samples/JetMETTau/Run2010A-Jul26thReReco-v1/RECO/CorrectJetPtSelection/HBHENoiseFilter/DPhiJetMET/PFAnalysis*root", "RD");

  float intLumi = 0.066;

  string sQCD80to120 = "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/*.root";
  Chain QCD80to120("Summary/tree", 
		   sQCD80to120.c_str(), "QCD80to120");
  Chain QCD80to120ria("RunInfoAccounting/RunInfoAccounting", 
		      sQCD80to120.c_str(), "QCD80to120ria");
  QCD80to120.SetWeight( RiaInfo( &QCD80to120ria )*intLumi,"global"); 


  string sQCD120to170 = "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/*.root";
  Chain QCD120to170("Summary/tree",sQCD120to170.c_str(),"QCD120to170");
  Chain QCD120to170ria("RunInfoAccounting/RunInfoAccounting", sQCD120to170.c_str(), "QCD120to170ria");
  QCD120to170.SetWeight(RiaInfo( &QCD120to170ria )*intLumi,"global"); 

  string sQCD170to300 = "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/*.root";
  Chain QCD170to300("Summary/tree",sQCD170to300.c_str(),"QCD170to300");
  Chain QCD170to300ria("RunInfoAccounting/RunInfoAccounting", sQCD170to300.c_str(), "QCD170to300ria");
  QCD170to300.SetWeight(RiaInfo( &QCD170to300ria )*intLumi,"global"); 

  string sQCD300to470 = "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/AODSIM/*.root";
  Chain QCD300to470("Summary/tree",sQCD300to470.c_str(),"QCD300to470");
  Chain QCD300to470ria("RunInfoAccounting/RunInfoAccounting", sQCD300to470.c_str(), "QCD300to470ria");
  QCD300to470.SetWeight(RiaInfo( &QCD300to470ria )*intLumi,"global"); 


  Samples samz2; 
  samz2.SetLegendCorners(0.7,0.7,0.93,0.88);
  samz2.AddTree(&RD, "DATA"); 
  samz2.AddTree(&QCD80to120, "QCD"); 
  samz2.AddTree(&QCD120to170, "QCD"); 
  samz2.AddTree(&QCD170to300, "QCD"); 
  samz2.AddTree(&QCD300to470, "QCD"); 
  
  Styles styles;
  samz2.SetStyle( "DATA", false, styles.sdata1 );
  samz2.SetStyle( "QCD", true, styles.spred );



  string sQCD80to120 = "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_80to120_Tune1_7TeV_pythia8/Fall10-START38_V12-v1/AODSIM/*.root";
  Chain QCD80to120("Summary/tree", 
		   sQCD80to120.c_str(), "QCD80to120");
  Chain QCD80to120ria("RunInfoAccounting/RunInfoAccounting", 
		      sQCD80to120.c_str(), "QCD80to120ria");
  QCD80to120.SetWeight( RiaInfo( &QCD80to120ria )*intLumi,"global"); 


  string sQCD120to170 = "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_120to170_Tune1_7TeV_pythia8/Fall10-START38_V12-v1/AODSIM/*.root";
  Chain QCD120to170("Summary/tree",sQCD120to170.c_str(),"QCD120to170");
  Chain QCD120to170ria("RunInfoAccounting/RunInfoAccounting", sQCD120to170.c_str(), "QCD120to170ria");
  QCD120to170.SetWeight(RiaInfo( &QCD120to170ria )*intLumi,"global"); 

  string sQCD170to300 = "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_170to300_Tune1_7TeV_pythia8/Fall10-START38_V12-v1/AODSIM/*.root";
  Chain QCD170to300("Summary/tree",sQCD170to300.c_str(),"QCD170to300");
  Chain QCD170to300ria("RunInfoAccounting/RunInfoAccounting", sQCD170to300.c_str(), "QCD170to300ria");
  QCD170to300.SetWeight(RiaInfo( &QCD170to300ria )*intLumi,"global"); 

  string sQCD300to470 = "/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET/QCD_Pt_300to470_Tune1_7TeV_pythia8/Fall10-START38_V12-v1/AODSIM/*.root";
  Chain QCD300to470("Summary/tree",sQCD300to470.c_str(),"QCD300to470");
  Chain QCD300to470ria("RunInfoAccounting/RunInfoAccounting", sQCD300to470.c_str(), "QCD300to470ria");
  QCD300to470.SetWeight(RiaInfo( &QCD300to470ria )*intLumi,"global"); 


  Samples sam1; 
  sam1.SetLegendCorners(0.7,0.7,0.93,0.88);
  sam1.AddTree(&RD, "DATA"); 
  sam1.AddTree(&QCD80to120, "QCD"); 
  sam1.AddTree(&QCD120to170, "QCD"); 
  sam1.AddTree(&QCD170to300, "QCD"); 
  sam1.AddTree(&QCD300to470, "QCD"); 
  
  Styles styles;
  sam1.SetStyle( "DATA", false, styles.sdata1 );
  sam1.SetStyle( "QCD", true, styles.spred );

 

//   samz2.Draw("MetPF.sumEt()", "JetsPF[0].pt()>100", 0.1, true); 

}
