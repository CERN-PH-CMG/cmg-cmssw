import FWCore.ParameterSet.Config as cms

import copy, os, fnmatch, sys, copy
import CMGTools.Production.eostools  as eostools 


def getListOfFiles(expr, baseDir, filePattern):
    if baseDir.startswith("/store"):
        return [ "root://eoscms/%s" % f for f in eostools.listFiles( expr.format( baseDir=baseDir, filePattern="" ) ) if fnmatch.fnmatch(f,filePattern) ]
    else:
        return expr.format( baseDir=baseDir, filePattern=filePattern ) 

process = cms.Process("MakeJetIDPlots")


#baseDir = '/store/cmst3/user/musella/JETID_NTUPLES/30_03_2012/'
baseDir = '/store/cmst3/user/malberti/JETID_NTUPLES/10_04_2012/'
filePattern = '*.root'

process.Input = cms.PSet(
    inputFile = cms.vstring(
    #"root://eoscms//eos/cms/store/cmst3/user/malberti/JETID_NTUPLES/26_03_2012/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/JetAnalysisTree_merged.root"),
    getListOfFiles('{baseDir}/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/{filePattern}',baseDir=baseDir, filePattern=filePattern)[:10]),
    inputTree = cms.string("chspfjetanalyzer"),
)

process.Output = cms.PSet(
    outputRootFilePath = cms.string("../test/scripts_martina/rootfiles/"),
    #outputRootFilePath = cms.string("./"),
    outputRootFileName = cms.string("histos_DYJetsToLL_chspfjets.root")
)

process.Options = cms.PSet(
    entryMIN = cms.int32(0),
    entryMAX = cms.int32(-1),
    dataFlag = cms.int32(0),
    mcPuFile    = cms.string("/afs/cern.ch/user/m/malberti/public/PU_Fall11.root"),
    dataPuFile  = cms.string("/afs/cern.ch/user/m/malberti/public/PU_data2011.root"),
    mcPuHisto   = cms.string("pileup"),
    dataPuHisto = cms.string("pileup"),
    minJetPt   = cms.double(10.),
    maxJetPt   = cms.double(500.),
    minJetEta  = cms.double(0.0),
    maxJetEta  = cms.double(5),

    impactParTkThreshold = cms.double(0.) ,
    tmvaWeights = cms.untracked.string("CMGTools/External/data/mva_JetID.weights.xml"),
    tmvaMethod  = cms.untracked.string("JetID"),                 

)
