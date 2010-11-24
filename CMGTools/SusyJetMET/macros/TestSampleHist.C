{
  // for MetDyn studies 
  gROOT->Macro("initTree.C");

  Chain RDtree("Events", "Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/*tree*.root", "RD");
  Aliases RDaliases("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/aliases.py");
  RDaliases.SetAliases( &RDtree );

  TFile RDfileRia("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/histograms.root"); 
  TFile RDfileHistos("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/histograms.root"); 
  DataSet RDdata( &RDtree, &RDfileRia, &RDfileHistos);

  double intLumi = 24;

  gROOT->Macro("DataSet_QCD_PU.C");

//   TH1 *h = QCD120to170data.DrawHist("pfJetLorentzVector/pT");
//   h->Add( QCD170to300data.DrawHist("pfJetLorentzVector/pT") );
//   h->Add( QCD300to470data.DrawHist("pfJetLorentzVector/pT") );
//   h->Add( QCD470to600data.DrawHist("pfJetLorentzVector/pT") );
//   h->Add( QCD600to800data.DrawHist("pfJetLorentzVector/pT") );

//   // integrated luminosity
//   RDdata.DrawHist("pfJetLorentzVector/PFJet/pT"); 
//   h->Draw("same");
//   gPad->SetLogy();



  Samples sam; 
  sam.SetLegendCorners(0.65,0.7,0.88,0.88);
  bool addData = true; 
  if( addData )
    sam.AddDataSet(&RDdata, "DATA"); 

  sam.AddDataSet(&QCD120to170data, "QCD"); 
  sam.AddDataSet(&QCD170to300data, "QCD"); 
  sam.AddDataSet(&QCD300to470data, "QCD"); 
  sam.AddDataSet(&QCD470to600data, "QCD"); 
  sam.AddDataSet(&QCD600to800data, "QCD"); 
//   sam.AddDataSet(&QCD800to1000data, "QCD"); 


  Styles styles;
  if( addData )
    sam.SetStyle( "DATA", false, styles.sdata1 );
  sam.SetStyle( "QCD", true, styles.spyellow );


  sam.DrawHist("mhtPFJets50LorentzVector/pT", true);
  gPad->SetLogy();
}
