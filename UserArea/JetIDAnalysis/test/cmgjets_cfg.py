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

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    getListOfFiles('{baseDir}/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12-PU_S7_START52_V5-v2/{filePattern}',baseDir="/store/cmst3/user/musella/PATtuples/19_04_2012", filePattern="*.root")[:10]
       ),
    skipEvents = cms.untracked.uint32(0)                        
)

from CMGTools.External.pujetidsequence_cff import loadPujetId
puJetIdSequence,inputsTag,mvaTags,idTags,puJetIdoutputCommands = loadPujetId(process,"selectedPatJetsPFlowNoPuSub",mvaOnly=False,isChs=False,release="52X")

process.load("CMGTools.Common.factories.cmgPFJet_cfi")
process.cmgPFJet.cfg.inputCollection = cms.InputTag("selectedPatJetsPFlowNoPuSub","")
process.cmgPFJet.cfg.puVariables = inputsTag
process.cmgPFJet.cfg.puMvas = cms.VInputTag(v for n,v in mvaTags.iteritems() )
process.cmgPFJet.cfg.puIds  = cms.VInputTag(v for n,v in idTags.iteritems() )

process.cmgPFJetHistosIds = cms.EDAnalyzer(
    'CandViewHistoAnalyzer',
    src = cms.InputTag('cmgPFJet'),
    histograms = cms.VPSet(
    cms.PSet(min          = cms.untracked.double(     0),
             max          = cms.untracked.double(     15),
             nbins        = cms.untracked.int32 (     15),
             name         = cms.untracked.string(v.getProductInstanceLabel().replace("Id","")),
             description  = cms.untracked.string(     ''),
             plotquantity = cms.untracked.string(   "puId('%s')" % v.getProductInstanceLabel().replace("Id","")),
             lazyParsing  = cms.untracked.bool(True)
             )
    for n,v in idTags.iteritems()
    )
)

process.cmgPFJetHistosMvas = cms.EDAnalyzer(
    'CandViewHistoAnalyzer',
    src = cms.InputTag('cmgPFJet'),
    histograms = cms.VPSet(
    cms.PSet(min          = cms.untracked.double(     -1),
             max          = cms.untracked.double(     1),
             nbins        = cms.untracked.int32 (     100),
             name         = cms.untracked.string(v.getProductInstanceLabel().replace("Discriminant","")),
             description  = cms.untracked.string(     ''),
             plotquantity = cms.untracked.string(   "puMva('%s')" % v.getProductInstanceLabel().replace("Discriminant","")),
             lazyParsing  = cms.untracked.bool(True)
             )
    for n,v in mvaTags.iteritems()
    )
)

process.cmgPFJetSeq = cms.Sequence(puJetIdSequence*process.cmgPFJet*(process.cmgPFJetHistosMvas+process.cmgPFJetHistosIds))

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("mytree.root"),
                                   closeFileFast = cms.untracked.bool(True)
)

process.p = cms.Path(process.cmgPFJetSeq)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               outputCommands = cms.untracked.vstring('drop *'))
process.out.outputCommands.extend([ "keep *_selectedPatJetsPFlowNoPuSub_*_*", "keep *_cmgPFJet_*_*" ] )
process.out.outputCommands.extend(puJetIdoutputCommands)

process.e = cms.EndPath( process.out )
