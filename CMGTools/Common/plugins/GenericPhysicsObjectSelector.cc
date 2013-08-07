#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"

// typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition> GenericPhysicsObjectSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::PFJet> > PFJetSelector;
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::BaseMET> > BaseMETSelector;

DEFINE_FWK_MODULE(PFJetSelector);
DEFINE_FWK_MODULE(BaseMETSelector);
