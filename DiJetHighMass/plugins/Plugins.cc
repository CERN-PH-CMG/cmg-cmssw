#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiJet> > DiJetSelector;
DEFINE_FWK_MODULE(DiJetSelector);
