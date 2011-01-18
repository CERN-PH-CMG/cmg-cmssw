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

  bool plotData = true;   // do data plots
  bool addData = false;    // superimpose data in Sample object
  bool weighted = true;   // do weighted plots
  bool plotPFPostProcessing = true;

  //  const char* passed = "inconsMuons.@obj.size()==0 && greedyMuons.@obj.size()==0";

  // gROOT->Macro("DataSet_QCD_HotSkim_InconsistentMuon.C");
  gROOT->Macro("DataSet_QCD_HotSkim_LeptonVeto.C");
  gROOT->Macro("DataSet_Data_HotSkim_387.C");
  
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
    sam.SetTitle(";PF #slashH_{T}/Calo #slashH_{T};Number of events");
  

    TCanvas c1("c1","c1 - no filtering", 700, 700);
    sam.Draw("pfMHT30.obj.pt()/caloMHT30.obj.pt()>>h1(100,0,50)","");
    Styles::FormatPad( &c1, false, true);
    cms35( 0.3,0.75,0.6,0.88, "Simulation");
    c1->Modified();
    c1->SaveAs("mhtPFoverCalo.png");
    c1->SaveAs("mhtPFoverCalo.pdf");

    TCanvas c2("c2","c2 - inconsistent && greedy", 700, 700);
    sam.Draw("pfMHT30.obj.pt()/caloMHT30.obj.pt()>>h2(100,0,100)","inconsMuons.@obj.size()==0 && greedyMuons.@obj.size()==0");
    Styles::FormatPad( &c2, false, true);
    cms35( 0.3,0.75,0.6,0.88, "Simulation");
    c2->Modified();
    c2->SaveAs("mhtPFoverCalo_passed.png");
    c2->SaveAs("mhtPFoverCalo_passed.pdf");  
 

    sam.SetTitle(";PF #slashH_{T} (GeV);Number of events");

    TCanvas c12("c12","c12 - no filtering", 700, 700);
    sam.Draw("pfMHT30.obj.pt()>>h12(100,0,2000)","");
    Styles::FormatPad( &c12, false, true);
    cms35( 0.3,0.75,0.6,0.88, "Simulation");
    c12->Modified();
    c12->SaveAs("mhtPF.png");
    c12->SaveAs("mhtPF.pdf");
    

    TCanvas c22("c22","c22 - inconsistent and greedy", 700, 700);
    sam.Draw("pfMHT30.obj.pt()>>h22(100,0,2000)","inconsMuons.@obj.size()==0 && greedyMuons.@obj.size()==0");
    Styles::FormatPad( &c22, false, true);
    cms35( 0.3,0.75,0.6,0.88, "Simulation");
    c22->Modified();
    c22->SaveAs("mhtPF_passed.png");
    c22->SaveAs("mhtPF_passed.pdf");
    
  }

  all.SetMarkerStyle(24);
  
  TH2F hSupport("hSupport", "", 50,0,4000, 50, 0, 4000);
  Styles::FormatHisto( &hSupport );
  hSupport.SetTitle(";Calo #slashH_{T} (GeV); PF #slashH_{T} (GeV)");

  TCanvas c3("c3","c3", 700, 700);
  hSupport.Draw();
  all.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()","","same");
  cms35( 0.3,0.75,0.6,0.88, "Simulation");
  Styles::FormatPad( &c3, false, false);
  c3->SaveAs("mhtPFvsCalo.png");
  c3->SaveAs("mhtPFvsCalo.pdf");

  TCanvas c31("c31","c31 - delta pT", 700, 700);
  hSupport.Draw();
  all.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()","wrongMomentumMuons.@obj.size()==0","same");
  cms35( 0.3,0.75,0.6,0.88, "Simulation");
  Styles::FormatPad( &c31, false, false);
  c31->SaveAs("mhtPFvsCalo_deltaPt.png");
  c31->SaveAs("mhtPFvsCalo_deltaPt.pdf");

  TCanvas c32("c32","c32 - inconsistent", 700, 700);
  hSupport.Draw();
  all.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()","inconsMuons.@obj.size()==0","same");
  cms35( 0.3,0.75,0.6,0.88, "Simulation");
  Styles::FormatPad( &c32, false, false);
  c32->SaveAs("mhtPFvsCalo_inconsistent.png");
  c32->SaveAs("mhtPFvsCalo_inconsistent.pdf");

  TCanvas c33("c33","c33 - greedy", 700, 700);
  hSupport.Draw();
  all.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()","greedyMuons.@obj.size()==0","same");
  cms35( 0.3,0.75,0.6,0.88, "Simulation");
  Styles::FormatPad( &c33, false, false);
  c33->SaveAs("mhtPFvsCalo_greedy.png");
  c33->SaveAs("mhtPFvsCalo_greedy.pdf");

  
  if( plotPFPostProcessing ) {
  
    TCanvas c34("c34","c34 - MET minimizing", 700, 700);
    hSupport.Draw();
    all.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()>>h34(50,0,4000,50,0,4000)","pfPPFullSize==0","same");
    cms35( 0.3,0.75,0.6,0.88, "Simulation");
    Styles::FormatPad( &c34, false, false);
    c34->SaveAs("mhtPFvsCalo_metMin.png");
    c34->SaveAs("mhtPFvsCalo_metMin.pdf");
  }

  TCanvas c4("c4","c4 - inconsistent && greedy", 700, 700);
  hSupport.Draw();
  all.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()","inconsMuons.@obj.size()==0 && greedyMuons.@obj.size()==0","same");
  cms35( 0.3,0.75,0.6,0.88, "Simulation");
  Styles::FormatPad( &c4, false, false);
  c4->SaveAs("mhtPFvsCalo_passed.png");
  c4->SaveAs("mhtPFvsCalo_passed.pdf");


  if( plotData ) {

    data.SetMarkerStyle(24);

    TCanvas dc1("dc1","dc1", 700, 700);
    data.Draw("pfMHT30.obj.pt()/caloMHT30.obj.pt()>>dh1(100,0,300)", "pfMHT30.obj.pt()>300");
    dh1->SetTitle(";PF #slashH_{T}/Calo #slashH_{T};Number of events");
    Styles::FormatPad( &dc1, false, true);
    Styles::FormatHisto( &dh1 );
    dh1->SetLineWidth(2);
    cms35( 0.3,0.75,0.6,0.88, "Data");
    dc1->Modified();
    dc1->SaveAs("data_mhtPFoverCalo.png");
    dc1->SaveAs("data_mhtPFoverCalo.pdf");

    TCanvas dc2("dc2","dc2 - inconsistent && greedy", 700, 700);
    data.Draw("pfMHT30.obj.pt()/caloMHT30.obj.pt()>>dh2(100,0,300)", "pfMHT30.obj.pt()>300 && inconsMuons.@obj.size()==0 && greedyMuons.@obj.size()==0");
    dh2->SetTitle(";PF #slashH_{T}/Calo #slashH_{T};Number of events");
    Styles::FormatPad( &dc2, false, true);
    Styles::FormatHisto( &dh2 );
    dh2->SetLineWidth(2);
    cms35( 0.3,0.75,0.6,0.88, "Data");
    dc2->Modified();
    dc2->SaveAs("data_mhtPFoverCalo_passed.png");
    dc2->SaveAs("data_mhtPFoverCalo_passed.pdf");

    TCanvas dc3("dc3","dc3", 700, 700);
    hSupport.Draw();
    data.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()>>dh3(50,0,4000,50,0,4000)","pfMHT30.obj.pt()>300","same");
    cms35( 0.3,0.75,0.6,0.88, "Data");
    Styles::FormatPad( &dc3, false, false);
    dc3->SaveAs("data_mhtPFvsCalo.png");
    dc3->SaveAs("data_mhtPFvsCalo.pdf");

    TCanvas dc4("dc4","dc4 - inconsistent && greedy", 700, 700);
    hSupport.Draw();
    data.Draw("pfMHT30.obj.pt():caloMHT30.obj.pt()>>dh4(50,0,4000,50,0,4000)","pfMHT30.obj.pt()>300 && inconsMuons.@obj.size()==0 && greedyMuons.@obj.size()==0","same");
    // dh4->GetZaxis()->SetRangeUser(0.001, 15);
    cms35( 0.3,0.75,0.6,0.88, "Data");
    Styles::FormatPad( &dc4, false, false);
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
