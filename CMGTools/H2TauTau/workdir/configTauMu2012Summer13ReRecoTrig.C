#include "TauMuPlotter.h"
#include "Sample.h"
#include <TString.h>
#include "constants.h"

TauMuPlotter * configTauMu2012Summer13ReRecoTrig(TString name, TString path){  

  TauMuPlotter * analysis = new TauMuPlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.06);
  analysis->setZTTType(1);
  analysis->mTCut_=30;
  //analysis->eventWeight_="pupWeights2*triggerEffWeightsMu2*selectionEffWeightsId2*selectionEffWeightsIso2*(taupt>18)";//ABC
  //analysis->eventWeight_="pupWeights3*triggerEffWeightsMu3*selectionEffWeightsId3*selectionEffWeightsIso3*(taupt>18)";//D
  analysis->eventWeight_="pupWeights4*triggerEffWeightsMu4*selectionEffWeightsId4*selectionEffWeightsIso4*(taupt>18)";//ABCD &&taudecaymode==0
  analysis->tauIsoCut_="(tauiso3hitraw<1.5)";
  analysis->tauIsoCutQCD_="(tauiso3hitraw<6.0)";

  Sample* TauPlusX2012A = new Sample("TauPlusX2012ReRecoA",path);
  TauPlusX2012A->setDataType("Data");
  TauPlusX2012A->setSampleLumi(96.4+396.9+395.99);
  analysis->addSample(TauPlusX2012A);

  Sample* TauPlusX2012B = new Sample("TauPlusX2012ReRecoB",path);
  TauPlusX2012B->setDataType("Data");
  TauPlusX2012B->setSampleLumi(4429);
  analysis->addSample(TauPlusX2012B);

  Sample* TauPlusX2012C = new Sample("TauPlusX2012ReRecoC",path);
  TauPlusX2012C->setDataType("Data");
  TauPlusX2012C->setSampleLumi(1783 + 5087 + 282.69);
  analysis->addSample(TauPlusX2012C);

  Sample* TauPlusX2012D = new Sample("TauPlusX2012ReRecoD",path);
  TauPlusX2012D->setDataType("Data");
  TauPlusX2012D->setSampleLumi(7318);
  analysis->addSample(TauPlusX2012D);


  /////////////Z+Jets
  float CrossectionScaleFactor=1.0;
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


 for(long n=1;n<=4;n++){
    Sample* ZNjet = new Sample(TString("Z")+n+"ToMuMu",path);
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



  //////////TTJets 
  float TTCrossectionScaleFactor=1.0;//apply this globally 
  cout<<"WARNING applying scale factor to TT MC "<<TTCrossectionScaleFactor<<endl;

  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(225.2*TTCrossectionScaleFactor);
  analysis->addSample(TTJets);


//   /////////W+jets
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
   



  ///////configure the colors
  analysis->setQCDColor(kMagenta-10);
  analysis->setWJetsColor(kRed+2);
  analysis->setTTJetsColor(kBlue-8);
  analysis->setZMuMuColor(kBlue);
  analysis->setZTauTauColor(kOrange-4);
  analysis->setSmearHistoRes(0.);
  char title[100];
  sprintf(title,"CMS Preliminary 2012,   %.1f fb^{-1},     #sqrt{s} = 8 TeV,        #tau_{#mu}#tau_{h}",(analysis->getTotalDataLumi())/1000.);
  analysis->setPlotTitle(title);

  return analysis;
}

