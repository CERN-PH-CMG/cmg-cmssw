#include "CMGTools/PFSim/interface/CMS.h"

#include <iostream> 
using namespace std;

float PFSim::CMS::electronEfficiency(const HepMC::FourVector& mom) const {
  float pt = mom.perp();
  float eta = mom.eta();
  float effvalue = 0.;
  if (pt>3. && fabs(eta)<2.5)
    effvalue = 0.99;
  return effvalue;
}


float PFSim::CMS::photonEfficiency(const HepMC::FourVector& mom) const {
  float energy = mom.e();
  float eta = mom.eta();
  float effvalue = 0.;
  if (energy>0.25 && fabs(eta)<2.95)
    effvalue = 1.0;
  return effvalue;
}


float PFSim::CMS::muonEfficiency(const HepMC::FourVector& mom) const {
  float pt = mom.perp();
  float eta = mom.eta();
  float effvalue = 0.;
  if (pt>5 && fabs(eta)<2.4)
    effvalue = 1.0;
  return effvalue;
}



float PFSim::CMS::chargedHadronEfficiency(const HepMC::FourVector& mom) const {
  float pt = mom.perp();
  float eta = mom.eta();
  float effvalue = 0.;
  if (pt>1.0 && fabs(eta)<2.5)
    effvalue = 0.90;
  return effvalue;
}


float PFSim::CMS::neutralHadronEfficiency(const HepMC::FourVector& mom) const {
  float energy = mom.e();
  float eta = mom.eta();
  float effvalue = 0.;
  if (energy>5.0 && fabs(eta)<5.0)
    effvalue = 1.0;
  return effvalue;
}


float PFSim::CMS::electronResolution(const HepMC::FourVector& mom) const {
  // need to take into account the fact that the tracker is used to compute 
  // the energy, thus improving resolution at low pT
  // for now, same resolution for photons and electrons
  return photonResolution( mom );
}

float PFSim::CMS::photonResolution(const HepMC::FourVector& mom) const {
  float energy = mom.e();
  float stoch = 0.03 / sqrt(energy);
  float noise = 0.12 / energy;
  float constant = 0.003;
  float relres = sqrt(stoch*stoch + noise*noise + constant*constant);

  relres *= 3.5; // resolution worse in the fast sim
  // maybe due to PFClusters? 
  return relres; // only returning stochastic term for now, should return relres
}

float PFSim::CMS::muonResolution(const HepMC::FourVector& mom) const {
  return 0.01;
}

float PFSim::CMS::chargedHadronResolution(const HepMC::FourVector& mom) const {
  return 0.013;
}

float PFSim::CMS::neutralHadronResolution(const HepMC::FourVector& mom) const {
  float energy = mom.e();
  float relres = 1.1 / sqrt(energy);
  return relres;
}
