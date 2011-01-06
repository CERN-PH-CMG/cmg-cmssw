{
  // CMG tree, data, Steven's hotskim, AN383.
  Aliases allAliases("Samples/QCD_SueAnn_HotSkim/SusyPatPFPath/CMGAllFilterResults/aliases.py");

  Chain data("Events", "Samples/HotSkims/Data/susyJetMET_tree_*.root", "DATA");
  allAliases.SetAliases( &data );


}
