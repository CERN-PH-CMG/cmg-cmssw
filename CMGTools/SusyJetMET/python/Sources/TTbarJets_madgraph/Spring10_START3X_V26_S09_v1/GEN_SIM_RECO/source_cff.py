
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_10_1_Pwp.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_11_1_H4Q.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_12_1_IcQ.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_13_1_pMj.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_14_1_F3P.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_15_1_JU4.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_16_1_rAU.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_17_1_uw9.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_18_1_pFR.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_19_1_mtn.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_1_1_Ldf.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_20_1_C3u.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_21_1_RYy.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_2_1_0Tm.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_3_1_90F.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_4_1_tpx.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_5_1_YH5.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_6_1_CWI.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_7_1_AdY.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_8_1_ZEZ.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/TTbarJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_9_1_uKd.root',
])
