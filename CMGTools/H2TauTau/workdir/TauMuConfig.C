//#if !defined(__CINT__) && !defined(__MAKECINT__)
//#include "TFile.h"
//#include "TString.h"
//#include "/afs/cern.ch/user/b/benitezj/scratch1/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/interface/TauMuAnalysis.h"
//#include "/afs/cern.ch/user/b/benitezj/scratch1/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/include/Sample.h"
//#endif
//#include "TString.h"
//class TauMuAnalysis;
//#include "/afs/cern.ch/user/b/benitezj/scratch1/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/interface/TauMuAnalysis.h"

TauMuAnalysis * TauMuConfig(const char * name){  

  TauMuAnalysis * analysis = new TauMuAnalysis(TString("analysis")+name);
  
  
  analysis->setInputTag("cmgTauMuCorSVFitFullSel");
  analysis->setdiLeptonVetoListName("cmgMuonSel");
  analysis->setQCDOStoSSRatio(1.11);//value from AN-11-390 v4
  analysis->setTransverseMassSignalMax(40);
  analysis->setTransverseMassSideBandMin(60);
  analysis->setVerbosity(0);
  //analysis->setTruncateEvents(1000); 
  analysis->makeAllHistos(1);
  analysis->setPrintFreq(1000);

  float MCEffCorrFactor = 1.0;// 0.968;// * 0.92; 

  //analysis->setPupWeightName("vertexWeight2011AB");//vertexWeightFall112011AB, vertexWeight2011B, vertexWeight2invfb, vertexWeight3D2011AB

  //TString localpath="/data/benitezj/Samples";
  //TString tag="/PAT_CMG_V2_4_0/H2TAUTAU_Dec2";     
  //TString outpath="./output/Test";
  //TString outpath="./output/V240Json";//compare embedded massT and vismass against this
  //TString outpath="./output/V240Embedded";
  //TString outpath="./output/V240TauTrigger";
  //TString outpath="./output/V240VVHiggs";
  //TString outpath="./output/V240EoverP";
  //TString outpath="./output/V240MuPt";  
  //TString outpath="./output/V240AB";//changes done: 1)tau eff, 2)mu eff, 3) vertex-weight
  //for B alone modified TauMuConfigB.C, TauMuAnalysis.cc, batchSample.C, processSamples.pl

  //TString localpath="/data/benitezj/Samples";
  //TString tag="/PAT_CMG_V2_4_1/H2TAUTAU_Jan16"; ///V241 samples  
  //TString outpath="./output/V241AB";//1)trig objs
  //TString outpath="./output/V241AB3D";//1)vtx w.
  //TString outpath="./output/V241ABTrigMatch";//1)trig match mu and tau
  //TString outpath="./output/V241ABTruthMatch";//1)change Z->tau tau truth-matching

  TString localpath="/data/benitezj/Samples/Jan26";//separated productions to be able to remove previous ones 
  TString tag="/PAT_CMG_V2_4_1/H2TAUTAU_Oct26"; //misnamed
  //TString outpath="./output/V241ABMuIso";
  //TString outpath="./output/V241ABQCD";
  //TString outpath="./output/V241ABQCD2";
  //TString outpath="./output/V241ABFall";
 
  TString outpath="./output/V241ABFallTrigEff";
  //TString tag="/PAT_CMG_V2_4_1/H2TAUTAU_Jan26Up"; //checkFailed.pl Summary : files: 112/115, events = 853415/28086242
  //TString outpath="./output/V241ABFallTrigEfftUp";
  //TString tag="/PAT_CMG_V2_4_1/H2TAUTAU_Jan26Down"; //checkFailed.pl Summary : files: 111/115, events = 791627/27991634
  //TString outpath="./output/V241ABFallTrigEfftDown";


  analysis->setOutputPath(outpath);
 
  /////////////////////////
  //define samples    
  /////////////////////////
  Sample * WJetsToLNu=new Sample("WJetsToLNu",(localpath+"/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"+tag).Data());
  WJetsToLNu->setDataType("MC");
  WJetsToLNu->setPileupWeight("vertexWeightFall112011AB");
  WJetsToLNu->setCrossection(31314);
  WJetsToLNu->setSampleGenEvents(81273381); 
  WJetsToLNu->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  WJetsToLNu->setEffCorrFactor(MCEffCorrFactor);
  WJetsToLNu->setApplyTauRateWeight(0);
  analysis->addSample(WJetsToLNu);  

  Sample* W2JetsToLNu = new Sample("W2JetsToLNu",(localpath+"/W2Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3"+tag).Data());
  W2JetsToLNu->setDataType("MCCat");
  W2JetsToLNu->setPileupWeight("vertexWeightFall112011AB");
  W2JetsToLNu->setCrossection(1435);//need to revisit this crossections look here: https://hypernews.cern.ch/HyperNews/CMS/get/generators/1324.html
  W2JetsToLNu->setSampleGenEvents(25057238); 
  W2JetsToLNu->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  W2JetsToLNu->setEffCorrFactor(MCEffCorrFactor);
  W2JetsToLNu->setApplyTauRateWeight(0);
  analysis->addSample(W2JetsToLNu);  

  Sample* W3JetsToLNu = new Sample("W3JetsToLNu",(localpath+"/W3Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3"+tag).Data());
  W3JetsToLNu->setDataType("MCCat");
  W3JetsToLNu->setPileupWeight("vertexWeightFall112011AB");
  W3JetsToLNu->setCrossection(304.0);//need to revisit this crossections look here: https://hypernews.cern.ch/HyperNews/CMS/get/generators/1324.html
  W3JetsToLNu->setSampleGenEvents(7506183); 
  W3JetsToLNu->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  W3JetsToLNu->setEffCorrFactor(MCEffCorrFactor);
  W3JetsToLNu->setApplyTauRateWeight(0);
  analysis->addSample(W3JetsToLNu);  
   
  Sample* TTJets = new Sample("TTJets",(localpath+"/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"+tag).Data());
  TTJets->setDataType("MC");
  TTJets->setPileupWeight("vertexWeightFall112011AB");
  TTJets->setCrossection(165.8);//157.5=NLO theory, 165.8=CMS TOP-11-024
  TTJets->setSampleGenEvents(3701947); 
  TTJets->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  TTJets->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(TTJets);

  Sample* ZToTauTau = new Sample("ZToTauTau",(localpath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"+tag).Data());
  ZToTauTau->setDataType("MC");
  ZToTauTau->setPileupWeight("vertexWeightFall112011AB");
  ZToTauTau->setGenEventType(5);
  ZToTauTau->setCrossection(3048);
  ZToTauTau->setSampleGenEvents(28086242);// 28480417 need to revise this, this number was changing in the tau scale processings 
  ZToTauTau->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  ZToTauTau->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(ZToTauTau);

  Sample* ZToMuMu = new Sample("ZToMuMu",(localpath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"+tag).Data());
  ZToMuMu->setDataType("MC");
  ZToMuMu->setPileupWeight("vertexWeightFall112011AB");
  ZToMuMu->setTruthEventType(3);
  ZToMuMu->setCrossection(ZToTauTau->getCrossection());
  ZToMuMu->setSampleGenEvents(ZToTauTau->getSampleGenEvents());
  ZToMuMu->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  ZToMuMu->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(ZToMuMu);

  Sample* ZToLJet = new Sample("ZToLJet",(localpath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2"+tag).Data());
  ZToLJet->setDataType("MC");
  ZToLJet->setPileupWeight("vertexWeightFall112011AB");
  ZToLJet->setTruthEventType(6);
  ZToLJet->setCrossection(ZToTauTau->getCrossection());
  ZToLJet->setSampleGenEvents(ZToTauTau->getSampleGenEvents());
  ZToLJet->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  ZToLJet->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(ZToLJet);
  

  //old Summer11 samples  
//   Sample* WJetsToLNu = new Sample("WJetsToLNu",(localpath+"/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag).Data());
//   WJetsToLNu->setDataType("MC");
//   WJetsToLNu->setPileupWeight("vertexWeight2011AB");
//   WJetsToLNu->setRecoilCorr("../data/recoilfit_wjets_njet.root"); 
//   WJetsToLNu->setCrossection(31314);
//   WJetsToLNu->setSampleGenEvents(53227112); //  53655290 = PFAOD integrity, 81352581 = "DBS"
//   //WJetsToLNu->addTrigPath("HLT_IsoMu12_v1");
//   WJetsToLNu->setEffCorrFactor(MCEffCorrFactor);
//   WJetsToLNu->setApplyTauRateWeight(0);
//   analysis->addSample(WJetsToLNu);  

//   Sample* TTJets = new Sample("TTJets",(localpath+"/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag).Data());
//   TTJets->setDataType("MC");
//   TTJets->setPileupWeight("vertexWeight2011AB");
//   TTJets->setCrossection(165.8);//157.5=NLO theory, 165.8=CMS TOP-11-024
//   TTJets->setSampleGenEvents(3542770); // 3701947=Enriques PFAOD integrity, 3701947="DBS"
//   //TTJets->addTrigPath("HLT_IsoMu12_v1");
//   TTJets->setEffCorrFactor(MCEffCorrFactor);
//   analysis->addSample(TTJets);
 
//   Sample* ZToTauTau = new Sample("ZToTauTau",(localpath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag).Data());
//   ZToTauTau->setDataType("MC");
//   ZToTauTau->setPileupWeight("vertexWeight2011AB");
//   ZToTauTau->setGenEventType(5);
//   ZToTauTau->setRecoilCorr("../data/recoilfit_zjets_ltau_njet.root");
//   ZToTauTau->setCrossection(3048);
//   ZToTauTau->setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
//   //ZToTauTau->addTrigPath("HLT_IsoMu12_v1");
//   ZToTauTau->setEffCorrFactor(MCEffCorrFactor);
//   analysis->addSample(ZToTauTau);

//   Sample* ZToMuMu = new Sample("ZToMuMu",(localpath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag).Data());
//   ZToMuMu->setDataType("MC");
//   ZToMuMu->setPileupWeight("vertexWeight2011AB");
//   ZToMuMu->setTruthEventType(3);
//   ZToMuMu->setRecoilCorr("../data/recoilfit_zmm42X_njet.root");
//   ZToMuMu->setCrossection(3048);
//   ZToMuMu->setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
//   //ZToMuMu->addTrigPath("HLT_IsoMu12_v1");
//   ZToMuMu->setEffCorrFactor(MCEffCorrFactor);
//   analysis->addSample(ZToMuMu);

//   Sample* ZToLJet = new Sample("ZToLJet",(localpath+"/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag).Data());
//   ZToLJet->setDataType("MC");
//   ZToLJet->setPileupWeight("vertexWeight2011AB");
//   ZToLJet->setTruthEventType(6);
//   ZToLJet->setRecoilCorr("../data/recoilfit_wjets_njet.root");
//   ZToLJet->setCrossection(3048);
//   ZToLJet->setSampleGenEvents(34915945);// 35035820=PFAOD integrity, 36277961="DBS"
//   //ZToLJet->addTrigPath("HLT_IsoMu12_v1");
//   ZToLJet->setEffCorrFactor(MCEffCorrFactor);
//   analysis->addSample(ZToLJet);

  Sample* WW = new Sample("WW",(localpath+"/WW_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag).Data());
  WW->setDataType("MC");
  WW->setPileupWeight("vertexWeight2011AB");
  WW->setCrossection(27.83);
  WW->setSampleGenEvents(4052865);
  //WW->addTrigPath("HLT_IsoMu12_v1");
  WW->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(WW);

  Sample* WZ = new Sample("WZ",(localpath+"/WZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag).Data());
  WZ->setDataType("MC");
  WZ->setPileupWeight("vertexWeight2011AB");
  WZ->setCrossection(10.47);
  WZ->setSampleGenEvents(4169751);
  //WZ->addTrigPath("HLT_IsoMu12_v1");
  WZ->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(WZ);

  Sample* ZZ = new Sample("ZZ",(localpath+"/ZZ_TuneZ2_7TeV_pythia6_tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2"+tag).Data());
  ZZ->setDataType("MC");
  ZZ->setPileupWeight("vertexWeight2011AB");
  ZZ->setCrossection(4.287);
  ZZ->setSampleGenEvents(4112445);
  //ZZ->addTrigPath("HLT_IsoMu12_v1");
  ZZ->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(ZZ);



  ///Crossections taken from here:
  ///https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt7TeV#gluon_gluon_Fusion_Process 
  ///Higgs --> tau tau branching ratios taken from http://arxiv.org/abs/1101.0593 v3
  long HiggsMass[8]={110,115,120,125,130,135,140,145};
  Float_t HiggsGGcross[8]={   19.84,   18.13,    16.63,    15.31,    14.12,   13.08,   12.13,   11.27};
  Float_t HiggsVBFcross[8]={   1.398,   1.332,    1.269,    1.211,    1.154,   1.100,   1.052,   1.004};
  Float_t HiggsWHcross[8]={  0.8754,  0.7546,   0.6561,   0.5729,   0.5008,  0.4390,  0.3857,  0.3406};
  Float_t HiggsZHcross[8]={  0.4721,  0.4107,   0.3598,   0.3158,   0.2778,  0.2453,  0.2172,  0.1930};
  Float_t HiggsttHcross[8]={  0.1257,  0.1106,  0.09756,  0.08634,  0.07658, 0.06810, 0.06072, 0.05435};
  Int_t   HiggsVHNevt[8]={  201334,  195000,   183147,   205000,   214501,   220000,  218704, 216885};
  Float_t HiggsTauTauBF[8]={ 8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};
  char nam[100];
  char pa[1000];
  for(Int_t i=0;i<8;i++){

    sprintf(nam,"HiggsGG%d",HiggsMass[i]);
    sprintf(pa,"%s/GluGluToHToTauTau_M-%d_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2%s",localpath.Data(),HiggsMass[i],tag.Data());
    Sample* HiggsGG = new Sample(nam,pa);
    HiggsGG->setDataType("Signal");
    HiggsGG->setPileupWeight("vertexWeight2011AB");
    HiggsGG->setCrossection(HiggsGGcross[i]*HiggsTauTauBF[i]);
    HiggsGG->setSampleGenEvents(196002);
    HiggsGG->setEffCorrFactor(MCEffCorrFactor);
    analysis->addSample(HiggsGG);    

    sprintf(nam,"HiggsVBF%d",HiggsMass[i]);
    sprintf(pa,"%s/VBF_HToTauTau_M-%d_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2%s",localpath.Data(),HiggsMass[i],tag.Data());
    Sample* HiggsVBF=new Sample(nam,pa);
    HiggsVBF->setDataType("Signal");
    HiggsVBF->setPileupWeight("vertexWeight2011AB");
    HiggsVBF->setCrossection(HiggsVBFcross[i]*HiggsTauTauBF[i]);
    HiggsVBF->setSampleGenEvents(196002);
    HiggsVBF->setEffCorrFactor(MCEffCorrFactor);
    analysis->addSample(HiggsVBF);    

    sprintf(nam,"HiggsVH%d",HiggsMass[i]);
    sprintf(pa,"%s/WH_ZH_TTH_HToTauTau_M-%d_7TeV-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2%s",localpath.Data(),HiggsMass[i],tag.Data());
    Sample* HiggsVH=new Sample(nam,pa);
    HiggsVH->setDataType("Signal");
    HiggsVH->setPileupWeight("vertexWeight2011AB");
    HiggsVH->setCrossection((HiggsWHcross[i]+HiggsZHcross[i]+HiggsttHcross[i])*HiggsTauTauBF[i]);
    HiggsVH->setSampleGenEvents(HiggsVHNevt[i]);//number of events varies due to bad crab jobs
    HiggsVH->setEffCorrFactor(MCEffCorrFactor);
    analysis->addSample(HiggsVH);    

  }

  //2011 Data samples
  Sample* TauPlusXMay = new Sample("TauPlusXMay",(localpath+"/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2"+tag).Data());
  TauPlusXMay->setDataType("Data");
  TauPlusXMay->setSampleLumi(168.597);
  TauPlusXMay->setRunRange(163262,170000);
  TauPlusXMay->addTrigPath("HLT_IsoMu12_LooseIsoPFTau10_v4","hltFilterIsoMu12IsoPFTau10LooseIsolation","hltSingleMuIsoL3IsoFiltered12");
  analysis->addSample(TauPlusXMay);

  Sample* TauPlusXv4 = new Sample("TauPlusXv4",(localpath+"/TauPlusX/Run2011A-PromptReco-v4/AOD/V2"+tag).Data());
  TauPlusXv4->setDataType("Data");
  TauPlusXv4->setSampleLumi(929.748);
  TauPlusXv4->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v2","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  TauPlusXv4->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v4","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  TauPlusXv4->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v5","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  TauPlusXv4->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v6","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  analysis->addSample(TauPlusXv4);

  Sample* TauPlusXAug = new Sample("TauPlusXAug",(localpath+"/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2"+tag).Data());
  TauPlusXAug->setDataType("Data");
  TauPlusXAug->setSampleLumi(373.349);
  TauPlusXAug->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  analysis->addSample(TauPlusXAug);

  Sample* TauPlusXOct3 = new Sample("TauPlusXOct3",(localpath+"/TauPlusX/Run2011A-03Oct2011-v1/AOD/V2"+tag).Data());
  TauPlusXOct3->setDataType("Data");
  TauPlusXOct3->setSampleLumi(658.886);
  TauPlusXOct3->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v8","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  TauPlusXOct3->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");
  analysis->addSample(TauPlusXOct3);

  Sample* TauPlusX2011B = new Sample("TauPlusX2011B",(localpath+"/TauPlusX/Run2011B-PromptReco-v1/AOD/V2"+tag).Data());
  TauPlusX2011B->setDataType("Data");
  TauPlusX2011B->setSampleLumi(2511);
  TauPlusX2011B->addTrigPath("HLT_IsoMu15_LooseIsoPFTau15_v9","hltPFTau15TrackLooseIso","hltSingleMuIsoL3IsoFiltered15");//to recover ~100pb-1 
  TauPlusX2011B->addTrigPath("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1");//used when HLT_IsoMu15_LooseIsoPFTau15_v9 got prescaled 
  TauPlusX2011B->addTrigPath("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1");
  TauPlusX2011B->addTrigPath("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6","hltPFTau20TrackLooseIso","hltSingleMuIsoL1s14L3IsoFiltered15eta2p1");
  analysis->addSample(TauPlusX2011B);


  Sample* EmbeddedMay = new Sample("EmbeddedMay",(localpath+"/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag).Data());
  EmbeddedMay->setDataType("Embedded");
  analysis->addSample(EmbeddedMay);

  Sample* EmbeddedV4 = new Sample("EmbeddedV4",(localpath+"/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag).Data());
  EmbeddedV4->setDataType("Embedded");
  analysis->addSample(EmbeddedV4);

  Sample* EmbeddedAug = new Sample("EmbeddedAug",(localpath+"/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag).Data());
  EmbeddedAug->setDataType("Embedded");
  analysis->addSample(EmbeddedAug);

  Sample* EmbeddedOct = new Sample("EmbeddedOct",(localpath+"/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag).Data());
  EmbeddedOct->setDataType("Embedded");
  analysis->addSample(EmbeddedOct);

  Sample* Embedded2011B = new Sample("Embedded2011B",(localpath+"/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/"+tag).Data());
  Embedded2011B->setDataType("Embedded");
  analysis->addSample(Embedded2011B);



  //////////////////////////////////////////////////////
  /////////////Same Sign samples////////////////////////
  //////////////////////////////////////////////////////


  Sample* WJetsToLNu_SS = new Sample("WJetsToLNu_SS",WJetsToLNu->GetTitle());
  WJetsToLNu_SS->setDataType("MC_SS");
  WJetsToLNu_SS->setPileupWeight(*WJetsToLNu->getPileupWeight());
  WJetsToLNu_SS->setRecoilCorr(WJetsToLNu->getRecoilCorrProcessFile());
  WJetsToLNu_SS->setCrossection(WJetsToLNu->getCrossection());
  WJetsToLNu_SS->setSampleGenEvents(WJetsToLNu->getSampleGenEvents());
  WJetsToLNu_SS->addTrigPaths(WJetsToLNu->getTrigPaths());
  WJetsToLNu_SS->setEffCorrFactor(MCEffCorrFactor);
  WJetsToLNu_SS->setApplyTauRateWeight(WJetsToLNu->getApplyTauRateWeight());
  analysis->addSample(WJetsToLNu_SS);

  Sample* W2JetsToLNu_SS = new Sample("W2JetsToLNu_SS",W2JetsToLNu->GetTitle());
  W2JetsToLNu_SS->setDataType("MCCat_SS");
  W2JetsToLNu_SS->setPileupWeight(*W2JetsToLNu->getPileupWeight());
  W2JetsToLNu_SS->setRecoilCorr(W2JetsToLNu->getRecoilCorrProcessFile());
  W2JetsToLNu_SS->setCrossection(W2JetsToLNu->getCrossection());
  W2JetsToLNu_SS->setSampleGenEvents(W2JetsToLNu->getSampleGenEvents());
  W2JetsToLNu_SS->addTrigPaths(W2JetsToLNu->getTrigPaths());
  W2JetsToLNu_SS->setEffCorrFactor(MCEffCorrFactor);
  W2JetsToLNu_SS->setApplyTauRateWeight(W2JetsToLNu->getApplyTauRateWeight());
  analysis->addSample(W2JetsToLNu_SS);

  Sample* W3JetsToLNu_SS = new Sample("W3JetsToLNu_SS",W3JetsToLNu->GetTitle());
  W3JetsToLNu_SS->setDataType("MCCat_SS");
  W3JetsToLNu_SS->setPileupWeight(*W3JetsToLNu->getPileupWeight());
  W3JetsToLNu_SS->setRecoilCorr(W3JetsToLNu->getRecoilCorrProcessFile());
  W3JetsToLNu_SS->setCrossection(W3JetsToLNu->getCrossection());
  W3JetsToLNu_SS->setSampleGenEvents(W3JetsToLNu->getSampleGenEvents());
  W3JetsToLNu_SS->addTrigPaths(W3JetsToLNu->getTrigPaths());
  W3JetsToLNu_SS->setEffCorrFactor(MCEffCorrFactor);
  W3JetsToLNu_SS->setApplyTauRateWeight(W3JetsToLNu->getApplyTauRateWeight());
  analysis->addSample(W3JetsToLNu_SS);
  
  Sample* TTJets_SS = new Sample("TTJets_SS",TTJets->GetTitle());
  TTJets_SS->setDataType("MC_SS");
  TTJets_SS->setPileupWeight(*TTJets->getPileupWeight());
  TTJets_SS->setCrossection(TTJets->getCrossection());
  TTJets_SS->setSampleGenEvents(TTJets->getSampleGenEvents());
  TTJets_SS->addTrigPaths(TTJets->getTrigPaths());
  TTJets_SS->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(TTJets_SS);

  Sample* ZToMuMu_SS = new Sample("ZToMuMu_SS",ZToMuMu->GetTitle());
  ZToMuMu_SS->setDataType("MC_SS");
  ZToMuMu_SS->setPileupWeight(*ZToMuMu->getPileupWeight());
  ZToMuMu_SS->setTruthEventType(ZToMuMu->getTruthEventType());
  ZToMuMu_SS->setRecoilCorr(ZToMuMu->getRecoilCorrProcessFile());
  ZToMuMu_SS->setCrossection(ZToMuMu->getCrossection());
  ZToMuMu_SS->setSampleGenEvents(ZToMuMu->getSampleGenEvents());
  ZToMuMu_SS->addTrigPaths(ZToMuMu->getTrigPaths());
  ZToMuMu_SS->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(ZToMuMu_SS);

  Sample* ZToTauTau_SS = new Sample("ZToTauTau_SS",ZToTauTau->GetTitle());
  ZToTauTau_SS->setDataType("MC_SS");
  ZToTauTau_SS->setPileupWeight(*ZToTauTau->getPileupWeight());
  ZToTauTau_SS->setTruthEventType(ZToTauTau->getTruthEventType());
  ZToTauTau_SS->setRecoilCorr(ZToTauTau->getRecoilCorrProcessFile());
  ZToTauTau_SS->setCrossection(ZToTauTau->getCrossection());
  ZToTauTau_SS->setSampleGenEvents(ZToTauTau->getSampleGenEvents());
  ZToTauTau_SS->addTrigPaths(ZToTauTau->getTrigPaths());
  ZToTauTau_SS->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(ZToTauTau_SS);

  Sample* ZToLJet_SS = new Sample("ZToLJet_SS",ZToLJet->GetTitle());
  ZToLJet_SS->setDataType("MC_SS");
  ZToLJet_SS->setPileupWeight(*ZToLJet->getPileupWeight());
  ZToLJet_SS->setTruthEventType(ZToLJet->getTruthEventType());
  ZToLJet_SS->setRecoilCorr(ZToLJet->getRecoilCorrProcessFile());
  ZToLJet_SS->setCrossection(ZToLJet->getCrossection());
  ZToLJet_SS->setSampleGenEvents(ZToLJet->getSampleGenEvents());
  ZToLJet_SS->addTrigPaths(ZToLJet->getTrigPaths());
  ZToLJet_SS->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(ZToLJet_SS);
    
  Sample* WW_SS = new Sample("WW_SS",WW->GetTitle());
  WW_SS->setDataType("MC_SS");
  WW_SS->setPileupWeight(*WW->getPileupWeight());
  WW_SS->setCrossection(WW->getCrossection());
  WW_SS->setSampleGenEvents(WW->getSampleGenEvents());
  WW_SS->addTrigPaths(WW->getTrigPaths());    
  WW_SS->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(WW_SS);

  Sample* WZ_SS = new Sample("WZ_SS",WZ->GetTitle());
  WZ_SS->setDataType("MC_SS");
  WZ_SS->setPileupWeight(*WZ->getPileupWeight());
  WZ_SS->setCrossection(WZ->getCrossection());
  WZ_SS->setSampleGenEvents(WZ->getSampleGenEvents());
  WZ_SS->addTrigPaths(WZ->getTrigPaths());
  WZ_SS->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(WZ_SS);

  Sample* ZZ_SS = new Sample("ZZ_SS",ZZ->GetTitle());
  ZZ_SS->setDataType("MC_SS");
  ZZ_SS->setPileupWeight(*ZZ->getPileupWeight());
  ZZ_SS->setCrossection(ZZ->getCrossection());
  ZZ_SS->setSampleGenEvents(ZZ->getSampleGenEvents());
  ZZ_SS->addTrigPaths(ZZ->getTrigPaths());
  ZZ_SS->setEffCorrFactor(MCEffCorrFactor);
  analysis->addSample(ZZ_SS);

  Sample* TauPlusXMay_SS = new Sample("TauPlusXMay_SS",TauPlusXMay->GetTitle());
  TauPlusXMay_SS->setDataType("Data_SS");
  TauPlusXMay_SS->setSampleLumi(TauPlusXMay->getLumi());
  TauPlusXMay_SS->setRunRange(TauPlusXMay->getFirstRun(),TauPlusXMay->getLastRun());
  TauPlusXMay_SS->addTrigPaths(TauPlusXMay->getTrigPaths());
  analysis->addSample(TauPlusXMay_SS);

  Sample* TauPlusXv4_SS = new Sample("TauPlusXv4_SS",TauPlusXv4->GetTitle());
  TauPlusXv4_SS->setDataType("Data_SS");
  TauPlusXv4_SS->setSampleLumi(TauPlusXv4->getLumi());
  TauPlusXv4_SS->addTrigPaths(TauPlusXv4->getTrigPaths());
  analysis->addSample(TauPlusXv4_SS);

  Sample* TauPlusXAug_SS = new Sample("TauPlusXAug_SS",TauPlusXAug->GetTitle());
  TauPlusXAug_SS->setDataType("Data_SS");
  TauPlusXAug_SS->setSampleLumi(TauPlusXAug->getLumi());
  TauPlusXAug_SS->addTrigPaths(TauPlusXAug->getTrigPaths());
  analysis->addSample(TauPlusXAug_SS);

  Sample* TauPlusXOct3_SS = new Sample("TauPlusXOct3_SS",TauPlusXOct3->GetTitle());
  TauPlusXOct3_SS->setDataType("Data_SS");
  TauPlusXOct3_SS->setSampleLumi(TauPlusXOct3->getLumi());
  TauPlusXOct3_SS->addTrigPaths(TauPlusXOct3->getTrigPaths());
  analysis->addSample(TauPlusXOct3_SS);

  Sample* TauPlusX2011B_SS = new Sample("TauPlusX2011B_SS",TauPlusX2011B->GetTitle());
  TauPlusX2011B_SS->setDataType("Data_SS");
  TauPlusX2011B_SS->setSampleLumi(TauPlusX2011B->getLumi());
  TauPlusX2011B_SS->addTrigPaths(TauPlusX2011B->getTrigPaths());
  analysis->addSample(TauPlusX2011B_SS);


  Sample* EmbeddedMay_SS = new Sample("EmbeddedMay_SS",EmbeddedMay->GetTitle());
  EmbeddedMay_SS->setDataType("Embedded_SS");
  analysis->addSample(EmbeddedMay_SS);

  Sample* EmbeddedV4_SS = new Sample("EmbeddedV4_SS",EmbeddedV4->GetTitle());
  EmbeddedV4_SS->setDataType("Embedded_SS");
  analysis->addSample(EmbeddedV4_SS);

  Sample* EmbeddedAug_SS = new Sample("EmbeddedAug_SS",EmbeddedAug->GetTitle());
  EmbeddedAug_SS->setDataType("Embedded_SS");
  analysis->addSample(EmbeddedAug_SS);

  Sample* EmbeddedOct_SS = new Sample("EmbeddedOct_SS",EmbeddedOct->GetTitle());
  EmbeddedOct_SS->setDataType("Embedded_SS");
  analysis->addSample(EmbeddedOct_SS);

  Sample* Embedded2011B_SS = new Sample("Embedded2011B_SS",Embedded2011B->GetTitle());
  Embedded2011B_SS->setDataType("Embedded_SS");
  analysis->addSample(Embedded2011B_SS);

  return analysis;
}

