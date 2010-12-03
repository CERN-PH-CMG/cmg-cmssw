
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_170to300.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_1800toInf.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_0to5.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_600to800.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_5to15.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_300to470.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_470to600.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_80to120.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_120to170.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_800to1000.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_15to30.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_50to80.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_30to50.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_1000to1400.root',
		'file:/afs/cern.ch/user/c/cbern/localscratch/Analysis/Susy/SusyJetMET/Samples/QCD_SueAnn_HotSkim/SusyPat/susypat_RA2_SueAnnHot_1400to1800.root',
])
