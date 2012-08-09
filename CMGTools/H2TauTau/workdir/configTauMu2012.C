#include "TauMuPlotter.h"
#include "Sample.h"
#include <TString.h>

TauMuPlotter * configTauMu2012(TString name, TString path){  

  TauMuPlotter * analysis = new TauMuPlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.05);//my measured value July 6
  
  Sample* TauPlusX2012A = new Sample("TauPlusX2012A",path);
  TauPlusX2012A->setDataType("Data");
  TauPlusX2012A->setSampleLumi(28+299+371);
  analysis->addSample(TauPlusX2012A);

  Sample* TauPlusX2012B = new Sample("TauPlusX2012B",path);
  TauPlusX2012B->setDataType("Data");
  TauPlusX2012B->setSampleLumi(4341);
  analysis->addSample(TauPlusX2012B);

  Sample* Embedded2012A = new Sample("Embedded2012A",path);
  Embedded2012A->setDataType("Embedded");
  analysis->addSample(Embedded2012A);

  Sample* Embedded2012B1 = new Sample("Embedded2012B1",path);
  Embedded2012B1->setDataType("Embedded");
  analysis->addSample(Embedded2012B1);

  Sample* Embedded2012B2 = new Sample("Embedded2012B2",path);
  Embedded2012B2->setDataType("Embedded");
  analysis->addSample(Embedded2012B2);

  Sample * WJetsToLNu=new Sample("WJetsToLNu",path);
  WJetsToLNu->setDataType("MC");
  WJetsToLNu->setCrossection(36257.2);//https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat8TeV
  WJetsToLNu->addRootFiles("WJetsToLNu2");//add previous sample with 18M events
  WJetsToLNu->setSampleGenEvents(78000000.);//this number needs to checked
  analysis->addSample(WJetsToLNu);  


//   Sample * W2JetsToLNu=new Sample("W2JetsToLNu",path);
//   W2JetsToLNu->setDataType("MCCat");
//   W2JetsToLNu->setCrossection(1435);//This is wrong only use for shape
//   W2JetsToLNu->setSampleGenEvents(25400546);//This is wrong only use for shape
//   analysis->addSample(W2JetsToLNu);  
   
  Sample * W3JetsToLNu=new Sample("W3JetsToLNu",path);
  W3JetsToLNu->setDataType("MCCat");
  W3JetsToLNu->setCrossection(304.0);
  W3JetsToLNu->setSampleGenEvents(7541595);//this probably wrong
  analysis->addSample(W3JetsToLNu);  
   
  float CrossectionScaleFactor=0.976;
  float BjetsscaleFactor=1.0;
  cout<<"WARNING applying scale factor for TTjets MC "<<CrossectionScaleFactor<<endl;
  cout<<"WARNING applying b-jet scale factor for TTjets MC "<<BjetsscaleFactor<<endl;
  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(225.2*CrossectionScaleFactor*BjetsscaleFactor);
  TTJets->setSampleGenEvents(13271892/2.);
  analysis->addSample(TTJets);

  cout<<"WARNING applying scale factor for Z->tau tau MC "<<CrossectionScaleFactor<<endl;
  Sample* ZToTauTau = new Sample("ZToTauTau",path);
  ZToTauTau->setDataType("MC");
  ZToTauTau->setCrossection(3503.71*CrossectionScaleFactor);
  ZToTauTau->setSampleGenEvents(60318194/2.);
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
  Int_t HiggsMass[8]={110,115,120,125,130,135,140,145};
  Float_t HiggsTauTauBF[8]={ 8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};

  //checked twiki on June 17 2012
  Float_t HiggsGGcross[8]={   19.84,   18.13,    16.63,    15.31,    14.12,   13.08,   12.13,   11.27};
  Int_t   HiggsGGNevt[8]={  199988,  196002,  199981,  199986,   199985,  198571,  191549, 199981};//V5
  Float_t HiggsVBFcross[8]={   1.398,   1.332,    1.269,    1.211,    1.154,   1.100,   1.052,   1.004};
  Int_t   HiggsVBFNevt[8]={  198435,  198051,   198632,   198612,   197236,  198682,  193846, 197095};//V5
  Float_t HiggsWHcross[8]={  0.8754,  0.7546,   0.6561,   0.5729,   0.5008,  0.4390,  0.3857,  0.3406};
  Float_t HiggsZHcross[8]={  0.4721,  0.4107,   0.3598,   0.3158,   0.2778,  0.2453,  0.2172,  0.1930};
  Float_t HiggsttHcross[8]={  0.1257,  0.1106,  0.09756,  0.08634,  0.07658, 0.06810, 0.06072, 0.05435};
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



 //  /////////////////SUSY Samples
//   Int_t  SUSYMass[19]={90,100,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};  
//   Int_t SUSYBBNevt[19]={220000,203850,2181784,220000,204580,220000,220000,220000,204448,2086880,220000,202330,220000,203260,207787,212314,210082,213214,203068};
//   Int_t SUSYGGNevt[19]={209002,220000,2093360,203464,201670,220000,220000,220000,204760,2095914,202010,220000,207040,220000,204475,206780,219229,215275,197248};//V5

//   for(Int_t i=0;i<19;i++){

// //       sprintf(nam,"SUSYBB%d",SUSYMass[i]);
// //       Sample* SUSYBB = new Sample(nam,path);
// //       SUSYBB->setDataType("Signal");
// //       SUSYBB->setCrossection(1.);
// //       SUSYBB->setSampleGenEvents(SUSYBBNevt[i]);
// //       SUSYBB->setSignalMass(SUSYMass[i]);
// //       analysis->addSample(SUSYBB);    

// //       sprintf(nam,"SUSYGG%d",SUSYMass[i]);
// //       Sample* SUSYGG = new Sample(nam,path);
// //       SUSYGG->setDataType("Signal");
// //       SUSYGG->setCrossection(1.);
// //       SUSYGG->setSampleGenEvents(SUSYGGNevt[i]);
// //       SUSYGG->setSignalMass(SUSYMass[i]);
// //       analysis->addSample(SUSYGG);    

//   }

  return analysis;
}

