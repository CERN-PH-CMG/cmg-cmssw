#ifndef __PFSim_Simulator__
#define __PFSim_Simulator__

#include <vector>


#include <cstddef>
#include <HepMC/GenEvent.h>
#include <HepMC/SimpleVector.h>
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "CMGTools/PFSim/interface/Particle.h"

namespace PFSim {
  
  class Simulator {
    
  public:
    typedef std::vector<Particle> Particles;

    void simulate(const HepMC::GenEvent& event);  
    
    const Particles& simParticles() const {return simParticles_;}
    
  private:

    Particles simParticles_; 
  };
}
  
#endif 
