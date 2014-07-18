#include "CMGTools/H2TauTau/interface/DiTauObjectFactory.h"
#include "CMGTools/H2TauTau/interface/DiTauWithSVFitProducer.h"
#include "CMGTools/H2TauTau/interface/DiObjectUpdateFactory.h"


typedef cmg::DiTauObjectFactory<reco::PFTau, pat::Muon> TauMuPOProducer;
typedef cmg::DiTauObjectFactory<reco::PFTau, pat::Electron> TauElePOProducer;
typedef cmg::DiTauObjectFactory<reco::PFTau, pat::Muon> MuElePOProducer;
typedef cmg::DiTauObjectFactory<reco::PFTau, reco::PFTau> DiTauPOProducer;

typedef DiTauWithSVFitProducer<reco::PFTau, pat::Muon> TauMuSVFitProducer;
typedef DiTauWithSVFitProducer<reco::PFTau, pat::Electron> TauEleSVFitProducer;
typedef DiTauWithSVFitProducer<reco::PFTau, pat::Muon> MuEleSVFitProducer;
typedef DiTauWithSVFitProducer<reco::PFTau, reco::PFTau> DiTauSVFitProducer;

typedef cmg::DiObjectUpdateFactory<reco::PFTau, pat::Muon> TauMuUpdateProducer;
typedef cmg::DiObjectUpdateFactory<reco::PFTau, pat::Electron> TauEleUpdateProducer;
typedef cmg::DiObjectUpdateFactory<reco::PFTau, pat::Muon> MuEleUpdateProducer;
typedef cmg::DiObjectUpdateFactory<reco::PFTau, reco::PFTau> DiTauUpdateProducer;
