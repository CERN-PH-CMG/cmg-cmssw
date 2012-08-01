#include "TauElePlotter.h"
#include "Sample.h"
#include <TString.h>

TauElePlotter * configTauEle2011(TString name, TString path){  

  TauElePlotter * analysis = new TauElePlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.07);

// HLT_Ele15_ _LooseIsoPFTau15_v1                  160404  161176  6.281
// HLT_Ele15_ _LooseIsoPFTau15_v2                  161216  163261  40.685
// HLT_Ele15_ _LooseIsoPFTau15_v4                  163269  163869  168.664
// HLT_Ele15_ _LooseIsoPFTau20_v6                  165088  165633  136.382
// HLT_Ele15_ _LooseIsoPFTau20_v8                  165970  166967  541.531
// HLT_Ele15_ _LooseIsoPFTau20_v9                  167039  167913  277.299
// HLT_Ele15_ _TightIsoPFTau20_v2                  170249  172619  389.876
// HLT_Ele15_ _TightIsoPFTau20_v2                  172620  173198  441.387
// HLT_Ele18_ _MediumIsoPFTau20_v1                 173236  173692  265.313
// HLT_Ele18_ _MediumIsoPFTau20_v1                 175832  178380  1829
// HLT_Ele20_ _MediumIsoPFTau20_v5                  178420  179889  756.508
// HLT_Ele20_ _MediumIsoPFTau20_v6                 179959  180252  128.945

  //2011 Data samples
  Sample* TauPlusXMay = new Sample("TauPlusXMay",path);
  TauPlusXMay->setDataType("Data");
  TauPlusXMay->setSampleLumi(168.66);// 168.597 
  analysis->addSample(TauPlusXMay);

  Sample* TauPlusXv4 = new Sample("TauPlusXv4",path);
  TauPlusXv4->setDataType("Data");
  TauPlusXv4->setSampleLumi(955.21);// 929.748 --> 951.43
  analysis->addSample(TauPlusXv4);

  Sample* TauPlusXAug = new Sample("TauPlusXAug",path);
  TauPlusXAug->setDataType("Data");
  TauPlusXAug->setSampleLumi(389.87);// 373.349  --> 383.798
  analysis->addSample(TauPlusXAug);

  Sample* TauPlusXOct3 = new Sample("TauPlusXOct3",path);
  TauPlusXOct3->setDataType("Data");
  TauPlusXOct3->setSampleLumi(706.70);// 658.886 --> 696.471
  analysis->addSample(TauPlusXOct3);

  Sample* TauPlusX2011B = new Sample("TauPlusX2011B",path);
  TauPlusX2011B->setDataType("Data");
  TauPlusX2011B->setSampleLumi(2714.45);// 2511  -->2714
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

  Sample* ZToEE = new Sample("ZToEE",path);
  ZToEE->setDataType("MC");
  ZToEE->setCrossection(ZToTauTau->getCrossection());
  ZToEE->setSampleGenEvents(ZToTauTau->getSampleGenEvents());
  analysis->addSample(ZToEE);

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


  Int_t HiggsMass[8]={110,115,120,125,130,135,140,145};
  Float_t HiggsTauTauBF[8]={ 8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};
  Float_t HiggsGGcross[8]={   19.84,   18.13,    16.63,    15.31,    14.12,   13.08,   12.13,   11.27};
  Int_t   HiggsGGNevt[8]={  199988,  196002,  199981,  199986,   199985,  198571,  191549, 199981};
  Float_t HiggsVBFcross[8]={   1.398,   1.332,    1.269,    1.211,    1.154,   1.100,   1.052,   1.004};
  Int_t   HiggsVBFNevt[8]={  198435,  198051,   198632,   198612,   197236,  198682,  193846, 197095};
  Float_t HiggsWHcross[8]={  0.8754,  0.7546,   0.6561,   0.5729,   0.5008,  0.4390,  0.3857,  0.3406};
  Float_t HiggsZHcross[8]={  0.4721,  0.4107,   0.3598,   0.3158,   0.2778,  0.2453,  0.2172,  0.1930};
  Float_t HiggsttHcross[8]={  0.1257,  0.1106,  0.09756,  0.08634,  0.07658, 0.06810, 0.06072, 0.05435};
  Int_t   HiggsVHNevt[8]={  216334,  220000,  2186302,   220000,   214501,   220000,  218704, 216885};

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
  long  SUSYMass[19]={90,100,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};  
  Int_t SUSYBBNevt[19]={220000,203850,2181784,220000,204580,220000,220000,220000,204448,2086880,220000,202330,220000,203260,207787,212314,210082,213214,203068};//V5 m=300 has 10x
  Int_t SUSYGGNevt[19]={209002,220000,2093360,203464,201670,220000,220000,220000,204760,2095914,202010,220000,207040,220000,204475,206780,219229,215275,197248};//V5

  for(Int_t i=0;i<19;i++){

      Sample* SUSYBB = new Sample(TString("SUSYBB")+SUSYMass[i],path);
      SUSYBB->setDataType("Signal");
      SUSYBB->setCrossection(1.);
      SUSYBB->setSampleGenEvents(SUSYBBNevt[i]);
      SUSYBB->setSignalMass(SUSYMass[i]);
      analysis->addSample(SUSYBB);    

      Sample* SUSYGG = new Sample(TString("SUSYGG")+SUSYMass[i],path);
      SUSYGG->setDataType("Signal");
      SUSYGG->setCrossection(1.);
      SUSYGG->setSampleGenEvents(SUSYGGNevt[i]);
      SUSYGG->setSignalMass(SUSYMass[i]);
      analysis->addSample(SUSYGG);    

  }

  return analysis;
}

