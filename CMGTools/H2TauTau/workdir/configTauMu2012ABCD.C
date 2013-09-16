#include "TauMuPlotter.h"
#include "Sample.h"
#include <TString.h>
#include "constants.h"

TauMuPlotter * configTauMu2012ABCD(TString name, TString path){  

  TauMuPlotter * analysis = new TauMuPlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.06);
  analysis->setZTTType(2);
  analysis->mTCut_=20;
  analysis->eventWeight_="pupWeights4*embeddedGenWeight*triggerEffWeightsTau4*triggerEffWeightsMu4*selectionEffWeightsId4*selectionEffWeightsIso4*signalWeight";//
  analysis->tauIsoCut_="(tauisomva>0.795)";
  analysis->tauIsoCutQCD_="(tauisomva>0.5)";

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
  //analysis->TTJetsCorrFactor[0]=1.;
  //analysis->TTJetsCorrFactor[1]=1.01/TTCrossectionScaleFactor;
  //analysis->TTJetsCorrFactor[2]=1.03/TTCrossectionScaleFactor;

  Sample* TTJets = new Sample("TTJets",path);
  TTJets->setDataType("MC");
  TTJets->setCrossection(225.2*TTCrossectionScaleFactor);
  analysis->addSample(TTJets);


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
   


  /////di-Bosons
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


  char nam[100];
  for(Int_t i=0;i<NMASS;i++){

    sprintf(nam,"HiggsGG%d",massValues[i]);
    Sample* HiggsGG = new Sample(nam,path);
    HiggsGG->setDataType("Signal");
    HiggsGG->setCrossection(HiggsGGcross[i]*HiggsTauTauBF[i]);
    analysis->addSample(HiggsGG);    

    sprintf(nam,"HiggsVBF%d",massValues[i]);
    Sample* HiggsVBF=new Sample(nam,path);
    HiggsVBF->setDataType("Signal");
    HiggsVBF->setCrossection(HiggsVBFcross[i]*HiggsTauTauBF[i]);
    analysis->addSample(HiggsVBF);    

    sprintf(nam,"HiggsVH%d",massValues[i]);
    Sample* HiggsVH=new Sample(nam,path);
    HiggsVH->setDataType("Signal");
    HiggsVH->setCrossection((HiggsWHcross[i]+HiggsZHcross[i]+HiggsttHcross[i])*HiggsTauTauBF[i]);
    analysis->addSample(HiggsVH);    
  }



//   /////////////////SUSY Samples
//   for(Int_t i=0;i<NSUSY;i++){
//       sprintf(nam,"SUSYBB%d",SUSYMass[i]);
//       Sample* SUSYBB = new Sample(nam,path);
//       SUSYBB->setDataType("Signal");
//       SUSYBB->setCrossection(1.);
//       SUSYBB->setSignalMass(SUSYMass[i]);
//       analysis->addSample(SUSYBB);    

//       sprintf(nam,"SUSYGG%d",SUSYMass[i]);
//       Sample* SUSYGG = new Sample(nam,path);
//       SUSYGG->setDataType("Signal");
//       SUSYGG->setCrossection(1.);
//       SUSYGG->setSignalMass(SUSYMass[i]);
//       analysis->addSample(SUSYGG);    

//   }




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

