
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_10_1_QFE.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_11_1_REj.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_12_1_VsT.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_13_1_kHY.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_14_1_Jmk.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_15_1_U4F.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_16_1_qp0.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_17_1_9tt.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_18_1_koS.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_19_1_2xo.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_1_1_rTr.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_20_1_NJQ.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_2_1_23t.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_3_1_J4T.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_4_1_uWv.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_5_1_7fj.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_6_1_G95.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_7_1_8mz.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_8_1_lwj.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_9_1_V5q.root',
])
