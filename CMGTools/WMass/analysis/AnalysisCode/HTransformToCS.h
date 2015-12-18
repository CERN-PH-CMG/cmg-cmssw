#ifndef HTRANSFORMTOCS_h
#define HTRANSFORMTOCS_h

#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include "TLorentzVector.h"
#include "TVector3.h"



//! class to compute the polar and azimuthal angle in the Collins-Soper frame
class HTransformToCS {

public:

  //! constructor, with default values for 7TeV protons
  HTransformToCS(const Double_t energy=7000.0, const Double_t mass=0.938272);

  ~HTransformToCS() { };

  //! change the energy
  void SetCmsEnergy(const Double_t cms_energy);

  //! call this function once per event.
  void TransformToCS(TLorentzVector muplus,TLorentzVector muminus);

  //! return theta in the Collins-Soper frame
  const Double_t GetTheta() { return theta_cs; }
  
  //! return cos theta in the Collins-Soper frame
  const Double_t GetCosTheta() { return cos_theta_cs; }

  //! return phi in the Collins-Soper frame
  const Double_t GetPhi() { return phi_cs; }

  //! return the TVector3 of the negative muon rotated to the Collins-Soper frame
  TVector3 GetTVector3CS() { return muminusVec; }

private:

  Double_t HcmsEnergy;
  Double_t HbeamPartMass;

  Double_t theta_cs;
  Double_t cos_theta_cs;
  Double_t phi_cs;

  TLorentzVector PF;
  TLorentzVector PW;
  TLorentzVector PF_org;
  TLorentzVector PW_org;
  TLorentzVector Wv;
  TVector3 b;
  bool PFMinus;
  TLorentzVector PBiSec;
  TVector3 PhiSecZ;
  TVector3 PhiSecW;
  TVector3 PhiSecY;
  TVector3 PhiSecX;
  TVector3 muminusVec;
  
  

};

#endif
