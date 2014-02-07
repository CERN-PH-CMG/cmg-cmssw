#include "CMGTools/PFSim/plugins/PFSimParticleProducer.h"


#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "AnalysisDataFormats/CMGTools/interface/SimpleParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

using namespace std;
using namespace edm;


static const int PDGCacheMax = 50000;

PFSimParticleProducer::PFSimParticleProducer(const edm::ParameterSet& iConfig) : 
 
  hepmcSrc_( iConfig.getParameter<InputTag>("hepmcSrc")),
  genSrc_( iConfig.getParameter<InputTag>("genSrc")),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose",false)),
  firstEvent_(true),
  simulator_( verbose_ )
{
  produces< OutputParticles > ();
}

PFSimParticleProducer::~PFSimParticleProducer() {}


void PFSimParticleProducer::beginJob() {}



void PFSimParticleProducer::produce(Event& iEvent, 
				    const EventSetup& iSetup) {
  
  
  if (firstEvent_) {
    iSetup.getData( pdt_ );
  }

  //For testing HepMC event interface
  //   edm::Handle<HepMCProduct> evt;
  //   iEvent.getByLabel(hepmcSrc_, evt);
  //   simulator_.simulate( *evt->GetEvent(), *(pdt_.product())); 

  //Manual conversion to internal Particle class
  edm::Handle< vector< reco::GenParticle > > genParticlesH;
  iEvent.getByLabel(genSrc_, genParticlesH);
  
  PFSim::Simulator::Particles inputParticles( genParticlesH->size() );
  for( unsigned i=0; i<genParticlesH->size(); ++i ) {
    const reco::GenParticle& genPtc = genParticlesH->at(i);
    float charge = 99.;
    HepMC::FourVector momentum( genPtc.px(), 
				genPtc.py(), 
				genPtc.pz(), 
				genPtc.energy() ); 
    PFSim::Particle ptc( PFSim::Particle::X, genPtc.pdgId(), charge, momentum ); 
    inputParticles.push_back(ptc);
    inputParticles.back().setStatus( genPtc.status() );
  }


  simulator_.simulate( inputParticles, *(pdt_.product() ) ); 

  // now translating simulated particles into types that can be stored in the EDM event
  auto_ptr< OutputParticles > outPtr( new OutputParticles );
  const PFSim::Simulator::Particles& ptcs = simulator_.simParticles();
  for( unsigned i=0; i<ptcs.size(); ++i) {
    // cout<<"pdgId "<<ptcs[i].pdgId()<<" charge "<<chargeTimesThree(ptcs[i].pdgId())<<endl; 
    reco::LeafCandidate::LorentzVector p4(ptcs[i].p4().px(), 
					  ptcs[i].p4().py(), 
					  ptcs[i].p4().pz(), 
					  ptcs[i].p4().e());
    int charge = ptcs[i].charge();
    OutputParticle outptc(charge, p4);
    outptc.setPdgId( ptcs[i].pdg_id() );
    outPtr->push_back( outptc );  
  }

  iEvent.put( outPtr  );
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PFSimParticleProducer);
