#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"
#include "CMGTools/DiJetHighMass/interface/DiJetHistograms.h"
#include "CMGTools/Common/plugins/HistogramAnalyzer.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiJet> > DiJetSelector;
DEFINE_FWK_MODULE(DiJetSelector);

typedef cmg::HistogramAnalyzer< cmg::DiJetHistograms >  DiJetHistogramsAnalyser;
DEFINE_FWK_MODULE(DiJetHistogramsAnalyser);
