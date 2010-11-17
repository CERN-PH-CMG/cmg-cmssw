
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_10_1_NlG.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_11_1_wg8.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_12_1_FUj.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_13_1_NYl.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_14_1_FKs.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_15_1_qes.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_16_1_cDn.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_17_1_1wn.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_18_1_yVb.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_19_1_b1r.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_1_1_KJH.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_20_1_f9j.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_2_1_zWy.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_3_1_apP.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_4_1_k1j.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_5_1_03g.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_6_1_Eri.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_7_1_Qrp.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_8_1_5pA.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/LM1_SUSY_sftsht_7TeV-pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/susypat_RA2_9_1_jBu.root',
])
