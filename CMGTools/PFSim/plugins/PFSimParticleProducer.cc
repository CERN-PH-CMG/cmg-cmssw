#include "CMGTools/PFSim/plugins/PFSimParticleProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

using namespace std;
using namespace edm;

PFSimParticleProducer::PFSimParticleProducer(const edm::ParameterSet& iConfig) {
  
  hepmcSrc_ = iConfig.getParameter<InputTag>("hepmcSrc");

  verbose_ = 
    iConfig.getUntrackedParameter<bool>("verbose",false);


  produces< std::vector<int> > ();
}

PFSimParticleProducer::~PFSimParticleProducer() {}


void PFSimParticleProducer::beginJob() {}


void PFSimParticleProducer::produce(Event& iEvent, 
				    const EventSetup& iSetup) {
  
  //Gathering the HepMCProduct information
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel(hepmcSrc_, evt);

  simulator_.simulate( *evt->GetEvent() ); 

  auto_ptr< std::vector<int> > out( new std::vector<int> );
  iEvent.put( out  );
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PFSimParticleProducer);
