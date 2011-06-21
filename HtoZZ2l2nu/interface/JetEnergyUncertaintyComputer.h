#ifndef jeunccomp_h
#define jeunccomp_h

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

namespace jet
{
  class UncertaintyComputer
  {
  public:

    enum UncertaintyVariations { JES_UP, JES_DOWN, JER };
    
    UncertaintyComputer(JetResolution *ptResol, JetResolution *etaResol, JetResolution *phiResol, JetCorrectionUncertainty *jecUnc) :
      ptResol_(ptResol),
      etaResol_(etaResol),
      phiResol_(phiResol),
      jecUnc_(jecUnc)
	{
	}
    ~UncertaintyComputer() {}
    
    void compute(LorentzVectorCollection &jets, LorentzVector &met);

    inline LorentzVectorCollection &getVariedJets(int variation) { return jetsVar[variation]; }
    inline LorentzVector &getVariedMet(int variation) { return metsVar[variation]; }
    
  private:
    JetResolution *ptResol_, *etaResol_, *phiResol_;
    JetCorrectionUncertainty *jecUnc_;
    std::vector<LorentzVectorCollection> jetsVar;
    LorentzVectorCollection metsVar;
  };

}

#endif
