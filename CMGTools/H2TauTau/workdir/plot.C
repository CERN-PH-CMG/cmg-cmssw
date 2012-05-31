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

  //for LooseMVA tau isolation
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"(muiso<0.1)","(tauisodiscmva>=1)",50,1);//region 1/2 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"(tauisodiscmva>=1)","(muiso<0.1)",50,1);//region 1/3

  ///Ratio for W+jets (mT>60)
  //analysis->plotTauFakeRateWJets("taujetpt",16,20,100,"",50,1);

  

  ////////////Inclusive plots
  Float_t c[4]={.6,.85,.42,.81};//default legend coordinates
  analysis->setSmearHistoRes(0);//default smearing for mass


  analysis->plotInc("nvtx",1,1,-1,50,-.5,49.5,""," # of reco. vertices ","",c);
  //analysis->plotInc("mupt",1,1,-1,50,0,100,""," mu pt   (GeV)","",c);
  //analysis->plotInc("mueta",1,1,-1,100,-2.5,2.5,""," muon  #eta","",c); 
  //analysis->plotInc("muiso",0,1,-1,100,0,.5,"(tauisodisc>=2)"," muon relIso(0.5)"," Events / 0.005",c,1);
  //analysis->plotInc("taupt",1,1,-1,50,0,100,""," tau pt   (GeV)","",c,0);
  //analysis->plotInc("taueta",1,1,-1,100,-2.5,2.5,""," tau  #eta","",c); 
  //analysis->plotInc("pfmetpt",1,1,-1,50,0,150,""," PFMET   (GeV)","Events / 3 GeV",c);    
  //analysis->plotInc("pfmetphi",1,1,-1,70,-3.5,3.5,""," PFMET  phi ","",c);    
  //analysis->plotInc("pftransversemass",1,0,-1,50,0,200,"","m_{T}   (GeV)","Events / 4 GeV",c);
  //analysis->plotInc("metpt",1,1,-1,50,0,150,""," MVAMET   (GeV)","Events / 3 GeV",c);    
  //analysis->plotInc("metphi",1,1,-1,70,-3.5,3.5,""," MVAMET  phi ","",c);    
  //analysis->plotInc("transversemass",1,0,-1,50,0,200,"","m_{T}   (GeV)","Events / 4 GeV",c);
  //analysis->plotInc("ditaumass",1,1,-1,40,0,200,"","m_{vis}   (GeV)","",c,0);
  //analysis->plotInc("svfitmass",1,1,-1,60,0,300,"","svfitmass   (GeV)","",c,0);
  //analysis->plotInc("njet",1,1,-1,5,-.5,4.5,""," njet ","",c,1);
  //analysis->plotInc("leadJetPt",1,1,-1,20,0,200,"(njet>=1)"," jet p_{T}   (GeV)","",c,1);
  //analysis->plotInc("leadJetEta",1,1,-1,20,-5,5,"(njet>=1)"," jet #eta","",c); 
  //analysis->plotInc("diJetMass",1,1,-1,20,0,1000,"(njet>=2)","M(jj)","",c); 
  //analysis->plotInc("abs(diJetDeltaEta)",1,1,-1,20,0,10,"(njet>=2)","#Delta#eta","",c); 
  //analysis->plotInc("leadBJetPt",1,1,-1,20,0,200,"(nbjet>=1)"," b jet p_{T}   (GeV)","",c,1);
  //analysis->plotInc("leadBJetEta",1,1,-1,20,-5,5,"(njet>=1)"," b jet #eta","",c); 

//   Float_t sigY=0;
//   sigY+=analysis->getSample("HiggsGG120")->Integral();
//   sigY+=analysis->getSample("HiggsVBF120")->Integral();
//   sigY+=analysis->getSample("HiggsVH120")->Integral();
//   cout<<"Signal yield "<<sigY<<endl;

  /////////////inclusive SS
  //analysis->plotIncSS("ditaumass",1,1,-1,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);
  //analysis->plotIncSS("taupt",1,1,-1,50,0,100,""," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("muiso",-1,1,-1,100,0,2," #mu relIsoDB","",c);
  //analysis->plotIncSS("tauiso",-1,1,-1,100,0,2," #tau relIso","",c);
  //analysis->plotIncSS("tauisodisc",-1,1,-1,6,-0.5,5.5," #tau iso disc.","",c);
  //analysis->plotIncSS("transversemass",1,0,-1,30,0,150,"","m_{T}   (GeV)","",c);
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
  //analysis->plotInc("taupt",1,1,0,8,20,100," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("taujetpt",1,1,-1,8,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,-1,60,0,300,"(njet==0)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,20,0,100,"(njet==0)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,20,0,100,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("metpt",1,1,-1,12,0,60,"(njet==0)"," MET   (GeV)","",c);    
  /////SS
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("taujetpt",1,1,-1,16,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("taupt",1,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  ////Anti
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&muiso<0.1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&tauisodisc>=2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&muiso>0.1&&tauisodisc<2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",2,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  ////SS anti
  //analysis->plotIncSS("transversemass",2,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("metpt",2,1,-1,12,0,60,"(njet==0)"," MET   (GeV)","",c);  
  //analysis->plotIncSS("taupt",2,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("taujetpt",2,1,-1,16,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  

  ////////////////////////////SM1//////////////////////////
  //analysis->plotInc("transversemass",1,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,1,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,20,0,100,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,20,0,100,"(njet==1)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("ditaumass",1,1,-1,30,0,300,"(njet==1)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("metpt",1,1,-1,15,0,150,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("taujetpt",1,1,-1,8,20,100,"(njet==1)"," #tau jet  p_{T}   (GeV)","",c);  
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotInc("(1-cos(muphi-metphi))",1,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  //analysis->plotInc("cos(tauphi-muphi)",1,1,-1,20,-1,1.001,"(njet==1)","cos(#mu_#phi-#tau_#phi)","",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet>=1)","m_{T}   (GeV)","",c);
  ////////////////////////////SS
  //analysis->plotIncSS("transversemass",1,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("taupt",1,1,1,20,0,200," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",1,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("mupt",1,1,-1,8,10,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",1,0,10,6,0,3," |#eta_{#tau}| ","",c); 
  //analysis->plotIncSS("metpt",1,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotIncSS("(1-cos(muphi-metphi))",1,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  //analysis->plotIncSS("taujetpt",1,1,-1,16,20,100,"(njet==1)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet>=1)","m_{T}   (GeV)","",c);
  //////////////////////////////anti
  //analysis->plotInc("transversemass",2,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("tauisodisc",2,1,1,6,-.5,5.5,"tau isolation discriminator ","",c,0);
  //analysis->plotInc("taupt",2,1,1,20,0,200," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("taujetpt",2,1,1,10,0,100,""," #tau jet  p_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",2,1,1,10,0,100,""," #mu p_{T}   (GeV)","",c);  
  //analysis->plotInc("ditaumass",2,1,1,15,0,300,"","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("metpt",2,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("metpt",2,3,-1,40,0,200,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("mupt",2,1,-1,10,0,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotInc("(1-cos(muphi-metphi))",2,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  ////////////////////////////SS anti
  //analysis->plotIncSS("taupt",2,1,1,20,0,200," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",2,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("transversemass",2,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,0,10,6,0,3," |#eta_{#tau}| ","",c); 
  //analysis->plotIncSS("transversemass",2,0,-1,15,0,150,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("metpt",2,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotIncSS("mupt",2,1,-1,10,0,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotIncSS("(1-cos(muphi-metphi))",2,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);


  ////////////////////////////SM2
  //analysis->plotInc("transversemass",1,0,2,15,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotInc("svfitmass",1,1,2,11,0,330,"","m_{vis}   (GeV)","Events / 30 GeV",c);
  //analysis->plotInc("taupt",1,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("mupt",1,1,2,10,0,100," #mu p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("nditau",1,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,10,0,100,"(njet==2)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,10,0,100,"(njet==2)","#tau  p_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,-1,15,0,300,"(njet==2)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("leadJetPt",1,1,-1,20,0,400,"(njet>=2)"," jet p_{T}   (GeV)","",c);
  //analysis->plotInc("leadJetEta",1,1,-1,18,-4.5,4.5,"(njet>=2)"," jet #eta","",c); 

  ///////////////////////////SS
  //analysis->plotIncSS("transversemass",1,0,2,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",1,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("nditau",1,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("transversemass",1,0,-1,15,0,150,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",1,0,20,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("metpt",1,0,20,20,0,200," MET   (GeV)","Events / 5 GeV",c);    
  //analysis->plotIncSS("taujetpt",1,1,-1,8,20,100,"(njet==2)"," #tau jet  p_{T}   (GeV)","",c);  
  ///////////////////////////anti
  //analysis->plotInc("transversemass",2,0,2,15,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",2,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("ditaumass",2,1,2,15,0,300,"","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotInc("nditau",2,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("transversemass",2,0,-1,15,0,150,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",2,1,20,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  ///////////////////////////SS anti
  //analysis->plotIncSS("transversemass",2,0,2,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("nditau",2,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("ditaumass",2,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("taupt",2,1,2,8,20,100," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,1,2,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("transversemass",2,0,-1,20,0,200,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,0,20,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("metpt",2,0,20,20,0,200," MET   (GeV)","Events / 5 GeV",c);    



  //////Tau Iso studies: LooseMVA=0.795, MediumMVA=0.884, TightMVA=0.921
  //inclusive
  //analysis->plotIncSS("transversemass",1,0,-1,30,0,150,"","m_{T}   (GeV)","",c);//Reference Loose deltaBetaCorr
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.795)","m_{T}   (GeV)","",c);//Loose MVA
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.884)","m_{T}   (GeV)","",c);//Medium MVA
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.921)","m_{T}   (GeV)","",c);//Tight MVA
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(muiso<0.1&&tauisomva>0.884)","m_{vis}   (GeV)","Events / 5 GeV",c);
  //////Muon Iso studies
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(muiso<0.1&&tauisodisc>=2)","m_{vis}   (GeV)","Events / 5 GeV",c);
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(((abs(mueta)<1.479)*(muisomva>0.96)+(abs(mueta)>1.479)*(muisomva>0.95))&&tauisodisc>=2)","m_{vis}   (GeV)","Events / 5 GeV",c);
  

  gROOT->ProcessLine(".q");
}

