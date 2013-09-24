#ifndef __PFSim_CMS__
#define __PFSim_CMS__

#include "CMGTools/PFSim/interface/Detector.h"

namespace PFSim {

  class CMS : public Detector {
  public:
    CMS() {}
    float electronEfficiency(const HepMC::FourVector& mom) const ;
    float electronResolution(const HepMC::FourVector& mom) const ;

    float photonEfficiency(const HepMC::FourVector& mom) const ;
    float photonResolution(const HepMC::FourVector& mom) const ;

    float muonEfficiency(const HepMC::FourVector& mom) const ;
    float muonResolution(const HepMC::FourVector& mom) const ;

    float chargedHadronEfficiency(const HepMC::FourVector& mom) const ;
    float chargedHadronResolution(const HepMC::FourVector& mom) const ;

    float neutralHadronEfficiency(const HepMC::FourVector& mom) const ;
    float neutralHadronResolution(const HepMC::FourVector& mom) const ;
  };

}

#endif
