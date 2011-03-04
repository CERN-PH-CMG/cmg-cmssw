from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import sys

### rename this process
process.setName_('CMG')

### SOURCES
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import getLocalSourceFor
theLocalSrc="GluGluToHToZZTo2L2Nu_M_400"
if(len(sys.argv)>2 ): theLocalSrc = sys.argv[2]
ffile=0
if(len(sys.argv)>3 ): ffile = int(sys.argv[3])
fstep=-1
if(len(sys.argv)>4 ): fstep = int(sys.argv[4])
fnames, trigMenu, isData = getLocalSourceFor( theLocalSrc, ffile, fstep ) 
process.source.fileNames=fnames
print ' ***** Local source is defined for ' + theLocalSrc + ' with ' + str(len(process.source.fileNames)) + ' files'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


### GENERATOR LEVEL
if( not isData ):
    process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
    process.genHiggsFinalState = cms.EDProducer(
        "GenParticlePruner",
        src = cms.InputTag("genParticles"),
        select = cms.vstring(
        "drop  *",
        "keep++ pdgId={h0}",
        "drop pdgId={h0} & status=2",
        "drop pdgId={Z0} & status=2",
        "drop pdgId={W+} & status=2",
        "drop pdgId={W-} & status=2"
        )
        )
    process.load('CMGTools.Common.factories.cmgBaseGenCandidates_cfi')
    process.cmgHiggsFinalState = process.cmgGenCandidates.clone()
    process.cmgHiggsFinalState.cfg.inputCollection = 'genHiggsFinalState'
    process.genSequence = cms.Sequence( process.genHiggsFinalState+process.cmgHiggsFinalState)
    
### TRIGGER selection
process.load('CMGTools.ZmumuJetsTutorial.patTriggerMatching_cff')
process.patMuonTriggerResults.processName=cms.string(trigMenu)
process.patMuonTriggerMatch.pathNames = cms.vstring('HLT_Mu9','HLT_Mu11','HLT_Mu15_v1')

### DI-MUON selection
process.load('CMGTools.Common.muon_cff')
process.load('CMGTools.Common.diMuon_cff')
process.cmgDiMuon.cuts.zmumu.pt = cms.string('pt() >= 0')
process.cmgDiMuon.cuts.zmumu.mass = cms.string('mass() >= 50')
process.cmgDiMuon.cuts.zmumu.leg1_quality = cms.string('leg1().getSelection(\"cuts_vbtfmuon_isGlobal\")')
process.cmgDiMuon.cuts.zmumu.leg1_kinematics.eta = cms.string('abs(leg1().eta()) < 2.4')
process.cmgMuon.cfg.inputCollection = cms.InputTag("triggeredPatMuons")

### JET selection
process.load('CMGTools.Common.jet_cff')
myjets=cms.InputTag('patJetsCachedReClusterZPUL1Corr')
process.cmgPFJet.cfg.inputCollection = myjets
process.cmgPFJet.cfg.btagType = cms.string('trackCountingHighEffBJetTags')
process.cmgPFBaseJet.cfg.inputCollection = myjets
process.cmgPFBaseJet.cfg.btagType = cms.string('trackCountingHighEffBJetTags')

#### MET
process.load('CMGTools.Common.met_cff')
#mymet=cms.InputTag('patMETsReClusterZPU')
mymet=cms.InputTag('patMETsPFlow')
process.cmgMHTPFJets.cfg.inputCollection = myjets
process.cmgMHTPFJets30.cfg.inputCollection = myjets
process.cmgMETPFCandidates.cfg.inputCollection = mymet
process.metSequence = cms.Sequence( process.cmgMHTPFJets+
                                    process.cmgMHTPFJets30+
                                    process.cmgMETPFCandidates)

### produce a di-muon+MET object
process.load('CMGTools.Common.factories.cmgDiMuonPlusMET_cfi')
process.selectedDiMuonMETSequence = cms.Sequence( process.cmgDiMuonPlusMET*process.selectedDiMuonPlusMETFilter )

### the sequence
process.analysisSequence = cms.Sequence(
    process.patMuonTrigger +
    process.muonSequence +
    process.diMuonSequence +
    process.pfJetSequence +
    process.metSequence +
    process.selectedDiMuonMETSequence )
    
### event content and output
from CMGTools.Common.EventContent.particleFlow_cff import particleFlow as particleFlowEventContent  
from CMGTools.Common.EventContent.particleFlow_cff import particleFlowBase as particleFlowEventContentBase
from CMGTools.Common.EventContent.particleFlow_cff import particleFlowMHT as particleFlowMHTContentBase
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( particleFlowEventContent ) 
process.out.outputCommands.extend( particleFlowEventContentBase )
process.out.outputCommands.extend( particleFlowMHTContentBase )
process.out.outputCommands.append( 'keep *_genHiggsFinalState_*_*' )
process.out.outputCommands.append( 'keep *_cmgHiggsFinalState_*_*' )
process.out.outputCommands.append( 'keep GenRunInfoProduct_generator_*_*' )
process.out.outputCommands.append( 'keep double*_*_MEtoEDMConverterRun_*' )
process.out.outputCommands.append( 'drop patMETs_*_*_*' )
process.out.fileName = cms.untracked.string('/tmp/psilva/'+theLocalSrc+'_'+str(ffile)+'_cmgTuple.root')

### output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('/tmp/psilva/'+theLocalSrc+'_'+str(ffile)+'_histograms.root'))

### configure the process
if(not isData):
    process.p = cms.Path(process.genSequence+process.analysisSequence)
    print '*** MC based analysis'
else :
    process.p = cms.Path(process.analysisSequence)
    print '*** Data based analysis'
print '*** Running the following path: '
print process.p

process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.suppressWarning=cms.untracked.vstring('patMuonTriggerResults')
