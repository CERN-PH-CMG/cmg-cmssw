#ifndef transversemasscomputer_h
#define transversemasscomputer_h

#include "DataFormats/Math/interface/deltaR.h"

#include "TMath.h"
#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

class TransverseMassComputer
{
 public:
  TransverseMassComputer(){}
  ~TransverseMassComputer(){};

  //compute the transverse mass of a visible+invisible system
  inline double compute(LorentzVector &visible, LorentzVector &invisible, bool assumeSameMass=false)
    {
      float transverseMass(0);

      if(assumeSameMass)
	{
	  LorentzVector sum=visible+invisible;
	  transverseMass = TMath::Power(TMath::Sqrt(TMath::Power(visible.pt(),2)+pow(visible.mass(),2))+TMath::Sqrt(TMath::Power(invisible.pt(),2)+pow(visible.mass(),2)),2);
	  transverseMass-=TMath::Power(sum.pt(),2);
	  transverseMass=TMath::Sqrt(transverseMass);
	}
      else
	{
	  double dphi=fabs(deltaPhi(invisible.phi(),visible.phi()));
	  transverseMass = TMath::Sqrt(2*invisible.pt()*visible.pt()*(1-TMath::Cos(dphi)));
	}

      return transverseMass;
    }
  
};

#endif
