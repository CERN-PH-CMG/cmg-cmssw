#include "CMGTools/Common/plugins/PFVertexProducer.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


PFVertexProducer::PFVertexProducer(const edm::ParameterSet & iConfig) {
  inputTagPFCandidates_ 
    = iConfig.getParameter<edm::InputTag>("PFCandidate");
  produces<reco::PFCandidateCollection>();
}

PFVertexProducer::~PFVertexProducer() {;}

void 
PFVertexProducer::beginRun(edm::Run& run,const edm::EventSetup & es) {;}

void 
PFVertexProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  std::auto_ptr<reco::PFCandidateCollection>
    pfCandidates_p(new reco::PFCandidateCollection);
  
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByLabel( inputTagPFCandidates_, pfCandidates );
    
  for( unsigned i=0; i<pfCandidates->size(); ++i) {

      edm::Ptr<reco::PFCandidate> candPtr(pfCandidates,i);
      reco::PFCandidate cand(candPtr);

      cand.setVertex( candPtr->vertex() );
      
      pfCandidates_p->push_back(cand);
    }      

  iEvent.put(pfCandidates_p);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFVertexProducer);
