{
	
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPFAnalysesRootTools.so");
  gSystem->Load("libAnalysisDataFormatsCMGTools.so");

  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();
 

  gROOT->LoadMacro("macros/rootlogon.C");

}


	
