from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint,sys, copy

process.out.SelectEvents.SelectEvents = cms.vstring()


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )
ext = 'RA2'


## Main control flags ################################################

doPATTuple = True       # is that still necessary?
selectEvents = True     # write out in SUSYPAT format only events passing the pf path
rejectedOutput = True   # store events rejected by the various filters in separate RECO files

MHTCut = 300             # skimming cut on MHT


sourceExt = 'hotskim_QCD'
# sourceExt = 'LM1'
# sourceExt = 'hotskim_Data'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

#######################################################################

print 'processing:'

process.setName_('PAT2')
runningOnMC = True
Spring10 = False
HLT = 'REDIGI38X' #Fall10
if sourceExt.find('hotskim_QCD')!=-1:
    process.load("CMGTools.SusyJetMET.Sources.QCD_SueAnn_HotSkim.sueann_highMHT_skim_cff")
    from CMGTools.SusyJetMET.Sources.QCD_SueAnn_HotSkim.hotSkim_cff import hotSkim
    hotSkim( process.source, sourceExt )

elif sourceExt == 'LM1':
    process.load("CMGTools.SusyJetMET.Sources.LM1_SUSY_sftsht_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.RECO.source_cff")
elif sourceExt == 'hotskim_Data':
    process.load("CMGTools.SusyJetMET.Sources.Data.HotSkim.source_cff")
    runningOnMC = False
    HLT = 'HLT'
else:
    print 'no such source definition: ', sourceExt
    sys.exit(1)

# process.source.eventsToProcess = cms.untracked.VEventRange( '1:243087' )

print process.source.fileNames


outFileNameExt = ext + '_' + sourceExt

# reinitializing stuff defined in patTemplate_cfg

process.load("CMGTools.Common.countingSequences_cff")

# depending on what we want to do, the various sequences will be added to this path.

    
from CMGTools.SusyJetMET.RA2OfficialSequence_cff import addSUSYPATRA2
# for Fall10 samples 
# addSUSYPATRA2( process, 'REDIGI38X')


addSUSYPATRA2( process, HLT, runningOnMC)

if Spring10:
    process.patJets.addTagInfos     = False
    process.patJets.addBTagInfo = False

# process.pfseq.remove( process.badPFMuonFilter ) 


 
process.pcalo = cms.Path(
    process.caloseq
    )

# loading filters
process.load("RecoParticleFlow.PostProcessing.METCorrelationFilter_cfi")
process.load("CMGTools.SusyJetMET.eventCleaningFilters_cff")
process.load("SandBox.Skims.RA2DeltaR_cff")
process.load("SandBox.Skims.leptonVetoFilter_cfi")


## Definition of the main sequence and path ################################################

# adding counters and filters to the particle flow path

process.mhtPFFilter.MinMHT = MHTCut

process.mainSequence = cms.Sequence(
    process.pfseq +
    process.mhtPFFilter +
    process.jetMHTPFDPhiFilter +
    process.leptonVetoPOG 
    )

process.p = cms.Path(
    process.startupSequence +
    # process.METCorrelationFilter +
    process.mainSequence +
    #WARNING
    # process.ra2PFCleaningIgnore + 
    process.ra2PFCleaning + 
    #process.selectGoodPFEventsSequence +
    # process.eventCleaningFilters +
    process.finalSequence  
    )


## Storing rejected events in separate files ###############################################

process.load("Configuration.EventContent.EventContent_cff")

## Store delta pT muons in RECO format

process.pDeltaPtMuons = cms.Path(
    process.mainSequence + 
    process.largeDeltaPtMuons +
    process.filterLargeDeltaPtMuons 
    )

process.outDeltaPtMuons = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_DeltaPtMuons.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pDeltaPtMuons') ),
    )

## Store greedy muons in RECO format

process.pGreedyMuons = cms.Path(
    process.mainSequence + 
    process.greedyMuons 
    )

process.outGreedyMuons = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_greedyMuons.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pGreedyMuons') ),
    )


## inconsistent muons 

process.pInconsistentMuons = cms.Path(
    process.mainSequence + 
    process.inconsistentMuons
    )

process.outInconsistentMuons = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_inconsistentMuons.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pInconsistentMuons') ),
    )


## PF PostProcessing 

process.pPfEventFilter = cms.Path(
    process.mainSequence +
    ~process.pfEventFilter
    )

process.outPfEventFilter = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_pfEventFilter.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pPfEventFilter') ),
    )


## TP filter.

# The filter itself only runs on data. For the QCD MC there is a
# hard-coded list of events that would have been rejected by the TP
# filter (obtained by using the raw events files).

process.pTPFilter = cms.Path()
if not runningOnMC:
    process.pTPFilter = cms.Path(
        process.mainSequence +
        ~process.ecalDeadCellTPonlyFilter
        )
elif sourceExt == "hotskim_QCD":
    process.pTPFilter = cms.Path(
        process.mainSequence +
        ~process.ecalDeadCellTPonlyFilterPythia6
        )

process.outTPFilter = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_TPFilter.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('pTPFilter')),
    )


## BE filter.

process.pBEFilter = cms.Path(
    process.mainSequence +
    process.ecalAnomalousFilter +
    ~process.ecalDeadCellBEonlyFilter
    )

process.outBEFilter = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_BEFilter.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pBEFilter') ),
    )


## HCAL noise

process.pHCALNoiseFilter = cms.Path(
    process.mainSequence + 
    process.vetoHCALNoiseEv
    )

process.outHCALNoiseFilter = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_HCALNoiseFilter.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pHCALNoiseFilter') ),
    )


## Beam halo

process.pBeamHaloFilter = cms.Path(
    process.mainSequence + 
    ~process.beamHaloFilter
    )

process.outBeamHaloFilter = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_BeamHaloFilter.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pBeamHaloFilter') ),
    )


## RingOfFire

process.pRingOfFireFilter = cms.Path(
    process.mainSequence + 
    ~process.eeNoiseFilter
    )

process.outRingOfFireFilter = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_RingOfFireFilter.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pRingOfFireFilter') ),
    )



## Tracking Failure

process.pTrackingFailureFilter = cms.Path(
    process.mainSequence + 
    ~process.trackingFailureFilter
    )

process.outTrackingFailureFilter = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('rejected_TrackingFailureFilter.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pTrackingFailureFilter') ),
    )



## Accepted events in RECO format

process.outRECO = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('accepted.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    )

## Accepted events + PF post-processing in RECO format

process.pAcceptedPlusPFEventFilter = copy.deepcopy( process.p )
process.pAcceptedPlusPFEventFilter += process.pfEventFilter

process.outAcceptedPlusPFEventFilter = cms.OutputModule(
    "PoolOutputModule",
    process.RECOSIMEventContent,
    fileName = cms.untracked.string('acceptedAndPFEventFilter.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pAcceptedPlusPFEventFilter') ),
    )


process.outpath += process.saveHistosInRunInfo


######### SCHEDULE ####################

process.schedule = cms.Schedule(
    # process.pcalo,
    process.p,
    )

if rejectedOutput:
    process.schedule.extend( [
        process.pDeltaPtMuons,
        process.pGreedyMuons,
        process.pInconsistentMuons,
        process.pPfEventFilter,
        process.pBEFilter,
        process.pHCALNoiseFilter,
        process.pBeamHaloFilter,
        process.pRingOfFireFilter,
        process.pTrackingFailureFilter ]
        )

    process.outpath += process.outDeltaPtMuons
    process.outpath += process.outGreedyMuons
    process.outpath += process.outInconsistentMuons
    process.outpath += process.outPfEventFilter
    if sourceExt != "LM1":
        process.outpath += process.outTPFilter
    process.outpath += process.outBEFilter
    process.outpath += process.outHCALNoiseFilter
    process.outpath += process.outBeamHaloFilter
    process.outpath += process.outRingOfFireFilter
    process.outpath += process.outTrackingFailureFilter
    if sourceExt != 'LM1':
        process.outpath += process.outRECO
    if not runningOnMC:
        process.outpath += process.outAcceptedPlusPFEventFilter


if sourceExt != "LM1":
    process.schedule.append( process.pAcceptedPlusPFEventFilter )
    process.schedule.append( process.pTPFilter )

process.schedule.append( process.outpath )

#process.dump = cms.EDAnalyzer("EventContentAnalyzer")
# process.p += process.dump


if selectEvents:
    # IMPORTANT:
    # pcalo lets a new tail event enter. It is probably filtered in the pf path, but I don't know where....
    
    # process.out.SelectEvents.SelectEvents.append('pcalo')
    process.out.SelectEvents.SelectEvents.append('p')

process.out.fileName = cms.untracked.string('susypat_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring( 'drop *' )

from CMGTools.SusyJetMET.EventContent.RA2Official_cff import RA2Official
process.out.outputCommands += RA2Official  
process.out.outputCommands.append( 'keep recoCaloMETs_*_*_*' )
process.out.outputCommands.append( 'keep recoPFMETs_*_*_*' )

from CMGTools.Common.EventContent.runInfoAccounting_cff import runInfoAccounting
process.out.outputCommands += runInfoAccounting

# process.out.outputCommands.append('keep *')

print process.out.dumpPython()

print 'output file: ', process.out.fileName


# print process.schedule

# message logger setup 
process.MessageLogger.cerr.FwkReport.reportEvery = 1




