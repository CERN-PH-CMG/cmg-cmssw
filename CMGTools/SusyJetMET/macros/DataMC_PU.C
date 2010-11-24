{
  // for MetDyn studies 
  gROOT->Macro("initTree.C");

  Chain RDtree("Events", "Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/*tree*.root", "RD");
  Aliases RDaliases("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/aliases.py");
  RDaliases.SetAliases( &RDtree );

  TFile RDfile("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/histograms.root"); 
  DataSet RDdata( &RDtree, &RDfile);

  double intLumi = 24;

  gROOT->Macro("DataSet_QCD_PU.C");

  TH1 *h = QCD120to170data.DrawHist("pfJetLorentzVector/pT");
  h->Add( QCD170to300data.DrawHist("pfJetLorentzVector/pT") );
  h->Add( QCD300to470data.DrawHist("pfJetLorentzVector/pT") );
  h->Add( QCD470to600data.DrawHist("pfJetLorentzVector/pT") );
  h->Add( QCD600to800data.DrawHist("pfJetLorentzVector/pT") );

  // integrated luminosity
  RDdata.DrawHist("pfJetLorentzVector/PFJet/pT"); 
  h->Draw("same");
  gPad->SetLogy();



  Samples sam; 
  sam.SetLegendCorners(0.65,0.7,0.88,0.88);
  bool addData = true; 
  if( addData )
    sam.AddTree(&RDdata.GetTree(), "DATA"); 

  sam.AddTree(QCD120to170data.GetTree(), "QCD"); 
  sam.AddTree(QCD170to300data.GetTree(), "QCD"); 
  sam.AddTree(QCD300to470data.GetTree(), "QCD"); 
  sam.AddTree(QCD470to600data.GetTree(), "QCD"); 
  sam.AddTree(QCD600to800data.GetTree(), "QCD"); 
//   sam.AddTree(QCD800to1000data.GetTree(), "QCD"); 


  Styles styles;
  if( addData )
    sam.SetStyle( "DATA", false, styles.sdata1 );
  sam.SetStyle( "QCD", true, styles.spyellow );


  TCut pt("pfBaseJets.obj[0].pt()>200 && pfBaseJets.obj[1].pt()>50 && pfBaseJets.obj[2].pt()>50");
  
  TCut dphi0("abs(pfBaseJets.obj[0].phi()-pfMET.obj[0].phi())>0.3");
  TCut dphi1("abs(pfBaseJets.obj[1].phi()-pfMET.obj[0].phi())>0.3");
  TCut dphi2("abs(pfBaseJets.obj[2].phi()-pfMET.obj[0].phi())>0.3");

  TCut dphi(dphi0 && dphi1 && dphi2 );
  
  TCut RA2( pt && dphi ); 


}
