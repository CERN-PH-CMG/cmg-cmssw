{

  gROOT->Macro("initTree.C");
  

//   Chain chain("Events","AllFilters/susyJetMET_tree_RA2_CMG.root");
  Chain chain("Events","SusyPatNoPFMuonFiltering/susyJetMET_tree_RA2_CMG_noSel.root");
  // Chain chain("Events","susyJetMET_tree_sueAnn_hot_patrickFiltered.root");
  // Chain chain("Events","susyJetMET_tree_steven.root");
  //  Chain chain("Events","susyJetMET_tree_LM0.root");

 
  chain.SetAlias("muons","recoPFCandidates_allMuons__ANA");
  chain.SetAlias("wrongMomentumMuons","recoPFCandidates_wrongMomentumMuons__ANA");
  chain.SetAlias("greedyMuons","recoPFCandidates_myGreedyMuons__ANA");

  chain.SetAlias("muonDeltaPt","muons.obj.pt()-muons.obj.muonRef().get().pt()");
  chain.SetAlias("badMuonDeltaPt","wrongMomentumMuons.obj.pt()-wrongMomentumMuons.obj.muonRef().get().pt()");

//   chain.SetAlias("caloMet","patMETs_patMETsAK5Calo__PAT2");
//   chain.SetAlias("pfMet","patMETs_patMETsPF__PAT2");
  chain.SetAlias("pfMet","recoPFMETs_pfMet__RECO");

  chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__RECO");
  chain.SetAlias("caloMetOld","recoCaloMETs_corMetGlobalMuons__RECO");
  chain.SetAlias("pfMetOld","recoPFMETs_pfMet__RECO");

  initCMG( "ANA", &chain );

//   TCanvas c1; 
//   chain.Draw("cmgPFMET.obj.pt():muonDeltaPt>>h", "","col");
//   Styles::FormatHisto( h, 0, ";p_{T}(PF)-p_{T}(recoMu); PFMET (GeV)");

//   Styles::FormatPad( &c1, false );
//   c1->SaveAs("pfmet.png");
   
//   TCanvas c2; 
//   chain.Draw("pfMet.obj.pt()>>h2", "","");
//   Styles::FormatHisto( h2, 0, ";PFMET (GeV)");
   
//   Styles::FormatPad( &c2, false );
//   c2->SaveAs("pfmet_vs_deltapT.png");

//   TCanvas c3("c3","c3 - full filtering effect",700,700); 
// //   chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__RECO");
// //   chain.SetAlias("pfMet","recoPFMETs_pfMet__RECO");
//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h3(50,0,2200,50,0,2200)","","col");
//   Styles::FormatPad( &c3, false );
//   Styles::FormatHisto( h3, 0, ";CaloMET (GeV);PFMET (GeV)");
//   c3->SaveAs("pfmet_vs_calomet.png");
  
//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()","wrongMomentumMuons.@obj.size()==0 && greedyMuons.@obj.size()==0","boxsame");

//   c3->SaveAs("pfmet_vs_calomet_fullMuonCleaningEffect.png");

  TCanvas c1("c1","c1 - no filter",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h1(50,0,4000,50,0,4000)","","col");
  Styles::FormatPad( &c1, false );
  Styles::FormatHisto( h1, 0, ";CaloMET (GeV);PFMET (GeV)");
  c1->SaveAs("pfmet_vs_calomet_noFilter.png");

  TCanvas c2("c2","c2 - delta pT filter",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h2(50,0,4000,50,0,4000)","wrongMomentumMuons.@obj.size()==0","col");
  Styles::FormatPad( &c2, false );
  Styles::FormatHisto( h2, 0, ";CaloMET (GeV);PFMET (GeV)");
  c2->SaveAs("pfmet_vs_calomet_deltapt.png");

  TCanvas c3("c3","c3 - delta pT, greedy filters",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h3(50,0,4000,50,0,4000)","wrongMomentumMuons.@obj.size()==0 && greedyMuons.@obj.size()==0","col");
  Styles::FormatPad( &c3, false );
  Styles::FormatHisto( h3, 0, ";CaloMET (GeV);PFMET (GeV)");
  c3->SaveAs("pfmet_vs_calomet_deltapt_greedy.png");

  TCanvas c4("c4","c4 - all filters",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h4(50,0,4000,50,0,4000)","edmTriggerResults_TriggerResults__ANA.obj.accept()","col");
  Styles::FormatPad( &c4, false );
  Styles::FormatHisto( h4, 0, ";CaloMET (GeV);PFMET (GeV)");
  c4->SaveAs("pfmet_vs_calomet_allFilters.png");


//   TCanvas c4("c4","c4 - wrong momentum muons filtered",700,700); 
// //   chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__RECO");
// //   chain.SetAlias("pfMet","recoPFMETs_pfMet__RECO");
//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h4(50,0,2200,50,0,2200)","wrongMomentumMuons.@obj.size()==0","col");
//   Styles::FormatPad( &c4, false );
//   Styles::FormatHisto( h4, 0, ";CaloMET (GeV);PFMET (GeV)");
//   c4->SaveAs("pfmet_vs_calomet_noBad.png");
  
//   TCanvas c5("c5","c5 - all bad muons filtered",700,700); 
//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h5(50,0,2200,50,0,2200)","wrongMomentumMuons.@obj.size()==0 && greedyMuons.@obj.size()==0","col");
//   Styles::FormatPad( &c5, false );
//   Styles::FormatHisto( h5, 0, ";CaloMET (GeV);PFMET (GeV)");
//   c5->SaveAs("pfmet_vs_calomet_noBad_noGreedy.png");
  
//   TCanvas c6("c6","c6 - greedy muons filtered",700,700); 
//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h6(50,0,2200,50,0,2200)","greedyMuons.@obj.size()==0","col");
//   Styles::FormatPad( &c6, false );
//   Styles::FormatHisto( h6, 0, ";CaloMET (GeV);PFMET (GeV)");
//   c6->SaveAs("pfmet_vs_calomet_noGreedy.png");
  
  
} 


