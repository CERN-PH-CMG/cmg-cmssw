{
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPFAnalysesRootTools.so");
  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();

  gROOT->LoadMacro("RiaInfo.C");
  gROOT->LoadMacro("InitChain.C");
  gROOT->LoadMacro("rootlogon.C");

}

