import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source (
    "PoolSource",    
    fileNames = cms.untracked.vstring(
        'file:corMETMiniAOD.root'
      ),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
    )

process.METReader = cms.EDAnalyzer("METReader",
      originalSlimmedMetlabel = cms.InputTag("slimmedMETs","","PAT"),
      newCorrectedSlimmedMetLabel = cms.InputTag("slimmedMETs","","RERUN"),

      rootOutputFile = cms.string("met.root"),                             

#that is mostly to see the TXY effect 
      T1TxyMETLabel = cms.InputTag("patPFMetT1Txy"),
)

process.p = cms.Path(process.METReader)



