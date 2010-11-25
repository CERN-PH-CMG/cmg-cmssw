
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_10_1_cwL.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_11_1_zxw.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_13_1_JN6.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_2_1_GGu.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_3_1_KN6.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_5_1_khe.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_7_1_4NV.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/W3Jets_Pt100to300-alpgen/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_8_1_UJy.root',
])
