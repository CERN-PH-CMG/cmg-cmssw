#include "TauMuPlotter.h"
#include "Sample.h"
#include <TString.h>

TauMuPlotter * configTauMu2011(TString name, TString path){  

  TauMuPlotter * analysis = new TauMuPlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.05);


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
  WJetsToLNu->setSampleGenEvents(81345381);//V5_4_1 missing files: 0/907
  analysis->addSample(WJetsToLNu);  

  Sample * W2JetsToLNu=new Sample("W2JetsToLNu",path);
  W2JetsToLNu->setDataType("MCCat");
  W2JetsToLNu->setCrossection(1435);//This is wrong only use for shape
  W2JetsToLNu->setSampleGenEvents(25400546);//This is wrong only use for shape
  analysis->addSample(W2JetsToLNu);  
   
  Sample * W3JetsToLNu=new Sample("W3JetsToLNu",path);
  W3JetsToLNu->setDataType("MCCat");
  W3JetsToLNu->setCrossection(304.0);
  W3JetsToLNu->setSampleGenEvents(7541595);
  analysis->addSample(W3JetsToLNu);  
   
  float CrossectionScaleFactor=0.95;
  float BjetsscaleFactor=0.97;
  cout<<"WARNING applying scale factor for TTjets MC "<<CrossectionScaleFactor<<endl;
  cout<<"WARNING applying b-jet scale factor for TTjets MC "<<BjetsscaleFactor<<endl;
  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(165.8*CrossectionScaleFactor*BjetsscaleFactor);// 157.5=NLO theory, 165.8=CMS TOP-11-024
  TTJets->setSampleGenEvents(59613991);//  V5_4_1 missing 3/5365 files 
  analysis->addSample(TTJets);


  ///DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  0.999437 ||     36189241

  cout<<"WARNING applying scale factor for Z->tau tau MC "<<CrossectionScaleFactor<<endl;
  Sample* ZToTauTau = new Sample("ZToTauTau",path);//+"_tDown"
  ZToTauTau->setDataType("MC");
  ZToTauTau->setCrossection(3048*CrossectionScaleFactor);
  ZToTauTau->setSampleGenEvents(36209629*0.9985);//V5_4_1 missing files: 4/2751 
  analysis->addSample(ZToTauTau);

//   Sample* ZToTauTauM10To50 = new Sample("ZToTauTauM10To50",path);
//   ZToTauTauM10To50->setDataType("MC");
//   ZToTauTauM10To50->setCrossection(3048*CrossectionScaleFactor);
//   ZToTauTauM10To50->setSampleGenEvents(31459189);
//   analysis->addSample(ZToTauTauM10To50);

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
  Int_t HiggsMass[8]={110,115,120,125,130,135,140,145};
  Float_t HiggsTauTauBF[8]={ 8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};

  //checked twiki on June 17 2012
// 110.0 	19.84 	+15.2 	-15.0 	+7.5 	-8.1 	+7.7 	-6.9
// 115.0 	18.14 	+15.1 	-15.0 	+7.4 	-8.0 	+7.7 	-7.0
// 120.0 	16.65 	+14.8 	-14.9 	+7.2 	-7.9 	+7.6 	-7.0
// 125.0 	15.32 	+14.7 	-14.9 	+7.1 	-7.8 	+7.6 	-7.1
// 130.0 	14.16 	+14.6 	-14.9 	+7.0 	-7.7 	+7.6 	-7.2
// 135.0 	13.11 	+14.5 	-14.9 	+6.9 	-7.6 	+7.6 	-7.3
// 140.0 	12.18 	+14.4 	-14.8 	+6.8 	-7.5 	+7.6 	-7.3
// 145.0 	11.33 	+14.3 	-14.9 	+6.7 	-7.5 	+7.6 	-7.4 
  Float_t HiggsGGcross[8]={   19.84,   18.13,    16.63,    15.31,    14.12,   13.08,   12.13,   11.27};


// /GluGluToHToTauTau_M-110_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  1.0 ||  199988
// /GluGluToHToTauTau_M-115_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  1.0 ||  196002
// /GluGluToHToTauTau_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  1.0 ||  199981
// /GluGluToHToTauTau_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  1.0 ||  199986
// /GluGluToHToTauTau_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  1.0 ||  199985
// /GluGluToHToTauTau_M-135_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  1.0 ||  198571
// /GluGluToHToTauTau_M-140_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  1.0 ||  191549
// /GluGluToHToTauTau_M-145_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||  1.0 ||  199981
  Int_t   HiggsGGNevt[8]={  199988,  196002,  199981,  199986,   199985,  198571,  191549, 199981};//V5


// 110.0 	1.398 	2.8 	-2.3 	0.5 	-0.2 	2.3 	-2.1
// 115.0 	1.332 	2.5 	-2.3 	0.2 	-0.2 	2.3 	-2.1
// 120.0 	1.269 	2.8 	-2.5 	0.3 	-0.4 	2.4 	-2.1
// 125.0 	1.211 	2.7 	-2.4 	0.3 	-0.3 	2.5 	-2.1
// 130.0 	1.154 	2.8 	-2.3 	0.3 	-0.2 	2.5 	-2.1
// 135.0 	1.100 	3.0 	-2.2 	0.5 	-0.1 	2.6 	-2.1
// 140.0 	1.052 	2.8 	-2.2 	0.2 	-0.2 	2.6 	-2.1
// 145.0 	1.004 	3.1 	-2.1 	0.4 	-0.0 	2.7 	-2.1 
  Float_t HiggsVBFcross[8]={   1.398,   1.332,    1.269,    1.211,    1.154,   1.100,   1.052,   1.004};

// /VBF_HToTauTau_M-110_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  198435
// /VBF_HToTauTau_M-115_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  198051
// /VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  198632
// /VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  198612
// /VBF_HToTauTau_M-130_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  197236
// /VBF_HToTauTau_M-135_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  198682
// /VBF_HToTauTau_M-140_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  193846
// /VBF_HToTauTau_M-145_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  197095  
  Int_t   HiggsVBFNevt[8]={  198435,  198051,   198632,   198612,   197236,  198682,  193846, 197095};//V5
  

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


// /WH_ZH_TTH_HToTauTau_M-110_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||        1.0 ||  216334
// /WH_ZH_TTH_HToTauTau_M-115_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||        1.0 ||  220000
// /WH_ZH_TTH_HToTauTau_M-120_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||        0.994118 ||     2186302
// /WH_ZH_TTH_HToTauTau_M-125_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||        1.0 ||  220000
// /WH_ZH_TTH_HToTauTau_M-130_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||        1.0 ||  214501
// /WH_ZH_TTH_HToTauTau_M-135_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||        1.0 ||  220000
// /WH_ZH_TTH_HToTauTau_M-140_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||        1.0 ||  218704
// /WH_ZH_TTH_HToTauTau_M-145_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||        1.0 ||  216885
  Int_t   HiggsVHNevt[8]={  216334,  220000,  2186302,   220000,   214501,   220000,  218704, 216885};// M=120 has 10x

  char nam[100];
  for(Int_t i=0;i<8;i++){

    sprintf(nam,"HiggsGG%d",HiggsMass[i]);
    Sample* HiggsGG = new Sample(nam,path);
    HiggsGG->setDataType("Signal");
    HiggsGG->setCrossection(HiggsGGcross[i]*HiggsTauTauBF[i]);
    HiggsGG->setSampleGenEvents(HiggsGGNevt[i]);
    analysis->addSample(HiggsGG);    

    sprintf(nam,"HiggsVBF%d",HiggsMass[i]);
    Sample* HiggsVBF=new Sample(nam,path);
    HiggsVBF->setDataType("Signal");
    HiggsVBF->setCrossection(HiggsVBFcross[i]*HiggsTauTauBF[i]);
    HiggsVBF->setSampleGenEvents(HiggsVBFNevt[i]);
    analysis->addSample(HiggsVBF);    

    sprintf(nam,"HiggsVH%d",HiggsMass[i]);
    Sample* HiggsVH=new Sample(nam,path);
    HiggsVH->setDataType("Signal");
    HiggsVH->setCrossection((HiggsWHcross[i]+HiggsZHcross[i]+HiggsttHcross[i])*HiggsTauTauBF[i]);
    HiggsVH->setSampleGenEvents(HiggsVHNevt[i]);
    analysis->addSample(HiggsVH);    
  }



  /////////////////SUSY Samples
  Int_t  SUSYMass[19]={90,100,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};  
// /SUSYBBHToTauTau_M-90_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||     1.0 ||  220000
// /SUSYBBHToTauTau_M-100_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    0.926591 ||     203850
// /SUSYBBHToTauTau_M-120_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    0.992792 ||     2181784
// /SUSYBBHToTauTau_M-130_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  220000
// /SUSYBBHToTauTau_M-140_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  204580
// /SUSYBBHToTauTau_M-160_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  220000
// /SUSYBBHToTauTau_M-180_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  220000
// /SUSYBBHToTauTau_M-200_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  220000
// /SUSYBBHToTauTau_M-250_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  204448
// /SUSYBBHToTauTau_M-300_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  2086880
// /SUSYBBHToTauTau_M-350_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  220000
// /SUSYBBHToTauTau_M-400_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  202330
// /SUSYBBHToTauTau_M-450_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  220000
// /SUSYBBHToTauTau_M-500_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  203260
// /SUSYBBHToTauTau_M-600_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  207787
// /SUSYBBHToTauTau_M-700_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  212314
// /SUSYBBHToTauTau_M-800_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  210082
// /SUSYBBHToTauTau_M-900_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||    1.0 ||  213214
// /SUSYBBHToTauTau_M-1000_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||   1.0 ||  203068
  Int_t SUSYBBNevt[19]={220000,203850,2181784,220000,204580,220000,220000,220000,204448,2086880,220000,202330,220000,203260,207787,212314,210082,213214,203068};//V5 m=300 has 10x

// /SUSYGluGluToHToTauTau_M-90_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||       1.0 ||  209002
// /SUSYGluGluToHToTauTau_M-100_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  220000
// /SUSYGluGluToHToTauTau_M-120_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  2093360
// /SUSYGluGluToHToTauTau_M-130_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  203464
// /SUSYGluGluToHToTauTau_M-140_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      0.916682 ||     201670
// /SUSYGluGluToHToTauTau_M-160_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  220000
// /SUSYGluGluToHToTauTau_M-180_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  220000
// /SUSYGluGluToHToTauTau_M-200_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  220000
// /SUSYGluGluToHToTauTau_M-250_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      0.930727 ||     204760
// /SUSYGluGluToHToTauTau_M-300_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      0.998333 ||     2095914
// /SUSYGluGluToHToTauTau_M-350_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      0.918227 ||     202010
// /SUSYGluGluToHToTauTau_M-400_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  220000
// /SUSYGluGluToHToTauTau_M-450_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  207040
// /SUSYGluGluToHToTauTau_M-500_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  220000
// /SUSYGluGluToHToTauTau_M-600_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  204475
// /SUSYGluGluToHToTauTau_M-700_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  206780
// /SUSYGluGluToHToTauTau_M-800_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  219229
// /SUSYGluGluToHToTauTau_M-900_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||      1.0 ||  215275
// /SUSYGluGluToHToTauTau_M-1000_7TeV-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5 ||     0.930837 ||     197248
  Int_t SUSYGGNevt[19]={209002,220000,2093360,203464,201670,220000,220000,220000,204760,2095914,202010,220000,207040,220000,204475,206780,219229,215275,197248};//V5

  for(Int_t i=0;i<19;i++){

      sprintf(nam,"SUSYBB%d",SUSYMass[i]);
      Sample* SUSYBB = new Sample(nam,path);
      SUSYBB->setDataType("Signal");
      SUSYBB->setCrossection(1.);
      SUSYBB->setSampleGenEvents(SUSYBBNevt[i]);
      SUSYBB->setSignalMass(SUSYMass[i]);
      analysis->addSample(SUSYBB);    

      sprintf(nam,"SUSYGG%d",SUSYMass[i]);
      Sample* SUSYGG = new Sample(nam,path);
      SUSYGG->setDataType("Signal");
      SUSYGG->setCrossection(1.);
      SUSYGG->setSampleGenEvents(SUSYGGNevt[i]);
      SUSYGG->setSignalMass(SUSYMass[i]);
      analysis->addSample(SUSYGG);    

  }

  return analysis;
}

