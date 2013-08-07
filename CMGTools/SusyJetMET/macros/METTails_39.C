{

  gROOT->Macro("initTree.C");
  

  Chain chain("Events","susyJetMET_tree_sueAnnReReco39.root");
 
  chain.SetAlias("muons","recoPFCandidates_allMuons__ANA");

  chain.SetAlias("wrongMomentumMuons","recoPFCandidates_wrongMomentumMuons__ANA");
  chain.SetAlias("greedyMuons","recoPFCandidates_myGreedyMuons__ANA");

  chain.SetAlias("muonDeltaPt","muons.obj.pt()-muons.obj.muonRef().get().pt()");
  chain.SetAlias("badMuonDeltaPt","wrongMomentumMuons.obj.pt()-wrongMomentumMuons.obj.muonRef().get().pt()");

  chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__REPROD");
  chain.SetAlias("pfMet","recoPFMETs_pfMet__REPROD");

  chain.SetAlias("caloMetOld","recoCaloMETs_corMetGlobalMuons__RECO");
  chain.SetAlias("pfMetOld","recoPFMETs_pfMet__RECO");

//   initCMG( "ANA", &chain );

  TCanvas c1("c1","c1 - MET vs Delta pT"); 
  chain.Draw("pfMet[0].obj.pt():muonDeltaPt>>h", "","col");
  Styles::FormatHisto( h, 0, ";p_{T}(PF)-p_{T}(recoMu); PFMET (GeV)");

  Styles::FormatPad( &c1, false );
  c1->SaveAs("pfmet.png");
   
  TCanvas c2("c2","c2 - MET, no cleaning"); 
  chain.Draw("pfMet.obj.pt()>>h2", "","");
  Styles::FormatHisto( h2, 0, ";PFMET (GeV)");
   
  Styles::FormatPad( &c2, false );
  c2->SaveAs("pfmet.png");

  TCanvas c3("c3","c3 - no cleaning, 38 (box) vs 39 (col)",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h3(50,0,2200,50,0,2200)","","col");
  Styles::FormatPad( &c3, false );
  Styles::FormatHisto( h3, 0, ";CaloMET (GeV);PFMET (GeV)");
  chain.Draw("pfMetOld.obj.pt():caloMetOld.obj.pt()","","boxsame");
  c3->SaveAs("pfmet_vs_calomet_39vs38.png");
  
  TCanvas c3b("c3b","c3b - no cleaning (col) vs full cleaning (box)",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h3b(50,0,2200,50,0,2200)","","col");
  Styles::FormatPad( &c3b, false );
  Styles::FormatHisto( h3b, 0, ";CaloMET (GeV);PFMET (GeV)");
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()","wrongMomentumMuons.@obj.size()==0 && greedyMuons.@obj.size()==0","boxsame");
  c3b->SaveAs("pfmet_vs_calomet_fullMuonCleaningEffect.png");
  

  // greedy cleaning
  
  TCanvas c4a("c4a","c4a - no cleaning (col) vs greedy cleaning (box)",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h4a(50,0,2200,50,0,2200)","","col");
  Styles::FormatPad( &c4a, false );
  Styles::FormatHisto( h4a, 0, ";CaloMET (GeV);PFMET (GeV)");
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()","greedyMuons.@obj.size()==0","boxsame");
  c4a->SaveAs("pfmet_vs_calomet_greedyCleaningEffect.png");
  
  TCanvas c4b("c4b","c4b - greedy cleaning",700,700); 
  chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h4b(50,0,2200,50,0,2200)","greedyMuons.@obj.size()==0","col");
  Styles::FormatPad( &c4b, false );
  Styles::FormatHisto( h4b, 0, ";CaloMET (GeV);PFMET (GeV)");
  c4b->SaveAs("pfmet_vs_calomet_greedyCleaning.png");
  

  

//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()","wrongMomentumMuons.@obj.size()==0 && greedyMuons.@obj.size()==0","boxsame");

//   c3->SaveAs("pfmet_vs_calomet_fullMuonCleaningEffect.png");

//   TCanvas c4("c4","c4 - wrong momentum muons filtered",700,700); 
//   chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__RECO");
//   chain.SetAlias("pfMet","recoPFMETs_pfMet__RECO");
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


