TauMuPlotter * tauMuConfig(const char * name, const char * path){  

  TauMuPlotter * analysis = new TauMuPlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.11);//value from AN-11-390 v4


  //2011 Data samples
  Sample* TauPlusXMay = new Sample("TauPlusXMay",path);
  TauPlusXMay->setDataType("Data");
  TauPlusXMay->setSampleLumi(168.66);// 168.597
  analysis->addSample(TauPlusXMay);

  Sample* TauPlusXv4 = new Sample("TauPlusXv4",path);
  TauPlusXv4->setDataType("Data");
  TauPlusXv4->setSampleLumi(955.21);// 929.748
  analysis->addSample(TauPlusXv4);

  Sample* TauPlusXAug = new Sample("TauPlusXAug",path);
  TauPlusXAug->setDataType("Data");
  TauPlusXAug->setSampleLumi(389.87);// 373.349
  analysis->addSample(TauPlusXAug);

  Sample* TauPlusXOct3 = new Sample("TauPlusXOct3",path);
  TauPlusXOct3->setDataType("Data");
  TauPlusXOct3->setSampleLumi(706.70);// 658.886
  analysis->addSample(TauPlusXOct3);

  Sample* TauPlusX2011B = new Sample("TauPlusX2011B",path);
  TauPlusX2011B->setDataType("Data");
  TauPlusX2011B->setSampleLumi(2714.45);// 2511
  analysis->addSample(TauPlusX2011B);


  Sample* EmbeddedMay = new Sample("EmbeddedMay",path);
  EmbeddedMay->setDataType("Embedded");
  analysis->addSample(EmbeddedMay);

  Sample* EmbeddedV4 = new Sample("EmbeddedV4",path);
  EmbeddedV4->setDataType("Embedded");
  analysis->addSample(EmbeddedV4);

  Sample* EmbeddedAug = new Sample("EmbeddedAug",path);
  EmbeddedAug->setDataType("Embedded");
  analysis->addSample(EmbeddedAug);

  Sample* EmbeddedOct = new Sample("EmbeddedOct",path);
  EmbeddedOct->setDataType("Embedded");
  analysis->addSample(EmbeddedOct);

  Sample* Embedded2011B = new Sample("Embedded2011B",path);
  Embedded2011B->setDataType("Embedded");
  analysis->addSample(Embedded2011B);


  Sample * WJetsToLNu=new Sample("WJetsToLNu",path);
  WJetsToLNu->setDataType("MC");
  WJetsToLNu->setCrossection(31314);
  WJetsToLNu->setSampleGenEvents(81345381);//V5_4_1 missing files: 0/907    // for V5_2_0 used 81308780
  analysis->addSample(WJetsToLNu);  
   
  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(165.8);//157.5=NLO theory, 165.8=CMS TOP-11-024
  TTJets->setSampleGenEvents(59613991);//  V5_4_1 missing 3/5365 files //for V5_2_0 used 59591548
  analysis->addSample(TTJets);

  Sample* ZToTauTau = new Sample("ZToTauTau",path);
  ZToTauTau->setDataType("MC");
  cout<<"WARNING applying scale factor for Z->tau tau MC"<<endl;
  ZToTauTau->setCrossection(3048*0.96);
  ZToTauTau->setSampleGenEvents(36209629*0.9985);//V5_4_1 missing files: 4/2751 , correct to previous mc and data yields *(38781.6/35988.8)*(55192./56400.) //for V5_2_0 used 36189241
  analysis->addSample(ZToTauTau);

  Sample* ZToMuMu = new Sample("ZToMuMu",path);
  ZToMuMu->setDataType("MC");
  ZToMuMu->setCrossection(ZToTauTau->getCrossection());
  ZToMuMu->setSampleGenEvents(ZToTauTau->getSampleGenEvents());
  analysis->addSample(ZToMuMu);

  Sample* ZToLJet = new Sample("ZToLJet",path);
  ZToLJet->setDataType("MC");
  ZToLJet->setCrossection(ZToTauTau->getCrossection());
  ZToLJet->setSampleGenEvents(ZToTauTau->getSampleGenEvents());
  analysis->addSample(ZToLJet);

  Sample* WW = new Sample("WW",path);
  WW->setDataType("MC");
  WW->setCrossection(27.83);
  WW->setSampleGenEvents(4225916);
  analysis->addSample(WW);

  Sample* WZ = new Sample("WZ",path);
  WZ->setDataType("MC");
  WZ->setCrossection(10.47);
  WZ->setSampleGenEvents(4265243);
  analysis->addSample(WZ);

  Sample* ZZ = new Sample("ZZ",path);
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
  Int_t   HiggsVHNevt[8]={  220000,  220000,   2200000,   220000,   220000,   220000,  220000, 220000};//V5_1_0 , M=120 has 10x ?



  char nam[100];
  char pa[1000];
  for(Int_t i=0;i<8;i++){

    sprintf(nam,"HiggsGG%d",HiggsMass[i]);
    Sample* HiggsGG = new Sample(nam,path);
    HiggsGG->setDataType("Signal");
    HiggsGG->setCrossection(HiggsGGcross[i]*HiggsTauTauBF[i]);
    HiggsGG->setSampleGenEvents(HiggsGGNevt);//[i]);
    analysis->addSample(HiggsGG);    

    sprintf(nam,"HiggsVBF%d",HiggsMass[i]);
    Sample* HiggsVBF=new Sample(nam,path);
    HiggsVBF->setDataType("Signal");
    HiggsVBF->setCrossection(HiggsVBFcross[i]*HiggsTauTauBF[i]);
    HiggsVBF->setSampleGenEvents(HiggsVBFNevt);//[i]);
    analysis->addSample(HiggsVBF);    

    sprintf(nam,"HiggsVH%d",HiggsMass[i]);
    Sample* HiggsVH=new Sample(nam,path);
    HiggsVH->setDataType("Signal");
    HiggsVH->setCrossection((HiggsWHcross[i]+HiggsZHcross[i]+HiggsttHcross[i])*HiggsTauTauBF[i]);
    HiggsVH->setSampleGenEvents(HiggsVHNevt[i]);
    analysis->addSample(HiggsVH);    
  }

  return analysis;
}

  //TString outpath="/data/benitezj/Samples/Mar2TrigEff";
  //TString outpath="/data/benitezj/Samples/MuTauAntiIso";
  //TString outpath="/data/benitezj/Samples/Mar19MuJet";
  //TString outpath="./output/ColinMay7A";
  //TString outpath="/data/benitezj/Samples/JoseMay8";
  //TString outpath="output/JoseMay9";
  //TString outpath="/data/benitezj/Samples/JoseMay9mva";
  //TString outpath="/data/benitezj/Samples/JoseMay9PFMuon";
  //TString outpath="output2/ColinMay12MetRaw";
  //TString outpath="output/JoseMay14";
  //TString outpath="/data/benitezj/Samples/JoseMay14nosvfit";
  //TString outpath="output/JoseMay16norecoilCut";
  //TString outpath="output/JoseMay16rawJetPt";
  //TString outpath="/data/benitezj/Samples/JoseMay16chs";
  //TString outpath="/data/benitezj/Samples/JoseMay16recoil";
  //TString outpath="output/JoseMay16rawJetPt";
  //TString outpath="output/JoseMay16oldJEC";
  //TString outpath="output/JoseMay16oldJECV4";
  //TString outpath="/data/benitezj/Samples/V520JoseMay16chs";
  //TString outpath="/data/benitezj/Samples/V541May30";
  //TString outpath="output/V541TauMuJune1muIsoChHad";
  //TString outpath="/data/benitezj/Samples/V541TauMuJune2newSVFit";//categoryMT defined using mvaMET 
  //TString outpath="/data/benitezj/Samples/TauMuV541June2_mT";//categoryMT defind using PFMET
  //TString outpath="/data/benitezj/Samples/TauMuV541June2_Embed";//fixed embedded weight
  //TString outpath="/data/benitezj/Samples/TauMuV541June2_MVA";//PFMET for vbf mva
