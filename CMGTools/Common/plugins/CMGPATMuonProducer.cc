
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/PatCandidates/interface/Muon.h"


#include <sstream>

class CMGPATMuonProducer : public edm::EDProducer {

public:
  
  typedef pat::Muon  ObjectType;
  typedef std::vector< pat::Muon > CollectionType;

  explicit CMGPATMuonProducer(const edm::ParameterSet & iConfig);
  virtual ~CMGPATMuonProducer() {}
  
private:
  
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  edm::InputTag srcIT_;
  
  bool verbose_;
};


CMGPATMuonProducer::CMGPATMuonProducer(const edm::ParameterSet & iConfig) : 
  srcIT_( iConfig.getParameter<edm::InputTag>("src") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) )

{
  produces<CollectionType>();
}


void CMGPATMuonProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {


  edm::Handle< CollectionType > objectsH;
  iEvent.getByLabel(srcIT_, objectsH);

  std::auto_ptr<CollectionType> pOut(new CollectionType() ); 
  pOut->reserve( objectsH->size() );

  typedef CollectionType::const_iterator IC;  
  for (IC icand = objectsH->begin(); icand != objectsH->end(); ++icand) {
    const ObjectType& object = *icand;
    pOut->push_back( object );
  }

  iEvent.put( pOut ); 
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CMGPATMuonProducer);
