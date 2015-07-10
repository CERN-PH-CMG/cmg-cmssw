#include "HTransformToCS.h"

using namespace std;

//--------------------------------------------------------------------------------------------
HTransformToCS::HTransformToCS(const Double_t energy,const Double_t mass)
{

  HbeamPartMass=mass;
  SetCmsEnergy(energy);
}

//--------------------------------------------------------------------------------------------
void HTransformToCS::SetCmsEnergy(const Double_t cms_energy)
{
  if(HcmsEnergy!=cms_energy) {
    HcmsEnergy = cms_energy;
    cout << "HTransformToCS::SetCmsEnergy: Set center of mass energy: " << HcmsEnergy << endl;
    cout << "HTransformToCS::SetCmsEnergy: Mass of beam particles:    " << HbeamPartMass << endl;

    PF_org.SetPxPyPzE(0.,0., sqrt(0.25*HcmsEnergy*HcmsEnergy-HbeamPartMass*HbeamPartMass),0.5*HcmsEnergy);
    PW_org.SetPxPyPzE(0.,0.,-sqrt(0.25*HcmsEnergy*HcmsEnergy-HbeamPartMass*HbeamPartMass),0.5*HcmsEnergy);
    /*
    PF_org.SetPxPyPzE(0.,0., 0.5*HcmsEnergy,0.5*HcmsEnergy);
    PW_org.SetPxPyPzE(0.,0.,-0.5*HcmsEnergy,0.5*HcmsEnergy);
    */
  }
}


//--------------------------------------------------------------------------------------------
void HTransformToCS::TransformToCS(TLorentzVector muplus,TLorentzVector muminus)
{
  Wv= muplus+muminus;// this is the Z boson 4vector
  b = Wv.BoostVector();
  muplus.Boost(-b);
  muminus.Boost(-b);
  
  PF = PF_org;
  PW = PW_org;

  PF.Boost(-b);
  PW.Boost(-b);
  PFMinus= true;
  // choose what to call proton and what anti-proton
  if(Wv.Angle(PF.Vect())<Wv.Angle(PW.Vect()))
    {
      PW= -PW;
    }
  else
    {
      PF= -PF;
      PFMinus=false;
    }
  PF=PF*(1.0/PF.Vect().Mag());
  PW=PW*(1.0/PW.Vect().Mag());

  // Bisector is the new Z axis
  PBiSec =PW+PF;
  PhiSecZ =  PBiSec.Vect().Unit();

  PhiSecY = (PhiSecZ.Cross(Wv.Vect().Unit())).Unit();

  muminusVec = muminus.Vect();
  TRotation roataeMe;

 // build matrix for transformation into CS frame
  roataeMe.RotateAxes(PhiSecY.Cross(PhiSecZ),PhiSecY,PhiSecZ);
  roataeMe.Invert();
  // tranfor into CS alos the "debugging" vectors
  muminusVec.Transform(roataeMe);

  theta_cs = muminusVec.Theta();
  cos_theta_cs = TMath::Cos(theta_cs);
  phi_cs = muminusVec.Phi();
}
