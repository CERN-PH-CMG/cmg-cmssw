{
	
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPFAnalysesRootTools.so");
  gSystem->Load("libAnalysisDataFormatsCMGTools.so");

  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();
 

  Chain chain("Events", "testCMGTools.root");
//   chain.Draw("cmgBaseJets_cmgjet__PAT.obj.pt()");

  gROOT->LoadMacro("macros/rootlogon.C");
  initCMG("ANA");

  chain.Draw("cmgMHTPFJets.obj.pt():cmgMHTCaloJets.obj.pt()","","col");
  
  // to test the connection to the pat object through the edm::Ptr: 
  //   TCanvas c3;
  //   chain.SetAlias("source", "cmgPFJets.obj.sourcePtr().get()");
  //   chain.Draw("cmgPFJets.obj.pt() - source.pt()");
  
}


	
