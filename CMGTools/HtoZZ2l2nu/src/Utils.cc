
/*
 *  See header file for a description of this class.
 *
 *  $Date: $
 *  $Revision: $
 *  \author G. Cerminara - CERN
 */

#include "CMGTools/HtoZZ2l2nu/interface/Utils.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

Utils::Utils(){}

Utils::~Utils(){}


double Utils::computeRelIsolation(const pat::Muon *muon) {
  //isolation
  double norm = std::max((double)20.0,(double)muon->pt());
  double ecalIso = muon->ecalIso();
  double hcalIso = muon->hcalIso();
  double trkIso = muon->trackIso();
  double relIso = (ecalIso+hcalIso+trkIso)/norm;
      
  return relIso;
}

int  Utils::muonType(const pat::Muon *muon) {
  int type = -1;
  if(muon->isTrackerMuon() && !muon->isGlobalMuon()) {
    type = 0;
    //muonTrack = muon->globalTrack();
  } else if(!muon->isTrackerMuon() && muon->isGlobalMuon()) {
    type = 1;
  } else if(muon->isTrackerMuon() && muon->isGlobalMuon()) {
    type = 2;
  }
  return type;
}


// TLorentzVector Utils::convert(const LorenzVector& original) {
//   return TLorentzVector(original.px(), original.py(), original.pz(), original.e());
// }
