{
  // for MetDyn studies 
  gROOT->Macro("initTree.C");
  gROOT->LoadMacro("cms35.C");

//   Chain RDtree("Events", "Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/*tree*.root", "RD");
//   Aliases RDaliases("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/MHT/aliases.py");
//   RDaliases.SetAliases( &RDtree );

//   TFile RDfile("Samples/MultiJetRun2010BPromptReco3/NoMETSkim/histograms.root"); 
//   DataSet RDdata( &RDtree, &RDfile);

  double intLumi = 35;

  bool plotData = false;   // do data plots
  bool addData = false;    // superimpose data in Sample object
  bool weighted = false;   // do weighted plots

  gROOT->Macro("DataSet_QCD_HotSkim_InconsistentMuon.C");
  
  // gROOT->Macro("DataSet_QCD_HotSkim_UnWeighted.C");

  Styles styles;

//   TH1 *h = QCD120to170data.DrawHist("pfJetLorentzVector/pT");
//   h->Add( QCD170to300data.DrawHist("pfJetLorentzVector/pT") );
//   h->Add( QCD300to470data.DrawHist("pfJetLorentzVector/pT") );
//   h->Add( QCD470to600data.DrawHist("pfJetLorentzVector/pT") );
//   h->Add( QCD600to800data.DrawHist("pfJetLorentzVector/pT") );

  // integrated luminosity
//   RDdata.DrawHist("pfJetLorentzVector/PFJet/pT"); 
//   h->Draw("same");

  Samples sam; 
  sam.SetLegendCorners(0.65,0.8,0.8,0.88);

  if(weighted) {

    if( addData ) {
      sam.AddTree(&RDdata.GetTree(), "DATA"); 
      sam.SetStyle( "DATA", false, styles.sdata1 );
    }
    
    //   sam.AddTree(QCD120to170data.GetTree(), "QCD"); 
    sam.AddTree(QCD170to300data.GetTree(), "QCD"); 
    sam.AddTree(QCD300to470data.GetTree(), "QCD"); 
    sam.AddTree(QCD470to600data.GetTree(), "QCD"); 
    sam.AddTree(QCD600to800data.GetTree(), "QCD"); 
    sam.AddTree(QCD800to1000data.GetTree(), "QCD"); 
    sam.AddTree(QCD1000to1400data.GetTree(), "QCD"); 
    sam.AddTree(QCD1400to1800data.GetTree(), "QCD"); 
    sam.AddTree(QCD1800toInfdata.GetTree(), "QCD"); 

    sam.SetStyle( "QCD", true, styles.spblue );
    sam.SetTitle(";PF MHT/Calo MHT;Number of events");
  

    TCanvas c1("c1","c1", 700, 700);
    sam.Draw("pfMHT30.obj.pt()/caloMHT30.obj.pt()>>h1(100,0,50)","");
    Styles::FormatPad( &c1, false, true);
    cms35( 0.3,0.75,0.6,0.88, "Simulation");
    c1->Modified();
    c1->SaveAs("mhtPFoverCalo.png");
    c1->SaveAs("mhtPFoverCalo.pdf");
    
    TCanvas c2("c2","c2", 700, 700);
    sam.Draw("pfMHT30.obj.pt()/caloMHT30.obj.pt()>>h2(100,0,50)","passed");
    Styles::FormatPad( &c2, false, true);
    cms35( 0.3,0.75,0.6,0.88, "Simulation");
    c2->Modified();
    c2->SaveAs("mhtPFoverCalo_passedAll.png");
    c2->SaveAs("mhtPFoverCalo_passedAll.pdf");  
  }
  
  TCanvas c3("c3","c3", 700, 700);
  all.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()>>h3(50,0,4000,50,0,4000)","","col");
  h3->SetTitle(";Calo MHT (GeV); PF MHT (GeV)");
  cms35( 0.3,0.75,0.6,0.88, "Simulation");
  Styles::FormatPad( &c3, false, false);
  Styles::FormatHisto( &h3 );
  c3->SaveAs("mhtPFvsCalo.png");
  c3->SaveAs("mhtPFvsCalo.pdf");

  TCanvas c4("c4","c4", 700, 700);
  all.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()>>h4(50,0,4000,50,0,4000)","passed","col");
  h4->SetTitle(";Calo MHT (GeV); PF MHT (GeV)");
  cms35( 0.3,0.75,0.6,0.88, "Simulation");
  Styles::FormatPad( &c4, false, false);
  Styles::FormatHisto( &h4 );
  c4->SaveAs("mhtPFvsCalo_passed.png");
  c4->SaveAs("mhtPFvsCalo_passed.pdf");


  if( plotData ) {

    TCanvas dc1("dc1","dc1", 700, 700);
    data.Draw("pfMHT30.obj.pt()/caloMHT30.obj.pt()>>dh1(100,0,300)", "pfMHT30.obj.pt()>300");
    dh1->SetTitle(";PF MHT/Calo MHT;Number of events");
    Styles::FormatPad( &dc1, false, true);
    Styles::FormatHisto( &dh1 );
    dh1->SetLineWidth(2);
    cms35( 0.3,0.75,0.6,0.88, "Data");
    dc1->Modified();
    dc1->SaveAs("data_mhtPFoverCalo.png");
    dc1->SaveAs("data_mhtPFoverCalo.pdf");

    TCanvas dc2("dc2","dc2", 700, 700);
    data.Draw("pfMHT30.obj.pt()/caloMHT30.obj.pt()>>dh2(100,0,300)", "pfMHT30.obj.pt()>300 && passed");
    dh2->SetTitle(";PF MHT/Calo MHT;Number of events");
    Styles::FormatPad( &dc2, false, true);
    Styles::FormatHisto( &dh2 );
    dh2->SetLineWidth(2);
    cms35( 0.3,0.75,0.6,0.88, "Data");
    dc2->Modified();
    dc2->SaveAs("data_mhtPFoverCalo_passed.png");
    dc2->SaveAs("data_mhtPFoverCalo_passed.pdf");

    TCanvas dc3("dc3","dc3", 700, 700);
    data.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()>>dh3(50,0,4000,50,0,4000)","pfMHT30.obj.pt()>300","col");
    dh3->SetTitle(";Calo MHT (GeV); PF MHT (GeV)");
    dh3->GetZaxis()->SetRangeUser(0.001, 15);
    cms35( 0.3,0.75,0.6,0.88, "Data");
    Styles::FormatPad( &dc3, false, false);
    Styles::FormatHisto( &dh3 );
    dc3->SaveAs("data_mhtPFvsCalo.png");
    dc3->SaveAs("data_mhtPFvsCalo.pdf");

    TCanvas dc4("dc4","dc4", 700, 700);
    data.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()>>dh4(50,0,4000,50,0,4000)","pfMHT30.obj.pt()>300 && passed","col");
    dh4->SetTitle(";Calo MHT (GeV); PF MHT (GeV)");
    dh4->GetZaxis()->SetRangeUser(0.001, 15);
    cms35( 0.3,0.75,0.6,0.88, "Data");
    Styles::FormatPad( &dc4, false, false);
    Styles::FormatHisto( &dh4 );
    dc4->SaveAs("data_mhtPFvsCalo_passed.png");
    dc4->SaveAs("data_mhtPFvsCalo_passed.pdf");


  }

  TCut pt("pfBaseJets.obj[0].pt()>200 && pfBaseJets.obj[1].pt()>50 && pfBaseJets.obj[2].pt()>50");
  
  TCut dphi0("abs(pfBaseJets.obj[0].phi()-pfMHT30.obj[0].phi())>0.3");
  TCut dphi1("abs(pfBaseJets.obj[1].phi()-pfMHT30.obj[0].phi())>0.3");
  TCut dphi2("abs(pfBaseJets.obj[2].phi()-pfMHT30.obj[0].phi())>0.3");

  TCut dphi(dphi0 && dphi1 && dphi2 );
  
  TCut RA2( pt && dphi ); 


}
