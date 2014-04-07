#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CMGTools/Common/interface/CMGJetPUIDSelectorDefinition.h"

typedef ObjectSelector<cmg::CMGJetPUIDSelectorDefinition> CMGJetPUIDSelector;

DEFINE_FWK_MODULE(CMGJetPUIDSelector);
