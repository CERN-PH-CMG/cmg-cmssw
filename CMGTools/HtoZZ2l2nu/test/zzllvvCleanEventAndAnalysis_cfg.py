import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
dtag, process.source.fileNames, outputFile = configureFromCommandLine(process)
print process.source.fileNames

#correct MET for jet energy scale
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5PFJet
process.metJESCorPFAK5 = metJESCorAK5PFJet.clone()
process.metJESCorPFAK5.inputUncorJetsLabel = "ak5PFJets"
process.metJESCorPFAK5.metType = "PFMET"
process.metJESCorPFAK5.inputUncorMetLabel = "pfMet" #patMETs
process.metJESCorPFAK5.useTypeII = False
process.metJESCorPFAK5.hasMuonsCorr = cms.bool(True)
process.metJESCorPFAK5.jetPTthreshold = cms.double(10.0)
process.metJESCorPFAK5.corrector = cms.string('ak5PFL1FastL2L3')
process.typeIMetSequence=cms.Sequence(process.metJESCorPFAK5)

process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.jec = cms.ESSource("PoolDBESSource",
                           DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(0)
    ),
                           timetype = cms.string('runnumber'),
                           toGet = cms.VPSet(
    cms.PSet(
    record = cms.string('JetCorrectionsRecord'),
    tag    = cms.string('JetCorrectorParametersCollection_Jec10V3_AK5PF'),
    label  = cms.untracked.string('AK5PF')
    )
    ),
                           ## here you add as many jet types as you need (AK5Calo, AK5JPT, AK7PF, AK7Calo, KT4PF, KT4Calo, KT6PF, KT6Calo)
                           connect = cms.string('sqlite_file:/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_1_3_patch2/src/JEC/Jec10V3.db')
                           )
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')


process.load('CMGTools.HtoZZ2l2nu.CleanEventProducer_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventFilter_cfi')
process.load('CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventAnalyzer_cfi')
process.evAnalyzer.dtag=cms.string(dtag)
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )
process.p = cms.Path(process.typeIMetSequence*process.puWeights*process.cleanEvent*process.cleanEventFilter*process.evAnalyzer)
#process.p = cms.Path(process.cleanEvent*process.evAnalyzer)


# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

