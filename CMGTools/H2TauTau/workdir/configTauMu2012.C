#include "TauMuPlotter.h"
#include "Sample.h"
#include <TString.h>

TauMuPlotter * configTauMu2012(TString name, TString path){  

  TauMuPlotter * analysis = new TauMuPlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.1);//1.05);//my measured value July 6
  
  Sample* TauPlusX2012A = new Sample("TauPlusX2012A",path);
  TauPlusX2012A->setDataType("Data");
  //TauPlusX2012A->setSampleLumi(28+299+371);
  TauPlusX2012A->setSampleLumi(700.);
  analysis->addSample(TauPlusX2012A);

  Sample* TauPlusX2012B = new Sample("TauPlusX2012B",path);
  TauPlusX2012B->setDataType("Data");
  //TauPlusX2012B->setSampleLumi(4341);
  TauPlusX2012B->setSampleLumi(4400.);
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

  Sample* Embedded2012B3 = new Sample("Embedded2012B3",path);
  Embedded2012B3->setDataType("Embedded");
  analysis->addSample(Embedded2012B3);


  float CrossectionScaleFactor=0.976;
  cout<<"WARNING applying scale factor for TTjets MC "<<CrossectionScaleFactor<<endl;


  /////////W+jets
  Sample * WJetsToLNu=new Sample("WJetsToLNu",path);
  WJetsToLNu->setDataType("MC");
  WJetsToLNu->setCrossection(36257.2);//https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat8TeV
  WJetsToLNu->addRootFiles("WJetsToLNu2");//add previous sample with 18M events
  analysis->addSample(WJetsToLNu);  

//   Sample * W2JetsToLNu=new Sample("W2JetsToLNu",path);
//   W2JetsToLNu->setDataType("MCCat");
//   W2JetsToLNu->setCrossection(1000.);//dummy
//   analysis->addSample(W2JetsToLNu);  
   
  Sample * W3JetsToLNu=new Sample("W3JetsToLNu",path);
  W3JetsToLNu->setDataType("MCCat");
  W3JetsToLNu->setCrossection(1000);//dummy value
  analysis->addSample(W3JetsToLNu);  
   

  /////////////Z+Jets
  cout<<"WARNING applying scale factor for Z->tau tau MC "<<CrossectionScaleFactor<<endl;
  Sample* ZToTauTau = new Sample("ZToTauTau",path);
  ZToTauTau->setDataType("MC");
  ZToTauTau->setCrossection(3503.71*CrossectionScaleFactor);
  analysis->addSample(ZToTauTau);

  Sample* ZToMuMu = new Sample("ZToMuMu",path);
  ZToMuMu->setDataType("MC");
  ZToMuMu->setCrossection(ZToTauTau->getCrossection());
  analysis->addSample(ZToMuMu);

  Sample* ZToLJet = new Sample("ZToLJet",path);
  ZToLJet->setDataType("MC");
  ZToLJet->setCrossection(ZToTauTau->getCrossection());
  analysis->addSample(ZToLJet);

  //////////TTJets
  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(225.2*CrossectionScaleFactor);
  analysis->addSample(TTJets);

  ///Di-Bosons
  Sample* WW = new Sample("WW",path);
  WW->setDataType("MC");
  WW->setCrossection(57.1097);
  analysis->addSample(WW);

  Sample* WZ = new Sample("WZ",path);
  WZ->setDataType("MC");
  WZ->setCrossection(32.3161);
  analysis->addSample(WZ);

  Sample* ZZ = new Sample("ZZ",path);
  ZZ->setDataType("MC");
  ZZ->setCrossection(8.25561);
  analysis->addSample(ZZ);

  ////SingleTop
//   Sample* TopS = new Sample("TopS",path);
//   TopS->setDataType("MC");
//   TopS->setCrossection(57.1097);
//   analysis->addSample(TopS);

//   Sample* TopT = new Sample("TopT",path);
//   TopT->setDataType("MC");
//   TopT->setCrossection(57.1097);
//   analysis->addSample(TopT);

  Sample* TopTW = new Sample("TopTW",path);
  TopTW->setDataType("MC");
  TopTW->setCrossection(11.1);
  analysis->addSample(TopTW);

//   Sample* TopBS = new Sample("TopBS",path);
//   TopBS->setDataType("MC");
//   TopBS->setCrossection(57.1097);
//   analysis->addSample(TopBS);

//   Sample* TopBT = new Sample("TopBT",path);
//   TopBT->setDataType("MC");
//   TopBT->setCrossection(57.1097);
//   analysis->addSample(TopBT);

  Sample* TopBTW = new Sample("TopBTW",path);
  TopBTW->setDataType("MC");
  TopBTW->setCrossection(11.1);
  analysis->addSample(TopBTW);


  ///Crossections taken from here:
  //https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt8TeV
  ///Higgs --> tau tau branching ratios taken from http://arxiv.org/abs/1101.0593 v3
  const int NSM=8;
  Int_t HiggsMass[NSM]={110,115,120,125,130,135,140,145};
  Float_t HiggsTauTauBF[NSM]={ 8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};
  Float_t HiggsGGcross[NSM]={25.04,22.96,21.13,19.52,18.07,16.79,15.63,14.59};
  Float_t HiggsVBFcross[NSM]={ 1.809,1.729,1.649,1.578,1.511,1.448,1.389,1.333 };
  Float_t HiggsWHcross[NSM]= {1.0600,0.9165,0.7966,0.6966,0.6095,0.5351,0.47130,0.41640};
  Float_t HiggsZHcross[NSM]= {0.5869,0.5117,0.4483,0.3943,0.3473,0.3074,0.27280,0.24240};
  Float_t HiggsttHcross[NSM]={0.1887,0.1663,0.1470,0.1302,0.1157,0.1031,0.09207,0.08246 };

  char nam[100];
  for(Int_t i=0;i<NSM;i++){

    sprintf(nam,"HiggsGG%d",HiggsMass[i]);
    Sample* HiggsGG = new Sample(nam,path);
    HiggsGG->setDataType("Signal");
    HiggsGG->setCrossection(HiggsGGcross[i]*HiggsTauTauBF[i]);
    analysis->addSample(HiggsGG);    

    sprintf(nam,"HiggsVBF%d",HiggsMass[i]);
    Sample* HiggsVBF=new Sample(nam,path);
    HiggsVBF->setDataType("Signal");
    HiggsVBF->setCrossection(HiggsVBFcross[i]*HiggsTauTauBF[i]);
    analysis->addSample(HiggsVBF);    

    sprintf(nam,"HiggsVH%d",HiggsMass[i]);
    Sample* HiggsVH=new Sample(nam,path);
    HiggsVH->setDataType("Signal");
    HiggsVH->setCrossection((HiggsWHcross[i]+HiggsZHcross[i]+HiggsttHcross[i])*HiggsTauTauBF[i]);
    analysis->addSample(HiggsVH);    
  }



  /////////////////SUSY Samples
  const int NSUSY=16;
  //Int_t  SUSYMass[NSUSY]={90,100,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};  
  Int_t  SUSYMass[NSUSY]={90,100,110,120,130,140,160,180,250,300,400,450,500,600,800,1000};  

  for(Int_t i=0;i<NSUSY;i++){

      sprintf(nam,"SUSYBB%d",SUSYMass[i]);
      Sample* SUSYBB = new Sample(nam,path);
      SUSYBB->setDataType("Signal");
      SUSYBB->setCrossection(1.);
      SUSYBB->setSignalMass(SUSYMass[i]);
      analysis->addSample(SUSYBB);    

      sprintf(nam,"SUSYGG%d",SUSYMass[i]);
      Sample* SUSYGG = new Sample(nam,path);
      SUSYGG->setDataType("Signal");
      SUSYGG->setCrossection(1.);
      SUSYGG->setSignalMass(SUSYMass[i]);
      analysis->addSample(SUSYGG);    

  }

  return analysis;
}

