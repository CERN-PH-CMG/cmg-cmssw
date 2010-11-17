
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_10_1_mfx.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_11_1_RA5.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_12_1_Qqs.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_13_1_HRC.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_14_1_WuX.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_15_1_a91.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_16_1_gJF.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_17_1_9NV.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_18_1_oav.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_19_1_6ql.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_1_1_MhR.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_20_1_0BG.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_2_1_7qp.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_3_1_vIe.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_4_1_WvI.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_5_1_wIO.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_6_1_weK.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_7_1_Kx0.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_8_1_ZtQ.root',
		'root://castorcms//castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/susypat_RA2_9_1_Yti.root',
])
