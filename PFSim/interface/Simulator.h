#ifndef __PFSim_Simulator__
#define __PFSim_Simulator__

#include <vector>


#include <cstddef>
#include <HepMC/GenEvent.h>
#include <HepMC/SimpleVector.h>

#include "CMGTools/PFSim/interface/Particle.h"


namespace PFSim {
  
  class Simulator {
    
  public:
    typedef std::vector<Particle> Particles;

    Simulator() {}

    void simulate(const HepMC::GenEvent& event );  
    
    const Particles& simParticles() const {return simParticles_;}
    
  private:

    Particle simulateParticle(PFSim::Particle::Type type, 
			      const HepMC::FourVector& momentum ) const;

    Particles simParticles_; 
      
  };
}
  
#endif 
