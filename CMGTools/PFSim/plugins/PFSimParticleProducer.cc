#include "CMGTools/PFSim/plugins/PFSimParticleProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "AnalysisDataFormats/CMGTools/interface/SimpleParticle.h"


using namespace std;
using namespace edm;

static const int PDGCacheMax = 50000;

PFSimParticleProducer::PFSimParticleProducer(const edm::ParameterSet& iConfig) : 
 
  hepmcSrc_( iConfig.getParameter<InputTag>("hepmcSrc")),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose",false)),
  firstEvent_(true),
  chargeP_( PDGCacheMax, 0 ),
  chargeM_( PDGCacheMax, 0 ) {
  produces< OutputParticles > ();
}

PFSimParticleProducer::~PFSimParticleProducer() {}


void PFSimParticleProducer::beginJob() {}



int PFSimParticleProducer::chargeTimesThree( int id ) const {
  if( std::abs( id ) < PDGCacheMax ) 
    return id > 0 ? chargeP_[ id ] : chargeM_[ - id ];
  map<int, int>::const_iterator f = chargeMap_.find( id );
  if ( f == chargeMap_.end() )  {
//     if ( abortOnUnknownPDGCode_ )
//       throw edm::Exception( edm::errors::LogicError ) 
// 	<< "invalid PDG id: " << id << endl;
//     else
    return HepPDT::ParticleID(id).threeCharge();
  }
  return f->second;
}


void PFSimParticleProducer::produce(Event& iEvent, 
				    const EventSetup& iSetup) {
  
  
  if (firstEvent_) {
    ESHandle<HepPDT::ParticleDataTable> pdt;
    iSetup.getData( pdt );
    for( HepPDT::ParticleDataTable::const_iterator p = pdt->begin(); p != pdt->end(); ++ p ) {
      const HepPDT::ParticleID & id = p->first;
      int pdgId = id.pid(), apdgId = std::abs( pdgId );
      int q3 = id.threeCharge();
      if ( apdgId < PDGCacheMax && pdgId>0 ) {
	chargeP_[ apdgId ] = q3;
	chargeM_[ apdgId ] = -q3;
      } else if ( apdgId < PDGCacheMax ) {
	chargeP_[ apdgId ] = -q3;
	chargeM_[ apdgId ] = q3;
      } else {
	chargeMap_[ pdgId ] = q3;
	chargeMap_[ -pdgId ] = -q3;
      }
    }
    firstEvent_ = false; 
  }

  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel(hepmcSrc_, evt);

  simulator_.simulate( *evt->GetEvent()); 

  // now translating simulated particles into types that can be stored in the EDM event
  auto_ptr< OutputParticles > outPtr( new OutputParticles );
  const PFSim::Simulator::Particles& ptcs = simulator_.simParticles();
  for( unsigned i=0; i<ptcs.size(); ++i) {
    cout<<"pdgId "<<ptcs[i].pdgId()<<" charge "<<chargeTimesThree(ptcs[i].pdgId())<<endl; 
    reco::LeafCandidate::LorentzVector p4(ptcs[i].p4().px(), 
					  ptcs[i].p4().py(), 
					  ptcs[i].p4().pz(), 
					  ptcs[i].p4().e());
    int charge = 0;
    outPtr->push_back( OutputParticle( charge, p4) );  
    //TODO deal with charge and pdgId
  }

  iEvent.put( outPtr  );
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PFSimParticleProducer);
