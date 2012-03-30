// system include files
#include <memory>
#include <cmath>
#include <algorithm>

#include "CMG/MetAnalysis/interface/MetUtilities.h"

MetUtilities::MetUtilities() {
}

MetUtilities::~MetUtilities() {}

bool MetUtilities::filter(const PFJet *iJet,Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2) { 

  double pDEta1 = iJet->eta() - iEta1;
  double pDPhi1 = fabs(iJet->phi() - iPhi1); if(pDPhi1 > 2.*TMath::Pi()-pDPhi1) pDPhi1 = 2.*TMath::Pi()-pDPhi1;
  double pDR1   = sqrt(pDEta1*pDEta1 + pDPhi1*pDPhi1);
  if(pDR1 < 0.5) return false;
  double pDEta2 = iJet->eta() - iEta2;
  double pDPhi2 = fabs(iJet->phi() - iPhi2); if(pDPhi2 > 2.*TMath::Pi()-pDPhi2) pDPhi2 = 2.*TMath::Pi()-pDPhi2;
  double pDR2   = sqrt(pDEta2*pDEta2 + pDPhi2*pDPhi2);
  if(pDR2 < 0.5) return false;
  return true;
}

bool MetUtilities::passPFLooseId(const PFJet *iJet) { 
  if(iJet->energy()== 0)       return false;
  if(iJet->neutralHadronEnergy()/iJet->energy() > 0.99)   return false;
  if(iJet->neutralEmEnergy()/iJet->energy()     > 0.99)   return false;
  if(iJet->nConstituents() <  2)                          return false;
  if(iJet->chargedHadronEnergy()/iJet->energy() <= 0 && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedEmEnergy()/iJet->energy() >  0.99  && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedMultiplicity()            < 1      && fabs(iJet->eta()) < 2.4 ) return false;
  return true;
}


/*
void MetUtilities::addNeut(const PFJet *iJet, TLorentzVector &iVec, Double_t &iSumEt, 
                           FactorizedJetCorrector *iJetCorrector,const PileupEnergyDensityCol *iPUEnergyDensity,
                           int iSign) { 
  TLorentzVector lVec(0,0,0,0);
  // double lPt = fJetIDMVA->correctedPt(iJet,iJetCorrector,iPUEnergyDensity);                       // chiara
  // lPt *= (iJet->neutralEmEnergy()/iJet->energy() + iJet->neutralHadronEnergy()/iJet->energy());   // chiara
  lVec.SetPtEtaPhiE(lPt,iJet->eta(),iJet->etaPhi(),iJet->mass());
  if(iSign > 0) iVec -= lVec;
  if(iSign < 0) iVec += lVec;
  iSumEt += lPt;
}
*/
