
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_10_2_ogn.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_11_2_4t9.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_12_2_txT.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_13_2_1i6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_14_2_gsY.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_15_2_0WU.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_16_2_afG.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_17_2_nOS.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_18_2_8wi.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_19_2_Xg9.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_1_2_gzZ.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_20_2_ToU.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_2_2_hWm.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_3_2_1Vi.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_4_2_9vu.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_5_2_UUu.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_6_2_cdq.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_7_2_mJg.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_8_2_H0d.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_9_2_syA.root',
])
