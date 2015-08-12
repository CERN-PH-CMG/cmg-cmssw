// adapted from FixedGridRhoProducerFastjet in the RecoJets/JetProducers CMSSW package

#ifndef CMGTools_RootTools_plugins_FixedGridSigmaProducerFastjet_h
#define CMGTools_RootTools_plugins_FixedGridSigmaProducerFastjet_h

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"


class FixedGridSigmaProducerFastjet : public edm::stream::EDProducer<> {

 public:
  explicit FixedGridSigmaProducerFastjet(const edm::ParameterSet& iConfig);
  virtual ~FixedGridSigmaProducerFastjet();

 private:
  virtual void produce(edm::Event&, const edm::EventSetup&);

  edm::InputTag pfCandidatesTag_;
  fastjet::GridMedianBackgroundEstimator bge_;

  edm::EDGetTokenT<edm::View<reco::Candidate> > input_pfcoll_token_;

};


#endif
