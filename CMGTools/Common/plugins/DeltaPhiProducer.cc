
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include <sstream>

class DeltaPhiProducer : public edm::EDProducer {

public:
  
  explicit DeltaPhiProducer(const edm::ParameterSet & iConfig);
  virtual ~DeltaPhiProducer() {}
  
private:
  
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  edm::InputTag metIT_;
  edm::InputTag objectsIT_;
  
  bool verbose_;
};


DeltaPhiProducer::DeltaPhiProducer(const edm::ParameterSet & iConfig) : 
  metIT_( iConfig.getParameter<edm::InputTag>("met") ), 
  objectsIT_( iConfig.getParameter<edm::InputTag>("objects") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false ) )

{

  produces<float>();
}


void DeltaPhiProducer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<edm::View<reco::LeafCandidate> > metH;
  iEvent.getByLabel(metIT_, metH);

  edm::Handle<edm::View<reco::LeafCandidate> > objectsH;
  iEvent.getByLabel(objectsIT_, objectsH);

  if( metH->size()!=1 ) {
    std::ostringstream err;
    err<<"'met' collection "<<metIT_<<" should contain only one object";
    edm::LogError("DeltaPhiProducer")<<err.str()<<std::endl;
    throw cms::Exception( "InvalidNumberOfObjects", err.str());
  }
  const reco::LeafCandidate& met = (*metH)[0];

  float deltaPhiMin = 999999999;
  
  typedef edm::View<reco::LeafCandidate>::const_iterator IC;  
  for (IC icand = objectsH->begin(); icand != objectsH->end(); ++icand) {
    const reco::LeafCandidate& cand = *icand;

    double deltaPhi = fabs(reco::deltaPhi( met.phi(), cand.phi() ) );
    if(deltaPhi<deltaPhiMin) deltaPhiMin = deltaPhi;
  }

  if(verbose_)  
    std::cout<<"Min delta phi between "<<metIT_<<" and "<<objectsIT_<<" = "<<deltaPhiMin<<std::endl;
  
  std::auto_ptr<float> pOut(new float(deltaPhiMin) ); 
  iEvent.put( pOut ); 
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DeltaPhiProducer);
