#include "TauElePlotter.h"
#include "Sample.h"
#include <TString.h>

TauElePlotter * configTauEle2012Trig(TString name, TString path){  

  TauElePlotter * analysis = new TauElePlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.06);
  analysis->setZTTType(1);
  analysis->mTCut_=20;
  //analysis->eventWeight_="pupWeights1*triggerEffWeightMu*selectionEffWeight*(ditaumass>10)*(mupt>28)*(taupt>16)";//AB
  //analysis->eventWeight_="pupWeight*triggerEffWeightMu*selectionEffWeight*(ditaumass>10)*(mupt>28)*(taupt>16)";  //ABC
  //analysis->eventWeight_="pupWeights3*triggerEffWeightMu*selectionEffWeight*(ditaumass>10)*(mupt>28)*(taupt>20)";  //D
  analysis->eventWeight_="pupWeights4*(ditaumass>10)*(mupt>28)*(taupt>18)";  //ABCD
 

  Sample* SingleElectron2012A = new Sample("SingleElectron2012A",path);
  SingleElectron2012A->setDataType("Data");
  SingleElectron2012A->setSampleLumi(96.977+316.128+396.213);
  analysis->addSample(SingleElectron2012A);

  Sample* SingleElectron2012B = new Sample("SingleElectron2012B",path);
  SingleElectron2012B->setDataType("Data");
  SingleElectron2012B->setSampleLumi(4403.);
  analysis->addSample(SingleElectron2012B);

  Sample* SingleElectron2012Cv1 = new Sample("SingleElectron2012Cv1",path);
  SingleElectron2012Cv1->setDataType("Data");
  SingleElectron2012Cv1->setSampleLumi(495.003);
  analysis->addSample(SingleElectron2012Cv1);

  Sample* SingleElectron2012Cv2 = new Sample("SingleElectron2012Cv2",path);
  SingleElectron2012Cv2->setDataType("Data");
  SingleElectron2012Cv2->setSampleLumi(1288 + 4828 + 281);
  analysis->addSample(SingleElectron2012Cv2);

  Sample* SingleElectron2012D = new Sample("SingleElectron2012D",path);
  SingleElectron2012D->setDataType("Data");
  //SingleElectron2012D->setSampleLumi(6040);
  SingleElectron2012D->setSampleLumi(7274);
  analysis->addSample(SingleElectron2012D);


  ///DY
  float CrossectionScaleFactor=1.011;
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

  ///TTJets
  float TTCrossectionScaleFactor=0.98;
  cout<<"WARNING applying scale factor to TT MC "<<TTCrossectionScaleFactor<<endl;

  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(225.2*TTCrossectionScaleFactor);
  analysis->addSample(TTJets);

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

