from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint

process.out.SelectEvents.SelectEvents = cms.vstring()


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )


ext = 'RA2'

doPATTuple = True
selectEvents = True

print 'processing:'

sourceExt = 'Test'
if sourceExt == 'Test':
    #process.source.fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Fall10-START38_V12-v1/GEN-SIM-RECO/RECO_1_2_v6F.root')
    # process.source.fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET/ZinvisibleJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/RECO_1_1_Gqw.root')
    process.source.fileNames = cms.untracked.vstring('file:/tmp/RECO.root')
if sourceExt == 'SueAnnHot':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff")
    process.setName_('PAT2')
 

# process.source.eventsToProcess = cms.untracked.VEventRange( '1:243087' )

print process.source.fileNames


outFileNameExt = ext

# reinitializing stuff defined in patTemplate_cfg

process.load("CMGTools.Common.countingSequences_cff")

# depending on what we want to do, the various sequences will be added to this path.

    
from CMGTools.SusyJetMET.RA2OfficialSequence_cff import addSUSYPATRA2
# for Fall10 samples 
# addSUSYPATRA2( process, 'REDIGI38X')

# WARNING 
addSUSYPATRA2( process, 'REDIGI')
process.patJets.addTagInfos     = False
process.patJets.addBTagInfo = False
process.pfseq.remove( process.badPFMuonFilter ) 

 
process.pcalo = cms.Path(
    process.caloseq
    )

process.load("RecoParticleFlow.PostProcessing.METCorrelationFilter_cfi")
process.p = cms.Path(
    process.startupSequence +
    #WARNING
    # process.METCorrelationFilter + 
    process.pfseq +
    process.finalSequence  
    )
process.outpath += process.saveHistosInRunInfo

process.schedule = cms.Schedule(
    process.pcalo,
    process.p,
    process.outpath
    )

#process.dump = cms.EDAnalyzer("EventContentAnalyzer")
# process.p += process.dump


process.out.SelectEvents.SelectEvents.append('pcalo')
if selectEvents:
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

#process.TFileService = cms.Service(
#    "TFileService",
#    fileName = cms.string("susyJetMET_histograms_%s.root" %  outFileNameExt )
#    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName


# print process.schedule

# message logger setup 
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )



# print process.dumpPython()
