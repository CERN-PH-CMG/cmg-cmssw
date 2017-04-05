// -*- C++ -*-
//
// Package:    CommonTools/RecoAlgos
// Class:      PFClusterMatchedToPhotonsSelector
// 
/**\class PFClusterMatchedToPhotonsSelector PFClusterMatchedToPhotonsSelector.cc CommonTools/RecoAlgos/plugins/PFClusterMatchedToPhotonsSelector.cc

 Description: Matches ECAL PF clusters to photons that do not convert

 Implementation:

*/
//
// Original Author:  RCLSA
//         Created:  Wed, 22 Mar 2017 18:01:40 GMT
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"

typedef reco::PFCluster::EEtoPSAssociation::value_type EEPSPair;
bool sortByKey(const EEPSPair& a, const EEPSPair& b) {
  return a.first < b.first;
}

class PFClusterMatchedToPhotonsSelector : public edm::stream::EDProducer<> {
public:
  PFClusterMatchedToPhotonsSelector(const edm::ParameterSet&);
  static void fillDescriptions(edm::ConfigurationDescriptions&);
  
private:
  virtual void produce(edm::Event&, const edm::EventSetup&);

  edm::EDGetTokenT<reco::GenParticleCollection> genParticleToken_; // genParticles
  edm::EDGetTokenT<reco::PFClusterCollection> particleFlowClusterECALToken_;
  edm::EDGetTokenT<reco::PFCluster::EEtoPSAssociation> associationToken_;
  edm::EDGetTokenT<TrackingParticleCollection> trackingParticleToken_;
  double matchMaxDR_;
  double matchMaxDEDR_;

  double volumeZ_EB_;
  double volumeRadius_EB_;
  double volumeZ_EE_;
};

PFClusterMatchedToPhotonsSelector::PFClusterMatchedToPhotonsSelector(const edm::ParameterSet& iConfig) {
  //now do what ever initialization is needed
  particleFlowClusterECALToken_ = consumes<reco::PFClusterCollection>(iConfig.getParameter<edm::InputTag>("pfClustersTag"));
  associationToken_ = consumes<reco::PFCluster::EEtoPSAssociation>(iConfig.getParameter<edm::InputTag>("pfClustersTag"));
  trackingParticleToken_ = consumes<TrackingParticleCollection>(iConfig.getParameter<edm::InputTag>("trackingParticleTag"));
  genParticleToken_ = consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticleTag"));

  matchMaxDR_      = iConfig.getParameter<double>("maxDR");
  volumeZ_EB_      = iConfig.getParameter<double>("volumeZ_EB");
  volumeRadius_EB_ = iConfig.getParameter<double>("volumeRadius_EB");
  volumeZ_EE_      = iConfig.getParameter<double>("volumeZ_EE");

  produces<reco::PFClusterCollection>();
  produces<reco::PFCluster::EEtoPSAssociation>();
  produces<edm::ValueMap<reco::GenParticleRef> >();
}


void PFClusterMatchedToPhotonsSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
}

void PFClusterMatchedToPhotonsSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<reco::PFClusterCollection> particleFlowClusterECALHandle_;
  edm::Handle<reco::PFCluster::EEtoPSAssociation> associationHandle_;
  edm::Handle<TrackingParticleCollection> trackingParticleHandle_;
  edm::Handle<reco::GenParticleCollection> genParticleHandle_;
  iEvent.getByToken(particleFlowClusterECALToken_, particleFlowClusterECALHandle_);
  iEvent.getByToken(trackingParticleToken_, trackingParticleHandle_);  
  iEvent.getByToken(genParticleToken_, genParticleHandle_);  
  iEvent.getByToken(associationToken_, associationHandle_);  

  std::unique_ptr<reco::PFClusterCollection>                      out   = std::make_unique<reco::PFClusterCollection>();
  std::unique_ptr<reco::PFCluster::EEtoPSAssociation> association_out   = std::make_unique<reco::PFCluster::EEtoPSAssociation>();
  std::unique_ptr<edm::ValueMap<reco::GenParticleRef> > genmatching_out = std::make_unique<edm::ValueMap<reco::GenParticleRef> >();

  std::vector<reco::GenParticleRef> genmatching;

  size_t iP(0);
  size_t iN(0);  
  for (auto&& pfCluster : *particleFlowClusterECALHandle_) {

    bool isMatched = false;
    reco::GenParticleRef::key_type matchedKey;

    for (auto&& trackingParticle : *trackingParticleHandle_) {
      if (trackingParticle.pdgId() != 22) continue;
      if (trackingParticle.status() != 1) continue;
      matchedKey = trackingParticle.genParticles().at(0).key();
      if (reco::deltaR(trackingParticle, pfCluster.position()) > matchMaxDR_) continue; 

      bool isConversion = false;
      for (auto&& vertRef : trackingParticle.decayVertices()) {
	if (vertRef->position().rho() > volumeRadius_EB_ && std::abs(vertRef->position().z()) < volumeZ_EB_) continue;
	if (std::abs(vertRef->position().z()) > volumeZ_EE_) continue;
	
	for(auto&& tpRef: vertRef->daughterTracks()) {
	  if(std::abs(tpRef->pdgId()) == 11) isConversion = true;
	  break;
	}
	if (isConversion) break;
      }      
      if (isConversion) continue;

      isMatched = true;           
      break;
    }

    if (isMatched) {
      out->push_back(pfCluster);
      for (size_t i=0; i<associationHandle_.product()->size(); i++) {
	if (associationHandle_.product()->at(i).first == iP) {
	  association_out->push_back(std::make_pair(iN, associationHandle_.product()->at(i).second));
	}
      }
      genmatching.push_back(edm::Ref<reco::GenParticleCollection>(genParticleHandle_,matchedKey));
    }
    iP++;
  }

  std::sort(association_out->begin(),association_out->end(),sortByKey);  
  edm::OrphanHandle<reco::PFClusterCollection> pfClusterHandle= iEvent.put(std::move(out));
  iEvent.put(std::move(association_out));

  edm::ValueMap<reco::GenParticleRef>::Filler mapFiller(*genmatching_out);
  mapFiller.insert(pfClusterHandle, genmatching.begin(), genmatching.end());
  mapFiller.fill();
  iEvent.put(std::move(genmatching_out));
}
 
//define this as a plug-in
DEFINE_FWK_MODULE(PFClusterMatchedToPhotonsSelector);
