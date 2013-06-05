#include "CMGTools/PFSim/interface/Simulator.h"


using namespace std;


static const int PDGCacheMax = 50000;


PFSim::Simulator::Simulator() :
  firstEvent_(true),
  chargeP_( PDGCacheMax, 0 ),
  chargeM_( PDGCacheMax, 0 ) {}



void PFSim::Simulator::simulate( const HepMC::GenEvent& event, 
				 const ParticleDataTable& pdt ) {

  if (firstEvent_) {
    for( HepPDT::ParticleDataTable::const_iterator p = pdt.begin(); 
	 p != pdt.end(); ++ p ) {
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

  simParticles_.clear();

  // cout<<"simulating"<<endl;

  // loop over all particles
  bool filled = false;
  for(HepMC::GenEvent::particle_const_iterator iter = event.particles_begin(); 
      iter!= event.particles_end() && !filled; ++iter) {
    const HepMC::GenParticle& p = **iter;
 
    if( p.status() != 1 ) continue; 

    Particle::Type type = Particle::X;

    switch( abs( p.pdg_id() )  ) {
    case 12:
    case 14:
    case 16: // neutrinos
      // invisible particles (need to add other fancy stuff)
      continue; 
    case 11: // electrons
      type = Particle::e;
      break;
    case 13: // muons
      type = Particle::mu;
      break; 
    case 211: // pi+-
    case 321: // K+-
    case 2212: // p
      type = Particle::h; 
      break;
    case 130:  // KLong
    case 310:  // KShort
    case 2112: // n
      type = Particle::h0; 
      break;
    case 22: // gamma
      type = Particle::gamma;
      break;
    } 
    
    int charge = chargeTimesThree( p.pdg_id() );
    Particle aPart = Particle(type, p.pdg_id(), charge, p.momentum() );
    simParticles_.push_back(aPart);    
  }
}



int PFSim::Simulator::chargeTimesThree( int id ) const {
  if( std::abs( id ) < PDGCacheMax ) 
    return id > 0 ? chargeP_[ id ] : chargeM_[ - id ];
  map<int, int>::const_iterator f = chargeMap_.find( id );
  if ( f == chargeMap_.end() )  {
    return HepPDT::ParticleID(id).threeCharge();
  }
  return f->second;
}
