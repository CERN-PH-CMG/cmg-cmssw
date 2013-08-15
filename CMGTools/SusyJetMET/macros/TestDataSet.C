{
  // for MetDyn studies 
  gROOT->Macro("initTree.C");

  Chain RDtree("Events", "Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/*tree*.root", "RD");
 
  TFile RDfile("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/histograms.root"); 
  DataSet RDdata( &RDtree, &RDfile);

  

  Chain QCD120to170tree("Events", "Samples/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD120to170");
  TFile QCD120to170file("Samples/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD120to170data( &QCD120to170tree, &QCD120to170file);

  Chain QCD170to300tree("Events", "Samples/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD170to300");
  TFile QCD170to300file("Samples/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD170to300data( &QCD170to300tree, &QCD170to300file);

  Chain QCD300to470tree("Events", "Samples/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD300to470");
  TFile QCD300to470file("Samples/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD300to470data( &QCD300to470tree, &QCD300to470file);

  Chain QCD470to600tree("Events", "Samples/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD470to600");
  TFile QCD470to600file("Samples/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD470to600data( &QCD470to600tree, &QCD470to600file);

  Chain QCD600to800tree("Events", "Samples/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/*tree*.root", "QCD600to800");
  TFile QCD600to800file("Samples/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/GEN-SIM-RECO/NoMETSkim/histograms.root"); 
  DataSet QCD600to800data( &QCD600to800tree, &QCD600to800file);

  TH1 *h = QCD120to170data.DrawHist("pfJetLorentzVector/pT");
  h->Add( QCD170to300data.DrawHist("pfJetLorentzVector/pT") );
  h->Add( QCD300to470data.DrawHist("pfJetLorentzVector/pT") );
  h->Add( QCD470to600data.DrawHist("pfJetLorentzVector/pT") );
  h->Add( QCD600to800data.DrawHist("pfJetLorentzVector/pT") );

  // integrated luminosity
  RDdata.DrawHist("pfJetLorentzVector/PFJet/pT"); 
  h->Scale( 24 );
  h->Draw("same");
  
  gPad->SetLogy();

}
