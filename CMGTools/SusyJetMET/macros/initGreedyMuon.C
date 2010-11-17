{
  gROOT->Macro("initTree.C");
  

  Chain chain("Events","susyJetMET_tree_sueAnn.root");
  // Chain chain("Events","susyJetMET_tree_steven.root");
//  Chain chain("Events","susyJetMET_tree_LM0.root");

 
  chain.SetAlias("muons","recoPFCandidates_allMuons__ANA");
  chain.SetAlias("badmuons","recoPFCandidates_badMuons__ANA");
  chain.SetAlias("greedyMuons","recoPFCandidates_isoMuonPFCandidateFilter__ANA");

  chain.SetAlias("muonDeltaPt","muons.obj.pt()-muons.obj.muonRef().get().pt()");
  chain.SetAlias("badMuonDeltaPt","badmuons.obj.pt()-badmuons.obj.muonRef().get().pt()");

  chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__RECO");
  chain.SetAlias("pfMet","recoPFMETs_pfMet__RECO");

  chain.Print();

  initCMG( "ANA", &chain );

//   TCanvas c1; 
//   chain.Draw("cmgPFMET.obj.pt():muonDeltaPt>>h", "","col");
//   Styles::FormatHisto( h, 0, ";p_{T}(PF)-p_{T}(recoMu); PFMET (GeV)");

//   Styles::FormatPad( &c1, false );
//   c1->SaveAs("pfmet.png");
   
//   TCanvas c2; 
//   chain.Draw("cmgPFMET.obj.pt()>>h2", "","");
//   Styles::FormatHisto( h2, 0, ";PFMET (GeV)");
   
//   Styles::FormatPad( &c2, false );
//   c2->SaveAs("pfmet_vs_deltapT.png");

//   TCanvas c3("c3","c3",700,700); 
//   chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__RECO");
//   chain.SetAlias("pfMet","recoPFMETs_pfMet__RECO");
//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h3(50,0,3200,50,0,3200)","","col");
//   Styles::FormatPad( &c3, false );
//   Styles::FormatHisto( h3, 0, ";CaloMET (GeV);PFMET (GeV)");
//   c3->SaveAs("pfmet_vs_calomet.png");
  
//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()","badmuons.@obj.size()==0","boxsame");

//   c3->SaveAs("pfmet_vs_calomet_cleaning.png");

//   TCanvas c4("c4","c4",700,700); 
//   chain.SetAlias("caloMet","recoCaloMETs_corMetGlobalMuons__RECO");
//   chain.SetAlias("pfMet","recoPFMETs_pfMet__RECO");
//   chain.Draw("pfMet.obj.pt():caloMet.obj.pt()>>h4(50,0,3200,50,0,3200)","badmuons.@obj.size()==0","col");
//   Styles::FormatPad( &c4, false );
//   Styles::FormatHisto( h4, 0, ";CaloMET (GeV);PFMET (GeV)");
//   c4->SaveAs("pfmet_vs_calomet_cleaning_nobox.png");
}
