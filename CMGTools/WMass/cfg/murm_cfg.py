import FWCore.ParameterSet.Config as cms
import pprint

from CMGTools.Common.Tools.cmsswRelease import isNewerThan, cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("MURM")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = -1
reportInterval = 100

##########



dataset_files = 'cmgTuple.*root'
dataset_user = 'cbern'
dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START44_V9B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B/DIMU_Colin24Sep'

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

process.source.inputCommands=cms.untracked.vstring(
    'keep *',
    'drop cmgStructuredPFJets_cmgStructuredPFJetSel__PAT'
    )


# restricting the number of files to process to a given number
if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]


###ProductionTaskHook$$$
    



# muon removal sequence
# ----

# need to clean that up a bit...
# mu faking tau small, but we could think of removing tau candidates corresponding to the removed mu

process.cmgMuonSel = cms.EDFilter(
    "MuonRemover",
    leptonSrc = cms.InputTag('cmgMuonSel'),
    verbose = cms.untracked.bool( False )
    )

# need to remove the leading jet corresponding to the removed muon

process.cmgPFJetSel =  cms.EDProducer(
    "DeltaRVetoProducerPFJet",
    inputCollection = cms.InputTag('cmgPFJetSel'),
    MatchingParams = cms.VPSet(
    cms.PSet(                                     
       vetoCollection=cms.InputTag("cmgMuonSel:removed"),
       minDeltaR=cms.double(0.3),
       removeMatchedObject=cms.bool(True)
       )
    ),
    verbose = cms.untracked.bool(False)
    )

process.cmgTauSel =  cms.EDProducer(
    "DeltaRVetoProducerTau",
    inputCollection = cms.InputTag('cmgTauSel'),
    MatchingParams = cms.VPSet(
    cms.PSet(                                     
       vetoCollection=cms.InputTag("cmgMuonSel:removed"),
       minDeltaR=cms.double(0.3),
       removeMatchedObject=cms.bool(True)
       )
    ),
    verbose = cms.untracked.bool(False)
    )

process.nopuMet = cms.EDProducer(
     'PFMetModificationProducer',
     candSrc = cms.InputTag('cmgMuonSel:removed'),
     metSrc = cms.InputTag('nopuMet'),
     operator = cms.string('-')
     )

# this is just the PFMET
process.pfMetForRegression = process.nopuMet.clone( metSrc = 'pfMetForRegression' )

process.pfMetModificationSequence = cms.Sequence(
    process.nopuMet+
    process.pfMetForRegression
    )

process.muRmSequence = cms.Sequence(
    process.cmgMuonSel +
    process.cmgPFJetSel +
    process.cmgTauSel +   
    process.pfMetModificationSequence    
    ) 

process.p = cms.Path(
    process.muRmSequence
    )


# OUTPUT definition ----------------------------------------------------------

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = cms.untracked.vstring( 'keep *' )
    )

process.outpath = cms.EndPath(process.out)

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = reportInterval
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

