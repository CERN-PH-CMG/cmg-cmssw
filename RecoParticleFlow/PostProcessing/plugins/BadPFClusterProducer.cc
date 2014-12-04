
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"

class BadPFClusterProducer : public edm::EDProducer {

  public:

    explicit BadPFClusterProducer(const edm::ParameterSet & iConfig);
    ~BadPFClusterProducer();

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  private:

    void store(const reco::PFRecHitCollection& recHits, std::vector<reco::PFCluster>& storage);

};



BadPFClusterProducer::BadPFClusterProducer(const edm::ParameterSet & iConfig) 
{
  produces<std::vector<reco::PFCluster> >("ECAL" );
  produces<std::vector<reco::PFCluster> >("HCAL" );
  produces<std::vector<reco::PFCluster> >("HFEM" );
  produces<std::vector<reco::PFCluster> >("HFHAD");
  produces<std::vector<reco::PFCluster> >("PS"   );
}


BadPFClusterProducer::~BadPFClusterProducer() {
}


void BadPFClusterProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) 
{
  // input tags
  static edm::InputTag    tagECALrh ("particleFlowRecHitECAL"   ,"Cleaned");
  static edm::InputTag    tagHCALrh ("particleFlowRecHitHCAL"   ,"Cleaned");
  static edm::InputTag    tagPSrh   ("particleFlowRecHitPS"     ,"Cleaned");
  static edm::InputTag    tagECALcl ("particleFlowClusterECAL"  ,"Cleaned");
  static edm::InputTag    tagHCALcl ("particleFlowClusterHCAL"  ,"Cleaned");
  static edm::InputTag    tagHFHADcl("particleFlowClusterHFHAD" ,"Cleaned");
  static edm::InputTag    tagHFEMcl ("particleFlowClusterHFEM"  ,"Cleaned");
  static edm::InputTag    tagPScl   ("particleFlowClusterPS"    ,"Cleaned");

  // read in the objects
  edm::Handle<reco::PFRecHitCollection>   ecalrh ;  iEvent.getByLabel(tagECALrh , ecalrh );
  edm::Handle<reco::PFRecHitCollection>   hcalrh ;  iEvent.getByLabel(tagHCALrh , hcalrh );
  edm::Handle<reco::PFRecHitCollection>   psrh   ;  iEvent.getByLabel(tagPSrh   , psrh   );
  edm::Handle<reco::PFRecHitCollection>   ecalcl ;  iEvent.getByLabel(tagECALcl , ecalcl );
  edm::Handle<reco::PFRecHitCollection>   hcalcl ;  iEvent.getByLabel(tagHCALcl , hcalcl );
  edm::Handle<reco::PFRecHitCollection>   hfhadcl;  iEvent.getByLabel(tagHFHADcl, hfhadcl);
  edm::Handle<reco::PFRecHitCollection>   hfemcl ;  iEvent.getByLabel(tagHFEMcl , hfemcl );
  edm::Handle<reco::PFRecHitCollection>   pscl   ;  iEvent.getByLabel(tagPScl   , pscl   );

  // storage
  std::auto_ptr<std::vector<reco::PFCluster> >      outECAL (new std::vector<reco::PFCluster>);
  std::auto_ptr<std::vector<reco::PFCluster> >      outHCAL (new std::vector<reco::PFCluster>);
  std::auto_ptr<std::vector<reco::PFCluster> >      outHFEM (new std::vector<reco::PFCluster>);
  std::auto_ptr<std::vector<reco::PFCluster> >      outHFHAD(new std::vector<reco::PFCluster>);
  std::auto_ptr<std::vector<reco::PFCluster> >      outPS   (new std::vector<reco::PFCluster>);

  // fill items
  store(*ecalrh , *outECAL );
  store(*hcalrh , *outHCAL );
  store(*psrh   , *outPS   );
  store(*ecalcl , *outECAL );
  store(*hcalcl , *outHCAL );
  store(*hfhadcl, *outHFEM );
  store(*hfemcl , *outHFHAD);
  store(*pscl   , *outPS   );

  // put in event
  iEvent.put(outECAL , "ECAL" );
  iEvent.put(outHCAL , "HCAL" );
  iEvent.put(outHFEM , "HFEM" );
  iEvent.put(outHFHAD, "HFHAD");
  iEvent.put(outPS   , "PS"   );
}


void BadPFClusterProducer::store(const reco::PFRecHitCollection& recHits, std::vector<reco::PFCluster>& storage)
{
  const unsigned int      numRecHits  = recHits.size();
  for (unsigned int iHit = 0; iHit < numRecHits; ++iHit) 
  {
    const reco::PFRecHit& recHit      = recHits[iHit];
    storage.push_back(reco::PFCluster(recHit.layer(), recHit.energy(), recHit.position().x(), recHit.position().y(), recHit.position().z()));
  } // end loop over recHits
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(BadPFClusterProducer);
