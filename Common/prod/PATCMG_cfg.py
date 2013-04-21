import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import isNewerThan

sep_line = '-'*67
print sep_line
print 'CMG PAT-tuplizer, contact Colin before any modification'
print sep_line

process = cms.Process("PAT")


print 'querying database for source files'


runOnMC      = True
runOnFastSim = False

from CMGTools.Production.datasetToSource import *
## This is used to get the correct global tag below, and to find the files
## It is *reset* automatically by ProductionTasks, so you can use it after the ProductionTasksHook
datasetInfo = (
    'cmgtools_group',
    '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B',
    '.*root')
process.source = datasetToSource(
    *datasetInfo
    )

process.source.fileNames = process.source.fileNames[:20]
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

###ProductionTaskHook$$$


print sep_line
print process.source.fileNames
print sep_line 

## Maximal Number of Events


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

    #if isNewerThan('CMSSW_5_2_0'):
    #    process.PATCMGJetSequenceCHSpruned.remove( process.jetMCSequenceCHSpruned )
    #    process.patJetsCHSpruned.addGenJetMatch = False
    #    process.patJetsCHSpruned.addGenPartonMatch = False

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
    #if isNewerThan('CMSSW_5_2_0'):
    #    process.patJetCorrFactorsCHSpruned.levels.append('L2L3Residual')


#Jose: fix to muon isolation in case the pf muon is not within the 1e-5 veto cone w.r.t reco muon (Muon expert should fix somewhere else)
process.muPFIsoDepositChargedAll.ExtractorPSet.DR_Veto = 1e-3

print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.PATCMGJetCHSSequence = cloneProcessingSnippet(process, process.PATCMGJetSequence, 'CHS')
process.PATCMGJetCHSSequence.insert( 0, process.ak5PFJetsCHS )
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.PATCMGJetCHSSequence, 'ak5PFJets', 'ak5PFJetsCHS')
replaceSrc( process.PATCMGJetCHSSequence, 'particleFlow', 'pfNoPileUp')
process.PATCMGJetCHSSequence.remove(process.outTracksCHS) # not needed for CHS jets
process.PATCMGJetCHSSequence.remove(process.ak5SoftTrackJetsForVbfHbbCHS) # not needed for CHS jets
jecPayload = 'AK5PFchs'
process.patJetsWithVarCHS.payload = jecPayload
process.patJetCorrFactorsCHS.payload = jecPayload
process.puJetIdCHS.jec = jecPayload
process.cmgPUJetMvaCHS.jec = jecPayload
process.selectedPatJetsCHS.cut = 'pt()>10'


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
# process.p.remove(process.PATCMGTriggerSequence)
# process.p.remove(process.PATCMGPhotonSequence)
# process.p.remove(process.PATCMGVertexSequence)
# process.p.remove(process.PATCMGPhotonSequence)
# process.p.remove(process.MetSignificanceSequence)
# process.p.remove(process.PATCMGMetRegressionSequence)
# process.p.remove(process.PATCMGJetSequenceCHSpruned)

if runOnFastSim :
    process.vertexWeightSequence.remove(process.vertexWeight3DMay10ReReco)
    process.vertexWeightSequence.remove(process.vertexWeight3DMay10ReReco)
    process.vertexWeightSequence.remove(process.vertexWeight3DPromptRecov4)
    process.vertexWeightSequence.remove(process.vertexWeight3D05AugReReco)
    process.vertexWeightSequence.remove(process.vertexWeight3DPromptRecov6)
    process.vertexWeightSequence.remove(process.vertexWeight3D2invfb)
    process.vertexWeightSequence.remove(process.vertexWeight3D2011B)
    process.vertexWeightSequence.remove(process.vertexWeight3D2011AB)
    process.vertexWeightSequence.remove(process.vertexWeight3DFall11May10ReReco)
    process.vertexWeightSequence.remove(process.vertexWeight3DFall11PromptRecov4)
    process.vertexWeightSequence.remove(process.vertexWeight3DFall1105AugReReco)
    process.vertexWeightSequence.remove(process.vertexWeight3DFall11PromptRecov6)
    process.vertexWeightSequence.remove(process.vertexWeight3DFall112invfb)
    process.vertexWeightSequence.remove(process.vertexWeight3DFall112011B)
    process.vertexWeightSequence.remove(process.vertexWeight3DFall112011AB)


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

process.outpath = cms.EndPath(
    # process.out
    )

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
## Conditions 
########################################################

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

########################################################
## Below, stuff that you probably don't want to modify
########################################################



## Geometry and Detector Conditions (needed for a few patTuple production steps)

from CMGTools.Common.PAT.patCMGSchedule_cff import getSchedule
process.schedule = getSchedule(process, runOnMC, runOnFastSim)
process.schedule.append( process.outpath )

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.suppressWarning = cms.untracked.vstring('ecalLaserCorrFilter')
## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )


if not runOnMC and isNewerThan('CMSSW_5_2_0'):
    process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")


## Last minute fixes
# process.PATJetSequenceCHS.remove(process.outTracksCHS)
# process.PATJetSequenceCHS.remove(process.ak5SoftTrackJetsForVbfHbbCHS)


print sep_line

print 'Fastjet instances (dominating our processing time...):'
from CMGTools.Common.Tools.visitorUtils import SeqVisitor
v = SeqVisitor('FastjetJetProducer')
process.p.visit(v)

### Set the global tag from the dataset name
from CMGTools.Common.Tools.getGlobalTag import getGlobalTagByDataset
process.GlobalTag.globaltag = getGlobalTagByDataset( runOnMC, datasetInfo[1])
print 'Global tag       : ', process.GlobalTag.globaltag
###

print sep_line

print 'starting CMSSW'

