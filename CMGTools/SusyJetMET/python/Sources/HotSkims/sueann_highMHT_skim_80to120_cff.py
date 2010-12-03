
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD80to120-pythia6.root',
])
