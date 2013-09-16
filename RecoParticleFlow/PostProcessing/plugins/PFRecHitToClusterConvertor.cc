
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"

class PFRecHitToClusterConvertor : public edm::EDProducer 
{
  public:
    explicit PFRecHitToClusterConvertor(const edm::ParameterSet & iConfig);
    ~PFRecHitToClusterConvertor();

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  protected:
    const edm::InputTag   source;

    void store(const reco::PFRecHitCollection& recHits, std::vector<reco::PFCluster>& storage);
};



PFRecHitToClusterConvertor::PFRecHitToClusterConvertor(const edm::ParameterSet & iConfig) 
  : source(iConfig.getParameter<edm::InputTag>("source"))
{
  produces<std::vector<reco::PFCluster> >();
}


PFRecHitToClusterConvertor::~PFRecHitToClusterConvertor() {
}


void PFRecHitToClusterConvertor::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) 
{
  // read in the objects
  edm::Handle<reco::PFRecHitCollection>           recHits;
  iEvent.getByLabel(source, recHits);

  // storage
  std::auto_ptr<std::vector<reco::PFCluster> >    clusters(new std::vector<reco::PFCluster>);

  // fill items
  store(*recHits, *clusters);

  // put in event
  iEvent.put(clusters);
}


void PFRecHitToClusterConvertor::store(const reco::PFRecHitCollection& recHits, std::vector<reco::PFCluster>& storage)
{
  const unsigned int      numRecHits  = recHits.size();
  for (unsigned int iHit = 0; iHit < numRecHits; ++iHit) 
  {
    const reco::PFRecHit& recHit      = recHits[iHit];
    storage.push_back(reco::PFCluster(recHit.layer(), recHit.energy(), recHit.position().x(), recHit.position().y(), recHit.position().z()));
  } // end loop over recHits
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFRecHitToClusterConvertor);
