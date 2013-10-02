#ifndef __PFSim_Detector__
#define __PFSim_Detector__

#include <HepMC/SimpleVector.h>

#include "CMGTools/PFSim/interface/Particle.h"

namespace PFSim {

  class Detector {
  public:
    virtual float electronEfficiency(const HepMC::FourVector& mom) const = 0;
    virtual float electronResolution(const HepMC::FourVector& mom) const = 0;

    virtual float photonEfficiency(const HepMC::FourVector& mom) const = 0;
    virtual float photonResolution(const HepMC::FourVector& mom) const = 0;

    virtual float muonEfficiency(const HepMC::FourVector& mom) const = 0;
    virtual float muonResolution(const HepMC::FourVector& mom) const = 0;

    virtual float chargedHadronEfficiency(const HepMC::FourVector& mom) const = 0;
    virtual float chargedHadronResolution(const HepMC::FourVector& mom) const = 0;

    virtual float neutralHadronEfficiency(const HepMC::FourVector& mom) const = 0;
    virtual float neutralHadronResolution(const HepMC::FourVector& mom) const = 0;

    float efficiency(Particle::Type type,
		     const HepMC::FourVector& mom ) const; 
    float resolution(Particle::Type type,
		     const HepMC::FourVector& mom ) const; 
  };
}

#endif
