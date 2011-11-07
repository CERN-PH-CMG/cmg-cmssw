#ifndef MacroUtils_h
#define MacroUtils_h

#include "DataFormats/Math/interface/deltaR.h"
#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

namespace jet
{
    enum UncertaintyVariations { JES_UP, JES_DOWN, JER };
    void computeVariation(LorentzVectorCollection &jets, LorentzVector &met,  std::vector<LorentzVectorCollection>& jetsVar, LorentzVectorCollection& metsVar, JetResolution* ptResol, JetResolution* etaResol, JetResolution* phiResol, JetCorrectionUncertainty* jecUnc);
}

#endif
