#include "CMGTools/Common/plugins/CMGTools.h"
#include "CMGTools/Common/plugins/TopProjector.h"
#include "FWCore/Framework/interface/MakerMacros.h"

// top projectors

DEFINE_FWK_MODULE(TPCMGPFJetOnPFJet); 

DEFINE_FWK_MODULE(MultiObjectPOProducer);
DEFINE_FWK_MODULE(PhotonPOProducer);

DEFINE_FWK_MODULE(DiPFCandidatePOProducer);
DEFINE_FWK_MODULE(DiPhotonPOProducer);
DEFINE_FWK_MODULE(HemispherePOProducer);
DEFINE_FWK_MODULE(FatJetPOProducer);
DEFINE_FWK_MODULE(DiHemispherePOProducer);
DEFINE_FWK_MODULE(DiElectronPOProducer);
DEFINE_FWK_MODULE(DiMuonPOProducer);
DEFINE_FWK_MODULE(DiJetPOProducer);
DEFINE_FWK_MODULE(DiFatJetPOProducer);
DEFINE_FWK_MODULE(DiPFJetPOProducer);
DEFINE_FWK_MODULE(DiTauPOProducer);
DEFINE_FWK_MODULE(TauEPOProducer);
DEFINE_FWK_MODULE(TauMuPOProducer);
DEFINE_FWK_MODULE(MuEPOProducer);
DEFINE_FWK_MODULE(ElectronPOProducer);
DEFINE_FWK_MODULE(MuonPOProducer);
DEFINE_FWK_MODULE(TauPOProducer);
DEFINE_FWK_MODULE(QuadMuonPOProducer);
DEFINE_FWK_MODULE(BaseJetPOProducer);
DEFINE_FWK_MODULE(BaseJetPOScaler);
DEFINE_FWK_MODULE(PFJetPOScaler);
DEFINE_FWK_MODULE(PFJetPOProducer);
DEFINE_FWK_MODULE(GenParticlePOProducer);
DEFINE_FWK_MODULE(GenJetPOProducer);
DEFINE_FWK_MODULE(BaseMETPOProducer);
DEFINE_FWK_MODULE(BaseMETModifier);
DEFINE_FWK_MODULE(PATMETPOProducer);
DEFINE_FWK_MODULE(TriggerObjectPOProducer);
DEFINE_FWK_MODULE(PFJetHistogramsAnalyzer);
DEFINE_FWK_MODULE(TriggerResultHistogramsAnalyzer);

DEFINE_FWK_MODULE(GenericBaseMETHistograms);
DEFINE_FWK_MODULE(GenericPOHistograms);
DEFINE_FWK_MODULE(GenericPatMETHistograms);
DEFINE_FWK_MODULE(CmgElectronHistograms);
DEFINE_FWK_MODULE(CmgMuonHistograms);
DEFINE_FWK_MODULE(CmgTauHistograms);
DEFINE_FWK_MODULE(CmgHemiHistograms);
DEFINE_FWK_MODULE(CmgPhotonHistograms);
DEFINE_FWK_MODULE(CmgMultiObjectHistograms);
DEFINE_FWK_MODULE(CmgDiMuonHistograms);
DEFINE_FWK_MODULE(CmgDiTauHistograms);
DEFINE_FWK_MODULE(CmgDiElectronHistograms);
DEFINE_FWK_MODULE(CmgDiHemiHistograms);

DEFINE_FWK_MODULE(CmgBaseJetHistograms);
DEFINE_FWK_MODULE(CmgPFJetHistograms);
DEFINE_FWK_MODULE(CmgFatJetHistograms);
DEFINE_FWK_MODULE(CmgDiFatJetHistograms);
DEFINE_FWK_MODULE(CmgDiJetHistograms);
DEFINE_FWK_MODULE(CmgDiPFJetHistograms);

DEFINE_FWK_MODULE(CmgTauEHistograms);
DEFINE_FWK_MODULE(CmgTauMuHistograms);
DEFINE_FWK_MODULE(CmgMuEHistograms);

DEFINE_FWK_MODULE(LeafCandidateHistograms);
DEFINE_FWK_MODULE(GenParticleHistograms);

DEFINE_FWK_MODULE(CmgDiMuonDataSet);
DEFINE_FWK_MODULE(CmgDiTauDataSet);

DEFINE_FWK_MODULE(RunInfoAccountingAnalyzer);
DEFINE_FWK_MODULE(PhysicsObjectPrinter);
DEFINE_FWK_MODULE(PFCandidatePOProducer);
DEFINE_FWK_MODULE(WENuPOProducer);
DEFINE_FWK_MODULE(WMuNuPOProducer);
DEFINE_FWK_MODULE(WTauNuPOProducer);
DEFINE_FWK_MODULE(CutSummaryAnalyzer);

DEFINE_FWK_MODULE(LeadingPatJetSelector);

DEFINE_FWK_MODULE(LeadingCMGPOSelector);
DEFINE_FWK_MODULE(LeadingCMGBaseJetSelector);
DEFINE_FWK_MODULE(LeadingCMGPFJetSelector);
DEFINE_FWK_MODULE(LeadingCMGElectronSelector);
DEFINE_FWK_MODULE(LeadingCMGMuonSelector);

DEFINE_FWK_MODULE(IndexCMGPOSelector);
DEFINE_FWK_MODULE(IndexCMGBaseJetSelector);
DEFINE_FWK_MODULE(IndexCMGPFJetSelector);
DEFINE_FWK_MODULE(IndexCMGElectronSelector);
DEFINE_FWK_MODULE(IndexCMGMuonSelector);

DEFINE_FWK_MODULE(CmgDiPFCandidateSelector);
DEFINE_FWK_MODULE(CmgDiFatJetSelector);
DEFINE_FWK_MODULE(CmgDiPFJetSelector);
DEFINE_FWK_MODULE(CmgDiBaseJetSelector);
DEFINE_FWK_MODULE(CmgPhotonSelector);
DEFINE_FWK_MODULE(CmgPFJetSelector);
DEFINE_FWK_MODULE(CmgBaseJetSelector);
DEFINE_FWK_MODULE(CmgBaseMETSelector);
DEFINE_FWK_MODULE(CmgElectronSelector);
DEFINE_FWK_MODULE(CmgMuonSelector);
DEFINE_FWK_MODULE(CmgTauSelector);
DEFINE_FWK_MODULE(CmgDiMuonSelector);
DEFINE_FWK_MODULE(CmgDiTauSelector);
DEFINE_FWK_MODULE(CmgDiElectronSelector);
DEFINE_FWK_MODULE(CmgWENuSelector);
DEFINE_FWK_MODULE(CmgWMuNuSelector);
DEFINE_FWK_MODULE(CmgTriggerObjectSelector);

DEFINE_FWK_MODULE(CmgTauESelector);
DEFINE_FWK_MODULE(CmgTauMuSelector);
DEFINE_FWK_MODULE(CmgMuESelector);

DEFINE_FWK_MODULE(GenParticleSelector);

DEFINE_FWK_MODULE(VertexSizeProducer);
DEFINE_FWK_MODULE(CandidateSizeProducer);

DEFINE_FWK_MODULE(BaseJetCorrector);
DEFINE_FWK_MODULE(PFJetCorrector);
