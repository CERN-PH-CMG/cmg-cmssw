#ifndef HTRANSFORMTOHELICITYFRAME_h
#define HTRANSFORMTOHELICITYFRAME_h

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "TLorentzVector.h"
#include "TVector3.h"

using namespace std;

//! class to compute the polar and azimuthal angle in the helicity frame
class HTransformToHelicityFrame {

public:

  //! constructor, with default values for 7TeV protons
  HTransformToHelicityFrame(const Double_t& energy=7000.0, const Double_t& mass=0.938272);

  ~HTransformToHelicityFrame() { };

  //! change the energy
  void SetCmsEnergy(const Double_t& cms_energy);

  //! call this function once per event. 
  void TransformToHelicityFrame(TLorentzVector muplus,TLorentzVector muminus);

  //! return theta in the helicity frame
  const Double_t& GetTheta() { return Htheta_star; }
  
  //! return cos theta in the helicity frame
  const Double_t& GetCosTheta() { return Hcos_theta_star; }

  //! return phi in the helicity frame
  const Double_t& GetPhi() { return Hphi_star; }

private:

  Double_t HcmsEnergy;
  Double_t HbeamPartMass;

  Double_t Htheta_star;
  Double_t Hcos_theta_star;
  Double_t Hphi_star;

  TLorentzVector PF;
  TLorentzVector PW;
  TLorentzVector PF_org;
  TLorentzVector PW_org;
  TLorentzVector Wv;
  TVector3 b;
  TVector3 Hproton_Z_plane_3v;
  TVector3 HMu_Z_plane_3v;

  
};

#endif
