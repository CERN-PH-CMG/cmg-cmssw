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

    Particle(Type type, int pdgId, float charge, const HepMC::FourVector& mom) {
      type_=type;
      pdgId_=pdgId;
      charge_=charge;
      momentum_ = mom;
    }

    Type type() const {return type_;}
    int  pdgId() const {return pdgId_;}
    float charge() const {return charge_;}
    const HepMC::FourVector& momentum() const {return momentum_;}
    const HepMC::FourVector& p4() const {return momentum();}

  private:
    Type               type_;
    int                pdgId_;
    HepMC::FourVector  momentum_; 
    float              charge_; 
  };
 
}

  
#endif 
