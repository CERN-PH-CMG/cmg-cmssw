#include "CMGTools/PFSim/interface/Simulator.h"


using namespace std;

void PFSim::Simulator::simulate( const HepMC::GenEvent& event ) {

  simParticles_.clear();

  cout<<"simulating"<<endl;

  // loop over all particles
  bool filled = false;
  for(HepMC::GenEvent::particle_const_iterator iter = event.particles_begin(); 
      iter!= event.particles_end() && !filled; ++iter) {
    const HepMC::GenParticle& p = **iter;
 
    if( p.status() != 1 ) continue; 

    // cout<<p.pdg_id()<<" "<<p.status()<<endl;

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

    Particle aPart = simulateParticle(type, p.momentum() );
    simParticles_.push_back(aPart);    
  }
}



PFSim::Particle PFSim::Simulator::simulateParticle(PFSim::Particle::Type type, 
						   const HepMC::FourVector& momentum ) const {
  // cout<<"type "<<type<<" pt "<<momentum.perp()<<" eta "<<momentum.eta()<<endl; 

  return Particle(type, momentum); 
}
