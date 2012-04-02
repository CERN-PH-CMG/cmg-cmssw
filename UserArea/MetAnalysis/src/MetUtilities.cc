// system include files
#include <memory>
#include <cmath>
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

MetUtilities::MetUtilities(const edm::ParameterSet &iConfig, bool isData) {

  // jet id
  fPUJetIdAlgo = new PileupJetIdNtupleAlgo(iConfig);

  // data or MC - for jet collector 
  fData = isData;
  
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

void MetUtilities::addNeut(const PFJet *iuncorrJet, const PFJet *icorrJet, Candidate::LorentzVector *iVec, Float_t *iSumEt, double iRho, int iSign) { 

 
  double lPt = icorrJet->pt();
  lPt *= (iuncorrJet->neutralEmEnergy()/iuncorrJet->energy() + iuncorrJet->neutralHadronEnergy()/iuncorrJet->energy());   

  Candidate::LorentzVector lVec(lPt,iuncorrJet->eta(),iuncorrJet->phi(),iuncorrJet->mass());  
  if(iSign > 0) *iVec -= lVec;
  if(iSign < 0) *iVec += lVec;  
  *iSumEt += lPt;               
}

float MetUtilities::pfCandDz(const PFCandidateRef iPFCandRef, const Vertex iPV) { 

  float lDz = 999;
  if(iPFCandRef->trackRef().isNonnull()) 
    lDz = fabs(iPFCandRef->trackRef()->dz(iPV.position()));
  else if(iPFCandRef->gsfTrackRef().isNonnull())
    lDz = fabs(iPFCandRef->gsfTrackRef()->dz(iPV.position()));
  else if(iPFCandRef->muonRef().isNonnull() && iPFCandRef->muonRef()->innerTrack().isNonnull())
    lDz = fabs(iPFCandRef->muonRef()->innerTrack()->dz(iPV.position()));
  return lDz;
}

float MetUtilities::passJetId(const PFJet *iuncorrJet, const PFJet *icorrJet, const Vertex iPV, const reco::VertexCollection &iAllvtx, double iRho) { 

  // Ref Pasquale stuff
  double lJec = icorrJet->pt()/iuncorrJet->pt();

  PileupJetIdentifier lPUJetId =  fPUJetIdAlgo->computeIdVariables(iuncorrJet,lJec,&iPV,iAllvtx,true);
  if(!passPFLooseId(iuncorrJet))                              return false;
  if(fabs(lPUJetId.dZ()) < 0.2)                               return true;   
  if(fabs(iuncorrJet->eta())  < 3.0 && lPUJetId.mva() > -0.8) return true;  
  if(fabs(iuncorrJet->eta())  > 3.0 && lPUJetId.mva() > -0.5) return true;  
  return false;
}
