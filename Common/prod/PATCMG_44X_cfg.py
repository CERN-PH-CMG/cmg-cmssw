import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X
if not cmsswIs44X():
    raise ValueError('Sorry, you are not working in 44X. use the correct cfg')

sep_line = '-'*67
print sep_line
print 'CMG PAT-tuplizer, contact Colin before any modification'
print sep_line

process = cms.Process("PAT")


print 'querying database for source files'


runOnMC = True


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
   'cmgtools_group',
   '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5',
   # '/TauPlusX/Run2011A-PromptReco-v4/AOD/V5'
   # 'CMS',
   # '/TauPlusX/Run2011A-03Oct2011-v1/AOD'
   )

## if runOnMC is False:
##     print 'OVERRIDING datasetToSource TO TEST RUNNING ON DATA'
##     process.source.fileNames = ['/store/data/Run2012A/DoubleMu/AOD/PromptReco-v1/000/191/859/66D9EE0B-EC8C-E111-9346-001D09F2AD84.root']
## process.source.fileNames = process.source.fileNames[:1]

print sep_line
print process.source.fileNames
print sep_line 

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

if runOnMC is False:
    # removing MC stuff
    print 'removing MC stuff, as we are running on Data'

    process.patElectrons.addGenMatch = False
    process.makePatElectrons.remove( process.electronMatch )
    
    process.patMuons.addGenMatch = False
    process.makePatMuons.remove( process.muonMatch )
    
    process.PATCMGSequence.remove( process.PATCMGGenSequence )
    process.PATCMGJetSequence.remove( process.jetMCSequence )
    process.PATCMGJetSequence.remove( process.patJetFlavourId )
    process.patJets.addGenJetMatch = False
    process.patJets.addGenPartonMatch = False

    if cmsswIs52X():
        process.PATCMGJetSequenceCHSpruned.remove( process.jetMCSequenceCHSpruned )
        process.patJetsCHSpruned.addGenJetMatch = False
        process.patJetsCHSpruned.addGenPartonMatch = False

    process.PATCMGTauSequence.remove( process.tauGenJets )
    process.PATCMGTauSequence.remove( process.tauGenJetsSelectorAllHadrons )
    process.PATCMGTauSequence.remove( process.tauGenJetMatch )
    process.PATCMGTauSequence.remove( process.tauMatch )
    process.patTaus.addGenJetMatch = False
    process.patTaus.addGenMatch = False

    process.patMETs.addGenMET = False 
    process.patMETsRaw.addGenMET = False 

    # setting up JSON file
    # json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt'
    # print 'using json file: ', json
    # from CMGTools.Common.Tools.applyJSON_cff import *
    # applyJSON(process, json )

    # adding L2L3Residual corrections
    process.patJetCorrFactors.levels.append('L2L3Residual')
    if cmsswIs52X():
        process.patJetCorrFactorsCHSpruned.levels.append('L2L3Residual')


print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.PATCMGJetCHSSequence = cloneProcessingSnippet(process, process.PATCMGJetSequence, 'CHS')
process.PATCMGJetCHSSequence.insert( 0, process.ak5PFJetsCHS )
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.PATCMGJetCHSSequence, 'ak5PFJets', 'ak5PFJetsCHS')
replaceSrc( process.PATCMGJetCHSSequence, 'particleFlow', 'pfNoPileUp')
process.patJetCorrFactorsCHS.payload = 'AK5PFchs'


########################################################
## Path definition
########################################################

process.dump = cms.EDAnalyzer('EventContentAnalyzer')

process.load('CMGTools.Common.PAT.addFilterPaths_cff')
process.p = cms.Path(
    process.prePathCounter + 
    process.PATCMGSequence + 
    process.PATCMGJetCHSSequence 
    )

process.p += process.postPathCounter

# For testing, you can remove some of the objects:
# NOTE: there are a few dependencies between these sequences
# process.PATCMGSequence.remove(process.PATCMGPileUpSubtractionSequence)
# process.PATCMGSequence.remove(process.PATCMGRhoSequence)
# process.PATCMGSequence.remove(process.PATCMGMuonSequence)
# process.PATCMGSequence.remove(process.PATCMGElectronSequence)
# process.PATCMGSequence.remove(process.PATCMGGenSequence)
# process.PATCMGSequence.remove(process.PATCMGJetSequence)
# process.PATCMGSequence.remove(process.PATCMGTauSequence)
# process.PATCMGSequence.remove(process.PATCMGMetSequence)
# process.p.remove(process.PATCMGJetCHSSequence)


########################################################
## PAT output definition
########################################################

## Output Module Configuration (expects a path 'p')
from CMGTools.Common.eventContent.patEventContentCMG_cff import patEventContentCMG
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = patEventContentCMG
                               )
# needed to override the CMG format, which drops the pat taus
process.out.outputCommands.append('keep patTaus_selectedPatTaus_*_*')

#FIXME now keeping the whole event content...
# process.out.outputCommands.append('keep *_*_*_*')

process.outpath = cms.EndPath(process.out)

########################################################
## CMG output definition
########################################################

from CMGTools.Common.eventContent.patEventContentCMG_cff import everything
process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = everything,
    dropMetaData = cms.untracked.string('PRIOR')
    )

process.outpath += process.outcmg


########################################################
## Below, stuff that you probably don't want to modify
########################################################









## Geometry and Detector Conditions (needed for a few patTuple production steps)

from CMGTools.Common.PAT.patCMGSchedule_cff import getSchedule
process.schedule = getSchedule(process, runOnMC)
process.schedule.append( process.outpath )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.load("Configuration.StandardSequences.MagneticField_cff")

print 'Global tag       : ', process.GlobalTag.globaltag

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

# process.MessageLogger.cerr.threshold = 'ERROR'
process.MessageLogger.suppressError = cms.untracked.vstring(
    'cmgTriggerObject',
    'patTrigger',
    'cmgTriggerObjectList'
    )
