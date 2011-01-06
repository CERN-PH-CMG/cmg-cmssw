
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/MuonAndBEFiltered/susypat_RA2_hotskim_QCD1000to1400.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/MuonAndBEFiltered/susypat_RA2_hotskim_QCD1400to1800.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/MuonAndBEFiltered/susypat_RA2_hotskim_QCD170to300.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/MuonAndBEFiltered/susypat_RA2_hotskim_QCD1800toInf.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/MuonAndBEFiltered/susypat_RA2_hotskim_QCD300to470.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/MuonAndBEFiltered/susypat_RA2_hotskim_QCD470to600.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/MuonAndBEFiltered/susypat_RA2_hotskim_QCD600to800.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/MuonAndBEFiltered/susypat_RA2_hotskim_QCD800to1000.root',
])
