
import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_0.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_1.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_10.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_11.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_12.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_13.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_2.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_3.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_4.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_5.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_6.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_7.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_8.root',
		'/store/cmst3/user/cbern/CMG/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_9.root',
])
