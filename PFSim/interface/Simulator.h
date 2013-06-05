#ifndef __PFSim_Simulator__
#define __PFSim_Simulator__

#include <vector>
#include <map>

#include <cstddef>
#include <HepMC/GenEvent.h>
#include <HepMC/SimpleVector.h>
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "CMGTools/PFSim/interface/Particle.h"

namespace PFSim {
  
  class Simulator {
    
  public:
    typedef std::vector<Particle> Particles;

    Simulator(); 

    void simulate(const HepMC::GenEvent& event, 
		  const ParticleDataTable& pdt );  
    
    const Particles& simParticles() const {return simParticles_;}
    
  private:
    int chargeTimesThree( int id ) const;

    Particles simParticles_; 

    bool firstEvent_; 
    std::vector<int>    chargeP_, chargeM_;
    std::map<int, int>  chargeMap_;

  };
}
  
#endif 
