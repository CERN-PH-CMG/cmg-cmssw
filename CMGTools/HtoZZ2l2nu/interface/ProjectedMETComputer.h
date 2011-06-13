#ifndef ProjectedMETComputer_H
#define ProjectedMETComputer_H

#include "Math/LorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "TMath.h"
#include "TVector2.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

class ProjectedMETComputer {
public:
  ProjectedMETComputer() : projEtMiss(0) {}
  
  //
  double compute(const LorentzVector& lepton1, const LorentzVector& lepton2, const LorentzVector& met)
  {
    double dphimin=min( deltaPhi(lepton1.phi(),met.phi()), deltaPhi(lepton2.phi(),met.phi()) );
    projEtMiss = met.pt() * ( dphimin > TMath::Pi()/2 ? 1.0 : fabs(TMath::Sin(dphimin)) );
    return projEtMiss;
  }

  double projectedMET() const { return projEtMiss; }
  
  ~ProjectedMETComputer() {}
  
private:

  double projEtMiss;

};

#endif
