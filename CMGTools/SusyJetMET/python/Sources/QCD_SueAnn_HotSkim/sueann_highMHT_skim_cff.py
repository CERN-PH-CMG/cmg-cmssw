
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
# no event in the first files
#		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD0to5-pythia6.root',
#		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD5to15-pythia6.root',
#		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD15to30-pythia6.root',
#		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD30to50-pythia6.root',
#		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD50to80-pythia6.root',
#		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD80to120-pythia6.root',
#		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD120to170-pythia6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD170to300-pythia6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD300to470-pythia6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD470to600-pythia6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD600to800-pythia6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD800to1000-pythia6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD1000to1400-pythia6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD1400to1800-pythia6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_SueAnn_HotSkim/hotskim_QCD1800toInf-pythia6.root',
])
