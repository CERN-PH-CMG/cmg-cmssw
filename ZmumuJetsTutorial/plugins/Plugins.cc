#include "CMGTools/Common/plugins/HistogramAnalyzer.h"
#include "CMGTools/ZmumuJetsTutorial/interface/JetCountingHistograms.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef cmg::HistogramAnalyzer<cmg::JetCountingHistograms> JetCountingHistogramsAnalyzer;
DEFINE_FWK_MODULE(JetCountingHistogramsAnalyzer);