void plotTauMu(){  
  gROOT->ProcessLine(".x ./TauMuConfig.C");
 

  //WJetsToLNu.setPlotLabel("W+jets");
  WJetsToLNu.setPlotLabel("Electroweak");
  ZToLJet.setPlotLabel("Z(l+jet)");
  TTJets.setPlotLabel("t#bar{t}");
  ZToMuMu.setPlotLabel("Z#rightarrow#mu^{+}#mu^{-}");
  ZToTauTau.setPlotLabel("Z#rightarrow#tau^{+}#tau^{-}");

  analysis.setQCDColor(kMagenta-10);
  analysis.setWJetsColor(kRed+2);
  analysis.setTTJetsColor(kBlue-8);
  analysis.setZMuMuColor(kRed);
  analysis.setZTauTauColor(kOrange-4);
    
  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}
  
  //analysis.printRawYields("diTauMassHisto");


  //can only do one plot at a time for now 
//   Float_t c[4]={0,0,0,0};
//   Float_t r[4]={160000,175000,0,400};
//   analysis.plot("runNumberHisto",0,"","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={-0.5,29.5,0,6000};
//   analysis.plot("nVertexHisto",1," # of reco. vertices ","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,100,0,5000};
//   analysis.plot("muPtHisto",2," #mu p_{T}   (GeV)","Events / 1 GeV",c,r);

//   Float_t c[4]={.15,.4,.55,.80};
//   Float_t r[4]={-2.4,2.4,0,2200};
//   analysis.plot("muEtaHisto",2," #eta_{#mu}","Events / 0.1",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,0.5,0,13000};
//   analysis.plot("muIsoHisto_muiso",2," muon relIso(0.5)"," Events / 0.01",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,100,0,8000};
//   analysis.plot("tauPtHisto",4," #tau p_{T}   (GeV)","Events / 2 GeV",c,r);

//   Float_t c[4]={.15,.35,.55,.80};
//   Float_t r[4]={-2.4,2.4,0,2200};
//   analysis.plot("tauEtaHisto",2," #eta_{#tau}","Events / 0.1",c,r);

//   Float_t c[4]={.25,.45,.42,.81};
//   Float_t r[4]={0.0,1.5,0,2600};
//   analysis.plot("tauEoPHisto_taueop",1," #tau E_ecal/p   (GeV)","Events/0.01 ",c,r);

//   Float_t c[4]={.25,.45,.42,.81};
//   Float_t r[4]={.95,1.05,0,700};
//   analysis.plot("tauEoP1Histo_taueop",5," #tau E_ecal/p   (GeV)","Events/0.05 ",c,r);

//   Float_t c[4]={.25,.45,.42,.81};
//   Float_t r[4]={0.0,1.5,0,1600};
//   analysis.plot("tauHoPHisto_taueop",1," #tau E_hcal/p   (GeV)","Events/0.01 ",c,r);

//   Float_t c[4]={.25,.45,.42,.81};
//   Float_t r[4]={0.0,1.5,0,1000};
//   analysis.plot("tauEHoPHisto_taueop",1," #tau (E_ecal + E_hcal)/p   (GeV)","Events/0.01 ",c,r);
  
//   Float_t c[4]={.15,.35,.55,.80};
//   Float_t r[4]={-2.4,2.4,0,200};
//   analysis.plot("tauEtaEoP1Histo",10," #eta_{#tau}","Events / 0.5",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,100,0,600};
//   analysis.plot("tauPtEoP1Histo",20," #tau p_{T}   (GeV)","Events / 10 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   analysis.plot("tauIsoHisto",2," tau isolation  ","",c,0);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,150,0,5000};
//   analysis.plot("metHisto",4," MET   (GeV)","Events / 4 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,150,0,5000};
//   analysis.plot("metHisto_massT",4," MET   (GeV)","Events / 4 GeV",c,r);
  
//   Float_t c[4]={.5,.75,.48,.83};
//   Float_t r[4]={-3.2,3.2,0,1600};
//   analysis.plot("metphiHisto_massT",1," MET #phi ","",c,r);

//    Float_t c[4]={.15,.45,.42,.81};
//    Float_t r[4]={-140,40,0,6000};
//    analysis.plot("pZetaHisto_pzeta",6,"P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)","Events / 6 GeV",c,r);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,0,8000};
//   analysis.plot("transverseMassHisto_massT",4,"m_{T}   (GeV)","Events / 4 GeV",c,r);

  Float_t c[4]={.5,.75,.42,.81};
  Float_t r[4]={0,300,0,7000};
  //   Float_t c[4]={.5,.75,.42,.81};
  //   Float_t r[4]={0,350,5,1e7};
  analysis.plot("diTauMassHisto",4,"m_{vis}   (GeV)","Events / 4 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,300,0,4000};
//   analysis.plot("diTauMassSVFitHisto",4," SVFit mass   (GeV)","Events / 4 GeV",c,r);

//   ////Jets at inclusive level
//   Float_t c[4]={.55,.8,.47,.81};
//   Float_t r[4]={-0.5,5.5,0,25000};
//   //Float_t r[4]={-0.5,5.5,1,500000};
//   analysis.plot("njetHisto",1,"# of jets","",c,r,0);

//   Float_t c[4]={.55,.8,.47,.81};
//   Float_t r[4]={-0.5,5.5,0,40000};
//   //Float_t r[4]={-0.5,5.5,1,500000};
//   analysis.plot("njetLCHisto",1,"# of jets","",c,r,0);

  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,350,0,4000};
//   analysis.plot("leadJetPtHisto",10," lead jet p_{T}   (GeV)","Events / 10 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,1000,0,1000};
//   analysis.plot("diJetMassHisto",50,"m_{jj} (GeV)","Events / 50 GeV",c,r,0);

//   //////////////SM0
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,300,0,6000};
//   analysis.plot("diTauMassHisto_SM0",4,"m_{vis}   (GeV)","Events / 4 GeV",c,r,0);

//   ///////////////SM1
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,500,0,200};
//   analysis.plot("diTauMassHisto_SM1",20,"m_{vis}   (GeV)","Events / 20 GeV",c,r,0);

//   ///////////////SM2
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,300,0,30};
//   analysis.plot("diTauMassHisto_SM2",20,"m_{vis}   (GeV)","Events / 20 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={300,1000,0,30};
//   analysis.plot("diJetMassHisto_SM2",50,"m_{jj} (GeV)","Events / 50 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={3,10,0,30};
//   analysis.plot("diJetDeltaEtaHisto_SM2",10,"|#Delta#eta_{jj}|","Events / 1",c,r,0);

//   ///////////////SM9
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,0,700};
//   analysis.plot("diTauMassHisto_SM9",10,"m_{vis}   (GeV)","Events / 10 GeV",c,r,0);


  gROOT->ProcessLine(".q");
}

//   WJetsToLNu.setColor(kRed+2);
//   WJetsToLNu.setLineColor(WJetsToLNu.getColor());

//   WJetsToLNu.setLegendOption("f");

//   TTJets.setColor(kBlue-8);
//   TTJets.setLineColor(TTJets.getColor());
//   TTJets.setLegendOption("f");

//   ZToMuMu.setColor(kRed);//kRed+2);
//   ZToMuMu.setLineColor(ZToMuMu.getColor());
//   ZToMuMu.setLegendOption("f");

//   ZToTauTau.setColor(kOrange-4);
//   ZToTauTau.setLineColor(ZToTauTau.getColor());
//   ZToTauTau.setLegendOption("f");
  
//   ZToLJet.setColor(kRed+2);
//   ZToLJet.setLineColor(ZToLJet.getColor());

//   ZToLJet.setLegendOption("f");

//   Higgs.setColor(0);
//   Higgs.setLineColor(1);
//   Higgs.setLineStyle(2);
//   Higgs.setEffCorrFactor(MCEffCorrFactor*1);
//   Higgs.setPlotLabel("1 x SM Higgs(m=115)");
//   Higgs.setLegendOption("L");


//   WJetsToLNu_SS.setColor(WJetsToLNu.getColor());
//   WJetsToLNu_SS.setLineColor(WJetsToLNu.getColor());

//   TTJets_SS.setColor(TTJets.getColor());
//   TTJets_SS.setLineColor(TTJets.getColor());
//   TTJets_SS.setLegendOption(TTJets.getLegendOption());

//   ZToMuMu_SS.setColor(ZToMuMu.getColor());
//   ZToMuMu_SS.setLineColor(ZToMuMu.getColor());
//   ZToMuMu_SS.setLegendOption(ZToMuMu.getLegendOption());

//   ZToTauTau_SS.setColor(ZToTauTau.getColor());
//   ZToTauTau_SS.setLineColor(ZToTauTau.getColor());
//   ZToTauTau_SS.setLegendOption(ZToTauTau.getLegendOption());

//   ZToLJet_SS.setColor(ZToLJet.getColor());
//   ZToLJet_SS.setLineColor(ZToLJet.getColor());
//   ZToLJet_SS.setLegendOption(ZToLJet.getLegendOption());
