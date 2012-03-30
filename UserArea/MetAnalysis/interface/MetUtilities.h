#ifndef METUTILITIES_H
#define METUTILITIES_H

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include <vector>

using namespace reco;

class MetUtilities {

public:

  MetUtilities();
  virtual ~MetUtilities();

  bool filter(const PFJet *iJet,Double_t iPhi1,Double_t iEta1,Double_t iPhi2,Double_t iEta2);

  bool passPFLooseId(const PFJet *iJet);

  // void addNeut(const PFJet *iJet, TLorentzVector &iVec, Double_t &iSumEt, FactorizedJetCorrector *iJetCorrector,const PileupEnergyDensityCol *iPUEnergyDensity, int iSign);  // chiara

protected:

};

#endif
