TauMuPlotter * tauMuConfig(const char * name){  

  TauMuPlotter * analysis = new TauMuPlotter(TString("analysis")+name);
  //TString outpath="/data/benitezj/Samples/Mar19MuJet";
  //TString outpath="./output/ColinMay7A";
  TString outpath="/data/benitezj/Samples/JoseMay8";
  analysis->setOutputPath(outpath);
  analysis->setQCDOStoSSRatio(1.11);//value from AN-11-390 v4


  /////////////////////////
  //define samples    
  /////////////////////////

  //2011 Data samples
  Sample* TauPlusXMay = new Sample("TauPlusXMay",outpath.Data());
  TauPlusXMay->setDataType("Data");
  TauPlusXMay->setSampleLumi(168.597);
  analysis->addSample(TauPlusXMay);

  Sample* TauPlusXv4 = new Sample("TauPlusXv4",outpath.Data());
  TauPlusXv4->setDataType("Data");
  TauPlusXv4->setSampleLumi(929.748);
  analysis->addSample(TauPlusXv4);

  Sample* TauPlusXAug = new Sample("TauPlusXAug",outpath.Data());
  TauPlusXAug->setDataType("Data");
  TauPlusXAug->setSampleLumi(373.349);
  analysis->addSample(TauPlusXAug);

  Sample* TauPlusXOct3 = new Sample("TauPlusXOct3",outpath.Data());
  TauPlusXOct3->setDataType("Data");
  TauPlusXOct3->setSampleLumi(658.886);
  analysis->addSample(TauPlusXOct3);

  Sample* TauPlusX2011B = new Sample("TauPlusX2011B",outpath.Data());
  TauPlusX2011B->setDataType("Data");
  TauPlusX2011B->setSampleLumi(2511);
  analysis->addSample(TauPlusX2011B);


  Sample* EmbeddedMay = new Sample("EmbeddedMay",outpath.Data());
  EmbeddedMay->setDataType("Embedded");
  analysis->addSample(EmbeddedMay);

  Sample* EmbeddedV4 = new Sample("EmbeddedV4",outpath.Data());
  EmbeddedV4->setDataType("Embedded");
  analysis->addSample(EmbeddedV4);

  Sample* EmbeddedAug = new Sample("EmbeddedAug",outpath.Data());
  EmbeddedAug->setDataType("Embedded");
  analysis->addSample(EmbeddedAug);

  Sample* EmbeddedOct = new Sample("EmbeddedOct",outpath.Data());
  EmbeddedOct->setDataType("Embedded");
  analysis->addSample(EmbeddedOct);

  Sample* Embedded2011B = new Sample("Embedded2011B",outpath.Data());
  Embedded2011B->setDataType("Embedded");
  analysis->addSample(Embedded2011B);


  Sample * WJetsToLNu=new Sample("WJetsToLNu",outpath.Data());
  WJetsToLNu->setDataType("MC");
  WJetsToLNu->setCrossection(31314);
  WJetsToLNu->setSampleGenEvents(81308780);                                  
  analysis->addSample(WJetsToLNu);  
   
  Sample* TTJets = new Sample("TTJets",outpath.Data());
  TTJets->setDataType("MC");
  TTJets->setCrossection(165.8);//157.5=NLO theory, 165.8=CMS TOP-11-024
  TTJets->setSampleGenEvents(59591548); 
  analysis->addSample(TTJets);

  Sample* ZToTauTau = new Sample("ZToTauTau",outpath.Data());
  ZToTauTau->setDataType("MC");
  ZToTauTau->setCrossection(3048);
  ZToTauTau->setSampleGenEvents(36189241);// 28480417 need to revise this, this number was changing in the tau scale processings 
  analysis->addSample(ZToTauTau);

  Sample* ZToMuMu = new Sample("ZToMuMu",outpath.Data());
  ZToMuMu->setDataType("MC");
  ZToMuMu->setCrossection(ZToTauTau->getCrossection());
  ZToMuMu->setSampleGenEvents(ZToTauTau->getSampleGenEvents());
  analysis->addSample(ZToMuMu);

  Sample* ZToLJet = new Sample("ZToLJet",outpath.Data());
  ZToLJet->setDataType("MC");
  ZToLJet->setCrossection(ZToTauTau->getCrossection());
  ZToLJet->setSampleGenEvents(ZToTauTau->getSampleGenEvents());
  analysis->addSample(ZToLJet);

  Sample* WW = new Sample("WW",outpath.Data());
  WW->setDataType("MC");
  WW->setCrossection(27.83);
  WW->setSampleGenEvents(4225916);
  analysis->addSample(WW);

  Sample* WZ = new Sample("WZ",outpath.Data());
  WZ->setDataType("MC");
  WZ->setCrossection(10.47);
  WZ->setSampleGenEvents(4265243);
  analysis->addSample(WZ);

  Sample* ZZ = new Sample("ZZ",outpath.Data());
  ZZ->setDataType("MC");
  ZZ->setCrossection(4.287);
  ZZ->setSampleGenEvents(4191045);
  analysis->addSample(ZZ);



  ///Crossections taken from here:
  ///https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt7TeV#gluon_gluon_Fusion_Process 
  ///Higgs --> tau tau branching ratios taken from http://arxiv.org/abs/1101.0593 v3
  long HiggsMass[8]={110,115,120,125,130,135,140,145};
  Float_t HiggsTauTauBF[8]={ 8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};

  Float_t HiggsGGcross[8]={   19.84,   18.13,    16.63,    15.31,    14.12,   13.08,   12.13,   11.27};
  //Int_t   HiggsGGNevt[8]={  199988,  196002,  199981,  199986,   199985,  198571,  191549, 199981};//V2_5_0
  Int_t HiggsGGNevt=199981;//V5_1_0

  Float_t HiggsVBFcross[8]={   1.398,   1.332,    1.269,    1.211,    1.154,   1.100,   1.052,   1.004};
  //Int_t   HiggsVBFNevt[8]={  198435,  198051,   198632,   198612,   197236,  198682,  193846, 197095};//V2_5_0
  Int_t HiggsVBFNevt=193846;//V5_1_0
  
  Float_t HiggsWHcross[8]={  0.8754,  0.7546,   0.6561,   0.5729,   0.5008,  0.4390,  0.3857,  0.3406};
  Float_t HiggsZHcross[8]={  0.4721,  0.4107,   0.3598,   0.3158,   0.2778,  0.2453,  0.2172,  0.1930};
  Float_t HiggsttHcross[8]={  0.1257,  0.1106,  0.09756,  0.08634,  0.07658, 0.06810, 0.06072, 0.05435};
  //Int_t   HiggsVHNevt[8]={  201334,  195000,   183147,   205000,   214501,   220000,  218704, 216885};//number of events varies due to bad crab jobs
  Int_t HiggsVHNevt=214501;//V5_1_0


  char nam[100];
  char pa[1000];
  for(Int_t i=0;i<8;i++){

    sprintf(nam,"HiggsGG%d",HiggsMass[i]);
    Sample* HiggsGG = new Sample(nam,outpath.Data());
    HiggsGG->setDataType("Signal");
    HiggsGG->setCrossection(HiggsGGcross[i]*HiggsTauTauBF[i]);
    HiggsGG->setSampleGenEvents(HiggsGGNevt);//[i]);
    analysis->addSample(HiggsGG);    

    sprintf(nam,"HiggsVBF%d",HiggsMass[i]);
    Sample* HiggsVBF=new Sample(nam,outpath.Data());
    HiggsVBF->setDataType("Signal");
    HiggsVBF->setCrossection(HiggsVBFcross[i]*HiggsTauTauBF[i]);
    HiggsVBF->setSampleGenEvents(HiggsVBFNevt);//[i]);
    analysis->addSample(HiggsVBF);    

//     sprintf(nam,"HiggsVH%d",HiggsMass[i]);
//     Sample* HiggsVH=new Sample(nam,outpath.Data());
//     HiggsVH->setDataType("Signal");
//     HiggsVH->setCrossection((HiggsWHcross[i]+HiggsZHcross[i]+HiggsttHcross[i])*HiggsTauTauBF[i]);
//     HiggsVH->setSampleGenEvents(HiggsVHNevt);//[i]);
//     analysis->addSample(HiggsVH);    
  }

  return analysis;
}

