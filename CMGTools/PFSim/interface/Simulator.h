#ifndef __PFSim_Simulator__
#define __PFSim_Simulator__

#include <vector>
#include <map>
#include <random>

#include <cstddef>
#include <HepMC/GenEvent.h>
#include <HepMC/SimpleVector.h>
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "CMGTools/PFSim/interface/Particle.h"
#include "CMGTools/PFSim/interface/Detector.h"

namespace PFSim {
  
  class Simulator {
    
  public:
    typedef std::vector<Particle> Particles;

    Simulator(bool verbose=false); 

    void simulate(const HepMC::GenEvent& event, 
		  const ParticleDataTable& pdt );  

    void simulate(const Particles& particles, 
		  const ParticleDataTable& pdt );  
    
    const Particles& simParticles() const {return simParticles_;}
    
  private:

    void cachePDT(const ParticleDataTable& pdt); 

    int chargeTimesThree( int id ) const;

    Particles simParticles_; 

    bool firstEvent_; 
    std::vector<int>    chargeP_, chargeM_;
    std::map<int, int>  chargeMap_;

    const Detector* detector_;

    std::default_random_engine generator_;
    std::uniform_real_distribution<float> uniform_;
    
    bool verbose_;
  };
}
  
#endif 
