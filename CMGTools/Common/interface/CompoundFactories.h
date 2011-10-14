#ifndef COMPOUNDFACTORIES_H_
#define COMPOUNDFACTORIES_H_

#include "CMGTools/Common/interface/DiObjectFactory.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

namespace cmg{

  
  typedef DiObjectFactory< cmg::DiPFCandidate::type1, cmg::DiPFCandidate::type2 > DiPFCandidateFactory;
  typedef DiObjectFactory< cmg::DiPhoton::type1, cmg::DiPhoton::type2 > DiPhotonFactory;
  typedef DiObjectFactory< cmg::DiHemisphere::type1, cmg::DiHemisphere::type2 > DiHemisphereFactory;
  typedef DiObjectFactory< cmg::DiFatJet::type1, cmg::DiFatJet::type2 > DiFatJetFactory;
  typedef DiObjectFactory< cmg::DiElectron::type1, cmg::DiElectron::type2 > DiElectronFactory;
  typedef DiObjectFactory< cmg::DiJet::type1, cmg::DiJet::type2 > DiJetFactory;
  typedef DiObjectFactory< cmg::DiMuon::type1, cmg::DiMuon::type2 > DiMuonFactory;
  typedef DiObjectFactory< cmg::DiPFJet::type1, cmg::DiPFJet::type2 > DiPFJetFactory;
  typedef DiObjectFactory< cmg::DiTau::type1, cmg::DiTau::type2 > DiTauFactory;

  typedef DiObjectFactory< cmg::TauE::type1, cmg::TauE::type2 > TauEFactory;
  typedef DiObjectFactory< cmg::TauMu::type1, cmg::TauMu::type2 > TauMuFactory;
  typedef DiObjectFactory< cmg::MuE::type1, cmg::MuE::type2 > MuEFactory;

  typedef DiObjectFactory< cmg::WENu::type1, cmg::WENu::type2 > WENuFactory;
  typedef DiObjectFactory< cmg::WMuNu::type1, cmg::WMuNu::type2 > WMuNuFactory;
  typedef DiObjectFactory< cmg::WTauNu::type1, cmg::WTauNu::type2 > WTauNuFactory;
    
  typedef DiObjectFactory< cmg::QuadMuon::type1, cmg::QuadMuon::type2 > QuadMuonFactory;
    
}

#endif /*COMPOUNDFACTORIES_H_*/
