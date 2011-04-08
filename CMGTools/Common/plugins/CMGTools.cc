#include "CMGTools/Common/plugins/CMGTools.h"
#include "CMGTools/Common/plugins/TopProjector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MultiObjectPOProducer);
DEFINE_FWK_MODULE(HemispherePOProducer);
DEFINE_FWK_MODULE(DiHemispherePOProducer);
DEFINE_FWK_MODULE(DiElectronPOProducer);
DEFINE_FWK_MODULE(DiMuonPOProducer);
DEFINE_FWK_MODULE(DiJetPOProducer);
DEFINE_FWK_MODULE(DiPFJetPOProducer);
DEFINE_FWK_MODULE(DiTauPOProducer);
DEFINE_FWK_MODULE(ElectronPOProducer);
DEFINE_FWK_MODULE(MuonPOProducer);
DEFINE_FWK_MODULE(TauPOProducer);
DEFINE_FWK_MODULE(QuadMuonPOProducer);
DEFINE_FWK_MODULE(BaseJetPOProducer);
DEFINE_FWK_MODULE(PFJetPOProducer);
DEFINE_FWK_MODULE(GenParticlePOProducer);
DEFINE_FWK_MODULE(GenJetPOProducer);
DEFINE_FWK_MODULE(BaseMETPOProducer);
DEFINE_FWK_MODULE(BaseMETModifier);
DEFINE_FWK_MODULE(PATMETPOProducer);
DEFINE_FWK_MODULE(PFJetHistogramsAnalyzer);

DEFINE_FWK_MODULE(GenericBaseMETHistograms);
DEFINE_FWK_MODULE(GenericPOHistograms);
DEFINE_FWK_MODULE(GenericPatMETHistograms);
DEFINE_FWK_MODULE(CmgElectronHistograms);
DEFINE_FWK_MODULE(CmgMuonHistograms);
DEFINE_FWK_MODULE(CmgHemiHistograms);
DEFINE_FWK_MODULE(CmgMultiObjectHistograms);
DEFINE_FWK_MODULE(CmgDiMuonHistograms);
DEFINE_FWK_MODULE(CmgDiElectronHistograms);
DEFINE_FWK_MODULE(CmgDiHemiHistograms);

DEFINE_FWK_MODULE(CmgDiMuonDataSet);

DEFINE_FWK_MODULE(RunInfoAccountingAnalyzer);
DEFINE_FWK_MODULE(PhysicsObjectPrinter);
DEFINE_FWK_MODULE(PFCandidatePOProducer);
DEFINE_FWK_MODULE(WENuPOProducer);
DEFINE_FWK_MODULE(WMuNuPOProducer);
DEFINE_FWK_MODULE(WTauNuPOProducer);
DEFINE_FWK_MODULE(CutSummaryAnalyzer);
DEFINE_FWK_MODULE(LeadingCMGPOSelector);
DEFINE_FWK_MODULE(LeadingCMGBaseJetSelector);
DEFINE_FWK_MODULE(LeadingCMGPFJetSelector);

DEFINE_FWK_MODULE(CmgPFJetSelector);
DEFINE_FWK_MODULE(CmgBaseJetSelector);
DEFINE_FWK_MODULE(CmgBaseMETSelector);
DEFINE_FWK_MODULE(CmgElectronSelector);
DEFINE_FWK_MODULE(CmgMuonSelector);
DEFINE_FWK_MODULE(CmgDiMuonSelector);
DEFINE_FWK_MODULE(CmgDiElectronSelector);
DEFINE_FWK_MODULE(CmgWENuSelector);
DEFINE_FWK_MODULE(CmgWMuNuSelector);
