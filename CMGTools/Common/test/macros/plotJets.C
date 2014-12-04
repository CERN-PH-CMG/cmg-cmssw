{
	
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPFAnalysesRootTools.so");
  gSystem->Load("libAnalysisDataFormatsCMGTools.so");

  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();
 

  Chain chain("Events", "testCMGTools.root");
//   chain.Draw("cmgBaseJet_cmgjet__PAT.obj.pt()");

  gROOT->LoadMacro("macros/rootlogon.C");
  initCMG("ANA", &chain);

  chain.Draw("cmgPFJet.obj.pt()");
  chain.Draw("cmgPFJet.obj.pt()","cmgPFJet.obj.getSelection(\"cuts_jetKinematics_pt\")","same");

  TCanvas c2;
  chain.Draw("cmgPFJet.obj.component(1).fraction()");
  
  // to test the connection to the pat object through the edm::Ptr: 
  //   TCanvas c3;
  //   chain.SetAlias("source", "cmgPFJet.obj.sourcePtr().get()");
  //   chain.Draw("cmgPFJet.obj.pt() - source.pt()");
  
}


	
