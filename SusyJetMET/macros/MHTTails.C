{

  gROOT->Macro("initTree.C");
  

//   Chain chain("Events","AllFilters/susyJetMET_tree_RA2_CMG.root");
  Chain chain("Events","SusyPatNoPFMuonFiltering/susyJetMET_tree_RA2_CMG_newMHT.root");
  // Chain chain("Events","susyJetMET_tree_sueAnn_hot_patrickFiltered.root");
  // Chain chain("Events","susyJetMET_tree_steven.root");
  //  Chain chain("Events","susyJetMET_tree_LM0.root");

 
  chain.SetAlias("muons","recoPFCandidates_allMuons__ANA");
  chain.SetAlias("wrongMomentumMuons","recoPFCandidates_largeDeltaPtMuons__ANA");
  chain.SetAlias("greedyMuons","recoPFCandidates_greedyMuons__ANA");
  
  chain.SetAlias("pfJets","cmgPFJets_cmgPFJetLead__ANA");
  chain.SetAlias("pfMET","cmgBaseMETs_cmgMETPFCandidates__ANA");

  chain.SetAlias("muonDeltaPt","muons.obj.pt()-muons.obj.muonRef().get().pt()");
  chain.SetAlias("badMuonDeltaPt","wrongMomentumMuons.obj.pt()-wrongMomentumMuons.obj.muonRef().get().pt()");

  chain.SetAlias("pfMht","cmgBaseMETs_cmgMHTPFJets30__ANA");
  chain.SetAlias("caloMht","cmgBaseMETs_cmgMHTCaloJets50__ANA");

  initCMG( "ANA", &chain );


  TCanvas c1("c1","c1 - no filter",700,700); 
  chain.Draw("pfMht.obj.pt():caloMht.obj.pt()>>h1(50,0,4000,50,0,4000)","","col");
  Styles::FormatPad( &c1, false );
  Styles::FormatHisto( h1, 0, ";CaloMHT (GeV);PFMHT (GeV)");
  c1->SaveAs("pfmht_vs_calomht_noFilter.png");
  c1->SaveAs("pfmht_vs_calomht_noFilter.pdf");

  TCanvas c2("c2","c2 - delta pT filter",700,700); 
  chain.Draw("pfMht.obj.pt():caloMht.obj.pt()>>h2(50,0,4000,50,0,4000)","wrongMomentumMuons.@obj.size()==0","col");
  Styles::FormatPad( &c2, false );
  Styles::FormatHisto( h2, 0, ";CaloMHT (GeV);PFMHT (GeV)");
  c2->SaveAs("pfmht_vs_calomht_deltapt.png");
  c2->SaveAs("pfmht_vs_calomht_deltapt.pdf");

  TCanvas c3("c3","c3 - delta pT, greedy filters",700,700); 
  chain.Draw("pfMht.obj.pt():caloMht.obj.pt()>>h3(50,0,4000,50,0,4000)","wrongMomentumMuons.@obj.size()==0 && greedyMuons.@obj.size()==0","col");
  Styles::FormatPad( &c3, false );
  Styles::FormatHisto( h3, 0, ";CaloMHT (GeV);PFMHT (GeV)");
  c3->SaveAs("pfmht_vs_calomht_deltapt_greedy.png");
  c3->SaveAs("pfmht_vs_calomht_deltapt_greedy.pdf");

  TCanvas c4("c4","c4 - all filters",700,700); 
  chain.Draw("pfMht.obj.pt():caloMht.obj.pt()>>h4(50,0,4000,50,0,4000)","edmTriggerResults_TriggerResults__ANA.obj.accept()","col");
  Styles::FormatPad( &c4, false );
  Styles::FormatHisto( h4, 0, ";CaloMHT (GeV);PFMHT (GeV)");
  c4->SaveAs("pfmht_vs_calomht_allFilters.png");
  c4->SaveAs("pfmht_vs_calomht_allFilters.pdf");

  TCanvas c5("c5","c5 - PF MHT, no filter",700,700); 
  chain.Draw("pfMht.obj.pt()>>h5(100,0,13000)","","");
  Styles::FormatPad( &c5, false, true );
  h5->GetXaxis()->SetNdivisions(10);
  Styles::FormatHisto( h5, 0, ";PF MHT (GeV)");
  c5->SaveAs("pfmht_noFilter.png");
  c5->SaveAs("pfmht_noFilter.pdf");

  Styles styles;
  TCanvas c6("c6","c6 - PF MHT / Calo MHT, full filtering effect",700,700); 
  chain.Draw("pfMht.obj.pt()/caloMht.obj.pt()>>h6a(250,0,250)","","");
  chain.Draw("pfMht.obj.pt()/caloMht.obj.pt()>>h6b(250,0,250)","edmTriggerResults_TriggerResults__ANA.obj.accept()","same");
  Styles::FormatPad( &c6, false, true );
  Styles::FormatHisto( h6a, 0, ";PF MHT / Calo MHT");
  Styles::FormatHisto( h6b, styles.spred, ";PF MHT / CaloMHT");
  c6->SaveAs("mhtPFoverCalo.png");
  c6->SaveAs("mhtPFoverCalo.pdf");


  
} 


