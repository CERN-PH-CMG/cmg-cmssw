
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'file:Samples/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/RECO/SusyPat//susypat_RA2_LM1.root',
])
