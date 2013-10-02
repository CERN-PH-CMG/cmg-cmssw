#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauElePlotter.h"
#include "configTauEle2012.C"
#include "configTauEle2012Trig.C"
#include "configTauEle2012AB.C"
#include "configTauEle2012ABC.C"
#include "configTauEle2012D.C"
#include "configTauEle2012ABCD.C"

#include "configTauEle2012Summer13.C"
#include "configTauEle2012Summer13ReReco.C"
#include "configTauEle2012Summer13ReRecoTrig.C"


void plotInclusive(TauElePlotter* analysis){
  analysis->plotInc("nvtx",50,-.5,49.5,1,1,1,"","",0,2,"# of reco. vertices ","",c,0,"nvtx_et_2012");
  
  analysis->plotInc("mupt",25,0,100,1,1,1,"","",0,2," electron pt   (GeV)","",c,0,"pt_1_et_2012");
  analysis->plotInc("mueta",40,-2.5,2.5,1,1,1,"","",0,2," electron  #eta","",c2,0,"eta_1_et_2012"); 
  analysis->plotInc("muphi",20,-3.5,3.5,1,1,1,"","",0,2," electron phi ","",c3,0,"phi_1_et_2012");    
  analysis->plotInc("muiso",100,0,1,1,0,1,"(tauiso3hitraw<1.5)","",0,2," electron relIso","",c,0,"iso_1_et_2012");

  analysis->plotInc("taumass",20,0,2,1,1,1,"","",0,2," tau mass   (GeV)","",c,0,"taumass_et_2012");
  analysis->plotInc("taupt",25,0,100,1,1,1,"","",0,2," tau pt   (GeV)","",c,0,"pt_2_et_2012");
  analysis->plotInc("taueta",40,-2.5,2.5,1,1,1,"","",0,2," tau  #eta","",c2,0,"eta_2_et_2012"); 
  analysis->plotInc("tauphi",20,-3.5,3.5,1,1,1,"","",0,2," tau phi ","",c3,0,"phi_2_et_2012");    
  //analysis->plotInc("tauisomva",20,-1.0,1.01,1,0,1,"(muiso<0.1)","",0,2,"tau iso 3hit","",c,0,"iso_2_et_2012");
  analysis->plotInc("tauiso3hitraw",40,0,10,1,0,1,"(muiso<0.1)","",0,2,"tau iso. 3hit","",c,0,"iso_2_et_2012");
  analysis->plotInc("taudecaymode",11,0,11,1,1,1,"","",0,2,"Tau decay mode","",c,0,"taudecaymode_et_2012");
  analysis->plotInc("tauehop",60,0,1.2,1,1,1,"","",0,2," tau (E+H)/p  (GeV)","",c,0,"tauehop_et_2012");

  analysis->plotInc("transversemass",40,0,200,2,1,0,"","",-1,2,"M_{T}   [GeV]","",c,0,"mt_1_met_SS_et_2012");
  analysis->plotInc("transversemass",40,0,200,1,1,0,"","",0,2,"M_{T}   [GeV]","",c,0,"mt_1_met_et_2012");
  analysis->plotInc("metpt",30,0,150,1,1,0,"","",0,2," MET   (GeV)","",c,0,"mvamet_et_2012");
  analysis->plotInc("ditaumass",20,0,200,2,1,1,"","",-1,2,"m_{vis}   (GeV)","",c,0,"mvis_SS_et_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_et_2012");
  analysis->plotInc("svfitmass",30,0,300,2,1,1,"","",-1,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_SS_et_2012");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_et_2012");
  analysis->plotInc("njet",5,-.5,4.5,1,1,1,"","",0,2," njet ","",c,0,"njets_et_2012");
}

void plot1Jet(TauElePlotter* analysis){
  analysis->plotInc("leadJetPt",20,0,200,1,1,1,"(njet>=1)","",0,2,"lead jet p_{T}   (GeV)","",c,0,"jetpt_1_et_2012");
  analysis->plotInc("leadJetEta",20,-5,5,1,1,1,"(njet>=1)","",0,2,"lead jet #eta","",c2,0,"jeteta_1_et_2012");
  analysis->plotInc("transversemass",30,0,300,1,1,0,"(njet>=1)","",0,2,"M_{T}   [GeV]","",c,0,"mt_1_met_1Jet_et_2012");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"(njet>=1)","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_1Jet_et_2012");
}

void plotBJet(TauElePlotter* analysis){
  ///nbjet>=1
  analysis->plotInc("transversemass",20,0,200,1,1,0,"(nbjet>=1&&njet<2)","",2,2,"M_{T}   [GeV]","",c,0,"mt_1_met_btag_et_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"(nbjet>=1&&njet<2)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_btag_et_2012");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"(nbjet>=1&&njet<2)","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_btag_et_2012");
  analysis->plotInc("leadBJetPt",20,0,200,1,1,1,"(nbjet>=1&&njet<2)","",4,2," b jet p_{T}   (GeV)","",c,0,"bpt_btag_et_2012");
  analysis->plotInc("leadBJetEta",10,-2.5,2.5,1,1,1,"(nbjet>=1&&njet<2)","",4,2," b jet #eta","",c2,0,"beta_btag_et_2012"); 
}

void plot2Jet(TauElePlotter* analysis){
  analysis->plotInc("taupt",25,0,100,1,1,1,"(njet>=2&&njetingap==0)","",0,2," tau pt   (GeV)","",c,0,"pt_2_2Jet0JetInGap_et_2012");
  analysis->plotInc("subleadJetPt",20,0,200,1,1,1,"(njet>=2)","",2,2,"sublead jet p_{T}   (GeV)","",c,0,"jetpt_2_et_2012");
  analysis->plotInc("subleadJetEta",20,-5,5,1,1,1,"(njet>=2)","",2,2,"sublead jet #eta","",c2,0,"jeteta_2_et_2012");  
  analysis->plotInc("vbfvars20120",20,0,1000,1,1,1,"(njet>=2&&njetingap==0)","",2,2,"M(jj)","",c,0,"mjj_et_2012");
  analysis->plotInc("vbfvars20121",20,0,10,1,1,1,"(njet>=2&&njetingap==0)","",2,2,"#Delta#eta(jj)","",c,0,"jdeta_et_2012");
  analysis->plotInc("ditaumass",20,0,200,1,1,1,"(njet>=2&&njetingap==0)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_et_2Jet0JetInGap_2012");
  analysis->plotInc("transversemass",30,0,300,1,1,0,"(njet>=2&&njetingap==0)","",0,2,"M_{T}   [GeV]","",c,0,"mt_1_met_2Jet0JetInGap_et_2012");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"(njet>=2&&njetingap==0)","",2,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_2Jet0JetInGap_et_2012");
}
  
void plot0JetLow(TauElePlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
//   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(0),"",0,2,"m_{vis}   (GeV)","",c,0,"mvis_0jet_low_et_2012");
//   analysis->plotInc("transversemass",40,0,200,1,1,0,analysis->getSMCategory(0),"",0,2,"m_{T} ","",c,0,"mt_1_met_0jet_low_et_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(0),"",0,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jet_low_et_2012");
}
void plot0JetMed(TauElePlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
  analysis->plotInc("ditaumass",70,0,350,1,1,1,analysis->getSMCategory(1),"",1,5,"m_{vis}   (GeV)","",c,0,"mvis_0jet_medium_et_2012");
  analysis->plotInc("transversemass",40,0,200,1,1,0,analysis->getSMCategory(1),"",1,5,"m_{T} ","",c,0,"mt_1_met_0jet_medium_et_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(1),"",1,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jet_medium_et_2012");
}
void plot0JetHigh(TauElePlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
  analysis->plotInc("ditaumass",70,0,350,1,1,1,analysis->getSMCategory(2),"",1,5,"m_{vis}   (GeV)","",c,0,"mvis_0jet_high_et_2012");
  analysis->plotInc("transversemass",40,0,200,1,1,0,analysis->getSMCategory(2),"",1,5,"m_{T} ","",c,0,"mt_1_met_0jet_high_et_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(2),"",1,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jet_high_et_2012");
}
void plot1JetMed(TauElePlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
  analysis->plotInc("transversemass",40,0,200,1,1,0,analysis->getSMCategory(3),"",0,5,"m_{T} ","",c,0,"mt_1_met_1jet_medium_et_2012");
  analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(3),mvBlindSel,0,5,"m_{vis}   (GeV)","",c,0,"mvis_1jet_medium_et_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(3),svBlindSel,0,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_medium_et_2012");  
} 
void plot1JetHighLow(TauElePlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
  analysis->plotInc("ditaumass",20,0,200,1,1,1,analysis->getSMCategory(4),mvBlindSel,1,4,"m_{vis}   (GeV)","",c,0,"mvis_1jet_high_lowhiggs_et_2012");
  analysis->plotInc("transversemass",40,0,200,1,1,0,analysis->getSMCategory(4),"",1,4,"m_{T} ","",c,0,"mt_1_met_1jet_high_lowhiggs_et_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(4),svBlindSel,1,4,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_high_lowhiggs_et_2012");
} 
void plot1JetHighHigh(TauElePlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
  analysis->plotInc("ditaumass",20,0,200,1,1,1,analysis->getSMCategory(5),mvBlindSel,8,4,"m_{vis}   (GeV)","",c,0,"mvis_1jet_high_highhiggs_et_2012");
  analysis->plotInc("transversemass",40,0,200,1,1,0,analysis->getSMCategory(5),"",8,4,"m_{T} ","",c,0,"mt_1_met_1jet_high_highhiggs_et_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(5),svBlindSel,8,4,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_high_highhiggs_et_2012");
}
void plotVBFLoose(TauElePlotter* analysis){
  analysis->setVariableBinning(13,xbinsValuesVBF);
  analysis->plotInc("ditaumass",10,0,200,1,1,1,analysis->getSMCategory(6),mvBlindSel,5,3,"m_{vis}   (GeV)","",c,0,"mvis_vbf_loose_et_2012");
  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMCategory(6),"",5,3,"m_{T} ","",c,0,"mt_1_met_vbf_loose_et_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(6),svBlindSel,5,3,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_loose_et_2012");
}  
void plotVBFTight(TauElePlotter* analysis){
  analysis->setVariableBinning(13,xbinsValuesVBF);
  analysis->plotInc("ditaumass",10,0,200,1,1,1,analysis->getSMCategory(7),mvBlindSel,9,7,"m_{vis}   (GeV)","",c,0,"mvis_vbf_tight_et_2012");
  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMCategory(7),"",9,7,"m_{T} ","",c,0,"mt_1_met_vbf_tight_et_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(7),svBlindSel,9,7,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_tight_et_2012");
}  

void makeZEESFplots(TauElePlotter* analysis){
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt>30)","",2,2," mvis","",c,0,"mvis_ZEE1Pi0EBSF_et_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&njet<2&&metpt<30&&taupt>30)","",2,2," mvis","",c,0,"mvis_ZEE1Pi0EESF_et_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt>30)","",2,2," mvis","",c,0,"mvis_ZEE0Pi0EBSF_et_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>1.5&&njet<2&&metpt<30&&taupt>30)","",2,2," mvis","",c,0,"mvis_ZEE0Pi0EESF_et_2012");
}

void plotTauEle2012(){  

  //TString path="/data/benitezj/Samples/TauEle2012V5_4_0_NewType1MET";
  //TString path="/data/benitezj/Samples/TauEle2012V5_4_0_NewType1MET_2011WJets";
  //TString path="/data/benitezj/Samples/TauEle2012V5_4_0_NewType1MET2";//Trigger corrections, Type1 MET
  //TString path="/data/benitezj/Samples/TauEle2012V5_4_0_NewType1MET2_2011WJets";//Trigger, Trigger corrections, Type1 MET
  //TString path="/data/benitezj/Samples/TauEle2012V5_4_0_NewType1MET3"; //new 2012 WJets sample
  //TString path="/data/benitezj/Samples/TauEle2012V5_4_0_NewType1MET4"; //Type 1 MET, embedded samples, W3Jets
  //TString path="/data/benitezj/Samples/TauEle2012V5_4_0_NewType1MET5"; //MVA MET
  //TString path="/data/benitezj/Samples/TauEle2012V551Aug7svfit";//debugged MVA MET
  //TString path="/data/benitezj/Samples/TauEle2012V551Aug8svfitPFMET";//PFMET all samples
  //TString path="/data/benitezj/Samples/eTau201253X_580Oct8B";//PFMET all samples
  //TString path="/data/benitezj/Samples/eTau201253X_580Oct14";//53X embedded samples, new Tau trigger turn-on, anti-e discriminator 
  //TString path="/data/benitezj/Samples/eTau201253X_580Oct18Sync";//fixed recoil correction on signals
  //TString path="/data/benitezj/Samples/eTau201253X_580Oct23";
  //TauElePlotter*analysis=configTauEle2012("analysis",path);

  //TString path="/data/benitezj/Samples/eTau201253XAB_580Oct16";
  //TauElePlotter*analysis=configTauEle2012AB("analysis",path);

  //TString path="/data/benitezj/Samples/eTau201253X_580Dec13";
  //TString path="/data/benitezj/Samples/eTau201253X_580Dec21";
  //TauElePlotter*analysis=configTauEle2012D("analysis",path);

  //TString path="/data/benitezj/Samples/eTau201253X_580Jan8";
  //TString path="/data/benitezj/Samples/eTau201253X_580Jan15";
  //TString path="/data/benitezj/Samples/eTau201253X_580Jan16";
  //TString path="/data/benitezj/Samples/eTau201253X_580Jan29";//corrected MET shifts
  //TString path="/data/benitezj/Samples/eTau201253X_580Feb7";//Tau ES corrections
  //TauElePlotter*analysis=configTauEle2012ABCD("analysis",path);

  //TString path="/data/benitezj/Samples/eTau201253X_5140Apr5";
  //TString path="/data/benitezj/Samples/eTau201253X_5140Apr7";//new anti-e discriminator
  //TString path="/data/benitezj/Samples/eTau201253X_5140Apr11";
  //TString path="/data/benitezj/Samples/eTau201253X_5140Apr13";
  //TString path="/data/benitezj/Samples/eTau201253X_5140Apr14";
  //TString path="/data/benitezj/Samples/eTau2012_5140May27";
  //TauElePlotter*analysis=configTauEle2012Summer13("analysis",path);

//   TString path="/data/benitezj/Samples/eTau2012_5160June10";
//  TString path="/data/benitezj/Samples/eTau2012_5160June23";
  TString path="/data/benitezj/Samples/eTau2012_5160July1";
  TauElePlotter*analysis=configTauEle2012Summer13ReReco("analysis",path);


  //TString path="/data/benitezj/Samples/eTau201253X_580Dec5Trig";
  //TString path="/data/benitezj/Samples/eTau201253X_580Dec19Trig";
  //TString path="/data/benitezj/Samples/eTau201253X_580Dec21Trig";
  //TauElePlotter*analysis=configTauEle2012Trig("analysis",path);
  //analysis->plotTauTrigger(2,"ABCD");

  //TString path="/data/benitezj/Samples/eTau2012_5160June18_TauTrig";
//   TString path="/data/benitezj/Samples/eTau2012_5160June28_TauTrig";
//   TString path="/data/benitezj/Samples/eTau2012_5160June30_TauTrig";
//   TauElePlotter*analysis=configTauEle2012Summer13ReRecoTrig("analysis",path);
//   analysis->plotTauTrigger(1,"Summer13ReReco");
//   analysis->plotTauTrigger(2,"Summer13ReReco");


  //analysis->compareZTTEmbedded();
  //analysis->plotQCDSSOSRatio();


  //makeZEESFplots(analysis);


  //analysis->plotInc("transversemass",40,0,200,2,1,0,"","",-1,2,"M_{T}   [GeV]","",c,0,"mt_1_met_SS_et_2012");
  //analysis->plotInc("transversemass",40,0,200,1,1,0,"","",0,2,"M_{T}   [GeV]","",c,0,"mt_1_met_et_2012");
  //analysis->plotInc("ditaumass",40,0,200,1,1,1,"","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_et_2012");
  //analysis->plotInc("taupt",25,0,100,1,1,1,"","",0,2," tau pt   (GeV)","",c,0,"pt_2_et_2012");

  //analysis->setVariableBinning(NXBINS0JET,xbinsValues0Jet);
  //analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcut(1),"",0,2," m_{vis}   (GeV)","",c,0,"mvis_0jethigh_et_2012");
  //analysis->plotInc("svfitmass",60,0,300,1,1,1,"","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_et_2012");
  //analysis->plotInc("tauantiemva3raw",50,0.9,1,1,1,1,"","",0,2,"anti-Electron MVA3 ","",c,0,"antiemva_et_2012");
  //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(3),svBlindSel,8,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jethigh_et_2012");

//   analysis->setVariableBinning(13,xbinsValuesVBF);
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(7),svBlindSel,9,7,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_tight_et_2012");

  //analysis->setVariableBinning(26,xbinsValues);
  //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(5),svBlindSel,8,4,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_high_highhiggs_et_2012");
  //analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=1&&nbjet==0&&taupt>45&&ditaumetpt<100&&metpt>0&&tauantiemva3>=3&&ditaumetpt>0)",svBlindSel,1,4,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_high_lowhiggs_et_2012");//analysis->getSMCategory(4)
 
//   plotInclusive(analysis);
//   plot1Jet(analysis);
//   plot2Jet(analysis);
//   plotBJet(analysis);
  plot0JetLow(analysis);
//   plot0JetMed(analysis);
//   plot0JetHigh(analysis);
//   plot1JetMed(analysis);
//   plot1JetHighLow(analysis);
//   plot1JetHighHigh(analysis);
//   plotVBFLoose(analysis);
//   plotVBFTight(analysis);


  
/* ----------------MSSM ----------------------------------------*/

//   Float_t c4[4]={.65,.98,.52,.87};
//   Float_t xbinsValuesMSSM[20]={0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,450,500,600,700,800};
//   analysis->setVariableBinning(19,xbinsValuesMSSM);
//   analysis->setMSSMFlag(1);

  //  analysis->plotInc("pZeta-1.5*pZetaVis",100,-150,100,1,1,0,"","",5,4,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMinclusive");
//   analysis->plotInc("nbjet",5,-.5,4.5,1,101,"","",0,0," nbjet ","",c4,0,"MSSMinclusive");
//   analysis->plotInc("ditaumass",60,0,300,1,101,"","",0,0,"m(#mu#tau)  (GeV)","",c4,0,"MSSMinclusive"); 
//   analysis->plotInc("svfitmass",70,0,350,1,101,"","",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c4,0,"MSSMinclusive");

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,1,0,"(nbjet>=1&&njet<2)","",5,4,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c2,0,"MSSMinclusiveBJet");
//   analysis->plotInc("leadBJetPt",20,0,200,1,1,101,"(nbjet>=1&&njet<2)","",5,4," b jet p_{T}   (GeV)","",c,0,"MSSMinclusiveBJet");
//   analysis->plotInc("leadBJetEta",20,-5,5,1,1,101,"(nbjet>=1&&njet<2)","",5,4," b jet #eta","",c2,0,"MSSMinclusiveBJet"); 
//   analysis->plotInc("svfitmass",0,0,350,1,101,"(nbjet>=1&&njet<2)","",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c4,0,"MSSMinclusiveBJet");
//   analysis->plotInc("taupt",15,0,150,1,101,"(nbjet>=1&&njet<2)","",4,22,"tau pt (GeV)","",c4,0,"MSSMinclusiveBJet"); 

//   analysis->plotInc("pZeta-1.5*pZetaVis",100,-150,100,1,0,analysis->getSMcut(0),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c,0,"MSSM0JetLow");
//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(0),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM0JetLow");
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",50,-150,100,1,0,analysis->getSMcut(1),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c,0,"MSSM0JetHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(1),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM0JetHigh");
  
//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,0,analysis->getSMcut(2),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c,0,"MSSMBoostedLow");
//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(2),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBoostedLow");  

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,0,analysis->getSMcut(3),"",0,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c,0,"MSSMBoostedHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(3),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSMBoostedHigh");     

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,-1,analysis->getSMcut(5),"",4,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c4,0,"MSSMBJetLow");
//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(5),"(svfitmass<100)",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c4,0,"MSSMBJetLow"); 

//   analysis->plotInc("pZeta-1.5*pZetaVis",25,-150,100,1,-1,analysis->getSMcut(6),"",4,0,"p_{#zeta} - 1.5 x p_{#zeta}^{vis}    [GeV]","",c4,0,"MSSMBJetHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(6),"(svfitmass<100)",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c4,0,"MSSMBJetHigh");
 

  gROOT->ProcessLine(".q");
}






  ////For ZEE Scale Factors: 
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5)","",0,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_et_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&taupt<40)","",0,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0BarrelLowPt_et_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&taupt>40)","",0,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0BarrelHighPt_et_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>=1.5)","",0,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Endcap_et_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5)","",2,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_et_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&taupt<40)","",0,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0BarrelLowPt_et_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&taupt>40)","",0,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0BarrelHighPt_et_2012");
//    analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>=1.5)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Endcap_et_2012");
//    analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&taupt<40)","",0,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0EndcapLowPt_et_2012");
//    analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&taupt>40)","",0,0,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0EndcapHighPt_et_2012");

  //analysis->plotInc("ditaumass",20,70,150,1,1,1,"","",2,5,"m_{vis}   (GeV)","",c,0,"mvis_ZEEZoom_et_2012");
  //analysis->plotInc("ditaumass",20,70,150,1,1,1,"(taudecaymode==0&&njet==0&&taupt>40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_ZEEZoom1Prong0Pi0_et_2012");
  //analysis->plotInc("ditaumass",20,70,150,1,1,1,"(taudecaymode==1&&njet==0&&taupt>40&&abs(taueta)<1.5)","",2,5,"m_{vis}   (GeV)","",c,0,"mvis_ZEEZoom1Prong1Pi0_et_2012");
  //analysis->plotInc("svfitmass",32,90,200,1,1,1,"(taudecaymode==1)","",2,5," m(#tau#tau)   (GeV)","",c,0,"m_sv_ZEEZoom1Prong1Pi0_et_2012");

//   analysis->plotInc("ditaumass",50,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30)","",0,2," mvis","",c,0,"mvis_ZEE0Pi0_et_2012");
//   analysis->plotInc("ditaumass",50,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30)","",0,2," mvis","",c,0,"mvis_ZEE1Pi0_et_2012");

//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt>40)","",2,2," mvis","",c,0,"mvis_ZEE1Pi0HighPt_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt>40)","",2,2," mvis","",c,0,"mvis_ZEE0Pi0HighPt_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,analysis->getSMcut(1),"",2,2,"m_{vis}   (GeV)","",c,0,"mvis_ZEE0jethigh_et_2012");

//   analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(1)+"*(taudecaymode==0&&abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",2,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_ZEE0Pi00jethigh_et_2012");
//   analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(1)+"*(taudecaymode==1&&abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",2,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_ZEE1Pi00jethigh_et_2012");
//   analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(1)+"*(abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",2,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_ZEE0jethigh_et_2012");
//   analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(1)+"*(abs(taueta)<1.5)","",2,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jethighZEECorrected_et_2012");

//   analysis->plotInc("metpt",25,0,100,1,1,0,analysis->getSMcut(1)+"*(taudecaymode==0&&abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",0,2," MET   (GeV)","",c,0,"mvamet_ZEE0Pi00jethigh_et_2012");
//   analysis->plotInc("metpt",25,0,100,1,1,0,analysis->getSMcut(1)+"*(taudecaymode==1&&abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",0,2," MET   (GeV)","",c,0,"mvamet_ZEE1Pi00jethigh_et_2012");
//   analysis->plotInc("metpt",25,0,100,1,1,0,analysis->getSMcut(1)+"*((taudecaymode==0||taudecaymode==1)&&abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",0,2," MET   (GeV)","",c,0,"mvamet_ZEE0jethigh_et_2012");

//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt<40)","",2,2," mvis","",c,0,"mvis_ZEE1Pi0LowPt_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt<40)","",2,2," mvis","",c,0,"mvis_ZEE0Pi0LowPt_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,analysis->getSMcut(0),"",2,2,"m_{vis}   (GeV)","",c,0,"mvis_ZEE0jetlow_et_2012");
//   analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(0)+"*(taudecaymode==0&&abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",2,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_ZEE0Pi00jetlow_et_2012");
//   analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(0)+"*(taudecaymode==1&&abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",2,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_ZEE1Pi00jetlow_et_2012");
//   analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(0)+"*(abs(taueta)<1.5&&80<ditaumass&&ditaumass<110)","",2,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_ZEE0jetlow_et_2012");
//   analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(0)+"*(abs(taueta)<1.5)","",2,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetlowZEECorrected_et_2012");

  //effect of new corrections on Boosted 
  //analysis->plotInc("svfitmass",100,0,300,1,1,1,analysis->getSMcut(3),"",7,4,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,1,"m_sv_1jethigh_ZEECheck_et_2012");  


//   ///for ZEECorrection :: Change mT cut to 40
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt<40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pTLow_et_2012");
//  analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt>40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pTHigh_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt<40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pTLow_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&taupt>40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pTHigh_et_2012");

//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&25<taupt&&taupt<30)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pT25_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&30<taupt&&taupt<35)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pT30_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&35<taupt&&taupt<40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pT35_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&40<taupt&&taupt<45)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pT40_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&45<taupt&&taupt<50)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pT45_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&50<taupt&&taupt<55)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pT50_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)<1.5&&njet<2&&metpt<30&&55<taupt&&taupt<60)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Barrel_pT55_et_2012");


//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>1.5&&njet<2&&metpt<30&&25<taupt&&taupt<30)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Endcap_pT25_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>1.5&&njet<2&&metpt<30&&30<taupt&&taupt<35)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Endcap_pT30_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>1.5&&njet<2&&metpt<30&&35<taupt&&taupt<40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Endcap_pT35_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>1.5&&njet<2&&metpt<30&&40<taupt&&taupt<45)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Endcap_pT40_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>1.5&&njet<2&&metpt<30&&45<taupt&&taupt<50)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Endcap_pT45_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>1.5&&njet<2&&metpt<30&&50<taupt&&taupt<55)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Endcap_pT50_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==0&&abs(taueta)>1.5&&njet<2&&metpt<30&&55<taupt&&taupt<60)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong0Pi0Endcap_pT55_et_2012");

//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&25<taupt&&taupt<30)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pT25_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&30<taupt&&taupt<35)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pT30_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&35<taupt&&taupt<40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pT35_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&40<taupt&&taupt<45)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pT40_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&45<taupt&&taupt<50)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pT45_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&50<taupt&&taupt<55)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pT50_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)<1.5&&njet<2&&metpt<30&&55<taupt&&taupt<60)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Barrel_pT55_et_2012");

//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&njet<2&&metpt<30&&25<taupt&&taupt<30)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Endcap_pT25_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&njet<2&&metpt<30&&30<taupt&&taupt<35)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Endcap_pT30_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&njet<2&&metpt<30&&35<taupt&&taupt<40)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Endcap_pT35_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&njet<2&&metpt<30&&40<taupt&&taupt<45)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Endcap_pT40_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&njet<2&&metpt<30&&45<taupt&&taupt<50)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Endcap_pT45_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&njet<2&&metpt<30&&50<taupt&&taupt<55)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Endcap_pT50_et_2012");
//   analysis->plotInc("ditaumass",100,0,200,1,1,1,"(taudecaymode==1&&abs(taueta)>1.5&&njet<2&&metpt<30&&55<taupt&&taupt<60)","",2,2,"m_{vis}   (GeV)","",c,0,"mvis_1Prong1Pi0Endcap_pT55_et_2012");






  /////////////////QCD templates
  //Float_t xbinsValues[27]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350};
  //analysis->setVariableBinning(26,xbinsValues);
  /////0JetLow (Use inclusive W+jets)
  //analysis->plotInc("transversemass",30,0,300,2,1,0,analysis->getSMcut(0),"",-1,0,"m_{T} ","",c,0,"mt_1_met_0jetlow_SS_et_2012");
  //analysis->plotInc("svfitmass",0,0,350,2,1,1,analysis->getSMcut(0),"",-1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetlow_SS_et_2012"); 
  //analysis->plotInc("svfitmass",0,0,350,2,0,1,analysis->getSMcut(0)+"*(tauisodiscmva>=1&&0.2<muiso&&muiso<0.5)","",-1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetlow_SS_et_2012_AntiIso"); 


   /////1JetLow (Use  Sum of exclusive W+njets)
  //analysis->plotInc("transversemass",30,0,300,2,1,0,analysis->getSMcut(2),"",-1,2,"m_{T} ","",c,0,"mt_1_met_1jetlow_SS_et_2012");
  //analysis->plotInc("svfitmass",0,0,350,2,1,1,analysis->getSMcut(2),"",-1,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_1jetlow_SS_et_2012"); 
  //analysis->plotInc("svfitmass",0,0,350,2,0,1,analysis->getSMcut(2)+"*(tauisodiscmva>=1&&0.2<muiso&&muiso<0.5)","",-1,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_1jetlow_SS_et_2012_AntiIso"); 


  ///For TauTau Boosted
  //analysis->plotInc("svfitmass",35,0,350,1,1,1,"(mupt>30&&taupt>45&&njet>=1&&leadJetPt>50&&vbfvars3>140)","",2,2," m(#tau#tau)   (GeV)","",c,1,"m_sv_TauTauBoost_et_2012");
  //analysis->plotInc("ditaumass",35,0,350,1,1,1,"(mupt>30&&taupt>45&&njet>=1&&leadJetPt>50&&vbfvars3>140)","",2,2," m(e#tau)   (GeV)","",c,1,"mvis_TauTauBoost_et_2012");



//   analysis->plotInc("vbfvars20122",20,0,10,1,1,1,"(njet>=2)","",2,2,"#Delta#eta(#mu#tau-j)","",c,0,"visjeteta_et_2012");
//   analysis->plotInc("vbfvars20123",20,0,200,1,1,1,"(njet>=2)","",2,2,"#mu#tau p_{T}","",c,0,"ptvis_et_2012");
//   analysis->plotInc("vbfmva2012",21,-1,1.1001,1,1,1,"(njet>=2&&njetingap==0)","(vbfmva2012<0.9)",2,2,"VBF MVA output","",c2,5,"vbfmva_et_2012");
//   analysis->plotInc("vbfmva2012",11,0.,1.1001,1,1,1,"(njet>=2&&njetingap==0)","(vbfmva2012<0.9)",2,2,"VBF MVA output","",c4,5,"vbfmva01_et_2012");


//    analysis->plotInc("nvtx",50,-.5,49.5,1,1,1,"","",5,4,"# of reco. vertices ","",c,0,"inclusive");
  
//   analysis->plotInc("mupt",25,0,100,1,1,1,"","",5,4," electron pt   (GeV)","",c,0,"inclusive");
//   analysis->plotInc("mueta",20,-2.5,2.5,1,1,1,"","",5,4," electron  #eta","",c2,0,"inclusive"); 
//   analysis->plotInc("muphi",20,-3.5,3.5,1,1,1,"","",5,4," electron phi ","",c3,0,"inclusive");    
//   analysis->plotInc("muiso",90,.1,1,1,0,1,"(tauisodiscmva>=1)","",0,1," electron relIso","",c,0,"inclusive");
//   analysis->plotInc("mudz",100,-.05,.05,1,1,1,"","",5,4," electron dz","",c,0,"inclusive");
//   analysis->plotInc("mudxy",100,-.03,.03,1,1,1,"","",5,4," electron dxy","",c,0,"inclusive");

//   analysis->plotInc("taupt",25,0,100,1,1,1,"","",5,4," tau pt   (GeV)","",c,0,"inclusive");
//   analysis->plotInc("taueta",20,-2.5,2.5,1,1,1,"","",5,4," tau  #eta","",c2,0,"inclusive"); 
//   analysis->plotInc("tauphi",20,-3.5,3.5,1,1,1,"","",5,4," tau phi ","",c3,0,"inclusive");    
//   analysis->plotInc("tauisomva",20,-1.0,1.01,1,0,1,"(muiso<0.1)","",0,1,"tau iso mva","",c,0,"inclusive");
//   analysis->plotInc("taudz",100,-.05,.05,1,1,1,"","",5,4," tau dz","",c,0,"inclusive");
//   analysis->plotInc("taudxy",100,-.03,.03,1,1,1,"","",5,4," tau dxy","",c,0,"inclusive");

//   analysis->plotInc("metpt",30,0,150,1,1,0,"","",5,4," MET   (GeV)","",c,0,"inclusive");    
//   analysis->plotInc("metphi",20,-3.5,3.5,1,1,0,"","",5,4," MET  phi ","",c,0,"inclusive");    
//   analysis->plotInc("1-cos(metphi-muphi)",40,0,2.001,1,1,0,"","",5,4," 1 - cos(#Delta#phi) ","",c,0,"inclusive");    
//   analysis->plotInc("metpt*cos(metphi)",30,-150,150,1,1,0,"","",5,4," MET  X  (GeV)","",c2,0,"inclusive");    
//   analysis->plotInc("metpt*sin(metphi)",30,-150,150,1,1,0,"","",5,4," MET  Y (GeV)","",c2,0,"inclusive");    
  
//  analysis->plotInc("transversemass",40,0,200,1,1,0,"","",0,0,"M_{T}   [GeV]","",c,0,"inclusive");
//  analysis->plotInc("ditaumass",40,0,200,1,1,1,"","",5,4,"m_{vis}   (GeV)","Events / 5 GeV",c,0,"inclusive");  
//  analysis->plotInc("svfitmass",60,0,300,1,1,1,"","",5,5," m(#tau#tau)   (GeV)","Events / 5 GeV",c,0,"inclusive");
  
  //  analysis->plotInc("transversemass",20,0,200,1,1,0,"(njet>=1)","",5,5,"m_{T}   (GeV)","",c,0,"inclusive1Jet");
  //   analysis->plotInc("leadJetPt",20,0,200,1,1,1,"(njet>=1)","",5,4,"lead jet p_{T}   (GeV)","",c,0,"inclusive1Jet");
  //   analysis->plotInc("leadJetEta",20,-5,5,1,1,1,"(njet>=1)","",5,4,"lead jet #eta","",c2,0,"inclusive1Jet");
  //  analysis->plotInc("svfitmass",18,0,360,1,1,1,"(njet>=1)","",5,5," m(#tau#tau)   (GeV)","",c,0,"inclusive1Jet");
  
//   analysis->plotInc("transversemass",30,0,300,1,1,0,"(njet>=2)","",5,5,"m_{T}   (GeV)","",c,0,"inclusive2Jet");
//   analysis->plotInc("leadJetPt",20,0,200,1,1,1,"(njet>=2)","",5,4,"lead jet p_{T}   (GeV)","",c,0,"inclusive2Jet");
//   analysis->plotInc("leadJetEta",20,-5,5,1,1,1,"(njet>=2)","",5,4,"lead jet #eta","",c,0,"inclusive2Jet");
//   analysis->plotInc("subleadJetPt",20,0,200,1,1,1,"(njet>=2)","",5,4,"sublead jet p_{T}   (GeV)","",c,0,"inclusive2Jet");
//   analysis->plotInc("subleadJetEta",20,-5,5,1,1,1,"(njet>=2)","",5,4,"sublead jet #eta","",c,0,"inclusive2Jet"); 
//   analysis->plotInc("diJetMass",20,0,1000,1,1,1,"(njet>=2)","",5,4,"M(jj)","",c,0,"inclusive2Jet"); 
//   analysis->plotInc("abs(diJetDeltaEta)",20,0,10,1,1,1,"(njet>=2)","",5,4,"#Delta#eta","",c,0,"nomina2Jet"); 
//   analysis->plotInc("svfitmass",18,0,360,1,1,1,"(njet>=2)","",5,5," m(#tau#tau)   (GeV)","",c,0,"inclusive2Jet");


  /////////////////////2012 categories

//   ////Plot the vbf mva variables
//   analysis->plotInc("vbfvars0",20,0,1000,1,1,1,"(njet>1)","",5,4,"M(jj)","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars1",20,0,10,1,1,1,"(njet>1)","",5,4,"#Delta#eta(jj)","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars2",20,0,3.4,1,1,1,"(njet>1)","",5,4,"#Delta#phi(jj)","",c2,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars3",20,0,200,1,1,1,"(njet>1)","",5,4,"#tau#tau   p_{T}","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars4",20,0,200,1,1,1,"(njet>1)","",5,4,"di-jet p_{T}","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars5",20,0,3.4,1,1,1,"(njet>1)","",5,4,"#Delta#phi(#tau#tau-jj)","",c2,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars6",20,0,10,1,1,1,"(njet>1)","",5,4,"#Delta#eta(#mu#tau-j)","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfvars7",20,0,200,1,1,1,"(njet>1)","",5,4,"#mu#tau p_{T}","",c,0,"inclusive2Jet");
//   analysis->plotInc("vbfmva",20,-1.,1.001,1,1,1,"(njet>1)","(vbfmva<0.5)",5,4,"VBF MVA output","",c,10,"inclusive2Jet");
//   analysis->plotInc("vbfmva",10,0.0,1.001,1,1,1,"(njet>1&&njetingap==0&&abs(svfitmass-125)<25.)","(vbfmva<0.5)",5,4,"VBF MVA output","",c,5,"inclusive2JetSignal");

  
//   analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(0),"",5,5,"m_{T} ","",c,0,"0JetLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(0),"(svfitmass<100||160<svfitmass)",5,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"0JetLow");
  
//   analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(1),"",5,5,"m_{T} ","",c,0,"0JetHigh");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(1),"(svfitmass<100||160<svfitmass)",5,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"0JetHigh");
  
//   analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(2),"",5,5,"m_{T} ","",c,0,"BoostedLow");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(2),"(svfitmass<100||160<svfitmass)",5,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"BoostedLow");  

//  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(3),"",5,5,"m_{T} ","",c,0,"BoostedHigh");
//  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(3),"(svfitmass<100||160<svfitmass)",5,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"BoostedHigh");     

//  analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMcut(4),"",5,5,"m_{T} ","",c,0,"VBF");
//  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(4),"(svfitmass<100||160<svfitmass)",5,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"VBF");
//  analysis->plotInc("ditaumass",0,0,260,1,1,1,analysis->getSMcut(4),"(ditaumass<80||140<ditaumass)",5,5,"m(#mu#tau)   (GeV)","",c,0,"VBF");





 // ////////////////////////////********************************************************************************************
  
  //misc checks
  //svfitmass at high mT
  //analysis->plotInc("svfitmass",30,0,300,1,3,analysis->getSMcut(1),"",0,0," m(#tau#tau)   (GeV)","Events / 5 GeV",c,0,"Check");

  
  ////////////Simple Tau fake rate ratios
  //analysis->plotTauFakeRate("taujetpt",16,20,100,"(njet==0)",50,1);//*(transversemass<15.)//*(15<=transversemass&&transversemass<30.)//*(30<=transversemass)
  //analysis->plotTauFakeRate("mupt",4,20,60,0);
  //analysis->plotTauFakeRate("taupt",4,20,60,2);
  //analysis->plotTauFakeRate("abs(taueta)",6,0,3.0);
  //analysis->plotTauFakeRate("abs(taujeteta)",6,0,3.0,0);
  //analysis->plotTauFakeRate("njet",4,-0.5,3.5);
  //analysis->plotTauFakeRate("transversemass",15,0,150,0);
  //analysis->plotTauFakeRate("cos(metphi-muphi)",10,-1,1.0001,1);

  /////////////Ratio for both mu and tau anti-Iso
  //AverageRatio values as a function of njets:
  //n=0--> 0.0246023, n=1-->0.0187078, n=2-->0.0125766, n=3-->0.0148136, n=4 -->0.0175073 +- 0.00520148

  /////there are 4 regions: 1=[tauIso,muIso] 2=[tauAntiIso,muIso]  3=[tauIso,muAntiIso] 4=[tauAntiIso,muAntiIso]
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"(muiso<0.1)","(tauisodisc>=2)",50,1);//region 1/2 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"(tauisodisc>=2)","(muiso<0.1)",50,1);//region 1/3
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"","(tauisodisc>=2)",50,1);//region 4 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"","(muiso<0.1)",50,1);//region 4

  //for LooseMVA tau isolation
  //analysis->plotIsoFakeRate("taujetpt",16,20,100,"(pftransversemass<40.&&muiso<0.1)","(tauisodiscmva>=1)",50,1);//region 1/2 
  //analysis->plotIsoFakeRate("mujetpt",16,17,97,"(pftransversemass<40.&&tauisodiscmva>=1)","(muiso<0.1)",50,1);//region 1/3

  ///Ratio for W+jets (mT>60)
  //analysis->plotTauFakeRateWJets("taujetpt",16,20,100,"(pftransversemass>60.&&muiso<0.1)",50,1);
  


////////////////////////////////////////////////////
//   Float_t sigY=0;
//   sigY+=analysis->getSample("HiggsGG120")->Integral();
//   sigY+=analysis->getSample("HiggsVBF120")->Integral();
//   sigY+=analysis->getSample("HiggsVH120")->Integral();
//   cout<<"Signal yield "<<sigY<<endl;

  /////////////inclusive SS
  //analysis->plotIncSS("ditaumass",1,1,-1,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);
  //analysis->plotIncSS("taupt",1,1,-1,50,0,100,""," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("muiso",-1,1,-1,100,0,2," #mu relIsoDB","",c);
  //analysis->plotIncSS("tauiso",-1,1,-1,100,0,2," #tau relIso","",c);
  //analysis->plotIncSS("tauisodisc",-1,1,-1,6,-0.5,5.5," #tau iso disc.","",c);
  //analysis->plotIncSS("pftransversemass",30,0,150,1,0,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("transversemass",1,0,-1,30,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("nditau",1,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("metpt",1,0,-1,50,0,200," MET   (GeV)","Events / 4 GeV",c);    
  //////////////inclusive anti
  //analysis->plotInc("nditau",2,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("taupt",2,1,-1,12,0,60," #tau p_{T}   (GeV)","Events / 5 GeV",c);  
  //analysis->plotInc("metpt",2,0,-1,50,0,200," MET   (GeV)","Events / 4 GeV",c);    
  //analysis->plotInc("transversemass",2,0,-1,50,0,200,"m_{T}   (GeV)","Events / 4 GeV",c);
  //////////////inclusive SS anti
  //analysis->plotIncSS("taupt",2,1,-1,8,20,100," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,1,-1,6,0,3," #eta_{#tau}","",c);   
  //analysis->plotIncSS("nditau",2,1,-1,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("ditaumass",2,1,-1,60,0,300,"m_{vis}   (GeV)","Events / 5 GeV",c,0);
  //analysis->plotIncSS("transversemass",2,0,-1,30,0,150,"m_{T}   (GeV)","",c);

  

  //////////// //////////////SM0/////////////////////////
  //analysis->plotInc("ditaumass",1,1,0,60,0,300,"","m_{vis}   (GeV)","Events / 5 GeV",c);
  //analysis->plotInc("taupt",1,1,0,8,20,100," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("taujetpt",1,1,-1,8,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,-1,60,0,300,"(njet==0)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,20,0,100,"(njet==0)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,20,0,100,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("metpt",1,1,-1,12,0,60,"(njet==0)"," MET   (GeV)","",c);    
  /////SS
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("taujetpt",1,1,-1,16,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("taupt",1,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  ////Anti
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&muiso<0.1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&tauisodisc>=2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==0&&muiso>0.1&&tauisodisc<2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",2,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  ////SS anti
  //analysis->plotIncSS("transversemass",2,0,-1,20,0,200,"(njet==0)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("metpt",2,1,-1,12,0,60,"(njet==0)"," MET   (GeV)","",c);  
  //analysis->plotIncSS("taupt",2,1,-1,6,0,60,"(njet==0)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("taujetpt",2,1,-1,16,20,100,"(njet==0)"," #tau jet  p_{T}   (GeV)","",c);  

  ////////////////////////////SM1//////////////////////////
  //analysis->plotInc("transversemass",1,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,1,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,20,0,100,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,20,0,100,"(njet==1)"," #tau p_{T}   (GeV)","",c);  
  //analysis->plotInc("ditaumass",1,1,-1,30,0,300,"(njet==1)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("metpt",1,1,-1,15,0,150,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("taujetpt",1,1,-1,8,20,100,"(njet==1)"," #tau jet  p_{T}   (GeV)","",c);  
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotInc("(1-cos(muphi-metphi))",1,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  //analysis->plotInc("cos(tauphi-muphi)",1,1,-1,20,-1,1.001,"(njet==1)","cos(#mu_#phi-#tau_#phi)","",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet>=1)","m_{T}   (GeV)","",c);
  ////////////////////////////SS
  //analysis->plotIncSS("transversemass",1,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("taupt",1,1,1,20,0,200," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",1,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("mupt",1,1,-1,8,10,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",1,0,10,6,0,3," |#eta_{#tau}| ","",c); 
  //analysis->plotIncSS("metpt",1,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotIncSS("(1-cos(muphi-metphi))",1,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  //analysis->plotIncSS("taujetpt",1,1,-1,16,20,100,"(njet==1)"," #tau jet  p_{T}   (GeV)","",c);  
  //analysis->plotIncSS("transversemass",1,0,-1,20,0,200,"(njet>=1)","m_{T}   (GeV)","",c);
  //////////////////////////////anti
  //analysis->plotInc("transversemass",2,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("tauisodisc",2,1,1,6,-.5,5.5,"tau isolation discriminator ","",c,0);
  //analysis->plotInc("taupt",2,1,1,20,0,200," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("taujetpt",2,1,1,10,0,100,""," #tau jet  p_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",2,1,1,10,0,100,""," #mu p_{T}   (GeV)","",c);  
  //analysis->plotInc("ditaumass",2,1,1,15,0,300,"","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotInc("transversemass",2,0,-1,20,0,200,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotInc("metpt",2,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("metpt",2,3,-1,40,0,200,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotInc("mupt",2,1,-1,10,0,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotInc("(1-cos(muphi-metphi))",2,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);
  ////////////////////////////SS anti
  //analysis->plotIncSS("taupt",2,1,1,20,0,200," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",2,1,1,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("transversemass",2,0,1,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,0,10,6,0,3," |#eta_{#tau}| ","",c); 
  //analysis->plotIncSS("transversemass",2,0,-1,15,0,150,"(njet==1)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("metpt",2,1,-1,12,0,60,"(njet==1)"," MET   (GeV)","",c);    
  //analysis->plotIncSS("mupt",2,1,-1,10,0,50,"(njet==1)"," #mu p_{T}   (GeV)","",c);
  //Float_t c[4]={.4,.65,.42,.81};  analysis->plotIncSS("(1-cos(muphi-metphi))",2,1,-1,21,0,2.1,"(njet==1)","1-cos#Delta#phi","",c);


  ////////////////////////////SM2
  //analysis->plotInc("transversemass",1,0,2,15,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotInc("svfitmass",1,1,2,14,0,350,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotInc("taupt",1,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("mupt",1,1,2,10,0,100," #mu p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("nditau",1,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("transversemass",1,0,-1,20,0,200,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("mupt",1,1,-1,10,0,100,"(njet==2)"," #mu p_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",1,1,-1,10,0,100,"(njet==2)","#tau  p_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",1,1,-1,15,0,300,"(njet==2)","m_{vis}   (GeV)","",c);
  //analysis->plotInc("leadJetPt",1,1,-1,20,0,400,"(njet>=2)"," jet p_{T}   (GeV)","",c);
  //analysis->plotInc("leadJetEta",1,1,-1,18,-4.5,4.5,"(njet>=2)"," jet #eta","",c); 

  ///////////////////////////SS
  //analysis->plotIncSS("transversemass",1,0,2,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("ditaumass",1,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("nditau",1,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("transversemass",1,0,-1,15,0,150,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",1,0,20,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("metpt",1,0,20,20,0,200," MET   (GeV)","Events / 5 GeV",c);    
  //analysis->plotIncSS("taujetpt",1,1,-1,8,20,100,"(njet==2)"," #tau jet  p_{T}   (GeV)","",c);  
  ///////////////////////////anti
  //analysis->plotInc("transversemass",2,0,2,15,0,150,"","m_{T}   (GeV)","",c);
  //analysis->plotInc("taupt",2,1,2,10,0,100," #tau p_{T}   (GeV)","Events / 10 GeV",c);
  //analysis->plotInc("ditaumass",2,1,2,15,0,300,"","m_{vis}   (GeV)","Events / 20 GeV",c);
  //analysis->plotInc("nditau",2,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotInc("transversemass",2,0,-1,15,0,150,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotInc("ditaumass",2,1,20,12,0,300,"m_{vis}   (GeV)","Events / 25 GeV",c);
  ///////////////////////////SS anti
  //analysis->plotIncSS("transversemass",2,0,2,20,0,200,"","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("nditau",2,1,2,5,-.5,4.5,"number of candidates","",c);
  //analysis->plotIncSS("ditaumass",2,1,2,12,0,300,"","m_{vis}   (GeV)","Events / 25 GeV",c);
  //analysis->plotIncSS("taupt",2,1,2,8,20,100," #tau p_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,1,2,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("transversemass",2,0,-1,20,0,200,"(njet==2)","m_{T}   (GeV)","",c);
  //analysis->plotIncSS("abs(taueta)",2,0,20,6,0,3," #eta_{#tau} ","",c); 
  //analysis->plotIncSS("metpt",2,0,20,20,0,200," MET   (GeV)","Events / 5 GeV",c);    



  //////Tau Iso studies: LooseMVA=0.795, MediumMVA=0.884, TightMVA=0.921
  //inclusive
  //analysis->plotIncSS("transversemass",1,0,-1,30,0,150,"","m_{T}   (GeV)","",c);//Reference Loose deltaBetaCorr
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.795)","m_{T}   (GeV)","",c);//Loose MVA
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.884)","m_{T}   (GeV)","",c);//Medium MVA
  //analysis->plotIncSS("transversemass",0,0,-1,30,0,150,"(muiso<0.1&&tauisomva>0.921)","m_{T}   (GeV)","",c);//Tight MVA
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(muiso<0.1&&tauisomva>0.884)","m_{vis}   (GeV)","Events / 5 GeV",c);
  //////Muon Iso studies
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(muiso<0.1&&tauisodisc>=2)","m_{vis}   (GeV)","Events / 5 GeV",c);
  //analysis->plotInc("ditaumass",0,1,-1,40,0,200,"(((abs(mueta)<1.479)*(muisomva>0.96)+(abs(mueta)>1.479)*(muisomva>0.95))&&tauisodisc>=2)","m_{vis}   (GeV)","Events / 5 GeV",c);
  
