{
  // initialize Fall10 QCD with Pile-Up datasets 
  // intLumi is declared in the main macro.

  Chain QCD120to170tree("Events", "Samples/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD120to170");
  Aliases QCD120to170aliases("Samples/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/aliases.py");
  QCD120to170aliases.SetAliases( &QCD120to170tree );
  TFile QCD120to170file("Samples/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD120to170data( &QCD120to170tree, 
			   &QCD120to170file );

  Chain QCD170to300tree("Events", "Samples/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD170to300");
  Aliases QCD170to300aliases("Samples/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/aliases.py");
  QCD170to300aliases.SetAliases( &QCD170to300tree );
  TFile QCD170to300file("Samples/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD170to300data( &QCD170to300tree, &QCD170to300file);

  Chain QCD300to470tree("Events", "Samples/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD300to470");
  Aliases QCD300to470aliases("Samples/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/aliases.py");
  QCD300to470aliases.SetAliases( &QCD300to470tree );
  TFile QCD300to470file("Samples/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD300to470data( &QCD300to470tree, &QCD300to470file);

  Chain QCD470to600tree("Events", "Samples/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD470to600");
  Aliases QCD470to600aliases("Samples/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/aliases.py");
  QCD470to600aliases.SetAliases( &QCD470to600tree );
  TFile QCD470to600file("Samples/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD470to600data( &QCD470to600tree, &QCD470to600file);

  Chain QCD600to800tree("Events", "Samples/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD600to800");
  Aliases QCD600to800aliases("Samples/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/aliases.py");
  QCD600to800aliases.SetAliases( &QCD600to800tree );
  TFile QCD600to800file("Samples/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD600to800data( &QCD600to800tree, &QCD600to800file);

  QCD120to170data.SetIntLumi( intLumi );
  QCD170to300data.SetIntLumi( intLumi );
  QCD300to470data.SetIntLumi( intLumi );
  QCD470to600data.SetIntLumi( intLumi );
  QCD600to800data.SetIntLumi( intLumi );

}
