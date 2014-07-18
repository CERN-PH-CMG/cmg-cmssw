#include "CMGTools/H2TauTau/interface/DiTauWithSVFitProducer.h"

#include "CMGTools/H2TauTau/interface/DiObjectUpdateFactory.h"
#include "CMGTools/H2TauTau/interface/DiTauObjectFactory.h"


namespace cmg {

typedef DiTauObjectFactory< reco::PFTau, reco::PFTau > DiTauFactory;
typedef DiTauObjectFactory< reco::PFTau, pat::Electron > TauEleFactory;
//typedef DiObjectFactory< cmg::TauMu::type1, cmg::TauMu::type2 > TauMuFactory;
typedef DiTauObjectFactory< reco::PFTau, pat::Muon > TauMuFactory;
typedef DiTauObjectFactory< pat::Muon, pat::Electron > MuEleFactory;

typedef DiObjectUpdateFactory< reco::PFTau, pat::Muon > TauMuUpdateFactory;
typedef DiObjectUpdateFactory< reco::PFTau, pat::Electron > TauEleUpdateFactory;
typedef DiObjectUpdateFactory< pat::Muon, pat::Electron  > MuEleUpdateFactory;
typedef DiObjectUpdateFactory< reco::PFTau, reco::PFTau> DiTauUpdateFactory;

}

typedef DiTauWithSVFitProducer< reco::PFTau, pat::Muon > TauMuWithSVFitProducer;
typedef DiTauWithSVFitProducer< reco::PFTau, pat::Electron > TauEleWithSVFitProducer;
typedef DiTauWithSVFitProducer< pat::Muon, pat::Electron > MuEleWithSVFitProducer;
typedef DiTauWithSVFitProducer< reco::PFTau, reco::PFTau > TauTauWithSVFitProducer;

