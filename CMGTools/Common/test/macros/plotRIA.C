{
	
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPFAnalysesRootTools.so");
  gSystem->Load("libAnalysisDataFormatsCMGTools.so");

  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();
 

  Chain chain("runInfoAccounting/RunInfoAccounting", "histograms.root");

  chain.Print();
  chain.GetEntries();
  chain.Draw("finalEvents/initialEvents");

}


	
