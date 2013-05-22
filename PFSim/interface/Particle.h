#ifndef __PFSim_Particle__
#define __PFSim_Particle__


#include <HepMC/SimpleVector.h>

namespace PFSim {
  
  class Particle { 

  public:

    enum Type {
      X=0,     // undefined
      h,       // charged hadron
      e,       // electron 
      mu,      // muon 
      gamma,   // photon
      h0,      // neutral hadron
      nu       // invisible
    };

    Particle(Type type, const HepMC::FourVector& mom) {
      type_=type;
      momentum_ = mom;
    }

  private:
    Type               type_;
    HepMC::FourVector  momentum_; 
  };
 
}

  
#endif 
