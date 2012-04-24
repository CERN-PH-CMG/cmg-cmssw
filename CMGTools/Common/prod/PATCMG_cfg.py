import FWCore.ParameterSet.Config as cms

sep_line = '-'*67
print sep_line
print 'CMG PAT-tuplizer, contact Colin before any modification'
print sep_line

process = cms.Process("PAT")


print 'querying database for source files'


runOnMC = False


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
   # 'CMS',
   # '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V5-v2/AODSIM'
   'CMS',
   '/DoubleMu/Run2012A-PromptReco-v1/AOD'
   )

if runOnMC is False:
    print 'OVERRIDING datasetToSource TO TEST RUNNING ON DATA'
    process.source.fileNames = ['/store/data/Run2012A/DoubleMu/AOD/PromptReco-v1/000/191/859/66D9EE0B-EC8C-E111-9346-001D09F2AD84.root']

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

if runOnMC is False:
    print 'removing MC stuff, as we are running on Data'

    process.patElectrons.addGenMatch = False
    process.makePatElectrons.remove( process.electronMatch )
    
    process.patMuons.addGenMatch = False
    process.makePatMuons.remove( process.muonMatch )
    
    process.PATCMGSequence.remove( process.genJetSequence )
    process.jetSequence.remove( process.jetMCSequence )
    process.jetSequence.remove( process.patJetFlavourId )
    process.patJets.addGenJetMatch = False
    process.patJets.addGenPartonMatch = False

    process.tauSequence.remove( process.tauGenJets )
    process.tauSequence.remove( process.tauGenJetsSelectorAllHadrons )
    process.tauSequence.remove( process.tauGenJetMatch )
    process.tauSequence.remove( process.tauMatch )
    process.patTaus.addGenJetMatch = False
    process.patTaus.addGenMatch = False

    process.patMETs.addGenMET = False 
    
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt'
    print 'using json file: ', json
    from CMGTools.Common.Tools.applyJSON_cff import *
    applyJSON(process, json )


print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.jetCHSSequence = cloneProcessingSnippet(process, process.jetSequence, 'CHS')
process.jetCHSSequence.insert( 0, process.ak5PFJetsCHS )
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.jetCHSSequence, 'ak5PFJets', 'ak5PFJetsCHS')
replaceSrc( process.jetCHSSequence, 'particleFlow', 'pfNoPileUpIso')


process.p = cms.Path(
    process.PATCMGSequence + 
    process.jetCHSSequence
    )


## Output Module Configuration (expects a path 'p')
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
                               )

process.outpath = cms.EndPath(process.out)



########################################################
## Below, stuff that you probably don't want to modify
########################################################









## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.load("Configuration.StandardSequences.MagneticField_cff")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

print sep_line

print 'Fastjet instances (dominating our processing time...):'
from CMGTools.Common.Tools.visitorUtils import SeqVisitor
v = SeqVisitor('FastjetJetProducer')
process.p.visit(v)

print sep_line

print 'starting CMSSW'
