
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_10_1_kJy.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_11_1_sr2.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_1_1_Qxl.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_2_1_n8V.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_3_1_b6X.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_4_1_rTt.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_5_1_f8a.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_6_1_WCq.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_7_1_UxP.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_8_1_4G7.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_9_1_7zV.root',
])
