#include "CMGTools/Common/plugins/HistogramAnalyzer.h"
#include "CMGTools/ZmumuJetsTutorial/interface/JetCountingHistograms.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef cmg::HistogramAnalyzer<cmg::JetCountingHistograms> JetCountingHistogramsAnalyzer;
DEFINE_FWK_MODULE(JetCountingHistogramsAnalyzer);

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiMuon> > DiMuonSelector;
DEFINE_FWK_MODULE(DiMuonSelector);