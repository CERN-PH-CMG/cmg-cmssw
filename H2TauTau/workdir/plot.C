void plot(){  
  //gROOT->ProcessLine(".L ./tauMuConfig.C");
  gROOT->ProcessLine(".L ./tauMuConfig2011Reload.C");

  TauMuPlotter*analysis=tauMuConfig("");
  analysis->setQCDColor(kMagenta-10);
  analysis->setWJetsColor(kRed+2);
  analysis->setTTJetsColor(kBlue-8);
  analysis->setZMuMuColor(kRed);
  analysis->setZTauTauColor(kOrange-4);

  //raw yields
  //analysis->printRawYields("(categoryIso==1)*eventweight");


  ////////////Simple Tau fake rate ratios
  //analysis->plotTauFakeRate("taujetpt",16,20,100,"(njet==0)",50,1);//*(transversemass<15.)//*(15<=transversemass&&transversemass<30.)//*(30<=transversemass)
  //analysis->plotTauFakeRate("mupt",4,20,60,0);
  //analysis->plotTauFakeRate("taupt",4,20,60,2);
  //analysis->plotTauFakeRate("abs(taueta)",6,0,3.0);
  //analysis->plotTauFakeRate("abs(taujeteta)",6,0,3.0,0);
  //analysis->plotTauFakeRate("njet",4,-0.5,3.5);
  //analysis->plotTauFakeRate("transversemass",15,0,150,0);
  //analysis->plotTauFakeRate("cos(metphi-muphi)",10,-1,1.0001,1);

  /////////////Ratio for both mu and tau anti-Iso
  //AverageRatio values as a function of njets:
  //n=0--> 0.0246023, n=1-->0.0187078, n=2-->0.0125766, n=3-->0.0148136, n=4 -->0.0175073 +- 0.00520148

  /////there are 4 regions: 1=[tauIso,muIso] 2=[tauAntiIso,muIso]  3=[tauIso,muAntiIso] 4=[tauAntiIso,muAntiIso]
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"(muiso<0.1)","(tauisodisc>=2)",50,1);//region 1/2 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"(tauisodisc>=2)","(muiso<0.1)",50,1);//region 1/3
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"","(tauisodisc>=2)",50,1);//region 4 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"","(muiso<0.1)",50,1);//region 4

  ///Ratio for W+jets (mT>60)
  //analysis->plotTauFakeRateWJets("taujetpt",16,20,100,"(njet==2)",50,1);
  

  ////////////Inclusive plots
  Float_t c[4]={.6,.85,.42,.81};//default legend coordinates
  analysis->setSmearHistoRes(0);//default smearing for mass

  //can only do one plot at a time for now 
  //Float_t c[4]={0,0,0,0};
  //Float_t r[4]={160000,175000,0,400};
  //analysis->plot("runNumberHisto",0,"","",c,r);
  //analysis->plotInc("nvertex",1,1,-1,50,-.5,49.5," # of reco. vertices ","",c);
  //analysis->plotInc("mupt",1,1,-1,100,0,100," #mu p_{T}   (GeV)","Events / 1 GeV",c);
  //analysis->plot("muEtaHisto",2," #eta_{#mu}","Events / 0.1",c,r);
  //analysis->plot("muiso",0,1,-1,100,0,1," muon relIso(0.5)"," Events / 0.005",c,1);
  //analysis->plotInc("taupt",1,1,-1,50,0,100," #tau p_{T}   (GeV)","Events / 2 GeV",c);
  //analysis->plot("tauEtaHisto",2," #eta_{#tau}","Events / 0.1",c,r); 
  //analysis->plot("metHisto",4," MET   (GeV)","Events / 4 GeV",c,r);    
  //analysis->plotInc("nditau",1,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("metpt",1,0,-1,50,0,200," MET   (GeV)","Events / 4 GeV",c);    
  //analysis->plotInc("metphi",1,1,-1,70,-3.5,3.5," MET  phi ","",c);    
  //analysis->plotInc("transversemass",1,0,-1,50,0,200,"m_{T}   (GeV)","Events / 4 GeV",c);
  //analysis->plotInc("ditaumass",1,1,-1,60,0,300,"","m_{vis}   (GeV)","Events / 5 GeV",c,0);
  //analysis->plotSM("ditaumass",1,1,-1,60,0,300,"","m_{vis}   (GeV)","",c);
  //analysis->plotSM("transversemass",1,0,-1,50,0,200,"","m_{T}   (GeV)","Events / 4 GeV",c);
 
  /////////////inclusive SS
  //analysis->plotIncSS("ditaumass",1,1,-1,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);
  //analysis->plotIncSS("taupt",1,1,-1,8,20,100," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("muiso",-1,1,-1,100,0,2," #mu relIsoDB","",c);
  //analysis->plotIncSS("tauiso",-1,1,-1,100,0,2," #tau relIso","",c);
  //analysis->plotIncSS("tauisodisc",-1,1,-1,6,-0.5,5.5," #tau iso disc.","",c);
  //analysis->plotIncSS("transversemass",1,0,-1,30,0,150,"m_{T}   (GeV)","",c);
  //analysis->plotIncSS("nditau",1,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("metpt",1,0,-1,50,0,200," MET   (GeV)","Events / 4 GeV",c);    
  //////////////inclusive anti
  //analysis->plotInc("nditau",2,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("taupt",2,1,-1,12,0,60," #tau p_{T}   (GeV)","Events / 5 GeV",c);  
  //analysis->plotInc("metpt",2,0,-1,50,0,200," MET   (GeV)","Events / 4 GeV",c);    
  //analysis->plotInc("transversemass",2,0,-1,50,0,200,"m_{T}   (GeV)","Events / 4 GeV",c);
  //////////////inclusive SS anti
  //analysis->plotIncSS("taupt",2,1,-1,8,20,100," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,1,-1,6,0,3," #eta_{#tau}","",c);   
  //analysis->plotIncSS("nditau",2,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("ditaumass",2,1,-1,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);
  //analysis->plotIncSS("transversemass",2,0,-1,30,0,150,"m_{T}   (GeV)","",c);

  

  //////////// //////////////SM0/////////////////////////
  //analysis->plotInc("ditaumass",1,1,0,60,0,300,"","m_{vis}   (GeV)","Events / 5 GeV",c);
  //analysis->plotSM("ditaumass",1,1,0,60,0,300,"","m_{vis}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,0,8,20,100," #tau p_{T}   (GeV)","",c);  
  //analysis->plotSM("taujetpt",1,1,-1,8,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotSM("transversemass",1,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotSM("ditaumass",1,1,-1,60,0,300,"(njet==0)","m_{vis}   (GeV)","",c);
  //analysis->plotSM("mupt",1,1,-1,20,0,100,"(njet==0)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotSM("taupt",1,1,-1,20,0,100,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotSM("metpt",1,1,-1,12,0,60,"(njet==0)"," MET   (GeV)","",c);    
  /////SS
  //analysis->plotSMSS("transversemass",1,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("taujetpt",1,1,-1,16,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotSMSS("taupt",1,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  ////Anti
  //analysis->plotSM("transversemass",2,0,-1,20,0,200,"(njet==0&&muiso<0.1)","m_{T}   (GeV)","",c);
  //analysis->plotSM("transversemass",2,0,-1,20,0,200,"(njet==0&&tauisodisc>=2)","m_{T}   (GeV)","",c);
  //analysis->plotSM("transversemass",2,0,-1,20,0,200,"(njet==0&&muiso>0.1&&tauisodisc<2)","m_{T}   (GeV)","",c);
  //analysis->plotSM("taupt",2,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  ////SS anti
  //analysis->plotSMSS("transversemass",2,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("metpt",2,1,-1,12,0,60,"(njet==0)"," MET   (GeV)","",c);  
  //analysis->plotSMSS("taupt",2,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotSMSS("taujetpt",2,1,-1,16,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  

  ////////////////////////////SM1//////////////////////////
  //analysis->plotSM("transversemass",1,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  analysis->plotInc("ditaumass",1,1,1,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotSM("transversemass",1,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotSM("mupt",1,1,-1,20,0,100,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotSM("taupt",1,1,-1,20,0,100,"(njet==1)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotSM("ditaumass",1,1,-1,30,0,300,"(njet==1)","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotSM("metpt",1,1,-1,15,0,150,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotSM("taujetpt",1,1,-1,8,20,100,"(njet==1)"," #tau jet  p_{T}   (GeV)","",c);  
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotSM("(1-cos(muphi-metphi))",1,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  //analysis->plotSM("cos(tauphi-muphi)",1,1,-1,20,-1,1.001,"(njet==1)","cos(#mu_#phi-#tau_#phi)","",c);
  ////////////////////////////SM1 SS
  //analysis->plotSMSS("transversemass",1,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("taupt",1,1,1,20,0,200," #tau p_{T}   (GeV)","",c);
  //analysis->plotSMSS("ditaumass",1,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotSMSS("transversemass",1,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("mupt",1,1,-1,8,10,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotSMSS("abs(taueta)",1,0,10,6,0,3," |#eta_{#tau}| ","",c); 
  //analysis->plotSMSS("metpt",1,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotSMSS("(1-cos(muphi-metphi))",1,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  //analysis->plotSMSS("taujetpt",1,1,-1,16,20,100,"(njet==1)"," #tau jet  p_{T}   (GeV)","",c);  
  //////////////////////////////SM1 anti
  //analysis->plotSM("transversemass",2,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotSM("tauisodisc",2,1,1,6,-.5,5.5,"tau isolation discriminator ","",c,0);
  //analysis->plotSM("taupt",2,1,1,20,0,200," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotSM("taujetpt",2,1,1,10,0,100,""," #tau jet  p_{T}   (GeV)","",c);
  //analysis->plotSM("mupt",2,1,1,10,0,100,""," #mu p_{T}   (GeV)","",c);  
  //analysis->plotSM("ditaumass",2,1,1,15,0,300,"","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotSM("transversemass",2,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotSM("metpt",2,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotSM("metpt",2,3,-1,40,0,200,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotSM("mupt",2,1,-1,10,0,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotSM("(1-cos(muphi-metphi))",2,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  ////////////////////////////SM1 SS anti
  //analysis->plotSMSS("taupt",2,1,1,20,0,200," #tau p_{T}   (GeV)","",c);
  //analysis->plotSMSS("ditaumass",2,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotSMSS("transversemass",2,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("abs(taueta)",2,0,10,6,0,3," |#eta_{#tau}| ","",c); 
  //analysis->plotSMSS("transversemass",2,0,-1,15,0,150,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("metpt",2,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotSMSS("mupt",2,1,-1,10,0,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotSMSS("(1-cos(muphi-metphi))",2,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);


  ////////////////////////////SM2/////
  //analysis->plotSM("transversemass",1,0,2,15,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotSM("ditaumass",1,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotSM("svfitmass",1,1,2,11,0,330,"","m_{vis}   (GeV)","Events / 30 GeV",c);
  //analysis->plotSM("taupt",1,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotSM("mupt",1,1,2,10,0,100," #mu p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotSM("nditau",1,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotSM("transversemass",1,0,-1,20,0,200,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotSM("mupt",1,1,-1,10,0,100,"(njet==2)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotSM("taupt",1,1,-1,10,0,100,"(njet==2)","#tau  p_{T}   (GeV)","",c);
  //analysis->plotSM("ditaumass",1,1,-1,15,0,300,"(njet==2)","m_{vis}   (GeV)","Events / 20 GeV",c);
  ///////////////////////////SM2 SS///
  //analysis->plotSMSS("transversemass",1,0,2,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("ditaumass",1,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotSMSS("nditau",1,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotSMSS("transversemass",1,0,-1,15,0,150,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("abs(taueta)",1,0,20,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotSMSS("metpt",1,0,20,20,0,200," MET   (GeV)","Events / 5 GeV",c);    
  //analysis->plotSMSS("taujetpt",1,1,-1,8,20,100,"(njet==2)"," #tau jet  p_{T}   (GeV)","",c);  
  ///////////////////////////SM2 anti
  //analysis->plotSM("transversemass",2,0,2,15,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotSM("taupt",2,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotSM("ditaumass",2,1,2,15,0,300,"","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotSM("nditau",2,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotSM("transversemass",2,0,-1,15,0,150,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotSM("ditaumass",2,1,20,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  ///////////////////////////SM2 SS anti
  //analysis->plotSMSS("transversemass",2,0,2,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("nditau",2,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotSMSS("ditaumass",2,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotSMSS("taupt",2,1,2,8,20,100," #tau p_{T}   (GeV)","",c);
  //analysis->plotSMSS("abs(taueta)",2,1,2,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotSMSS("transversemass",2,0,-1,20,0,200,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotSMSS("abs(taueta)",2,0,20,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotSMSS("metpt",2,0,20,20,0,200," MET   (GeV)","Events / 5 GeV",c);    


  gROOT->ProcessLine(".q");
}

