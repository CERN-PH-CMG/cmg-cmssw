{
  // initialize Fall10 QCD with Pile-Up datasets 
  // intLumi is declared in the main macro.

  Aliases allAliases("Samples/QCD_SueAnn_HotSkim/SusyPatPFPath/CMGInconsistentMuons/aliases.py");

//   Chain data("Events", "Samples/Data/HotSkim/CMGInconsistentMuons/susyJetMET_tree_RA2_CMG_*.root");
//   allAliases.SetAliases( &data );

  Chain all("Events", "susyJetMET_tree*root", "ALL");
  allAliases.SetAliases( &all );
}
