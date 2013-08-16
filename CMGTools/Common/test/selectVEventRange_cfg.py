import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")

pickRelVal = False

import datetime,os
d = datetime.datetime.today()
tag = os.getenv('USER') + '_' + d.strftime('%d%h%y-%Hh%Mm%Ss')

process.source = cms.Source(
        "PoolSource",

        noEventSort = cms.untracked.bool(True),
        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
        fileNames = cms.untracked.vstring()
)
process.source.fileNames.extend([
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_7.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_6.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_5.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_12.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_0.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_1.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_10.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_2.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_11.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_3.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_13.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_8.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_4.root',
                'file:2011/HT/Run2011A-HighMET-PromptSkim-v4/RAW-RECO/PAT_CMG_NewJEC/RA2/susy_tree_CMG_9.root',
])

process.load("CMGTools.Common.sources.HT.Run2011A_HighMET_PromptSkim_v4.RAW_RECO.source_cff")

process.source.eventsToProcess = cms.untracked.VEventRange("166699:354:380002939", "166763:540:604376409", "166894:127:120113417", "165567:449:562584685", "166782:525:565817524")

print process.source

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
#    process.AODSIMEventContent,
    outputCommands =  cms.untracked.vstring( 'keep *'),
    fileName = cms.untracked.string('selectVEventRange_'+tag+'_out.root'),
    )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

outFile = open('selectVEventRange_' + tag + '_source.py','w')
outFile.write('import FWCore.ParameterSet.Config as cms\n')
outFile.write(process.source.dumpPython() )
outFile.close()
