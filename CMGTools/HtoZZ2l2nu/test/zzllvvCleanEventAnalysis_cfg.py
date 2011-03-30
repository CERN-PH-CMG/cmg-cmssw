import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nuAN")

#configure the source
import sys
dtag='GluGluToHToZZTo2L2NuM400'
if(len(sys.argv)>2 ): dtag=sys.argv[2]
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor
process.source = cms.Source("PoolSource",
                            fileNames = fillFromCastor('/castor/cern.ch/user/p/psilva/Dileptons/'+dtag+'/')
                            )

#load the analyzer
process.load('CMGTools.HtoZZ2l2nu.CleanEventAnalyzer_cfi')
process.evAnalyzer.dtag=cms.string(dtag)
process.TFileService = cms.Service("TFileService", fileName = cms.string('Histograms.root') )
process.p = cms.Path(process.evAnalyzer)

# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

