#include "CMGTools/H2TauTau/interface/DiTauWithSVFitProducer.h"

#include "CMGTools/H2TauTau/interface/DiObjectUpdateFactory.h"
#include "CMGTools/H2TauTau/interface/DiTauObjectFactory.h"


namespace cmg {

typedef DiTauObjectFactory< pat::Tau, pat::Tau > DiTauFactory;
typedef DiTauObjectFactory< pat::Tau, pat::Electron > TauEleFactory;
//typedef DiObjectFactory< cmg::TauMu::type1, cmg::TauMu::type2 > TauMuFactory;
typedef DiTauObjectFactory< pat::Tau, pat::Muon > TauMuFactory;
typedef DiTauObjectFactory< pat::Muon, pat::Electron > MuEleFactory;

typedef DiObjectUpdateFactory< pat::Tau, pat::Muon > TauMuUpdateFactory;
typedef DiObjectUpdateFactory< pat::Tau, pat::Electron > TauEleUpdateFactory;
typedef DiObjectUpdateFactory< pat::Muon, pat::Electron  > MuEleUpdateFactory;
typedef DiObjectUpdateFactory< pat::Tau, pat::Tau> DiTauUpdateFactory;

}

typedef DiTauWithSVFitProducer< pat::Tau, pat::Muon > TauMuWithSVFitProducer;
typedef DiTauWithSVFitProducer< pat::Tau, pat::Electron > TauEleWithSVFitProducer;
typedef DiTauWithSVFitProducer< pat::Muon, pat::Electron > MuEleWithSVFitProducer;
typedef DiTauWithSVFitProducer< pat::Tau, pat::Tau > TauTauWithSVFitProducer;

