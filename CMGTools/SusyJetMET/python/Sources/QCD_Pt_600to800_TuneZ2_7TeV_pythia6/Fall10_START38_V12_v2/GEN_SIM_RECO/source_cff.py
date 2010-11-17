
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_10_1_FLf.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_11_1_nVs.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_12_1_Qc6.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_13_1_uAZ.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_14_1_p72.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_15_1_m6J.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_16_1_m92.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_17_1_7WT.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_18_1_1Js.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_19_1_Kxw.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_1_1_xHi.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_20_1_DbE.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_2_1_O5g.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_3_1_ZbR.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_4_1_suA.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_5_1_eyB.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_6_1_tVY.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_7_1_W1D.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_8_1_5ju.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v2/GEN-SIM-RECO/susypat_RA2_9_1_Kkk.root',
])
