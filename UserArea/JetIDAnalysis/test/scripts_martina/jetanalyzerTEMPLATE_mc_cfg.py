import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *

process = cms.Process("analysis")

runOnMC=True

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from CMGTools.External.jec_2012_cff import use2012JecPreview
from Configuration.AlCa.autoCond import autoCond
if(runOnMC) : process.GlobalTag.globaltag=cms.string(autoCond.get('startup',autoCond['mc']))
else        : process.GlobalTag.globaltag=cms.string(autoCond['com10'])
use2012JecPreview(process)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(LISTOFFILES),
    skipEvents = cms.untracked.uint32(0)                        
)


# muon filter
process.load('PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi')

process.MuonsFilter = countPatMuons.clone(
    src       = cms.InputTag("selectedPatMuonsPFlow"),
    minNumber = cms.uint32(2)
)

#jet analyzer 
from CMG.JetIDAnalysis.jetanalyzer_cfi import *
process.load("CMGTools.External.pujetidsequence_cff")

process.pfjetanalyzer = jetanalyzer.clone(
    JetTag      = cms.InputTag("selectedPatJetsPFlowNoPuSub",""),
    GenJetTag   = cms.InputTag("selectedPatJetsPFlowNoPuSub","genJets"),
    dataFlag = cms.untracked.bool(False),
    applyJec = cms.bool(True)
)

process.chspfjetanalyzer = jetanalyzer.clone(
    JetTag      = cms.InputTag("selectedPatJetsPFlow",""),            
    GenJetTag   = cms.InputTag("selectedPatJetsPFlow","genJets"), 
    dataFlag = cms.untracked.bool(False),
    MvaTags = cms.untracked.VInputTag(cms.InputTag("puJetMvaChs","simpleDiscriminant"),
                                      cms.InputTag("puJetMvaChs","fullDiscriminant"),
                                      cms.InputTag("puJetMvaChs","cutbasedDiscriminant"),
                                      ),
    IdTags = cms.untracked.VInputTag( cms.InputTag("puJetMvaChs","simpleId"),
                                      cms.InputTag("puJetMvaChs","fullId"),
                                      cms.InputTag("puJetMvaChs","cutbasedId"),
                                      ),
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("OUTPUTFILENAME"),
    closeFileFast = cms.untracked.bool(False)
)


## jet id sequence
process.load("CMGTools.External.pujetidsequence_cff")
process.puJetId.jets = "selectedPatJetsPFlowNoPuSub"
process.puJetMva.jets = "selectedPatJetsPFlowNoPuSub"
process.puJetId.applyJec = True
process.puJetMva.applyJec = True

process.puJetIdChs.jets = "selectedPatJetsPFlow"
process.puJetMvaChs.jets = "selectedPatJetsPFlow"

process.ana = cms.Sequence(process.pfjetanalyzer+process.chspfjetanalyzer)
process.p = cms.Path(process.MuonsFilter*process.puJetIdSqeuence*process.puJetIdSqeuenceChs*process.ana)
