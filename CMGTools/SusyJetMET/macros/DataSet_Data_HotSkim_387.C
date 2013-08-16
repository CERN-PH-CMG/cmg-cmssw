{
  // CMG tree, data, Steven's hotskim, AN383.
  // produced with the new release, 387
  // - jet ID not considered anymore
  // - no cleaning applied
  // - mht filter checked : was already applied by Steven at skimming level
  // - dphi filter checked : also already applied by Steven at skimming level  

  // reusing an old alias. Anyway, such things usually don't change
  Aliases allAliases("Samples/Data/HotSkim/SusyPat387/CMG/aliases.py");

  Chain data("Events", "Samples/Data/HotSkim/SusyPat387/CMG/susyJetMET_tree_*.root", "DATA");
  allAliases.SetAliases( &data );


}
