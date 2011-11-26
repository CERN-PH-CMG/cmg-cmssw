void plotTauMu(){  
  gROOT->ProcessLine(".x ./TauMuConfig.C");
  
  WJetsToLNu.setColor(kRed+2);
  WJetsToLNu.setLineColor(WJetsToLNu.getColor());
  WJetsToLNu.setPlotLabel("W");
  WJetsToLNu.setLegendOption("f");

  TTJets.setColor(kBlue-8);
  TTJets.setLineColor(TTJets.getColor());
  TTJets.setPlotLabel("t#bar{t}");
  TTJets.setLegendOption("f");

  ZToMuMu.setColor(kRed);//kRed+2);
  ZToMuMu.setLineColor(ZToMuMu.getColor());
  ZToMuMu.setPlotLabel("Z#rightarrow#mu^{+}#mu^{-}");
  ZToMuMu.setLegendOption("f");

  ZToTauTau.setColor(kOrange-4);
  ZToTauTau.setLineColor(ZToTauTau.getColor());
  ZToTauTau.setPlotLabel("Z#rightarrow#tau^{+}#tau^{-}");
  ZToTauTau.setLegendOption("f");
  
  ZToLJet.setColor(kRed+2);
  ZToLJet.setLineColor(ZToLJet.getColor());
  ZToLJet.setPlotLabel("Z(l+jet)");
  ZToLJet.setLegendOption("f");

//   Higgs.setColor(0);
//   Higgs.setLineColor(1);
//   Higgs.setLineStyle(2);
//   Higgs.setEffCorrFactor(MCEffCorrFactor*1);
//   Higgs.setPlotLabel("1 x SM Higgs(m=115)");
//   Higgs.setLegendOption("L");


  WJetsToLNu_SS.setColor(WJetsToLNu.getColor());
  WJetsToLNu_SS.setLineColor(WJetsToLNu.getColor());

  TTJets_SS.setColor(TTJets.getColor());
  TTJets_SS.setLineColor(TTJets.getColor());
  TTJets_SS.setLegendOption(TTJets.getLegendOption());

  ZToMuMu_SS.setColor(ZToMuMu.getColor());
  ZToMuMu_SS.setLineColor(ZToMuMu.getColor());
  ZToMuMu_SS.setLegendOption(ZToMuMu.getLegendOption());

  ZToTauTau_SS.setColor(ZToTauTau.getColor());
  ZToTauTau_SS.setLineColor(ZToTauTau.getColor());
  ZToTauTau_SS.setLegendOption(ZToTauTau.getLegendOption());

  ZToLJet_SS.setColor(ZToLJet.getColor());
  ZToLJet_SS.setLineColor(ZToLJet.getColor());
  ZToLJet_SS.setLegendOption(ZToLJet.getLegendOption());


  analysis.setQCDColor(kMagenta-10);
    
  WJetsToLNu.setPlotOrder(1);   WJetsToLNu_SS.setPlotOrder(1);
  ZToLJet.setPlotOrder(2);      ZToLJet_SS.setPlotOrder(2);
  TTJets.setPlotOrder(3);       TTJets_SS.setPlotOrder(3);
  ZToMuMu.setPlotOrder(4);      ZToMuMu_SS.setPlotOrder(4);
  ZToTauTau.setPlotOrder(5);    ZToTauTau_SS.setPlotOrder(5);
  //Higgs.setPlotOrder(6);  
 

  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}
  
  analysis.printRawYields("diTauMassHisto_massT");


  //can only do one plot at a time for now 
//   Float_t c[4]={0,0,0,0};
//   Float_t r[4]={160000,175000,0,400};
//   analysis.plot("runNumberHisto",0,"","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={-0.5,24.5,0,5000};
//   analysis.plot("nVertexHisto",1," # of reco. vertices ","",c,r);

//     Float_t c[4]={.5,.75,.42,.81};
//     Float_t r[4]={0,100,0,5000};
//     analysis.plot("muPtHisto",5," #mu p_{T}   (GeV)","Events / 2.5 GeV",c,r);

//   Float_t c[4]={.6,.85,.42,.81};
//   Float_t r[4]={-2.5,2.5,0,2000};
//   analysis.plot("muEtaHisto",2," #eta_{#mu}","Events / 0.1",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,0.5,0,8000};
//   analysis.plot("muIsoHisto_muiso",2," muon relIso()","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,100,0,5000};
//   analysis.plot("tauPtHisto",5," #tau p_{T}   (GeV)","Events / 2.5 GeV",c,r);

//   Float_t c[4]={.6,.85,.42,.81};
//   Float_t r[4]={-2.5,2.5,0,2000};
//   analysis.plot("tauEtaHisto",2," #eta_{#tau}","Events / 0.1",c,r);


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
//   Float_t r[4]={0,200,0,5000};
//   analysis.plot("transverseMassHisto_massT",4,"m_{T}   (GeV)","Events / 2 GeV",c,r);

//   Float_t c[4]={.6,.85,.42,.81};
//   //Float_t r[4]={-0.5,5.5,0,25000};
//   Float_t r[4]={-0.5,5.5,1,500000};
//   analysis.plot("njetHisto",1,"# of jets","",c,r,1);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,0,3500};
// //   Float_t c[4]={.5,.75,.42,.81};
// //   Float_t r[4]={0,350,5,1e7};
//   analysis.plot("diTauMassHisto",4,"m_{vis}   (GeV)","Events / 4 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,300,0,4000};
//   analysis.plot("diTauMassSVFitHisto",8," mass   (GeV)","Events / 8 GeV",c,r);

//    Float_t c[4]={.5,.75,.42,.81};
//    Float_t r[4]={0,400,0,3000};
//    analysis.plot("svFitCov00Histo",10,"matrix element 00","Events",c,r);

//    Float_t c[4]={.5,.75,.42,.81};
//    Float_t r[4]={0,400,0,3500};
//    analysis.plot("svFitEigen0Histo",10,"eigenvalue 0","Events",c,r);

//    Float_t c[4]={.5,.75,.42,.81};
//    Float_t r[4]={0,400,0,3500};
//    analysis.plot("svFitEigen1Histo",10,"eigenvalue 1","Events",c,r);


//    Float_t c[4]={.5,.75,.42,.81};
//    Float_t r[4]={0,4,0,30000};
//    analysis.plot("svFitConvergeHisto",1,"convergence","Events",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,199,0,22};
//   analysis.plot("diTauMassVBFHisto",20,"m_{vis}   (GeV)","Events / 20 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={300,1000,0,30};
//   analysis.plot("diJetMassVBFHisto",100,"m_{jj} (GeV)","Events / 100 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={3,10,0,60};
//   analysis.plot("diJetDeltaEtaVBFHisto",10,"|#Delta#eta_{jj}|","Events / 1",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,0,35};
//   analysis.plot("diTauMassBoostedHisto",20,"m_{vis}   (GeV)","Events / 20 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,200,0,3500};
//   analysis.plot("diTauMassInclusiveHisto",4,"m_{vis}   (GeV)","Events / 4 GeV",c,r,0);   


  gROOT->ProcessLine(".q");
}

