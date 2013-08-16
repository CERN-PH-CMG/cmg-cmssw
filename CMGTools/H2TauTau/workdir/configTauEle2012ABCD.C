#include "TauElePlotter.h"
#include "Sample.h"
#include <TString.h>

TauElePlotter * configTauEle2012ABCD(TString name, TString path){  

  TauElePlotter * analysis = new TauElePlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.06);
  analysis->setZTTType(2);
  analysis->mTCut_=20;
  analysis->eventWeight_="pupWeights4*embeddedGenWeight*triggerEffWeightsTau4*triggerEffWeightsMu4*selectionEffWeightsId4*selectionEffWeightsIso4*signalWeight";


  //////Data
  Sample* TauPlusX2012A = new Sample("TauPlusX2012A",path);
  TauPlusX2012A->setDataType("Data");
  TauPlusX2012A->setSampleLumi(96.977+316.128+396.213);
  analysis->addSample(TauPlusX2012A);

  Sample* TauPlusX2012B = new Sample("TauPlusX2012B",path);
  TauPlusX2012B->setDataType("Data");
  TauPlusX2012B->setSampleLumi(4403.);
  analysis->addSample(TauPlusX2012B);

  Sample* TauPlusX2012Cv1 = new Sample("TauPlusX2012Cv1",path);
  TauPlusX2012Cv1->setDataType("Data");
  TauPlusX2012Cv1->setSampleLumi(495.003);
  analysis->addSample(TauPlusX2012Cv1);

  Sample* TauPlusX2012Cv2 = new Sample("TauPlusX2012Cv2",path);
  TauPlusX2012Cv2->setDataType("Data");
  TauPlusX2012Cv2->setSampleLumi(1288 + 4828 + 281);
  analysis->addSample(TauPlusX2012Cv2);

  Sample* TauPlusX2012D = new Sample("TauPlusX2012D",path);
  TauPlusX2012D->setDataType("Data");
  TauPlusX2012D->setSampleLumi(7274);
  analysis->addSample(TauPlusX2012D);

  //embedded samples
  Sample* Embedded2012A = new Sample("Embedded2012A",path);
  Embedded2012A->setDataType("Embedded");
  analysis->addSample(Embedded2012A);

  Sample* Embedded2012B = new Sample("Embedded2012B",path);
  Embedded2012B->setDataType("Embedded");
  analysis->addSample(Embedded2012B);

  Sample* Embedded2012Cv1 = new Sample("Embedded2012Cv1",path);
  Embedded2012Cv1->setDataType("Embedded");
  analysis->addSample(Embedded2012Cv1);

  Sample* Embedded2012Cv2 = new Sample("Embedded2012Cv2",path);
  Embedded2012Cv2->setDataType("Embedded");
  analysis->addSample(Embedded2012Cv2);

  Sample* Embedded2012D = new Sample("Embedded2012D",path);
  Embedded2012D->setDataType("Embedded");
  analysis->addSample(Embedded2012D);


  ///DY
  float CrossectionScaleFactor=1.0;
  cout<<"WARNING applying scale factor for Z->tau tau MC "<<CrossectionScaleFactor<<endl;
  Sample* ZToTauTau = new Sample("ZToTauTau",path);
  ZToTauTau->setDataType("MC");
  ZToTauTau->setCrossection(3503.71*CrossectionScaleFactor);
  analysis->addSample(ZToTauTau);

  Sample* ZToEE = new Sample("ZToEE",path);
  ZToEE->setDataType("MC");
  ZToEE->setCrossection(ZToTauTau->getCrossection());
  analysis->addSample(ZToEE);

  Sample* ZToLJet = new Sample("ZToLJet",path);
  ZToLJet->setDataType("MC");
  ZToLJet->setCrossection(ZToTauTau->getCrossection());
  analysis->addSample(ZToLJet);

  /////////W+jets
  Sample * WJetsToLNu=new Sample("WJetsToLNu",path);
  WJetsToLNu->setDataType("MC");
  WJetsToLNu->setCrossection(36257.2);//https://twiki.cern.ch/twiki/bin/view/CMS/StandardModelCrossSectionsat8TeV
  WJetsToLNu->addRootFiles("WJetsToLNu2");//add previous sample with 18M events
  analysis->addSample(WJetsToLNu);  

  Sample * W1JetsToLNu=new Sample("W1JetsToLNu",path);
  W1JetsToLNu->setDataType("MCCat");
  W1JetsToLNu->setCrossection(WJetsToLNu->getCrossection()*0.175999);
  analysis->addSample(W1JetsToLNu);  

  Sample * W2JetsToLNu=new Sample("W2JetsToLNu",path);
  W2JetsToLNu->setDataType("MCCat");
  W2JetsToLNu->setCrossection(WJetsToLNu->getCrossection()*0.0562617);
  analysis->addSample(W2JetsToLNu);  
   
  Sample * W3JetsToLNu=new Sample("W3JetsToLNu",path);
  W3JetsToLNu->setDataType("MCCat");
  W3JetsToLNu->setCrossection(WJetsToLNu->getCrossection()*0.0168926);
  analysis->addSample(W3JetsToLNu);  

  Sample * W4JetsToLNu=new Sample("W4JetsToLNu",path);
  W4JetsToLNu->setDataType("MCCat");
  W4JetsToLNu->setCrossection(WJetsToLNu->getCrossection()*0.00692218);
  analysis->addSample(W4JetsToLNu);  
   

  ///TTJets
  float TTCrossectionScaleFactor=1.00;
  cout<<"WARNING applying scale factor to TT MC "<<TTCrossectionScaleFactor<<endl;

  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(225.2*TTCrossectionScaleFactor);
  analysis->addSample(TTJets);


  //Di-Bosons
  Sample* WW2L2Nu = new Sample("WW2L2Nu",path);
  WW2L2Nu->setDataType("MC");
  WW2L2Nu->setCrossection(5.824);
  analysis->addSample(WW2L2Nu);

  Sample* WZ3LNu = new Sample("WZ3LNu",path);
  WZ3LNu->setDataType("MC");
  WZ3LNu->setCrossection(1.058);
  analysis->addSample(WZ3LNu);

  Sample* WZ2L2Q = new Sample("WZ2L2Q",path);
  WZ2L2Q->setDataType("MC");
  WZ2L2Q->setCrossection(2.207);
  analysis->addSample(WZ2L2Q);

  Sample* ZZ4L = new Sample("ZZ4L",path);
  ZZ4L->setDataType("MC");
  ZZ4L->setCrossection(0.181);
  analysis->addSample(ZZ4L);

  Sample* ZZ2L2Nu = new Sample("ZZ2L2Nu",path);
  ZZ2L2Nu->setDataType("MC");
  ZZ2L2Nu->setCrossection(0.716);
  analysis->addSample(ZZ2L2Nu);

  Sample* ZZ2L2Q = new Sample("ZZ2L2Q",path);
  ZZ2L2Q->setDataType("MC");
  ZZ2L2Q->setCrossection(2.502);
  analysis->addSample(ZZ2L2Q);

  ////SingleTop
  Sample* TopTW = new Sample("TopTW",path);
  TopTW->setDataType("MC");
  TopTW->setCrossection(11.1);
  analysis->addSample(TopTW);

  Sample* TopBTW = new Sample("TopBTW",path);
  TopBTW->setDataType("MC");
  TopBTW->setCrossection(11.1);
  analysis->addSample(TopBTW);


  //copied from mu-tau 
  Int_t HiggsMass[8]={110,115,120,125,130,135,140,145};
  Float_t HiggsTauTauBF[8]={ 8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};
  Float_t HiggsGGcross[8]={25.04,22.96,21.13,19.52,18.07,16.79,15.63,14.59};
  Float_t HiggsVBFcross[8]={ 1.809,1.729,1.649,1.578,1.511,1.448,1.389,1.333 };
  Float_t HiggsWHcross[8]={1.060,0.9165,0.7966,0.6966,0.6095,0.5351,0.4713,0.4164};
  Float_t HiggsZHcross[8]={0.5869,0.5117,0.4483,0.3943,0.3473,0.3074,0.2728,0.2424 };
  Float_t HiggsttHcross[8]={ 0.1887,0.1663,0.1470,0.1302,0.1157,0.1031,0.09207,0.08246 };

  char nam[100];
  for(Int_t i=0;i<8;i++){

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
  analysis->setQCDColor(kMagenta-10);
  analysis->setWJetsColor(kRed+2);
  analysis->setTTJetsColor(kBlue-8);
  analysis->setZMuMuColor(kBlue);
  analysis->setZTauTauColor(kOrange-4);
  char title[100];
  sprintf(title,"CMS Preliminary 2012,   %.1f fb^{-1},     #sqrt{s} = 8 TeV,        #tau_{e}#tau_{h}",(analysis->getTotalDataLumi())/1000.);
  analysis->setPlotTitle(title);
  return analysis;
}

