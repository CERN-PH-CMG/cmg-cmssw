{
  // initialize Fall10 QCD with Pile-Up datasets 
  // intLumi is declared in the main macro.

  Aliases allAliases("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/aliases.py");

//   Chain data("Events", "Samples/Data/HotSkim/CMGAllFilterResults/susyJetMET_tree_RA2_CMG_*.root");
//   allAliases.SetAliases( &data );

  Chain all("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_*.root", "ALL");
  allAliases.SetAliases( &all );


  Chain QCD170to300tree("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_QCD170to300.root", "QCD170to300");
  allAliases.SetAliases( &QCD170to300tree );
  TFile QCD170to300file("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_histograms_RA2_CMG_SueAnnHot_hotskim_QCD170to300.root"); 
  DataSet QCD170to300data( &QCD170to300tree, 
			   &QCD170to300file );

  QCD170to300data.SetIntLumi( intLumi );
  QCD170to300data.SetNInitial( 3220080 );

  Chain QCD300to470tree("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_QCD300to470.root", "QCD300to470");
  allAliases.SetAliases( &QCD300to470tree );
  TFile QCD300to470file("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_histograms_RA2_CMG_SueAnnHot_hotskim_QCD300to470.root"); 
  DataSet QCD300to470data( &QCD300to470tree, 
			   &QCD300to470file );

  QCD300to470data.SetIntLumi( intLumi );
  QCD300to470data.SetNInitial( 3171240 );


  Chain QCD470to600tree("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_QCD470to600.root", "QCD470to600");
  allAliases.SetAliases( &QCD470to600tree );
  TFile QCD470to600file("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_histograms_RA2_CMG_SueAnnHot_hotskim_QCD470to600.root"); 
  DataSet QCD470to600data( &QCD470to600tree, 
			   &QCD470to600file );

  QCD470to600data.SetIntLumi( intLumi );
  QCD470to600data.SetNInitial( 2019732 );

  Chain QCD600to800tree("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_QCD600to800.root", "QCD600to800");
  allAliases.SetAliases( &QCD600to800tree );
  TFile QCD600to800file("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_histograms_RA2_CMG_SueAnnHot_hotskim_QCD600to800.root"); 
  DataSet QCD600to800data( &QCD600to800tree, 
			   &QCD600to800file );

  QCD600to800data.SetIntLumi( intLumi );
  QCD600to800data.SetNInitial( 1979055 );


  Chain QCD800to1000tree("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_QCD800to1000.root", "QCD800to1000");
  allAliases.SetAliases( &QCD800to1000tree );
  TFile QCD800to1000file("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_histograms_RA2_CMG_SueAnnHot_hotskim_QCD800to1000.root"); 
  DataSet QCD800to1000data( &QCD800to1000tree, 
			   &QCD800to1000file );

  QCD800to1000data.SetIntLumi( intLumi );
  QCD800to1000data.SetNInitial( 2084404 );

  Chain QCD1000to1400tree("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_QCD1000to1400.root", "QCD1000to1400");
  allAliases.SetAliases( &QCD1000to1400tree );
  TFile QCD1000to1400file("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_histograms_RA2_CMG_SueAnnHot_hotskim_QCD1000to1400.root"); 
  DataSet QCD1000to1400data( &QCD1000to1400tree, 
			   &QCD1000to1400file );

  QCD1000to1400data.SetIntLumi( intLumi );
  QCD1000to1400data.SetNInitial( 1086966 );

  Chain QCD1400to1800tree("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_QCD1400to1800.root", "QCD1400to1800");
  allAliases.SetAliases( &QCD1400to1800tree );
  TFile QCD1400to1800file("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_histograms_RA2_CMG_SueAnnHot_hotskim_QCD1400to1800.root"); 
  DataSet QCD1400to1800data( &QCD1400to1800tree, 
			   &QCD1400to1800file );

  QCD1400to1800data.SetIntLumi( intLumi );
  QCD1400to1800data.SetNInitial( 1021510 );

  Chain QCD1800toInftree("Events", "Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_tree_RA2_CMG_SueAnnHot_hotskim_QCD1800toInf.root", "QCD1800toInf");
  allAliases.SetAliases( &QCD1800toInftree );
  TFile QCD1800toInffile("Samples/QCD_SueAnn_HotSkim/SusyPat387/CMG/susyJetMET_histograms_RA2_CMG_SueAnnHot_hotskim_QCD1800toInf.root"); 
  DataSet QCD1800toInfdata( &QCD1800toInftree, 
			   &QCD1800toInffile );

  QCD1800toInfdata.SetIntLumi( intLumi );
  QCD1800toInfdata.SetNInitial( 529360 );

}
