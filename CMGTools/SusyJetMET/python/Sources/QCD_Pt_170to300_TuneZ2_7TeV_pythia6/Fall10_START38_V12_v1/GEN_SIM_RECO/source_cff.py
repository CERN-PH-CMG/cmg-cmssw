
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_10_2_8ck.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_11_2_ykh.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_12_2_DrY.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_13_2_FR6.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_14_2_aKp.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_15_2_1Y7.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_16_2_12r.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_17_2_NHF.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_18_2_w0R.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_19_2_B6B.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_1_2_nmp.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_20_2_VOk.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_21_2_jSI.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_22_2_V3M.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_2_2_v26.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_3_2_E8b.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_4_2_1WJ.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_5_2_dNI.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_6_2_VSH.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_7_2_560.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_8_2_pQx.root',
		'/store/cmst3/user/cbern/RA2SusyJetMET/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_9_2_w9L.root',
])
