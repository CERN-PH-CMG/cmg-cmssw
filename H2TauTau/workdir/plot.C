void plot(){  
  gROOT->ProcessLine(".L ./tauMuConfig.C");

  TauMuPlotter*analysis=tauMuConfig("");
  analysis->setQCDColor(kMagenta-10);
  analysis->setWJetsColor(kRed+2);
  analysis->setTTJetsColor(kBlue-8);
  analysis->setZMuMuColor(kRed);
  analysis->setZTauTauColor(kOrange-4);

  //can only do one plot at a time for now 
//   Float_t c[4]={0,0,0,0};
//   Float_t r[4]={160000,175000,0,400};
//   analysis->plot("runNumberHisto",0,"","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={-0.5,29.5,0,6000};
//   analysis->plot("nVertexHisto",0,1," # of reco. vertices ","",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,100,0,5000};
//   analysis->plot("muPtHisto",2," #mu p_{T}   (GeV)","Events / 1 GeV",c,r);

//   Float_t c[4]={.15,.4,.55,.80};
//   Float_t r[4]={-2.4,2.4,0,2200};
//   analysis->plot("muEtaHisto",2," #eta_{#mu}","Events / 0.1",c,r);

//  Float_t c[4]={.5,.75,.42,.81};
//  analysis->plot("muiso",0,1,-1,100,0,1," muon relIso(0.5)"," Events / 0.005",c,1);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,100,0,4000};
//   analysis->plot("tauPtHisto",2," #tau p_{T}   (GeV)","Events / 1 GeV",c,r);

//   Float_t c[4]={.15,.35,.55,.80};
//   Float_t r[4]={-2.4,2.4,0,2200};
//   analysis->plot("tauEtaHisto",2," #eta_{#tau}","Events / 0.1",c,r);
 
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,150,0,8000};
//   analysis->plot("metHisto",4," MET   (GeV)","Events / 4 GeV",c,r);
    
//  Float_t c[4]={.5,.75,.42,.81};
//  analysis->plot("transversemass",1,0,-1,50,0,200,"m_{T}   (GeV)","Events / 4 GeV",c);

//   Float_t c[4]={.5,.75,.42,.81};
//   analysis->plot("ditaumass",1,1,-1,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,350,0,5000};
//   analysis->plot("diTauMassSVFitHisto",0,5," mass   (GeV)","Events / 5 GeV",c,r);


//   Float_t c[4]={.55,.8,.47,.81};
//   Float_t r[4]={-0.5,5.5,0,40000};
//   //Float_t r[4]={-0.5,5.5,1,500000};
//   analysis->plot("njetLCHisto",0,1,"# of jets","",c,r,0);
  
//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,350,0,4000};
//   analysis->plot("leadJetPtHisto",10," lead jet p_{T}   (GeV)","Events / 10 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,1000,0,1000};
//   analysis->plot("diJetMassHisto",50,"m_{jj} (GeV)","Events / 50 GeV",c,r,0);


//    //////////////SM0
//   Float_t c[4]={.5,.75,.42,.81};
//   analysis->plot("ditaumass",1,1,0,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);

//   ///////////////SM1

//    analysis->setSmearHistoRes(15.0);
//    Float_t c[4]={.5,.75,.42,.81};
//    analysis->plot("ditaumass",1,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,350,0,250};
//   analysis->plot("diTauMassSVFitHisto_SM1",1,25," mass   (GeV)","Events / 25 GeV",c,r);


  ///////////////SM2
//   analysis->setSmearHistoRes(15.0);
//   Float_t c[4]={.5,.75,.42,.81};
//   analysis->plot("ditaumass",1,1,2,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);

//   Float_t c[4]={.5,.75,.42,.81};
//   analysis->plot("taupt",1,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);

  Float_t c[4]={.5,.75,.42,.81};
  analysis->plot("mupt",1,1,2,10,0,100," #mu p_{T}   (GeV)","Events / 10 GeV",c);


//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={0,350,0,30};
//   analysis->plot("diTauMassSVFitHisto_SM2",2,25," mass   (GeV)","Events / 25 GeV",c,r);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={300,1000,0,30};
//   analysis->plot("diJetMassHisto_SM2",2,50,"m_{jj} (GeV)","Events / 50 GeV",c,r,0);

//   Float_t c[4]={.5,.75,.42,.81};
//   Float_t r[4]={3,10,0,30};
//   analysis->plot("diJetDeltaEtaHisto_SM2",2,10,"|#Delta#eta_{jj}|","Events / 1",c,r,0);


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
