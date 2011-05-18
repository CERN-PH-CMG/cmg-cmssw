#include "CMGTools/Common/plugins/HistogramAnalyzer.h"
#include "CMGTools/Susy/interface/PFBJetHistograms.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef cmg::HistogramAnalyzer<cmg::PFBJetHistograms> PFBJetHistograms;

DEFINE_FWK_MODULE(PFBJetHistograms);

