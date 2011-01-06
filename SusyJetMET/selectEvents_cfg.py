import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")

process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff")

print process.source.fileNames

# selectForDisplay = 'DeltaPt'
selectForDisplay = 'METCor1_200'

if selectForDisplay!=None:
    from CMGTools.SusyJetMET.selectEvents_cff import selectEvents as addVEventRange
    addVEventRange( selectForDisplay, process.source )



process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff")

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule("PoolOutputModule",
                               process.RECOSIMEventContent,
                               # outputCommands =  cms.untracked.vstring('keep *'),
                               fileName = cms.untracked.string('selected_%s.root' % selectForDisplay),
                               # fileName = cms.untracked.string('AODSIM.root'),
                               )


#from PFAnalyses.TTBar.topDecayFilters_cff import topFilterEventContent
#process.out.outputCommands.extend( topFilterEventContent)



process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
