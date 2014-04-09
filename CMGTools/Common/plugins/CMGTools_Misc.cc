#include "CMGTools/Common/plugins/CMGTools_Misc.h"
#include "CMGTools/Common/plugins/TopProjector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

// top projectors

DEFINE_FWK_MODULE(TPCMGPFJetOnPFJet); 


// DEFINE_FWK_MODULE(GenericBaseMETHistograms);
// DEFINE_FWK_MODULE(GenericPOHistograms);
// DEFINE_FWK_MODULE(GenericPatMETHistograms);
// DEFINE_FWK_MODULE(CmgElectronHistograms);
// DEFINE_FWK_MODULE(CmgMuonHistograms);
// DEFINE_FWK_MODULE(CmgTauHistograms);
// DEFINE_FWK_MODULE(CmgHemiHistograms);
// DEFINE_FWK_MODULE(CmgPhotonHistograms);
// DEFINE_FWK_MODULE(CmgMultiObjectHistograms);
// DEFINE_FWK_MODULE(CmgDiMuonHistograms);
// DEFINE_FWK_MODULE(CmgDiTauHistograms);
// DEFINE_FWK_MODULE(CmgDiElectronHistograms);
// DEFINE_FWK_MODULE(CmgDiHemiHistograms);
// DEFINE_FWK_MODULE(PFJetHistogramsAnalyzer);
// DEFINE_FWK_MODULE(TriggerResultHistogramsAnalyzer);

// DEFINE_FWK_MODULE(CmgBaseJetHistograms);
// DEFINE_FWK_MODULE(CmgPFJetHistograms);
// DEFINE_FWK_MODULE(CmgFatJetHistograms);
// DEFINE_FWK_MODULE(CmgDiFatJetHistograms);
// DEFINE_FWK_MODULE(CmgDiJetHistograms);
// DEFINE_FWK_MODULE(CmgDiPFJetHistograms);

// DEFINE_FWK_MODULE(CmgTauEleHistograms);
// DEFINE_FWK_MODULE(CmgTauMuHistograms);
// DEFINE_FWK_MODULE(CmgMuEleHistograms);

// DEFINE_FWK_MODULE(LeafCandidateHistograms);
// DEFINE_FWK_MODULE(GenParticleHistograms);

DEFINE_FWK_MODULE(CmgDiMuonDataSet);
DEFINE_FWK_MODULE(CmgDiTauDataSet);

DEFINE_FWK_MODULE(RunInfoAccountingAnalyzer);
DEFINE_FWK_MODULE(PhysicsObjectPrinter);
DEFINE_FWK_MODULE(CutSummaryAnalyzer);

DEFINE_FWK_MODULE(VertexSizeProducer);
DEFINE_FWK_MODULE(CandidateSizeProducer);

DEFINE_FWK_MODULE(BaseJetCorrector);
DEFINE_FWK_MODULE(PFJetCorrector);
DEFINE_FWK_MODULE(StructuredPFJetCorrector);
