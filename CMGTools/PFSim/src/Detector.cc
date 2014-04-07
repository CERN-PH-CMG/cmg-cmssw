#include "CMGTools/PFSim/interface/Detector.h"


float PFSim::Detector::efficiency(PFSim::Particle::Type type,
				  const HepMC::FourVector& mom ) const {
  switch( type ) {
  case Particle::h:
    return chargedHadronEfficiency(mom);
    break;
  case Particle::e:
    return electronEfficiency(mom);
    break;
  case Particle::mu:
    return muonEfficiency(mom);
    break;
  case Particle::gamma:
    return photonEfficiency(mom);
    break;
  case Particle::h0:
    return neutralHadronEfficiency(mom);
    break;
  case Particle::X:
  default:
    return -1; 
  }
}



float PFSim::Detector::resolution(PFSim::Particle::Type type,
				  const HepMC::FourVector& mom ) const {
  switch( type ) {
  case Particle::h:
    return chargedHadronResolution(mom);
    break;
  case Particle::e:
    return electronResolution(mom);
    break;
  case Particle::mu:
    return muonResolution(mom);
    break;
  case Particle::gamma:
    return photonResolution(mom);
    break;
  case Particle::h0:
    return neutralHadronResolution(mom);
    break;
  case Particle::X:
  default:
    return -1; 
  }
}




