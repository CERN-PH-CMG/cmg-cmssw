// system include files
#include <memory>
#include <cmath>
#include "TMath.h"
#include <algorithm>

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include <DataFormats/MuonReco/interface/Muon.h>

#include "CMG/MetAnalysis/interface/MetUtilities.h"

using namespace std;

using namespace reco;

MetUtilities::MetUtilities(const edm::ParameterSet &iConfig) { //, bool isData) {

  // jet id
  fPUJetIdAlgo = new PileupJetIdNtupleAlgo(iConfig);
  edm::ParameterSet jetConfig = iConfig.getParameter<edm::ParameterSet>("JetIdParams");
  for(int i0 = 0; i0 < 3; i0++) { 
    std::string lCutType                            = "Tight";
    if(i0 == PileupJetIdentifier::kMedium) lCutType = "Medium";
    if(i0 == PileupJetIdentifier::kLoose)  lCutType = "Loose";
    std::vector<double> pt010  = jetConfig.getParameter<std::vector<double> >(("Pt010_" +lCutType).c_str());
    std::vector<double> pt1020 = jetConfig.getParameter<std::vector<double> >(("Pt1020_"+lCutType).c_str());
    std::vector<double> pt2030 = jetConfig.getParameter<std::vector<double> >(("Pt2030_"+lCutType).c_str());
    std::vector<double> pt3050 = jetConfig.getParameter<std::vector<double> >(("Pt3050_"+lCutType).c_str());
	  for(int i1 = 0; i1 < 4; i1++) mvacut_[i0][0][i1] = pt010 [i1];
	  for(int i1 = 0; i1 < 4; i1++) mvacut_[i0][1][i1] = pt1020[i1];
	  for(int i1 = 0; i1 < 4; i1++) mvacut_[i0][2][i1] = pt2030[i1];
	  for(int i1 = 0; i1 < 4; i1++) mvacut_[i0][3][i1] = pt3050[i1];
  }

  // data or MC - for jet collector 
  //fData = isData;
  
  /* 
  // jet corrector - still to be fixed
  TString path(getenv("CMSSW_BASE"));
  path += "/src/CMG/MetAnalysis/data/";  
  std::vector<JetCorrectorParameters> correctionParameters;
  correctionParameters.push_back(JetCorrectorParameters((path+"GR_R_42_V23_AK5PF_L1FastJet.txt").Data()));
  correctionParameters.push_back(JetCorrectorParameters((path+"GR_R_42_V23_AK5PF_L2Relative.txt").Data()));
  correctionParameters.push_back(JetCorrectorParameters((path+"GR_R_42_V23_AK5PF_L3Absolute.txt").Data()));
  if(fData) correctionParameters.push_back(JetCorrectorParameters((path+"GR_R_42_V23_AK5PF_L2L3Residual.txt").Data()));
  fJetCorrector = new FactorizedJetCorrector(correctionParameters);
  */
}

MetUtilities::~MetUtilities() {
  
  delete fPUJetIdAlgo;

  // delete fJetCorrector;
}

bool MetUtilities::filter(const PFJet *iJet,Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2) { 
  double pDEta1 = iJet->eta() - iEta1;
  double pDPhi1 = fabs(iJet->phi() - iPhi1); 
  if(pDPhi1 > 2.*TMath::Pi()-pDPhi1) pDPhi1 = 2.*TMath::Pi()-pDPhi1;
  double pDR1   = sqrt(pDEta1*pDEta1 + pDPhi1*pDPhi1);
  if(pDR1 < 0.5) return false;
  double pDEta2 = iJet->eta() - iEta2;
  double pDPhi2 = fabs(iJet->phi() - iPhi2); if(pDPhi2 > 2.*TMath::Pi()-pDPhi2) pDPhi2 = 2.*TMath::Pi()-pDPhi2;
  double pDR2   = sqrt(pDEta2*pDEta2 + pDPhi2*pDPhi2);
  if(pDR2 < 0.5) return false;
  return true;
}

/*
float MetUtilities::correctedJetPt(const PFJet *iJet,double iRho) { 

  fJetCorrector->setJetEta(iJet->eta());
  fJetCorrector->setJetPt (iJet->pt());
  fJetCorrector->setJetPhi(iJet->phi());
  fJetCorrector->setJetE  (iJet->energy());   // fixme: was rawMom.E()) 
  fJetCorrector->setRho   (iRho);
  fJetCorrector->setJetA  (iJet->jetArea());
  fJetCorrector->setJetEMF(-99.0);     
  Double_t correction = fJetCorrector->getCorrection();
  return iJet->pt()*correction;
}
*/

bool MetUtilities::passPFLooseId(const PFJet *iJet) { 
  if(iJet->energy()== 0)                                  return false;
  if(iJet->neutralHadronEnergy()/iJet->energy() > 0.99)   return false;
  if(iJet->neutralEmEnergy()/iJet->energy()     > 0.99)   return false;
  if(iJet->nConstituents() <  2)                          return false;
  if(iJet->chargedHadronEnergy()/iJet->energy() <= 0 && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedEmEnergy()/iJet->energy() >  0.99  && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedMultiplicity()            < 1      && fabs(iJet->eta()) < 2.4 ) return false;
  return true;
}

void MetUtilities::addNeut(const PFJet *iuncorrJet, const PFJet *icorrJet, Candidate::LorentzVector *iVec, double *iSumEt,int iSign) { 
  double lPt = icorrJet->pt();
  lPt *= (iuncorrJet->neutralEmEnergy()/iuncorrJet->energy() + iuncorrJet->neutralHadronEnergy()/iuncorrJet->energy());   

  Candidate::LorentzVector lVec(lPt,iuncorrJet->eta(),iuncorrJet->phi(),iuncorrJet->mass());  
  if(iSign > 0) *iVec -= lVec;
  if(iSign < 0) *iVec += lVec;  
  *iSumEt += lPt;               
}
void MetUtilities::addNeut(const PFJet *icorrJet, Candidate::LorentzVector *iVec, double *iSumEt, int iSign) { 
  double lPt = icorrJet->pt();
  lPt *= (icorrJet->neutralEmEnergy()/icorrJet->energy() + icorrJet->neutralHadronEnergy()/icorrJet->energy());   

  Candidate::LorentzVector lVec(lPt,icorrJet->eta(),icorrJet->phi(),icorrJet->mass());  
  if(iSign > 0) *iVec -= lVec;
  if(iSign < 0) *iVec += lVec;  
  *iSumEt += lPt;               
}

float MetUtilities::pfCandDz(const PFCandidate* iPFCand, const Vertex *iPV) { 
  float lDz = -999;
  if(iPFCand->trackRef().isNonnull()) 
    lDz = fabs(iPFCand->trackRef()->dz(iPV->position()));
  else if(iPFCand->gsfTrackRef().isNonnull())
    lDz = fabs(iPFCand->gsfTrackRef()->dz(iPV->position()));
  else if(iPFCand->muonRef().isNonnull() && iPFCand->muonRef()->innerTrack().isNonnull())
    lDz = fabs(iPFCand->muonRef()->innerTrack()->dz(iPV->position()));
  return lDz;
}

bool MetUtilities::passJetId(const PFJet *iuncorrJet, const PFJet *icorrJet, const Vertex iPV, const reco::VertexCollection &iAllvtx) { 
  double lJec = icorrJet->pt()/iuncorrJet->pt();
  PileupJetIdentifier lPUJetId =  fPUJetIdAlgo->computeIdVariables(iuncorrJet,lJec,&iPV,iAllvtx,true);
  if(!passPFLooseId(iuncorrJet))                              return false;
  if(lPUJetId.idFlag() > 0)                                   return true;
  return false;
}
double MetUtilities::jetMVA (const PFJet *iuncorrJet,double iJec, const Vertex iPV, const reco::VertexCollection &iAllvtx) { 
  PileupJetIdentifier lPUJetId =  fPUJetIdAlgo->computeIdVariables(iuncorrJet,iJec,&iPV,iAllvtx,true);
  return lPUJetId.mva();
}
const PFMET MetUtilities::clean(const PFMET *iMet,double iEta1,double iPhi1,double iEta2,double iPhi2,const PFJetCollection *iJets) { 
  Candidate::LorentzVector lVec   = iMet->p4();
  double                   lSumEt = iMet->sumEt();
  for(unsigned int i0 = 0; i0 < iJets->size(); i0++) {
    const PFJet     *pJet   = &(iJets->at(i0));
    if(!filter(pJet,iPhi1, iEta1,iPhi2,iEta2)) {addNeut(pJet,&lVec,&lSumEt,-1); continue; }
  }
  return PFMET(iMet->getSpecific(),lSumEt,lVec,iMet->vertex());
}

const PFMET MetUtilities::recoil(const PFMET *iMet,double iPt1,double iEta1,double iPhi1,double iPt2,double iEta2,double iPhi2,const PFJetCollection *iJets) { 
  PFMET lMet = *iMet;
  if(iJets != 0) lMet = (clean(iMet,iEta1,iPhi1,iEta2,iPhi2,iJets));
  TLorentzVector lL1(0,0,0,0); lL1.SetPtEtaPhiM(iPt1,iEta1,iPhi1,0);
  TLorentzVector lL2(0,0,0,0); lL2.SetPtEtaPhiM(iPt2,iEta2,iPhi2,0);
  lL1+= lL2;
  Candidate::LorentzVector lVis(lL1.Px(),lL1.Py(),lL1.Pz(),lL1.E()); //poor coding
  Candidate::LorentzVector lVec   = lMet.p4();
  lVec += lVis;
  //lOutput.met   = lVec.Pt()+lL1.Pt();
  double lSumEt = iMet->sumEt()-iPt1-iPt2;
  return PFMET(iMet->getSpecific(),lSumEt,lVec,iMet->vertex());
}
const PFMET MetUtilities::recoil(const PFMET *iMet,double iPtVis,double iPhiVis,double iSumEtVis) { 
  const PFMET *lMet = iMet;
  TLorentzVector lL (0,0,0,0); lL.SetPtEtaPhiM(iPtVis,0,iPhiVis,0);
  Candidate::LorentzVector lVis(lL.Px(),lL.Py(),lL.Pz(),lL.E());     //poor coding
  Candidate::LorentzVector lVec   = lMet->p4();
  lVec+=lVis;
  double lSumEt = iSumEtVis;
  return PFMET(iMet->getSpecific(),lSumEt,lVec,iMet->vertex());
}
bool MetUtilities::passMVA(std::pair<LorentzVector,double> jet) { 
  int ptId = 0; 
  if(jet.first.pt() > 10 && jet.first.pt() < 20) ptId = 1;
  if(jet.first.pt() > 20 && jet.first.pt() < 30) ptId = 2;
  if(jet.first.pt() > 30                       ) ptId = 3;
  
  int etaId = 0;
  if(fabs(jet.first.eta()) > 2.5  && fabs(jet.first.eta()) < 2.75) etaId = 1; 
  if(fabs(jet.first.eta()) > 2.75 && fabs(jet.first.eta()) < 3.0 ) etaId = 2; 
  if(fabs(jet.first.eta()) > 3.0  && fabs(jet.first.eta()) < 5.0 ) etaId = 3; 
  
  return (jet.second  > mvacut_[2][ptId][etaId]);
}
MetUtilities::LorentzVector *MetUtilities::leadPt(std::vector<std::pair<LorentzVector,double> > &iJets,bool iFirst) {
  double lPt0 = 0; double lPt1 = 0;
  LorentzVector *lLead=0; 
  LorentzVector *l2nd =0;
  for(int i0 = 0; i0 < int(iJets.size()); i0++) {
    if(iJets[i0].first.pt() > lPt0) {lPt0 = iJets[i0].first.pt(); lLead = &(iJets[i0].first); continue;}
    if(iJets[i0].first.pt() > lPt1) {lPt1 = iJets[i0].first.pt(); l2nd  = &(iJets[i0].first); continue;}
  }
  if(iFirst) return lLead;
  return l2nd;
}
int MetUtilities::NJets(std::vector<std::pair<LorentzVector,double> > &iJets,double iPt) {
  int lNJet = 0; for(int i0 = 0; i0 < int(iJets.size()); i0++) if(iJets[i0].first.pt() > iPt) lNJet++;
  return lNJet;
}
double MetUtilities::deltaR(LorentzVector &iVec1,LorentzVector &iVec2) { 
  double lDPhi = fabs(iVec1.phi()-iVec2.phi());
  if(lDPhi > 2.*TMath::Pi()-lDPhi) lDPhi =  2.*TMath::Pi()-lDPhi;
  double lDEta = iVec1.eta()-iVec2.eta();
  return sqrt(lDPhi*lDPhi+lDEta*lDEta);
}
void MetUtilities::cleanJets(std::vector<LorentzVector> &iVis,std::vector<std::pair<LorentzVector,double> > &iJets) { 
  for(int i0   = 0; i0 < int(iJets.size()); i0++) { 
    for(int i1 = 0; i1 < int(iVis.size());  i1++) { 
      if(deltaR(iVis[i1],iJets[i0].first) < 0.5) {
	iJets.erase (iJets.begin()+i0); i0--;
      }
    }
  }
}
std::pair<MetUtilities::LorentzVector,double> MetUtilities::TKMet(std::vector<std::pair<LorentzVector,double> > &iCands,int iDZ,bool iLowDz) { 
  double            lSumEt = 0;
  LorentzVector     lVec;
  for(int i0 = 0; i0 < int(iCands.size()); i0++) { 
    if(iCands[i0].second < 0   &&  iLowDz != 2) continue;
    if(iCands[i0].second > iDZ &&  iLowDz == 0) continue;
    if(iCands[i0].second < iDZ &&  iLowDz == 1) continue;
    lVec    += iCands[i0].first;
    lSumEt  += iCands[i0].first.pt();
  }
  std::pair<LorentzVector,double> lTKMet(lVec,lSumEt);
  return lTKMet;
}
std::pair<MetUtilities::LorentzVector,double> MetUtilities::JetMet(std::vector<std::pair<LorentzVector,double> > &iJets,bool iPassMVA) { 
  double            lSumEt = 0;
  LorentzVector           lVec;
  for(int i0 = 0; i0 < int(iJets.size()); i0++) { 
    if( passMVA(iJets[i0])  &&  iPassMVA) continue;
    if(!passMVA(iJets[i0])  && !iPassMVA) continue;
    lVec    += iJets[i0].first;
    lSumEt  += iJets[i0].first.pt();
  }
  std::pair<LorentzVector,double> lJetMet(lVec,lSumEt);
  return lJetMet;
}
std::pair<MetUtilities::LorentzVector,double> MetUtilities::NoPUMet(std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,double iDZ) { 
  double            lSumEt = 0;
  LorentzVector     lVec;
  std::pair<LorentzVector,double> lTKMet  = TKMet (iCands,iDZ,0);
  std::pair<LorentzVector,double> lJetMet = JetMet(iJets     ,true);
  lVec += lTKMet.first;     lSumEt += lTKMet .second;
  lVec += lJetMet.first;    lSumEt += lJetMet.second; 
  std::pair<LorentzVector,double> lNoPUMet(lVec,lSumEt);
  return lNoPUMet;
}
std::pair<MetUtilities::LorentzVector,double> MetUtilities::PUMet  (std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,double iDZ) { 
  double            lSumEt = 0;
  LorentzVector     lVec;
  std::pair<LorentzVector,double> lTKMet  = TKMet (iCands,iDZ,   1);
  std::pair<LorentzVector,double> lJetMet = JetMet(iJets     ,false);
  lVec += lTKMet .first;    lSumEt += lTKMet .second;
  lVec += lJetMet.first;    lSumEt += lJetMet.second; 
  std::pair<LorentzVector,double> lPUMet(lVec,lSumEt);
  return lPUMet;
}
std::pair<MetUtilities::LorentzVector,double>  MetUtilities::PUCMet (std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,double iDZ) { 
  double            lSumEt = 0;
  LorentzVector     lVec;
  std::pair<LorentzVector,double> lPFMet  = TKMet (iCands,iDZ,   2);
  std::pair<LorentzVector,double> lTKMet  = TKMet (iCands,iDZ,   1);
  std::pair<LorentzVector,double> lJetMet = JetMet(iJets     ,false);
  lVec += lPFMet .first;     lSumEt += lPFMet .second;
  lVec -= lTKMet .first;     lSumEt -= lTKMet .second; 
  lVec -= lJetMet.first;     lSumEt -= lJetMet.second; 
  std::pair<LorentzVector,double> lPUCMet(lVec,lSumEt);
  return lPUCMet;
}
std::pair<MetUtilities::LorentzVector,double> MetUtilities::PFRecoil(double iSumEt,LorentzVector iVis,std::vector<std::pair<LorentzVector,double> > &iCands,double iDZ) { 
  std::pair<LorentzVector,double> lPFMet = TKMet(iCands,iDZ,2);
  LorentzVector lVec; lVec += lPFMet.first; lVec += iVis;
  double        lSumEt      = lPFMet.second-iSumEt; 
  std::pair<LorentzVector,double> lPFRecoil(lVec,lSumEt);
  return lPFRecoil;
}
std::pair<MetUtilities::LorentzVector,double> MetUtilities::TKRecoil(double iSumEt,LorentzVector iVis,std::vector<std::pair<LorentzVector,double> > &iCands,double iDZ) { 
  std::pair<LorentzVector,double> lTKMet = TKMet(iCands,iDZ,0);
  LorentzVector lVec; lVec += lTKMet.first; lVec += iVis;
  double lSumEt      = lTKMet.second-iSumEt; 
  std::pair<LorentzVector,double> lTKRecoil(lVec,lSumEt);
  return lTKRecoil;
}
std::pair<MetUtilities::LorentzVector,double> MetUtilities::NoPURecoil(double iSumEt,LorentzVector iVis,
								       std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,
								       double iDZ) { 
  std::pair<MetUtilities::LorentzVector,double> lNPMet = NoPUMet(iCands,iJets,iDZ);
  LorentzVector lVec; lVec += lNPMet.first; lVec += iVis;
  double lSumEt      = lNPMet.second-iSumEt; 
  std::pair<LorentzVector,double> lNPRecoil(lVec,lSumEt);
  return lNPRecoil;
}
std::pair<MetUtilities::LorentzVector,double> MetUtilities::PUCRecoil(double iSumEt,LorentzVector iVis,
								      std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<std::pair<LorentzVector,double> > &iJets,
								      double iDZ) { 
  std::pair<LorentzVector,double> lPUCMet = PUCMet(iCands,iJets,iDZ);
  LorentzVector lVec; lVec += lPUCMet.first; lVec += iVis;
  double lSumEt      = lPUCMet.second-iSumEt; 
  std::pair<LorentzVector,double> lPUCRecoil(lVec,lSumEt);
  return lPUCRecoil;
}
