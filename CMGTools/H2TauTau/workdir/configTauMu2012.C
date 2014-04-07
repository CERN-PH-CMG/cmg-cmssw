#include "TauMuPlotter.h"
#include "Sample.h"
#include <TString.h>

TauMuPlotter * configTauMu2012(TString name, TString path){  

  TauMuPlotter * analysis = new TauMuPlotter(name);
  analysis->setOutputPath(path);
  analysis->setQCDOStoSSRatio(1.06);
  analysis->setZTTType(2);
  analysis->mTCut_=20;
 
// 2012A
// | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 |       2823 | 96.977(/pb) | 96.977(/pb) |
// | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 |       5260 | 316.128(/pb) | 316.128(/pb) |
// | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 |       6516 | 396.213(/pb) | 396.213(/pb) |
// 2012B
// ./Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt
// | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2 |      50953 | 4.403(/fb) | 4.403(/fb) |
// 2012Cv1
// ./Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt
// | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 |       6719 | 495.003(/pb) | 495.003(/pb) |
// 2012Cv2
// ./Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON_v2_For2012Cv2Lumi.txt
// | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 |      15358 | 1.288(/fb) | 1.288(/fb) |
// | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6 |      52308 | 4.828(/fb) | 4.828(/fb) |
// | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 |       3425 | 281.155(/pb) | 281.155(/pb) |


 
  Sample* TauPlusX2012A = new Sample("TauPlusX2012A",path);
  TauPlusX2012A->setDataType("Data");
  TauPlusX2012A->setSampleLumi(96.977+316.128+396.213);
  analysis->addSample(TauPlusX2012A);

//   Sample* TauPlusX2012A2 = new Sample("TauPlusX2012A2",path);
//   TauPlusX2012A2->setDataType("Data");
//   TauPlusX2012A2->setSampleLumi(82.136);
//   analysis->addSample(TauPlusX2012A2);

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

  Sample* Embedded2012A = new Sample("Embedded2012A",path);
  Embedded2012A->setDataType("Embedded");
  analysis->addSample(Embedded2012A);

//   Sample* Embedded2012A2 = new Sample("Embedded2012A2",path);
//   Embedded2012A2->setDataType("Embedded");
//   analysis->addSample(Embedded2012A2);

  Sample* Embedded2012B = new Sample("Embedded2012B",path);
  Embedded2012B->setDataType("Embedded");
  analysis->addSample(Embedded2012B);

  Sample* Embedded2012Cv1 = new Sample("Embedded2012Cv1",path);
  Embedded2012Cv1->setDataType("Embedded");
  analysis->addSample(Embedded2012Cv1);

  Sample* Embedded2012Cv2 = new Sample("Embedded2012Cv2",path);
  Embedded2012Cv2->setDataType("Embedded");
  analysis->addSample(Embedded2012Cv2);


  /////////////Z+Jets
  float CrossectionScaleFactor=1.011;
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
  float TTCrossectionScaleFactor=0.98;//apply this globally 
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



//   /////////////////SUSY Samples
//   const int NSUSY=16;
//   //Int_t  SUSYMass[NSUSY]={90,100,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};  
//   Int_t  SUSYMass[NSUSY]={90,100,110,120,130,140,160,180,250,300,400,450,500,600,800,1000};  

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
  analysis->setZMuMuColor(kRed);
  analysis->setZTauTauColor(kOrange-4);
  analysis->setSmearHistoRes(0.);
  char title[100];
  sprintf(title,"CMS Preliminary 2012,   %.1f fb^{-1},     #sqrt{s} = 8 TeV,        #tau_{#mu}#tau_{h}",(analysis->getTotalDataLumi())/1000.);
  analysis->setPlotTitle(title);

  return analysis;
}

