
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_10_1_j2J.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_11_1_O4d.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_12_1_PEm.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_13_1_sPS.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_14_1_Duk.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_15_1_HHa.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_16_1_0mR.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_17_1_aM1.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_18_1_8fI.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_19_1_h59.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_1_1_rrX.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_20_1_Ttv.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_2_1_msH.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_3_1_VEr.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_4_1_ryH.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_5_1_B8J.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_6_1_YoW.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_7_1_hAp.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_8_1_Oc5.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_9_1_z6a.root',
])
