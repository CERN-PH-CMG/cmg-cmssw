#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauMuPlotter.h"
#include "configTauMu2012.C"
#include "configTauMu2012AB.C"
#include "configTauMu2012D.C"
#include "configTauMu2012ABCD.C"
#include "configTauMu2012ABCDTrig.C"
#include "configTauMu2012Summer13.C"
#include "configTauMu2012Summer13ReReco.C"
#include "configTauMu2012Summer13ReRecoTrig.C"


void plotInclusive(TauMuPlotter* analysis){
//   //analysis->plotInc("lhenup",10,4.5,14.5,1,1,1,"","",0,-2,"lhenup ","",c,0,"lhenup_mt_2012");
//   analysis->plotInc("nvtx",50,-.5,49.5,1,1,1,"","",0,2,"# of reco. vertices ","",c,0,"nvtx_mt_2012");
  
//   analysis->plotInc("mupt",25,0,100,1,1,1,"","",0,2," muon pt   (GeV)","",c,0,"pt_1_mt_2012");
//   analysis->plotInc("mueta",40,-2.5,2.5,1,1,1,"","",0,2," muon  #eta","",c2,0,"eta_1_mt_2012"); 
//   analysis->plotInc("muphi",20,-3.5,3.5,1,1,1,"","",0,2," muon phi ","",c3,0,"phi_1_mt_2012");    
//   analysis->plotInc("muiso",50,0,1,1,0,1,"(tauiso3hitraw<1.5)","",0,2," muon relIso","",c,0,"iso_1_mt_2012");
//   //analysis->plotInc("muiso",50,0,1,2,0,1,"(tauiso3hitraw<1.5)","",0,2," muon relIso","",c,0,"iso_1_SS_mt_2012");

  analysis->plotInc("taumass",20,0,2,1,1,1,"","",0,2," tau mass   (GeV)","",c,0,"taumass_mt_2012");
  analysis->plotInc("taupt",25,0,100,1,1,1,"","",0,2," tau pt   (GeV)","",c,0,"pt_2_mt_2012");
//   analysis->plotInc("taueta",40,-2.5,2.5,1,1,1,"","",0,2," tau  #eta","",c2,0,"eta_2_mt_2012"); 
//   analysis->plotInc("tauphi",20,-3.5,3.5,1,1,1,"","",0,2," tau phi ","",c3,0,"phi_2_mt_2012");    
//   //analysis->plotInc("tauisomva2raw",20,-1.0,1.01,1,0,1,"(muiso<0.1)","",0,2,"tau iso mva","",c,0,"iso_2_mt_2012");
//   analysis->plotInc("tauiso3hitraw",40,0,10,1,0,1,"(muiso<0.1)","",0,2,"tau iso. 3hit","",c,0,"iso_2_mt_2012");
//   analysis->plotInc("taudecaymode",11,0,11,1,1,1,"","",0,2,"Tau decay mode","",c,0,"taudecaymode_mt_2012");
//   analysis->plotInc("tauehop",60,0,1.2,1,1,1,"","",0,2," tau (E+H)/p  (GeV)","",c,0,"tauehop_mt_2012");

//   analysis->plotInc("transversemass",40,0,200,2,1,0,"","",-1,2,"M_{T}   [GeV]","",c,0,"mt_1_met_SS_mt_2012");
//   analysis->plotInc("transversemass",40,0,200,1,1,0,"","",0,2,"M_{T}   [GeV]","",c,0,"mt_1_met_mt_2012");
//   analysis->plotInc("metpt",30,0,150,1,1,0,"","",0,2," MET   (GeV)","",c,0,"mvamet_mt_2012");
//   analysis->plotInc("ditaumass",20,0,200,2,1,1,"","",-1,2,"m_{vis}   (GeV)","",c,0,"mvis_SS_mt_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_mt_2012");
//   //analysis->plotInc("svfitmass",30,0,300,2,1,1,"","",-1,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_SS_mt_2012");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_mt_2012");
//   analysis->plotInc("njet",5,-.5,4.5,1,1,1,"","",0,2," njet ","",c,0,"njets_mt_2012");

}

void plot1Jet(TauMuPlotter* analysis){
  //njet>=1 
  //analysis->plotInc("taudecaymode",11,0,11,1,1,1,"(njet>=1)","",0,2,"Tau decay mode","",c,0,"taudecaymode_1Jet_mt_2012");
  analysis->plotInc("transversemass",60,0,300,1,1,0,"(njet>=1)","",0,2,"M_{T}   [GeV]","",c,0,"mt_1_met_1Jet_mt_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_1Jet_mt_2012");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"(njet>=1)","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_1Jet_mt_2012");
  analysis->plotInc("leadJetPt",20,0,200,1,1,1,"(njet>=1)","",0,2,"lead jet p_{T}   (GeV)","",c,0,"jetpt_1_mt_2012");
  analysis->plotInc("leadJetEta",40,-5,5,1,1,1,"(njet>=1)","",0,2,"lead jet #eta","",c2,0,"jeteta_1_mt_2012");
}

void plotBJet(TauMuPlotter* analysis){
  ///nbjet>=1
  analysis->plotInc("transversemass",20,0,200,1,1,0,"(nbjet>=1&&njet<2)","",2,2,"M_{T}   [GeV]","",c,0,"mt_1_met_btag_mt_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"(nbjet>=1&&njet<2)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_btag_mt_2012");
  analysis->plotInc("svfitmass",60,0,300,1,1,1,"(nbjet>=1&&njet<2)","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_btag_mt_2012");
  analysis->plotInc("leadBJetPt",20,0,200,1,1,1,"(nbjet>=1&&njet<2)","",4,2," b jet p_{T}   (GeV)","",c,0,"bpt_btag_mt_2012");
  analysis->plotInc("leadBJetEta",25,-2.5,2.5,1,1,1,"(nbjet>=1&&njet<2)","",4,2," b jet #eta","",c4,0,"beta_btag_mt_2012"); 
}

void plot2Jet(TauMuPlotter* analysis){
  //njet>=2 Plots
  //analysis->plotInc("taudecaymode",11,0,11,1,1,1,"(njet>=1)","",0,2,"Tau decay mode","",c,0,"taudecaymode_1Jet_mt_2012");
  //analysis->plotInc("transversemass",60,0,300,2,1,0,"(njet>=2&&njetingap==0)","",-1,2,"M_{T}   [GeV]","",c,0,"mt_1_met_2Jet0JetInGap_SS_mt_2012");
  analysis->plotInc("transversemass",60,0,300,1,1,0,"(njet>=2&&njetingap==0)","",2,2,"M_{T}   [GeV]","",c,0,"mt_1_met_2Jet0JetInGap_mt_2012");
  analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=2)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_2Jet_mt_2012"); 
  analysis->plotInc("svfitmass",30,0,300,1,1,1,"(njet>=2&&njetingap==0)","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_2Jet0JetInGap_mt_2012");
  analysis->plotInc("subleadJetPt",20,0,200,1,1,1,"(njet>=2)","",2,2,"sublead jet p_{T}   (GeV)","",c,0,"jetpt_2_mt_2012");
  analysis->plotInc("subleadJetEta",20,-5,5,1,1,1,"(njet>=2)","",2,2,"sublead jet #eta","",c2,0,"jeteta_2_mt_2012");  
  analysis->plotInc("vbfvars20120",20,0,1000,1,1,1,"(njet>=2)","",2,2,"M(jj)","",c,0,"mjj_mt_2012");
  analysis->plotInc("vbfvars20121",20,0,10,1,1,1,"(njet>=2)","",2,2,"#Delta#eta(jj)","",c,0,"jdeta_mt_2012");
}

void plot0JetLow(TauMuPlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
  analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMCategory(0),"",0,2,"m_{T} ","",c,0,"mt_1_met_0jetlow_mt_2012");
  analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(0),"",0,2,"m_{vis}   (GeV)","",c,0,"mvis_0jetlow_mt_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(0),"",0,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetlow_mt_2012");
}
void plot0JetMed(TauMuPlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
  analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMCategory(1),"",0,2,"m_{T} ","",c,0,"mt_1_met_0jetmed_mt_2012");
  analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(1),"",0,2,"m_{vis}   (GeV)","",c,0,"mvis_0jetmed_mt_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(1),"",0,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetmed_mt_2012");
}
void plot0JetHigh(TauMuPlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
//   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(2),"",2,0,"m_{vis}   (GeV)","",c,0,"mvis_0jethigh_mt_2012");
//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMCategory(2),"",1,0,"m_{T} ","",c,0,"mt_1_met_0jethigh_mt_2012");
  //analysis->plotInc("svfitmass",35,0,350,2,0,1,analysis->getSMCategory(2)+"*(0.2<muiso&&muiso<0.5&&tauiso3hitraw<1.5)","",-1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jethigh_SS_MuIsoSide_mt_2012");
  analysis->plotInc("svfitmass",35,0,350,1,1,1,analysis->getSMCategory(2),"",1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jethigh_mt_2012");
}
void plot1JetMed(TauMuPlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMCategory(3),"",1,0,"m_{T} ","",c,0,"mt_1_met_1jet_medium_mt_2012");
//   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(3),mvBlindSel,1,0,"m_{vis}   (GeV)","",c,0,"mvis_1jet_medium_mt_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(3),svBlindSel,1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_medium_mt_2012");
}
void plot1JetHighLow(TauMuPlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMCategory(4),"",1,0,"m_{T} ","",c,0,"mt_1_met_1jet_high_lowhiggs_mt_2012");
//   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(4),mvBlindSel,1,0,"m_{vis}   (GeV)","",c,0,"mvis_1jet_high_lowhiggs_mt_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(4),svBlindSel,1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_high_lowhiggs_mt_2012");
}
void plot1JetHighHigh(TauMuPlotter* analysis){
  analysis->setVariableBinning(26,xbinsValues);
//   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMCategory(5),"",8,2,"m_{T} ","",c,0,"mt_1_met_1jet_high_highhiggs_mt_2012");
//   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(5),mvBlindSel,8,2,"m_{vis}   (GeV)","",c,0,"mvis_1jet_high_highhiggs_mt_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(5),svBlindSel,8,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_high_highhiggs_mt_2012");
}
void plotVBFLoose(TauMuPlotter* analysis){
  analysis->setVariableBinning(13,xbinsValuesVBF);
//   analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMCategory(6),"",5,3,"m_{T} ","",c,0,"mt_1_met_vbf_loose_mt_2012");
//   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(6),mvBlindSel,5,3,"m_{vis}   (GeV)","",c,5,"mvis_vbf_loose_mt_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(6),svBlindSel,5,6,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_loose_mt_2012");
}
void plotVBFTight(TauMuPlotter* analysis){
  analysis->setVariableBinning(13,xbinsValuesVBF);
//   analysis->plotInc("transversemass",15,0,300,1,1,0,analysis->getSMCategory(7),"",9,6,"m_{T} ","",c,0,"mt_1_met_vbf_tight_mt_2012");
//   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(7),mvBlindSel,9,6,"m_{vis}   (GeV)","",c,5,"mvis_vbf_tight_mt_2012");
  analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(7),svBlindSel,9,7,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_tight_mt_2012");
}


///0jet 
void plot0JetTauTau(TauMuPlotter* analysis){
   analysis->setVariableBinning(26,xbinsValues);
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(1),mvBlindSel,1,5,"m_{vis}   (GeV)","",c,0,"mvis_0jethighTauTau_mt_2012");
   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMcutTauTau(1),"",1,5,"m_{T} ","",c,0,"mt_1_met_0jethighTauTau_mt_2012");
   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcutTauTau(1),"",1,5,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jethighTauTau_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(1)+"*(taudecaymode==0)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_0jethighDecayMode0_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(1)+"*(taudecaymode==1)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_0jethighDecayMode1_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(1)+"*(taudecaymode==10)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_0jethighDecayMode10_mt_2012");
}

void plot1JetTauTau(TauMuPlotter* analysis){
   analysis->setVariableBinning(26,xbinsValues);
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(3),mvBlindSel,2,4,"m_{vis}   (GeV)","",c,0,"mvis_1jethighTauTau_mt_2012");
   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMcutTauTau(3),"",2,4,"m_{T} ","",c,0,"mt_1_met_1jethighTauTau_mt_2012");
   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcutTauTau(3),svBlindSel,2,4,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jethighTauTau_mt_2012");
   analysis->plotInc("ditaudeltaR",30,0,6,1,1,1,analysis->getSMcutTauTau(3),"",2,4," deltaR(#mu#tau) ","",c,1,"deltaR_1jethighTauTau_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(3)+"*(taudecaymode==0)",mvBlindSel,2,4,"m_{vis}   (GeV)","",c,0,"mvis_1jethighDecayMode0_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(3)+"*(taudecaymode==1)",mvBlindSel,2,4,"m_{vis}   (GeV)","",c,0,"mvis_1jethighDecayMode1_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(3)+"*(taudecaymode==10)",mvBlindSel,2,4,"m_{vis}   (GeV)","",c,0,"mvis_1jethighDecayMode10_mt_2012");
}


void plotVBFTauTau(TauMuPlotter* analysis){
   analysis->setVariableBinning(13,xbinsValuesVBF);
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(4),mvBlindSel,5,3,"m_{vis}   (GeV)","",c,0,"mvis_vbfTauTau_mt_2012");
   analysis->plotInc("transversemass",30,0,300,1,1,0,analysis->getSMcutTauTau(4),"",5,3,"m_{T} ","",c,0,"mt_1_met_vbfTauTau_mt_2012");
   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcutTauTau(4),svBlindSel,5,3,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbfTauTau_mt_2012");
   analysis->plotInc("ditaudeltaR",30,0,6,1,1,1,analysis->getSMcutTauTau(4),"",5,3," deltaR(#mu#tau) ","",c,1,"deltaR_vbfTauTau_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(4)+"*(taudecaymode==0)",mvBlindSel,5,3,"m_{vis}   (GeV)","",c,0,"mvis_vbfDecayMode0_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(4)+"*(taudecaymode==1)",mvBlindSel,5,3,"m_{vis}   (GeV)","",c,0,"mvis_vbfDecayMode1_mt_2012");
   analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMcutTauTau(4)+"*(taudecaymode==10)",mvBlindSel,5,3,"m_{vis}   (GeV)","",c,0,"mvis_vbfDecayMode10_mt_2012");
}

void plotIsoStudy(TauMuPlotter* analysis){
//   analysis->plotInc("tauiso",6,-0.5,5.5,1,0,1,"","",0,2,"#Delta#beta corr. discriminator","",c,0,"tauisodbcorr_mt_2012");
//   analysis->plotInc("tauisomvaraw",50,-1.,1.001,1,0,1,"","",0,2,"MVA isolation  ","",c,0,"tauisomvaraw_mt_2012");
//   analysis->plotInc("tauisomva2raw",50,-1.,1.001,1,0,1,"","",0,2,"MVA2 isolation ","",c,0,"tauisomva2raw_mt_2012");
//   analysis->plotInc("tauiso3hitraw",50,0,15,1,0,1,"","",0,2,"3hit isolation","",c,0,"tauiso3hitraw_mt_2012");

  analysis->plotInc("ditaumass",40,0,200,1,0,1,"(muiso<0.1&&tauiso>=2)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_tauisodBcorrLoose_mt_2012");//2,3,4
  analysis->plotInc("ditaumass",40,0,200,1,0,1,"(muiso<0.1&&tauisomvaraw>0.795)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_tauisoMVALoose_mt_2012");//0.795,0.884,0.921
  analysis->plotInc("ditaumass",40,0,200,1,0,1,"(muiso<0.1&&tauisomva2raw>0.85)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_tauisoMVA2Loose_mt_2012");//0.85, 0.90, 0.94
  analysis->plotInc("ditaumass",40,0,200,1,0,1,"(muiso<0.1&&tauiso3hitraw<2)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_tauiso3hitLoose_mt_2012");//2,1,0.8

}


void plotTauDecayModeStudy(TauMuPlotter* analysis){
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&20<taupt&&taupt<30&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt2030DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&20<taupt&&taupt<30&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt2030DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&20<taupt&&taupt<30&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt2030DecayMode10_mt_2012");

//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&30<taupt&&taupt<45&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt3045DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&30<taupt&&taupt<45&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt3045DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&30<taupt&&taupt<45&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt3045DecayMode10_mt_2012");

//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&taupt>45&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt45DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&taupt>45&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt45DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet==0&&taupt>45&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt45DecayMode10_mt_2012");

}

void plotTauMu2012(){  

  //TString path="/data/benitezj/Samples/TauMu2012V5_4_0_NewType1MET";//no recoil corr
  //TString path="/data/benitezj/Samples/TauMu2012V5_4_0_NewType1MET2";//applied 2012 recoil corrections
  //TString path="/data/benitezj/Samples/TauMu2012V5_4_0_NewType1MET3";//applied 2011 recoil corrections
  //TString path="/data/benitezj/Samples/TauMu2012V540_July7";//Type1 MET
  //TString path="/data/benitezj/Samples/TauMu2012V540_July7_2011WJets";//Type1 MET
  //TString path="/data/benitezj/Samples/TauMu2012V5_4_0_NewType1MET4"; //applied 2012 Type2 recoil corrections and trigger-matching
  //TString path="/data/benitezj/Samples/TauMu2012V5_4_0_NewType1MET4_2011WJets"; //replaced WJets root files with 2011 ones

  //TString path="/data/benitezj/Samples/TauMu2012V540_July7_2012WJets"; // includes new 2012 W+jets sample

  //TString path="/data/benitezj/Samples/TauMu2012V551Aug6svfit"; //debugged MVA MET
  //TString path="/data/benitezj/Samples/TauMu2012V551Aug8svfitPFMET";//PFMET all samples

  //TString path="/data/benitezj/Samples/muTau2012V551Sep4"; //fixed svfit and weights
  //TString path="/data/benitezj/Samples/muTau2012V56052XSep25"; //fixed pu weight, added SUSY samples, embeddedB3, singleTop

  ////-------------HCP 53X prod    ///////Nominal 2012 ABC 
  //TString path="/data/benitezj/Samples/muTau201253X_580Oct5"; //No embedded, new recoil corrector, 
  //TString path="/data/benitezj/Samples/muTau201253X_580Oct5B"; //No embedded, new recoil corrector, Josh's trigger eff, Rebecas Id+Iso, jets tagprob
  //TString path="/data/benitezj/Samples/muTau201253X_580Oct6"; //fixed vbf mva, jet lists, b-tag prob, (current diff is 3.5% --> 2.8% with 1.05qcdRatio)
  //TString path="/data/benitezj/Samples/muTau201253X_580Oct14"; //changed pu jet id,new e/mu/tau turn on curves + id/iso scale factors, 
  //TString path="/data/benitezj/Samples/muTau201253X_580Oct18Sync"; //fixed recoil for signals
  //TString path="/data/benitezj/Samples/muTau201253X_580Oct23"; //updated muon turn on curves, changed b-tagging.  remove iso SF in embedded, new 2012D
  //TauMuPlotter*analysis=configTauMu2012("analysis",path);

  /////////////2012A+B crosscheck
  //TString path="/data/benitezj/Samples/muTau201253XAB_580Oct16";
  //TauMuPlotter*analysis=configTauMu2012AB("analysis",path);

  //////////////2012D
  //TString path="/data/benitezj/Samples/muTau201253X_580Dec11"; //2012D only, used my measured tau trigger turn on
  //TString path="/data/benitezj/Samples/muTau201253X_580Dec13"; //2012D only, corrected tau turn-ons
  //TString path="/data/benitezj/Samples/muTau201253X_580Dec21"; //2012D turn-on curves and id/iso SF's
  //TauMuPlotter*analysis=configTauMu2012D("analysis",path);

  /////////////2012ABCD
  //TString path="/data/benitezj/Samples/muTau201253X_580Jan15";
  //TString path="/data/benitezj/Samples/muTau201253X_580Jan16";//new recoil corrections, added dR,dEta,dPhi
  //TString path="/data/benitezj/Samples/muTau201253X_580Jan29";//fixed MET shifts
  //TString path="/data/benitezj/Samples/muTau201253X_580Feb7";//applied Tau ES corrections
  //TauMuPlotter*analysis=configTauMu2012ABCD("analysis",path);


  //////For Summer13
  //TString path="/data/benitezj/Samples/muTau201253X_580Feb7";//old prod for comparison
  //TString path="/data/benitezj/Samples/muTau201253X_5140Apr5";//new Tau software, mva met, JEC, pu jet id 
  //TString path="/data/benitezj/Samples/muTau201253X_5140Apr7";//reverted e/p cut on Taus
  //TString path="/data/benitezj/Samples/muTau201253X_5140Apr11";//
  //TString path="/data/benitezj/Samples/muTau201253X_5140Apr14";//no anti-muon discr. cut
  //TString path="/data/benitezj/Samples/muTau201253X_5140Apr18Isolations";// OR of all Loose isolations
  //TString path="/data/benitezj/Samples/muTau201253X_5140Apr26_MVA2Iso_newPUJetIdWP";// MVA2 isolation, and new pu wp from Phil
  //TString path="/data/benitezj/Samples/muTau201253X_5140May2_TauIso3HitorMVA2";
  //TString path="/data/benitezj/Samples/muTau2012_5140May27";
  //TString path="/data/benitezj/Samples/muTau2012_5140May27_oldAntiMuon";
  //TString path="/data/benitezj/Samples/muTau2012_5140May31";
  //TauMuPlotter*analysis=configTauMu2012Summer13("analysis",path);

  //TString path="/data/benitezj/Samples/muTau2012_5160June10";
  //TString path="/data/benitezj/Samples/muTau2012_5160June23";
  //TString path="/data/benitezj/Samples/muTau2012_5160June28";//corrected Tau Trigger efficiencies
  //TString path="/data/benitezj/Samples/muTau2012_5160June28NoTauES";
  //TString path="/data/benitezj/Samples/muTau2012_5160June30";
  TString path="/data/benitezj/Samples/muTau2012_5160July1";
  TauMuPlotter*analysis=configTauMu2012Summer13ReReco("analysis",path); 

  //////////////Tau Trigger measurements
  //TString path="/data/benitezj/Samples/muTau201253X_580Nov14Trig"; 
  //TString path="/data/benitezj/Samples/muTau201253X_580Nov20Trig";   //fixed trigger flag, required status==3 for gen particle matching
  //TString path="/data/benitezj/Samples/muTau201253X_580Nov21Trig";   //removed 3rd lepton veto to compare to Josh
  //TString path="/data/benitezj/Samples/muTau201253X_580Dec13Trig";   //re-added 3rd lep veto, added 2012D pupWeight
  //TString path="/data/benitezj/Samples/muTau201253X_580Dec21Trig";     //added 2012ABCD pup weight 
//   TauMuPlotter*analysis=configTauMu2012ABCDTrig("analysis",path);
//   analysis->plotTauTrigger(1,"ABCD");
  //analysis->plotTauTriggerReal(2,"ABCD");//cross-check 
  
  //TString path="/data/benitezj/Samples/muTau2012_5160June10_TauTrig";
  //   TString path="/data/benitezj/Samples/muTau2012_5160June28_TauTrig";
//   TString path="/data/benitezj/Samples/muTau2012_5160June30_TauTrig";
//   TauMuPlotter*analysis=configTauMu2012Summer13ReRecoTrig("analysis",path);
//   analysis->plotTauTrigger(2,"Summer13ReReco");


  //////other studies
  //analysis->compareZTTEmbedded();
  //analysis->compareZTTEmbeddedUnfolding();
  //analysis->plotQCDSSOSRatio();
  //analysis->plotQCDSSOSRatioBTag();
  //analysis->plotZTTShape();
  //analysis->compareSignalShapes();


  //plots:incl., high pT, decay modes, MC , Emb
   
  //TString cut="(njet==0&&nbjet==0&&taudecaymode==0&&tauiso3hitraw>0.4&&tauiso3hitraw<1.0)";

  ////test plots
  //analysis->plotInc("nvtx",50,-.5,49.5,1,1,1,"","",0,2,"# of reco. vertices ","",c,0,"nvtx_mt_2012");
  //analysis->plotInc("transversemass",20,0,200,2,0,0,"","",-1,2,"M_{T}   [GeV]","",c,0,"mt_1_met_SS_mt_2012");

//   analysis->plotInc("transversemass",20,0,200,1,0,0,cut,"",0,2,"M_{T}   [GeV]","",c,0,"mt_1_met_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,1,1,cut,"",0,2," tau pt   (GeV)","",c,0,"pt_2_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,1,3,cut,"",0,2," tau pt   (GeV)","",c,0,"pt_2_W_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,0,1,cut+"*(muiso>0.2&&muiso<0.5)","",0,2," tau pt   (GeV)","",c,0,"pt_2_QCD_mt_2012");

  //  analysis->plotInc("taumass",20,0,2,1,1,1,"","",0,2," tau mass   (GeV)","",c,0,"taumass_mt_2012");
  //analysis->plotInc("ditaumass",40,0,200,2,0,1,"(njet==0&&nbjet==0&&taudecaymode==0&&muiso<0.1&&tauiso3hitraw>0.4&&tauiso3hitraw<1.0)","",-1,2,"m_{vis}   (GeV)","",c,0,"mvis_SS_mt_2012");
  //  analysis->plotInc("ditaumass",40,0,200,1,1,1,cut,"",0,2,"m_{vis}   (GeV)","",c,0,"mvis_mt_2012");
//  analysis->plotInc("ditaumetpt",40,0,200,1,1,1,"","",0,2,"pT(#mu#tau,MET)   (GeV)","",c,0,"higgspt_mt_2012");
//  analysis->plotInc("svfitmass",60,0,300,1,1,1,"","",0,2," m(#tau#tau)   (GeV)","",c,0,"m_sv_mt_2012");
//  analysis->plotInc("tauiso3hitraw",20,0,4,1,0,1,"(njet==0&&nbjet==0&&taudecaymode==0&&muiso<0.1)","",0,2,"tau iso. 3hit","",c,0,"iso_2_mt_2012");

  //analysis->setVariableBinning(NXBINS0JET,xbinsValues0Jet);
  //analysis->plotInc("ditaumass",0,0,200,1,1,1,analysis->getSMcutSummer13(0),"",0,2,"m(#mu#tau)  (GeV)","dN/dm(#mu#tau)",c,0,"mvis_0jetlow_mt_2012");
  //analysis->plotInc("ditaumass",0,0,200,1,1,1,analysis->getSMcutSummer13(1),"",1,5,"m(#mu#tau)  (GeV)","dN/dm(#mu#tau)",c,0,"mvis_0jethigh_mt_2012");

  //analysis->setVariableBinning(13,xbinsValuesVBF);
  //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(4),svBlindSel,5,3,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_mt_2012");
  //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(6),svBlindSel,5,3,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_loose_mt_2012");
  //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(7),svBlindSel,9,6,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_tight_mt_2012");

  //analysis->setVariableBinning(26,xbinsValues);
  //analysis->plotInc("ditaumass",0,0,350,1,1,1,analysis->getSMCategory(0),"",0,0,"m_{vis}   (GeV)","",c,0,"mvis_0jetlow_mt_2012");
  //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(5),svBlindSel,8,4,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_1jet_high_highhiggs_mt_2012");

//   analysis->setVariableBinning(NXBINSMSSM,xbinsValuesMSSM);
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMCategory(12),svBlindSel,0,4,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_btag_mt_2012");



  //plotIsoStudy(analysis);
  //plotTauDecayModeStudy(analysis);

  //  plotInclusive(analysis);
//   plot1Jet(analysis);
//   plot2Jet(analysis);
//   plotBJet(analysis);
//   plot0JetLow(analysis);
//   plot0JetMed(analysis);
  plot0JetHigh(analysis);
//  plot1JetMed(analysis);
//   plot1JetHighLow(analysis);
//  plot1JetHighHigh(analysis);
//   plotVBFLoose(analysis);
//  plotVBFTight(analysis);

//   plot0JetTauTau(analysis);
//   plot1JetTauTau(analysis);
//   plotVBFTauTau(analysis);




  ///////////////////////////////////
  //analysis->plotInc("taumass",20,0,2,1,1,1,"(taumass>0.2&&taupt>40)","",0,2," tau mass   (GeV)","",c2,0,"taumass_TauPt40_mt_2012");
  //analysis->plotInc("taumass",20,0,2,1,1,1,"(taumass>0.2&&taupt>40&&taudecaymode==0)","",0,2," tau mass   (GeV)","",c2,0,"taumass_TauPt40DecayMode0_mt_2012");
  //analysis->plotInc("taumass",20,0,2,1,1,1,"(taumass>0.2&&taupt>40&&taudecaymode==1)","",0,2," tau mass   (GeV)","",c2,0,"taumass_TauPt40DecayMode1_mt_2012");
  //analysis->plotInc("taumass",20,0,2,1,1,1,"(taumass>0.2&&taupt>40&&taudecaymode==10)","",0,2," tau mass   (GeV)","",c2,0,"taumass_TauPt40DecayMode10_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,1,1,"(abs(taueta)<1.5)","",0,2," tau pt   (GeV)","",c,0,"pt_2_Barrel_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,1,1,"(abs(taueta)>1.5)","",0,2," tau pt   (GeV)","",c,0,"pt_2_Endcap_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,1,1,"(taudecaymode==0)","",0,2," tau pt   (GeV)","",c,0,"pt_2_DecayMode0_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,1,1,"(taudecaymode==1)","",0,2," tau pt   (GeV)","",c,0,"pt_2_DecayMode1_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,1,1,"(taudecaymode==10)","",0,2," tau pt   (GeV)","",c,0,"pt_2_DecayMode10_mt_2012");


//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=1&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_1JetDecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=1&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_1JetDecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=1&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_1JetDecayMode10_mt_2012");

//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=1&&taupt<40&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_1JetTauPtL40DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=1&&taupt<40&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_1JetTauPtL40DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=1&&taupt<40&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_1JetTauPtL40DecayMode10_mt_2012");

//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_DecayMode10_mt_2012");

//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taupt<40&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPtL40DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taupt<40&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPtL40DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taupt<40&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPtL40DecayMode10_mt_2012");

//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taupt>40&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt40DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taupt>40&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt40DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(taupt>40&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_TauPt40DecayMode10_mt_2012");


//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=2&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_2JetDecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=2&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_2JetDecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=2&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_2JetDecayMode10_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=2&&taupt<40&&taudecaymode==0)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_2JetTauPtL40DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=2&&taupt<40&&taudecaymode==1)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_2JetTauPtL40DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",40,0,200,1,1,1,"(njet>=2&&taupt<40&&taudecaymode==10)","",0,2,"m_{vis}   (GeV)","",c,0,"mvis_2JetTauPtL40DecayMode10_mt_2012");



//   // // ////////////////////   //For TauTau categories/////////////////////////////////////////////
//   analysis->plotInc("mupt",25,0,100,1,1,1,"(taupt>45)","",0,4," mu pt   (GeV)","",c,0,"pt_1_TauPt45_mt_2012");
//   analysis->plotInc("taupt",25,0,100,1,1,1,"(mupt>30)","",0,4," tau pt   (GeV)","",c,0,"pt_2_MuPt30_mt_2012");
//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt45_mt_2012");

//   analysis->plotInc("taudecaymode",11,0,11,1,1,1,"(taupt>45)","",0,4,"Tau decay mode","",c,0,"taudecaymode_TauPt45_mt_2012");
//   analysis->plotInc("taudecaymode",11,0,11,1,1,1,"(taupt>45&&njet>=1)","",2,4,"Tau decay mode","",c,0,"taudecaymode_TauPt451Jet_mt_2012");
//   analysis->plotInc("taudecaymode",11,0,11,1,1,1,"(taupt>45&&njet>=2)","",2,4,"Tau decay mode","",c,0,"taudecaymode_TauPt452Jet_mt_2012");

//   analysis->plotInc("taumass",20,0,2,1,1,1,"(taupt>45&&taudecaymode==0)","",0,4," tau mass   (GeV)","",c,0,"taumass_TauPt45DecayMode0_mt_2012");
//   analysis->plotInc("taumass",20,0,2,1,1,1,"(taumass>0.2&&taupt>45&&taudecaymode==1)","",0,4," tau mass   (GeV)","",c2,0,"taumass_TauPt45DecayMode1_mt_2012");
//   analysis->plotInc("taumass",20,0,2,1,1,1,"(taumass>0.2&&taupt>45&&taudecaymode==10)","",0,4," tau mass   (GeV)","",c2,0,"taumass_TauPt45DecayMode10_mt_2012");

//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&taudecaymode==0)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt45DecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&taudecaymode==1)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt45DecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&taudecaymode==10)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt45DecayMode10_mt_2012");
   
//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&njet>=1&&taudecaymode==0)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt451JetDecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&njet>=1&&taudecaymode==1)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt451JetDecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&njet>=1&&taudecaymode==10)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt451JetDecayMode10_mt_2012");

//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&njet>=2&&taudecaymode==0)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt452JetDecayMode0_mt_2012");
//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&njet>=2&&taudecaymode==1)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt452JetDecayMode1_mt_2012");
//   analysis->plotInc("ditaumass",35,0,350,1,1,1,"(taupt>45&&njet>=2&&taudecaymode==10)",mvBlindSel,0,4,"m_{vis}   (GeV)","",c,0,"mvis_TauPt452JetDecayMode10_mt_2012");


   ///For Phil
   //analysis->plotInc("sqrt(2*taupt*metpt*(1-cos(tauphi-metphi)))",40,0,200,1,1,0,"(taupt>40&&transversemass<20)","",0,0,"M_{T}   [GeV]","",c,0,"mt_2_met_mt_2012_highTauPt");
   //analysis->plotInc("svfitmass",60,0,300,1,1,0,"(taupt>40&&transversemass<20&&sqrt(2*taupt*metpt*(1-cos(tauphi-metphi)))>50)","",0,0,"  ","",c,0,"m_sv_mt_2012_highTauPt");
   //analysis->plotInc("svfitmass",60,0,300,1,1,0,"(taupt>40&&transversemass<20&&sqrt(2*taupt*metpt*(1-cos(tauphi-metphi)))<50)","",0,0,"  ","",c,0,"m_sv_mt_2012_highTauPtLowMT2");


  //    ///////Z->mu mu fake rate
  //analysis->plotInc("ditaumass",200,0,200,1,1,1,"((taudecaymode==0||taudecaymode==1)&&njet<2&&metpt<30)","",0,2,"m_{vis}   (GeV)","",c,1,"mvis_mt_2012_ZMuMu");
//   analysis->plotInc("ditaumass",200,0,200,1,1,1,"((taudecaymode==0||taudecaymode==1)&&njet<2&&metpt<30&&taupt<40)","",0,2,"m_{vis}   (GeV)","",c,1,"mvis_mt_2012_ZMuMu_lowTauPt");
//   analysis->plotInc("ditaumass",200,0,200,1,1,1,"((taudecaymode==0||taudecaymode==1)&&njet<2&&metpt<30&&taupt>40)","",0,2,"m_{vis}   (GeV)","",c,1,"mvis_mt_2012_ZMuMu_highTauPt");


  //analysis->plotInc("ditaumass",20,70,150,1,1,1,"(metpt<20&&taudecaymode==0)","",0,4,"m_{vis}   (GeV)","",c,10,"mvis_mt_2012_ZMuMu1P0Pi0");//1Prong-0pi0
   //analysis->plotInc("ditaumass",20,70,150,1,1,1,"(metpt<20&&taudecaymode==1)","",0,4,"m_{vis}   (GeV)","",c,10,"mvis_mt_2012_ZMuMu1P1Pi0");//1Prong-1pi0

   //analysis->plotInc("svfitmass",50,50,200,1,1,1,"(metpt<20&&taudecaymode==0)","",0,4,"m(#tau#tau)  (GeV)","",c,10,"m_sv_mt_2012_ZMuMu");
   //analysis->plotInc("svfitmass",50,50,200,1,1,1,"(metpt<20&&taudecaymode==0&&(ditaumass<85||95<ditaumass))","",0,4,"m(#tau#tau)  (GeV)","",c,0,"m_sv_mt_2012_ZMuMuCut");

   //analysis->plotInc("svfitmass",50,50,200,1,1,1,"(1*(taudecaymode!=0)+(taudecaymode==0)*(ditaumass<85||95<ditaumass))","",0,4,"m(#tau#tau)  (GeV)","",c,0,"m_sv_mt_2012_ZMuMuCutAll");
   //analysis->plotInc("svfitmass",50,50,200,1,1,1,"","",0,4,"m(#tau#tau)  (GeV)","",c,0,"m_sv_mt_2012_NoZMuMuCutAll");


   //analysis->plotInc("ditaumass",100,0,200,1,1,1,"(metpt<20&&taudecaymode==0&&taupt>40)","",0,4,"m_{vis}   (GeV)","",c,10,"mvis_mt_2012_ZMuMu1P0Pi0_highTauPt");//1Prong-0pi0
   //analysis->plotInc("ditaumass",100,0,200,1,1,1,"(metpt<20&&taudecaymode==1&&taupt>40)","",0,4,"m_{vis}   (GeV)","",c,10,"mvis_mt_2012_ZMuMu1P1Pi0_highTauPt");//1Prong-1pi0

   //analysis->plotInc("taudecaymode*(taudecaymode<10)+3*(taudecaymode==10)",4,-0.5,3.5,1,1,1,"(taupt<40)","",0,0," tau pt   (GeV)","",c,0,"taudecaymode_mt_2012_lowTauPt");
   //analysis->plotInc("taudecaymode*(taudecaymode<10)+3*(taudecaymode==10)",4,-0.5,3.5,1,1,1,"(taupt>40)","",0,0," tau pt   (GeV)","",c,0,"taudecaymode_mt_2012_highTauPt");
   

   //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(1)+"*(taudecaymode==0)",svBlindSel,2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jethigh_mt_2012_1Prong0Pi0");
   //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(1)+"*(taudecaymode==1)",svBlindSel,2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jethigh_mt_2012_1Prong1Pi0");
   //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(0)+"*(taudecaymode==0)",svBlindSel,2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetlow_mt_2012_1Prong0Pi0");
   //analysis->plotInc("svfitmass",0,0,350,1,1,1,analysis->getSMcut(0)+"*(taudecaymode==1)",svBlindSel,2,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetlow_mt_2012_1Prong1Pi0");



   //HCP analysis with old VBF mva
   //analysis->plotInc("vbfmva",40,-1,1.1001,1,1,1,"(njet>=2&&njetingap==0)","",2,2,"VBF MVA output","",c2,0,"vbfmvaICHEP_2Jet0JetInGap_mt_2012");



  /////////////////QCD templates
  //Float_t xbinsValues[27]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350};
  //analysis->setVariableBinning(26,xbinsValues);
   /////0JetLow (Use inclusive W+jets)
   //analysis->plotInc("mupt",15,0,60,2,0,1,analysis->getSMcut(0)+"*(tauisodiscmva>=1&&0.<muiso&&muiso<0.1)","",-1,0," muon pt   (GeV)","",c,0,"pt_1_SS_mt_2012");
   //analysis->plotInc("taupt",15,0,60,2,0,1,analysis->getSMcut(0)+"*(tauisodiscmva>=1&&0.2<muiso&&muiso<0.5)","",-1,0," tau pt   (GeV)","",c,0,"pt_2_SS_mt_2012");
   //analysis->plotInc("transversemass",30,0,300,2,1,0,analysis->getSMcut(0),"",-1,0,"m_{T} ","",c,0,"mt_1_met_0jetlow_SS_mt_2012");
   //analysis->plotInc("svfitmass",0,0,350,2,1,1,analysis->getSMcut(0),"",-1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetlow_SS_mt_2012"); 
   //analysis->plotInc("svfitmass",0,0,350,2,0,1,analysis->getSMcut(0)+"*(tauisodiscmva>=1&&0.2<muiso&&muiso<0.5)","",-1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jetlow_SS_mt_2012_AntiIso"); 

   /////1JetLow (Use  Sum of exclusive W+njets)
   //analysis->plotInc("transversemass",30,0,300,2,1,0,analysis->getSMcut(2),"",-1,2,"m_{T} ","",c,0,"mt_1_met_1jetlow_SS_mt_2012");
   //analysis->plotInc("svfitmass",0,0,350,2,1,1,analysis->getSMcut(2),"",-1,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_1jetlow_SS_mt_2012"); 
   //analysis->plotInc("svfitmass",0,0,350,2,0,1,analysis->getSMcut(2)+"*(tauisodiscmva>=1&&0.2<muiso&&muiso<0.5)","",-1,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_1jetlow_SS_mt_2012_AntiIso"); 

   /////0JetHigh (Use inclusive W+jets)
   //analysis->plotInc("transversemass",30,0,300,2,1,0,analysis->getSMcut(1),"",-1,0,"m_{T} ","",c,0,"mt_1_met_0jethigh_SS_mt_2012");
   //analysis->plotInc("svfitmass",0,0,350,2,1,1,analysis->getSMcut(1),"",-1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jethigh_SS_mt_2012"); 
   //analysis->plotInc("svfitmass",0,0,350,2,0,1,analysis->getSMcut(1)+"*(tauisodiscmva>=1&&0.2<muiso&&muiso<0.5)","",-1,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_0jethigh_SS_mt_2012_AntiIso"); 

   /////1JetLow (Use  Sum of exclusive W+njets)
   //analysis->plotInc("transversemass",30,0,300,2,1,0,analysis->getSMcut(3),"",-1,2,"m_{T} ","",c,0,"mt_1_met_1jethigh_SS_mt_2012");
   //analysis->plotInc("svfitmass",0,0,350,2,1,1,analysis->getSMcut(3),"",-1,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_1jethigh_SS_mt_2012"); 
   //analysis->plotInc("svfitmass",0,0,350,2,0,1,analysis->getSMcut(3)+"*(tauisodiscmva>=1&&0.2<muiso&&muiso<0.5)","",-1,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_1jethigh_SS_mt_2012_AntiIso"); 



   ///***********Trigger Measurement*******************////
//    TString select="(taupt>20)*(1./(triggerEffWeightTau))*(trigTest1==1||trigTest2==1||trigTest3==1||trigTest4==1||trigTest5==1)";// 
//    analysis->plotInc("transversemass",40,0,200,2,1,0,select,"",-1,0,"M_{T}   [GeV]","",c,0,"mt_1_met_SS_mt_2012_Trig");
//    analysis->plotInc("mupt",50,0,100,1,1,1,select,"",0,0," muon pt   (GeV)","",c,0,"pt_1_mt_2012_Trig");
//    analysis->plotInc("taupt",50,0,100,1,1,1,select,"",0,0," tau pt   (GeV)","",c,0,"pt_2_mt_2012_Trig");
//    analysis->plotInc("metpt",30,0,150,1,1,0,select,"",0,0," MET   (GeV)","",c,0,"met_mt_2012_Trig");
//    analysis->plotInc("ditaumass",40,0,200,1,1,1,select,"",0,0,"m_{vis}   (GeV)","",c,0,"mvis_mt_2012_Trig");
//    analysis->plotInc("mueta",40,-2.5,2.5,1,1,1,select,"",0,0," muon  #eta","",c2,0,"eta_1_mt_2012_Trig"); 
//    analysis->plotInc("taueta",40,-2.5,2.5,1,1,1,select,"",0,0," tau  #eta","",c2,0,"eta_2_mt_2012_Trig"); 
//    analysis->plotInc("transversemass",40,0,200,1,1,0,select,"",0,0,"M_{T}   [GeV]","",c,0,"mt_1_met_mt_2012_Trig");
//    analysis->plotInc("taudecaymode*(taudecaymode<10)+3*(taudecaymode==10)",4,-0.5,3.5,1,1,1,select,"",0,0," tau pt   (GeV)","",c,0,"taudecaymode_mt_2012_Trig");


   ////////////////////
   //analysis->plotInc("transversemass",30,0,300,2,0,0,"(tauisomva>0.7&&0.05<muiso&&muiso<0.15)","",-1,2,"m_{T} ","",c,0,"mt_1_met_SS_mt_2012");
   //analysis->plotInc("svfitmass",0,0,350,2,0,1,analysis->getSMcut(2)+"*(tauisomva>0.7&&0.3<muiso&&muiso<0.5)","",-1,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_1jetlow_SS_mt_2012"); 
   //analysis->plotInc("svfitmass",0,0,350,2,0,1,analysis->getSMcut(3)+"*(tauisomva>-1.&&0.05<muiso&&muiso<0.15)","",-1,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"m_sv_SS_mt_2012"); 
  //analysis->plotInc("svfitmass",30,0,300,2,0,1,"(tauisodiscmva>=1&&0.05<muiso&&muiso<0.15)","",0,0," m(#tau#tau)   (GeV)","",c,0,"m_sv_mt_2012");


   ///For Ricardo tau_h  tau_h W+jets correction factors: &&leadJetPt>100&&subleadJetPt>30&&vbfvars20120>250&&abs(vbfvars20121)>2.5
   //analysis->plotInc("transversemass",30,0,300,2,1,0,"(mupt>30&&taupt>45&&njet>=1&&leadJetPt>50&&vbfvars3>140)","",-1,2,"m_{T} ","",c,0,"mt_1_met_SS_TauTauBoost_mt_2012");
   //analysis->plotInc("transversemass",30,0,300,2,1,0,"(mupt>30&&taupt>45&&vbfvars3>110&&leadJetPt>50&&abs(leadJetEta)<3.0)*(njet>=2&&njetingap==0&&diJetMass>250.&&abs(diJetDeltaEta)>2.5)","",-1,2,"m_{T} ","",c,0,"mt_1_met_SS_TauTauVBF_mt_2012");
   
   


/* ----------------MSSM ----------------------------------------*/

//   Float_t c4[4]={.65,.98,.52,.87};
//   Float_t xbinsValuesMSSM[20]={0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,450,500,600,700,800};
//   analysis->setVariableBinning(19,xbinsValuesMSSM);
//   analysis->setMSSMFlag(1);

//   analysis->plotInc("nbjet",5,-.5,4.5,1,101,"","",0,0," nbjet ","",c4,0,"MSSMinclusive");
//   analysis->plotInc("ditaumass",60,0,300,1,101,"","",0,0,"m(#mu#tau)  (GeV)","",c4,0,"MSSMinclusive"); 
//   analysis->plotInc("svfitmass",70,0,350,1,101,"","",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c4,0,"MSSMinclusive");

//  analysis->plotInc("leadBJetPt",20,0,200,1,101,"(nbjet>=1&&njet<2)","",0,2," b jet p_{T}   (GeV)","",c4,0,"MSSMinclusiveBJet");
//   analysis->plotInc("leadBJetEta",20,-5,5,1,101,"(nbjet>=1&&njet<2)","",4,22," b jet #eta","",c4,0,"MSSMinclusiveBJet"); 
//   analysis->plotInc("svfitmass",0,0,350,1,101,"(nbjet>=1&&njet<2)","",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c4,0,"MSSMinclusiveBJet");
//   analysis->plotInc("taupt",15,0,150,1,101,"(nbjet>=1&&njet<2)","",4,22,"tau pt (GeV)","",c4,0,"MSSMinclusiveBJet"); 

//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(0),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM0JetLow");

//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(1),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM0JetHigh");

//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(2),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM1JetLow");  

//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(3),"(svfitmass<100)",0,0,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,0,"MSSM1JetHigh");     

//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(5),"(svfitmass<100)",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c4,0,"MSSMBJetLow"); 

//   analysis->plotInc("svfitmass",0,0,350,1,101,analysis->getSMcut(6),"(svfitmass<100)",4,22,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c4,0,"MSSMBJetHigh");
  

  gROOT->ProcessLine(".q");
}



//   ////efficiency loss for 2012D:
//   analysis->scaleSamplesLumi();
//   analysis->plotvar_="ditaumass";
//   analysis->nbins_=50;
//   analysis->xmin_=0;
//   analysis->xmax_=500;
//   analysis->Chcat_=1;
//   analysis->Isocat_=1;

//   analysis->MTcat_=0;
//   analysis->extrasel_="1";
//   TH1F*hHiggsVBF=analysis->getSample("HiggsVBF125");
//   TH1F*hHiggsGG=analysis->getSample("HiggsGG125");
//   cout<<"Higgs VBF 125 : "<<hHiggsVBF->Integral()<<endl;
//   cout<<"Higgs GG  125 : "<<hHiggsGG->Integral()<<endl;

//   analysis->MTcat_=1;
//   analysis->extrasel_="1";
//   TH1F*hHiggsVBF=analysis->getSample("HiggsVBF125");
//   TH1F*hHiggsGG=analysis->getSample("HiggsGG125");
//   cout<<"Higgs VBF 125 : "<<hHiggsVBF->Integral()<<endl;
//   cout<<"Higgs GG  125 : "<<hHiggsGG->Integral()<<endl;

//   analysis->MTcat_=1;
//   analysis->extrasel_=analysis->getSMcut(4);
//   TH1F*hHiggsVBF=analysis->getSample("HiggsVBF125");
//   analysis->extrasel_=analysis->getSMcut(3);
//   TH1F*hHiggsGG=analysis->getSample("HiggsGG125");
//   cout<<"Higgs VBF 125 : "<<hHiggsVBF->Integral()<<endl;
//   cout<<"Higgs GG  125 : "<<hHiggsGG->Integral()<<endl;
  

//   analysis->plotInc("vbfvars20122",20,0,10,1,1,1,"(njet>=2)","",2,2,"#Delta#eta(#mu#tau-j)","",c,0,"visjeteta_mt_2012");
//   analysis->plotInc("vbfvars20123",20,0,200,1,1,1,"(njet>=2)","",2,2,"#mu#tau p_{T}","",c,0,"ptvis_mt_2012");
//   analysis->plotInc("vbfmva2012",40,-1,1.1001,1,1,1,"(njet>=2&&njetingap==0)","(vbfmva2012<0.9)",2,2,"VBF MVA output","",c2,5,"vbfmva_2Jet0JetInGap_mt_2012");
//   analysis->plotInc("vbfmva2012",50,0.,1.001,1,1,1,"(njet>=2&&njetingap==0)","(vbfmva2012<0.9)",2,2,"VBF MVA output","",c2,5,"vbfmva01_2Jet0JetInGap_mt_2012");
//   analysis->plotInc("vbfmva2012",40,0.8,1.001,1,1,1,"(njet>=2&&njetingap==0)","(vbfmva2012<0.95)",2,2,"VBF MVA output","",c2,5,"vbfmva081_2Jet0JetInGap_mt_2012");

 //////////////////////////////Study the VBF MVA///////////////////////////////////
  //Cut Based VBF Mjj > 500 GeV, delta_eta > 3.5
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_njet2CentralJetVeto_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&diJetMass>500&&abs(diJetDeltaEta)>3.5)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbfCutBased_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&diJetMass>400.0&&abs(diJetDeltaEta)>4.0)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbfCutBased2011_mt_2012");

//   //scan VBF MVA
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.900)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA900_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.905)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA905_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.910)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA910_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.915)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA915_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.920)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA920_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.925)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA925_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.930)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA930_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.935)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA935_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.940)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA940_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.945)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA945_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.950)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA950_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.955)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA955_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.960)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA960_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.965)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA965_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.970)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA970_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.975)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA975_mt_2012");
//   analysis->plotInc("svfitmass",0,0,350,1,1,1,"(njet>=2&&njetingap==0&&vbfmva2012>0.980)","(svfitmass<100||160<svfitmass)",5,2,"m(#tau#tau)  (GeV)","dN/dm(#tau#tau)",c,5,"m_sv_vbf_VBFMVA980_mt_2012");


