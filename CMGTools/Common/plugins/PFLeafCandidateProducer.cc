#include "CMGTools/Common/plugins/PFLeafCandidateProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "AnalysisDataFormats/CMGTools/interface/SimpleParticle.h"

using namespace std;
using namespace edm;

PFLeafCandidateProducer::PFLeafCandidateProducer(const edm::ParameterSet& iConfig) {
  
//   hepmcSrc_ = iConfig.getParameter<InputTag>("hepmcSrc");

//   verbose_ = 
//     iConfig.getUntrackedParameter<bool>("verbose",false);


  produces< std::vector<cmg::SimpleParticle> > ();
}

PFLeafCandidateProducer::~PFLeafCandidateProducer() {}


void PFLeafCandidateProducer::beginJob() {}


void PFLeafCandidateProducer::produce(Event& iEvent, 
				    const EventSetup& iSetup) {
  
  //Gathering the HepMCProduct information
//   edm::Handle<HepMCProduct> evt;
//   iEvent.getByLabel(hepmcSrc_, evt);

//   simulator_.simulate( *evt->GetEvent() ); 

//   // now translating simulated particles into types that can be stored in the EDM event
//   auto_ptr< std::vector<cmg::SimpleParticle> > outPtr( new std::vector<cmg::SimpleParticle> );
//   const PFSim::Simulator::Particles& ptcs = simulator_.simParticles();
//   for( unsigned i=0; i<ptcs.size(); ++i) {
//     outPtr->push_back( cmg::SimpleParticle( ptcs[i].type(),
// 					    ptcs[i].p4().perp(), 
// 					    ptcs[i].p4().eta(), 
// 					    ptcs[i].p4().phi(), 
// 					    ptcs[i].p4().m()
// 					    ) );  
//   }

//   iEvent.put( outPtr  );
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PFLeafCandidateProducer);
