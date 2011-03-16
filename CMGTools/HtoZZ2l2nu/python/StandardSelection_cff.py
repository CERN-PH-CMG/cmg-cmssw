import FWCore.ParameterSet.Config as cms


### GENERATOR LEVEL
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *
genHiggsFinalState = cms.EDProducer(
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
from CMGTools.Common.runInfoCounter_cfi import *
genPath = cms.Path( runInfoCounterSequence+genHiggsFinalState )

### SINGLE LEPTON
from CMGTools.Common.muon_cff import *
cmgMuon.cfg.inputCollection = cms.InputTag('selectedPatMuonsPFlow')
cmgMuon.cfg.muonIDType = cms.string('TMLastStationLoose')
cmgMuon.cuts = cms.PSet( pt = cms.string('pt()>15 && abs(eta())<2.4'),
                         id = cms.string('sourcePtr().muonID(\"AllGlobalMuons\") && sourcePtr().muonID(\"AllTrackerMuons\")') #tracker && global
                         )
from CMGTools.Common.electron_cff import *
cmgElectron.cfg.inputCollection = cms.InputTag('selectedPatElectronsPFlow')
cmgElectron.cfg.electronIDType = cms.string('simpleEleId90relIso')
cmgElectron.cuts = cms.PSet( pt = cms.string('pt()>15 && abs(eta())<2.5'),
                             id = cms.string('sourcePtr().electronID(\"simpleEleId90relIso\")>=4') #no conversions
                             )

### DI-MUON selection
from CMGTools.Common.diMuon_cff import *
cmgDiMuon.cuts.zmumu.isolation=cms.string('')
cmgDiMuon.cuts.zmumu.pt = cms.string('pt() >= 0')
cmgDiMuon.cuts.zmumu.mass = cms.string('mass() >= 50')
cmgDiMuon.cuts.zmumu.leg1_quality = cms.string('')
cmgDiMuon.cuts.zmumu.leg1_kinematics.eta = cms.string('')
cmgDiMuon.cuts.zmumu.leg2_quality = cmgDiMuon.cuts.zmumu.leg1_quality
cmgDiMuon.cuts.zmumu.leg2_kinematics.eta = cmgDiMuon.cuts.zmumu.leg1_kinematics.eta
cmgDiMuonCount.minNumber = cms.uint32(1)

### DI-ELECTRON selection
from CMGTools.Common.diElectron_cff import *
cmgDiElectron.cuts.zee.pt = cms.string('pt() >= 0')
cmgDiElectron.cuts.zee.mass = cms.string('mass() >= 50')
cmgDiElectron.cuts.zee.leg1_quality = cms.string('')
cmgDiElectron.cuts.zee.leg1_kinematics.eta = cms.string('')
cmgDiElectron.cuts.zee.leg2_quality = cmgDiElectron.cuts.zee.leg1_quality
cmgDiElectron.cuts.zee.leg2_kinematics.eta = cmgDiElectron.cuts.zee.leg1_kinematics.eta
cmgDiElectronCount.minNumber = cms.uint32(1)

### E-MU selection
from CMGTools.HtoZZ2l2nu.emuDilepton_cff import *
cmgEmuDilepton.cuts.emukinematics.pt = cms.string('pt() >= 0')
cmgEmuDilepton.cuts.emukinematics.mass = cms.string('mass() >= 0')
cmgEmuDilepton.cuts.emukinematics.leg1_quality = cmgDiElectron.cuts.zee.leg1_quality
cmgEmuDilepton.cuts.emukinematics.leg1_kinematics.eta = cmgDiElectron.cuts.zee.leg1_kinematics
cmgEmuDilepton.cuts.emukinematics.leg2_quality = cmgDiMuon.cuts.zmumu.leg2_quality
cmgEmuDilepton.cuts.emukinematics.leg2_kinematics.eta = cmgDiMuon.cuts.zmumu.leg2_kinematics.eta
cmgEmuDileptonCount.minNumber = cms.uint32(1)

### JET selection
from CMGTools.Common.jet_cff import *
myjets=cms.InputTag('selectedPatJetsPFlow')
cmgPFJet.cfg.inputCollection = myjets
cmgPFJet.cfg.btagType = cms.string('trackCountingHighEffBJetTags')
cmgPFBaseJet.cfg.inputCollection = myjets
cmgPFBaseJet.cfg.btagType = cms.string('trackCountingHighEffBJetTags')

#### MET
from CMGTools.Common.met_cff import *
mymet=cms.InputTag('patMETsPFlow')
cmgMHTPFJets.cfg.inputCollection = myjets
cmgMHTPFJets30.cfg.inputCollection = myjets
cmgMETPFCandidates.cfg.inputCollection = mymet
metSequence = cms.Sequence( cmgMHTPFJets+
                                    cmgMHTPFJets30+
                                    cmgMETPFCandidates)

### the sequences
leptonPath = cms.Path(electronSequence*muonSequence)
eePath = cms.Path(diElectronSequence)
mumuPath = cms.Path(diMuonSequence)
emuPath = cms.Path(emuSequence)
jetmetPath = cms.Path( pfJetSequence*metSequence )
### configure the process



#MessageLogger.suppressWarning=cms.untracked.vstring('patMuonTriggerResults')

### event content and output
from CMGTools.Common.EventContent.particleFlow_cff import particleFlow as particleFlowEventContent  
from CMGTools.Common.EventContent.particleFlow_cff import particleFlowBase as particleFlowEventContentBase
from CMGTools.Common.EventContent.particleFlow_cff import particleFlowMHT as particleFlowMHTContentBase
outputConfig = cms.PSet(
    outputCommandsHtoZZ = cms.untracked.vstring( 'drop *')
)
outputConfig.outputCommandsHtoZZ.extend( particleFlowEventContent ) 
outputConfig.outputCommandsHtoZZ.extend( particleFlowEventContentBase )
outputConfig.outputCommandsHtoZZ.extend( particleFlowMHTContentBase )
outputConfig.outputCommandsHtoZZ.append( 'keep *_genHiggsFinalState_*_*' )
outputConfig.outputCommandsHtoZZ.append( 'keep *_patMETsPFlow_*_*' ) # MET
outputConfig.outputCommandsHtoZZ.append( 'keep *_cmgEmuDilepton*_*_*' )
outputConfig.outputCommandsHtoZZ.append( 'keep GenRunInfoProduct_generator_*_*' )
outputConfig.outputCommandsHtoZZ.append( 'keep double*_*_MEtoEDMConverterRun_*' )
outputConfig.outputCommandsHtoZZ.append( 'drop patMETs_*_*_*' )
