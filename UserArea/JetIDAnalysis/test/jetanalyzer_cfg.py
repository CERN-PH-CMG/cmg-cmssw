import FWCore.ParameterSet.Config as cms


#from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *
import copy, os, fnmatch, sys, copy
import CMGTools.Production.eostools  as eostools 
from CMGTools.External.jec_2012_cff import use2012JecPreview

def getListOfFiles(expr, baseDir, filePattern):
    if baseDir.startswith("/store"):
        return [ "root://eoscms/%s" % f for f in eostools.listFiles( expr.format( baseDir=baseDir, filePattern="" ) ) if fnmatch.fnmatch(f,filePattern) ]
    else:
        return expr.format( baseDir=baseDir, filePattern=filePattern ) 

process = cms.Process("analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

baseDir = '/store/cmst3/user/psilva/Data4/G'
filePattern = '*.root'

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
runOnMC=True
from Configuration.AlCa.autoCond import autoCond
if(runOnMC) : process.GlobalTag.globaltag=cms.string(autoCond.get('startup',autoCond['mc']))
else        : process.GlobalTag.globaltag=cms.string(autoCond['com10'])
use2012JecPreview(process)
                            
process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
    getListOfFiles('{baseDir}/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12-PU_S7_START50_V15-v1_AODSIM/{filePattern}',baseDir="/store/cmst3/user/musella/PATtuples/19_04_2012/", filePattern="*.root")[:10]
       ),
    skipEvents = cms.untracked.uint32(0)                        
)


# muon fileter
process.load('PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi')
process.MuonsFilter = process.countPatMuons.clone(
    src       = cms.InputTag("selectedPatMuonsPFlow"),
    minNumber = cms.uint32(0)
)

from CMG.JetIDAnalysis.jetanalyzer_cfi import *

process.pfjetanalyzer = jetanalyzer.clone(
    JetTag      = cms.InputTag("selectedPatJetsPFlowNoPuSub",""),
    GenJetTag   = cms.InputTag("selectedPatJetsPFlowNoPuSub","genJets"),
    dataFlag = cms.untracked.bool(False),
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
    applyJec = cms.bool(True)
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("myTree.root"),
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

