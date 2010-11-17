{

  gROOT->Macro("initTree.C");
  

  Chain chain("Events","NewDaniele/susyJetMET_tree_RA2_CMG.root");
 
  chain.SetAlias("muons","recoPFCandidates_allMuons__ANA");

  chain.SetAlias("wrongMomentumMuons","recoPFCandidates_wrongMomentumMuons__ANA");
  chain.SetAlias("greedyMuons","recoPFCandidates_myGreedyMuons__ANA");

  chain.SetAlias("muonDeltaPt","muons.obj.pt()-muons.obj.muonRef().get().pt()");
  chain.SetAlias("badMuonDeltaPt","wrongMomentumMuons.obj.pt()-wrongMomentumMuons.obj.muonRef().get().pt()");

  chain.SetAlias("caloMetOld","recoCaloMETs_corMetGlobalMuons__RECO");
  chain.SetAlias("pfMetOld","recoPFMETs_pfMet__RECO");


  TCanvas c1("c1","c1 - no filter",700,700); 
  chain.Draw("pfMetOld.obj.pt():caloMetOld.obj.pt()>>h1(50,0,4000,50,0,4000)","","col");
  Styles::FormatPad( &c1, false );
  Styles::FormatHisto( h1, 0, ";CaloMET (GeV);PFMET (GeV)");
  c1->SaveAs("pfmet_vs_calomet_noFilter_daniele_old.png");
  
  chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__REPROD");
  chain.SetAlias("pfMet","recoPFMETs_pfMet__REPROD");

  TCanvas c2("c2","c2 - no filter",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h2(50,0,4000,50,0,4000)","","col");
  Styles::FormatPad( &c2, false );
  Styles::FormatHisto( h2, 0, ";CaloMET (GeV);PFMET (GeV)");
  c2->SaveAs("pfmet_vs_calomet_noFilter_daniele_new.png");
  
  
} 


