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

  float CrossectionScaleFactor=0.95;

  Sample * WJetsToLNu=new Sample("WJetsToLNu",path);
  WJetsToLNu->setDataType("MC");
  WJetsToLNu->setCrossection(31314);
  WJetsToLNu->setSampleGenEvents(81345381);//V5_4_1 missing files: 0/907
  analysis->addSample(WJetsToLNu);  

  Sample * W3JetsToLNu=new Sample("W3JetsToLNu",path);
  W3JetsToLNu->setDataType("MCCat");
  W3JetsToLNu->setCrossection(304.0);//This is wrong only use for shape
  W3JetsToLNu->setSampleGenEvents(7541595);//This is wrong only use for shape
  analysis->addSample(W3JetsToLNu);  
   
  float BjetsscaleFactor=0.95;
  cout<<"WARNING applying scale factor for TTjets MC "<<CrossectionScaleFactor<<endl;
  //cout<<"WARNING applying b-jet scale factor for TTjets MC "<<BjetsscaleFactor<<endl;
  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(165.8*CrossectionScaleFactor);// 157.5=NLO theory, 165.8=CMS TOP-11-024
  TTJets->setSampleGenEvents(59613991);//  V5_4_1 missing 3/5365 files //for V5_2_0 used 59591548
  analysis->addSample(TTJets);

  cout<<"WARNING applying scale factor for Z->tau tau MC "<<CrossectionScaleFactor<<endl;
  Sample* ZToTauTau = new Sample("ZToTauTau",path);
  ZToTauTau->setDataType("MC");
  ZToTauTau->setCrossection(3048*CrossectionScaleFactor);
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

  //checked twick on June 17 2012
// 110.0 	19.84 	+15.2 	-15.0 	+7.5 	-8.1 	+7.7 	-6.9
// 115.0 	18.14 	+15.1 	-15.0 	+7.4 	-8.0 	+7.7 	-7.0
// 120.0 	16.65 	+14.8 	-14.9 	+7.2 	-7.9 	+7.6 	-7.0
// 125.0 	15.32 	+14.7 	-14.9 	+7.1 	-7.8 	+7.6 	-7.1
// 130.0 	14.16 	+14.6 	-14.9 	+7.0 	-7.7 	+7.6 	-7.2
// 135.0 	13.11 	+14.5 	-14.9 	+6.9 	-7.6 	+7.6 	-7.3
// 140.0 	12.18 	+14.4 	-14.8 	+6.8 	-7.5 	+7.6 	-7.3
// 145.0 	11.33 	+14.3 	-14.9 	+6.7 	-7.5 	+7.6 	-7.4 
  Float_t HiggsGGcross[8]={   19.84,   18.13,    16.63,    15.31,    14.12,   13.08,   12.13,   11.27};
  //Int_t   HiggsGGNevt[8]={  199988,  196002,  199981,  199986,   199985,  198571,  191549, 199981};//V2_5_0
  Int_t HiggsGGNevt=199981;//V5_1_0

// 110.0 	1.398 	2.8 	-2.3 	0.5 	-0.2 	2.3 	-2.1
// 115.0 	1.332 	2.5 	-2.3 	0.2 	-0.2 	2.3 	-2.1
// 120.0 	1.269 	2.8 	-2.5 	0.3 	-0.4 	2.4 	-2.1
// 125.0 	1.211 	2.7 	-2.4 	0.3 	-0.3 	2.5 	-2.1
// 130.0 	1.154 	2.8 	-2.3 	0.3 	-0.2 	2.5 	-2.1
// 135.0 	1.100 	3.0 	-2.2 	0.5 	-0.1 	2.6 	-2.1
// 140.0 	1.052 	2.8 	-2.2 	0.2 	-0.2 	2.6 	-2.1
// 145.0 	1.004 	3.1 	-2.1 	0.4 	-0.0 	2.7 	-2.1 
  Float_t HiggsVBFcross[8]={   1.398,   1.332,    1.269,    1.211,    1.154,   1.100,   1.052,   1.004};
  //Int_t   HiggsVBFNevt[8]={  198435,  198051,   198632,   198612,   197236,  198682,  193846, 197095};//V2_5_0
  Int_t HiggsVBFNevt=193846;//V5_1_0
  

// //WH
// 110.0 	0.8754 	4.1 	-4.5 	0.3 	-0.7 	3.8 	-3.8
// 115.0 	0.7546 	4.3 	-4.7 	0.4 	-0.8 	3.9 	-3.9
// 120.0 	0.6561 	3.8 	-4.1 	0.4 	-0.7 	3.4 	-3.4
// 125.0 	0.5729 	3.7 	-4.3 	0.2 	-0.8 	3.5 	-3.5
// 130.0 	0.5008 	3.8 	-4.3 	0.3 	-0.8 	3.5 	-3.5
// 135.0 	0.4390 	4.1 	-3.8 	0.7 	-0.4 	3.4 	-3.4
// 140.0 	0.3857 	4.0 	-4.0 	0.5 	-0.5 	3.5 	-3.5
// 145.0 	0.3406 	4.0 	-4.6 	0.2 	-0.8 	3.8 	-3.8 
// //ZH
// 110.0 	0.4721 	5.3 	-5.3 	1.2 	-1.2 	4.1 	-4.1
// 115.0 	0.4107 	5.5 	-5.4 	1.3 	-1.2 	4.2 	-4.2
// 120.0 	0.3598 	5.0 	-4.7 	1.5 	-1.2 	3.5 	-3.5
// 125.0 	0.3158 	4.9 	-5.1 	1.4 	-1.6 	3.5 	-3.5
// 130.0 	0.2778 	5.2 	-5.1 	1.5 	-1.4 	3.7 	-3.7
// 135.0 	0.2453 	5.3 	-5.0 	1.7 	-1.4 	3.6 	-3.6
// 140.0 	0.2172 	5.2 	-5.3 	1.5 	-1.6 	3.7 	-3.7
// 145.0 	0.1930 	5.8 	-5.8 	1.8 	-1.8 	4.0 	-4.0 
// //ttH
// 110.0 	0.1257 	12.1 	-18.0 	3.6 	-9.5 	8.5 	-8.5
// 115.0 	0.1106 	11.9 	-17.8 	3.5 	-9.4 	8.4 	-8.4
// 120.0 	0.09756 	11.8 	-17.8 	3.4 	-9.4 	8.4 	-8.4
// 125.0 	0.08634 	11.8 	-17.8 	3.3 	-9.3 	8.5 	-8.5
// 130.0 	0.07658 	11.6 	-17.7 	3.2 	-9.3 	8.4 	-8.4
// 135.0 	0.06810 	11.5 	-17.6 	3.1 	-9.2 	8.4 	-8.4
// 140.0 	0.06072 	11.4 	-17.6 	3.0 	-9.2 	8.4 	-8.4
// 145.0 	0.05435 	11.4 	-17.6 	2.9 	-9.1 	8.5 	-8.5 
  Float_t HiggsWHcross[8]={  0.8754,  0.7546,   0.6561,   0.5729,   0.5008,  0.4390,  0.3857,  0.3406};
  Float_t HiggsZHcross[8]={  0.4721,  0.4107,   0.3598,   0.3158,   0.2778,  0.2453,  0.2172,  0.1930};
  Float_t HiggsttHcross[8]={  0.1257,  0.1106,  0.09756,  0.08634,  0.07658, 0.06810, 0.06072, 0.05435};
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

