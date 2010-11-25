
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_1_1_Sta.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_2_1_Nc9.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_3_1_O32.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_4_1_EIX.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_5_1_vez.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_6_1_Dbg.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_7_1_yyo.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_8_1_OPU.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/Z4Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_9_1_PVe.root',
])
