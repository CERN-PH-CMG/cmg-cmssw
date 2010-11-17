{
	
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPFAnalysesRootTools.so");
  gSystem->Load("libAnalysisDataFormatsCMGTools.so");

  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();

  gROOT->Macro("initTree.C");
  

  Chain chain("Events", "tree.root");

  initCMG("ANA", &chain);

  TCanvas c1;
  chain.Draw("cmgPFBaseJets.obj.pt()");
  TCanvas c2;
  chain.Draw("cmgPFJets.obj.component(4).fraction()");      
//   chain.Draw("cmgPFJet.obj.pt()","cmgPFJet.obj.getSelection(\"cuts_jetKinematics_pt\")","same");

}


	
