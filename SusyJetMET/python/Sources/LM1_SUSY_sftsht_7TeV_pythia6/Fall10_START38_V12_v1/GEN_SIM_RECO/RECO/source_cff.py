
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/RECO/RECO_1_1_LyX.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/RECO/RECO_2_1_2gl.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/RECO/RECO_3_1_4on.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/RECO/RECO_4_1_5S0.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/RECO/RECO_5_1_VpK.root',
])
