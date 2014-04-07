#include "TauElePlotter.h"
#include "Sample.h"
#include <TString.h>
#include "constants.h"


TauElePlotter * configTauEle2012Summer13ReRecoTrig(TString name, TString path){  

  TauElePlotter * analysis = new TauElePlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.06);
  analysis->setZTTType(1);
  analysis->mTCut_=30;
  //analysis->eventWeight_="pupWeights2*triggerEffWeightsMu2*selectionEffWeightsId2*selectionEffWeightsIso2*(tauantiemva3new>=2&&mupt>27&&taupt>18)";//ABC
  //analysis->eventWeight_="pupWeights3*triggerEffWeightsMu3*selectionEffWeightsId3*selectionEffWeightsIso3*(tauantiemva3new>=2&&mupt>27&&taupt>18)";//D
  //analysis->eventWeight_="pupWeights4*triggerEffWeightsMu4*selectionEffWeightsId4*selectionEffWeightsIso4*(tauantiemva3new>=2&&mupt>27&&taupt>18)";//ABCD for usual procedure (fakes dont affect too much)
  analysis->eventWeight_="pupWeights4*triggerEffWeightsMu4*selectionEffWeightsId4*selectionEffWeightsIso4*(tauantiemva3new>=2&&mupt>27&&taupt>18&&taudecaymode==0&&40<ditaumass&&ditaumass<80)";//ABCD (for one prong fakes affect high pT)


  //////Data
  Sample* SingleElectron2012A = new Sample("SingleElectron2012ReRecoA",path);
  SingleElectron2012A->setDataType("Data");
  SingleElectron2012A->setSampleLumi(96.4+396.9+395.99);
  analysis->addSample(SingleElectron2012A);

  Sample* SingleElectron2012B = new Sample("SingleElectron2012ReRecoB",path);
  SingleElectron2012B->setDataType("Data");
  SingleElectron2012B->setSampleLumi(4429);
  analysis->addSample(SingleElectron2012B);

  Sample* SingleElectron2012C = new Sample("SingleElectron2012ReRecoC",path);
  SingleElectron2012C->setDataType("Data");
  SingleElectron2012C->setSampleLumi(1783 + 5087 + 282.69);
  analysis->addSample(SingleElectron2012C);

  Sample* SingleElectron2012D = new Sample("SingleElectron2012ReRecoD",path);
  SingleElectron2012D->setDataType("Data");
  SingleElectron2012D->setSampleLumi(7318);
  analysis->addSample(SingleElectron2012D);

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

  for(long n=1;n<=4;n++){
    Sample* ZNjet = new Sample(TString("Z")+n+"ToEE",path);
    ZNjet->setDataType("MCCat");
    ZNjet->setCrossection(ZToTauTau->getCrossection()*DYNJetFrac[n-1]);
    analysis->addSample(ZNjet);
  }
  for(long n=1;n<=4;n++){
    Sample* ZNjet = new Sample(TString("Z")+n+"ToLJet",path);
    ZNjet->setDataType("MCCat");
    ZNjet->setCrossection(ZToTauTau->getCrossection()*DYNJetFrac[n-1]);
    analysis->addSample(ZNjet);
  }

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

