import FWCore.ParameterSet.Config as cms


#from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *
import copy, os, fnmatch, sys, copy
import CMGTools.Production.eostools  as eostools 


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

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
    ###getListOfFiles('{baseDir}/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/{filePattern}',baseDir="/store/cmst3/user/psilva/Data4/G", filePattern="*.root")[:10]
    getListOfFiles('{baseDir}/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12-PU_S7_START52_V5-v2/{filePattern}',baseDir="/store/cmst3/user/musella/PATtuples/19_04_2012", filePattern="*.root")[:10]
    
    ## 'file:/tmp/pharris/test.root'
       ),
    skipEvents = cms.untracked.uint32(0)                        
)


# muon fileter
#process.load('PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi')

#process.MuonsFilter = countPatMuons.clone(
#    src       = cms.InputTag("selectedPatMuonsPFlow"),
#    minNumber = cms.uint32(0)
#)

from CMG.JetIDAnalysis.jetanalyzer_cfi import *

process.pfjetanalyzer = jetanalyzer.clone(
    JetTag      = cms.InputTag("selectedPatJetsPFlowNoPuSub",""),
    GenJetTag   = cms.InputTag("selectedPatJetsPFlowNoPuSub","genJets"),
    dataFlag = cms.untracked.bool(False),
    MvaTags = cms.untracked.VInputTag(cms.InputTag("puJetMva","simpleDiscriminant"),
                                      cms.InputTag("puJetMva","fullDiscriminant"),
                                      cms.InputTag("puJetMva","cutbasedDiscriminant"),
                                      ),
    IdTags = cms.untracked.VInputTag( cms.InputTag("puJetMva","simpleId"),
                                      cms.InputTag("puJetMva","fullId"),
                                      cms.InputTag("puJetMva","cutbasedId"),
                                      ),
)

process.chspfjetanalyzer = jetanalyzer.clone(
    JetTag      = cms.InputTag("selectedPatJetsPFlow",""),
    GenJetTag   = cms.InputTag("selectedPatJetsPFlow","genJets"),
    dataFlag = cms.untracked.bool(False)
)

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("mytree.root"),
                                   closeFileFast = cms.untracked.bool(True)
)

process.load("CMGTools.External.pujetidsequence_cff")
process.puJetId.jets = "selectedPatJetsPFlowNoPuSub"
process.puJetMva.jets = "selectedPatJetsPFlowNoPuSub"

process.ana = cms.Sequence(process.pfjetanalyzer+process.chspfjetanalyzer)
process.p = cms.Path(process.puJetIdSqeuence*process.ana)

### process.out = cms.OutputModule("PoolOutputModule",
###                                fileName = cms.untracked.string('patTuple.root'),
###                                SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
###                                outputCommands = cms.untracked.vstring('keep *'))
### process.e = cms.EndPath( process.out )
