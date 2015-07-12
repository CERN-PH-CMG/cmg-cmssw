#include "HTransformToHelicityFrame.h"

using namespace std;

//--------------------------------------------------------------------------------------------
HTransformToHelicityFrame::HTransformToHelicityFrame(const Double_t& energy,const Double_t& mass)
{

  HbeamPartMass=mass;
  SetCmsEnergy(energy);
}

//--------------------------------------------------------------------------------------------
void HTransformToHelicityFrame::SetCmsEnergy(const Double_t& cms_energy)
{
  if(HcmsEnergy!=cms_energy) {
    HcmsEnergy = cms_energy;
    cout << "HTransformToHelicityFrame::SetCmsEnergy: Set center of mass energy: " << HcmsEnergy << endl;
    cout << "HTransformToHelicityFrame::SetCmsEnergy: Mass of beam particles:    " << HbeamPartMass << endl;

    PF_org.SetPxPyPzE(0.,0., sqrt(0.25*HcmsEnergy*HcmsEnergy-HbeamPartMass*HbeamPartMass),0.5*HcmsEnergy);
    PW_org.SetPxPyPzE(0.,0.,-sqrt(0.25*HcmsEnergy*HcmsEnergy-HbeamPartMass*HbeamPartMass),0.5*HcmsEnergy);
    /*
    PF_org.SetPxPyPzE(0.,0., 0.5*HcmsEnergy,0.5*HcmsEnergy);
    PW_org.SetPxPyPzE(0.,0.,-0.5*HcmsEnergy,0.5*HcmsEnergy);
    */
  }
}


//--------------------------------------------------------------------------------------------
void HTransformToHelicityFrame::TransformToHelicityFrame(TLorentzVector muplus,TLorentzVector muminus)
{
  Wv= muplus+muminus;// this is the Z boson 4vector
  b = Wv.BoostVector();
  muplus.Boost(-b);
  muminus.Boost(-b);
  
  PF = PF_org;
  PW = PW_org;

  PF.Boost(-b);
  PW.Boost(-b);

  Htheta_star = muminus.Vect().Angle(Wv.Vect());
  Hcos_theta_star = TMath::Cos(Htheta_star);
  // choose what to call proton and what anti-proton
  if(Wv.Angle(PF.Vect())>Wv.Angle(PW.Vect()))
    {
      Hproton_Z_plane_3v = Wv.Vect().Cross(PF.Vect()).Unit();
    }
  else
    {
      Hproton_Z_plane_3v = Wv.Vect().Cross(PW.Vect()).Unit();
    }
  HMu_Z_plane_3v = (Wv.Vect().Cross(muminus.Vect())).Unit();
  Hphi_star = HMu_Z_plane_3v.Angle(Hproton_Z_plane_3v);
  if(Hproton_Z_plane_3v.Dot(muminus.Vect()) < 0.0) {
    Hphi_star = -Hphi_star;
  }
}
