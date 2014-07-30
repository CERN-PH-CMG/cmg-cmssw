#include "CMGTools/H2TauTau/interface/DiTauObjectFactory.h"
#include "CMGTools/H2TauTau/interface/DiTauWithSVFitProducer.h"
#include "CMGTools/H2TauTau/interface/DiObjectUpdateFactory.h"


typedef cmg::DiTauObjectFactory<pat::Tau, pat::Muon> TauMuPOProducer;
typedef cmg::DiTauObjectFactory<pat::Tau, pat::Electron> TauElePOProducer;
typedef cmg::DiTauObjectFactory<pat::Tau, pat::Muon> MuElePOProducer;
typedef cmg::DiTauObjectFactory<pat::Tau, pat::Tau> DiTauPOProducer;

typedef DiTauWithSVFitProducer<pat::Tau, pat::Muon> TauMuSVFitProducer;
typedef DiTauWithSVFitProducer<pat::Tau, pat::Electron> TauEleSVFitProducer;
typedef DiTauWithSVFitProducer<pat::Tau, pat::Muon> MuEleSVFitProducer;
typedef DiTauWithSVFitProducer<pat::Tau, pat::Tau> DiTauSVFitProducer;

typedef cmg::DiObjectUpdateFactory<pat::Tau, pat::Muon> TauMuUpdateProducer;
typedef cmg::DiObjectUpdateFactory<pat::Tau, pat::Electron> TauEleUpdateProducer;
typedef cmg::DiObjectUpdateFactory<pat::Tau, pat::Muon> MuEleUpdateProducer;
typedef cmg::DiObjectUpdateFactory<pat::Tau, pat::Tau> DiTauUpdateProducer;
