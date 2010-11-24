{
  // for MetDyn studies 
  gROOT->Macro("initTree.C");

  Chain RDtree("Events", "Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/*tree*.root", "RD");
 
  TFile RDfile("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/histograms.root"); 
  DataSet RDdata( &RDtree, &RDfile);

  Aliases aliases("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/aliases.py");
  aliases.SetAliases( &RDtree );
}
