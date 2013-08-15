
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_10_1_Rw7.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_11_1_ADO.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_12_1_WW4.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_13_1_f1T.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_14_1_ake.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_15_1_0AD.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_16_1_l7v.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_17_1_QpU.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_18_1_UHk.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_19_1_hre.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_1_1_kHI.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_20_1_vbr.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_21_1_UME.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_22_1_Lqa.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_23_1_NfU.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_2_1_uz4.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_3_1_Vyy.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_4_1_rYO.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_5_1_qZn.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_6_1_IZ2.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_7_1_vxO.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_8_1_EGW.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/LM0_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_9_1_ZfI.root',
])
