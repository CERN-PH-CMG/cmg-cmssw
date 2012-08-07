#include "CMGTools/LEP3/interface/FourJetEpKinFitter.h"
#include "CMGTools/LEP3/interface/FourJetEpMKinFitter.h"
#include "CMGTools/LEP3/interface/DiJetMKinFitter.h"
#include <TLorentzVector.h>

namespace {
  struct CMGTools_LEP3 {

    FourJetEpKinFitter fourJetEpKinFitter; 
    DiJetMKinFitter diJetMKinFitter;
    FourJetEpMKinFitter fourJetEpMKinFitter;
    std::pair<TLorentzVector,TLorentzVector> pairOfTLV;
    std::vector<TLorentzVector> vectorOfTLV;

  };
}
