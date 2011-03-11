from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import sys

### rename this process
process.setName_('CMG')

isData=False

### SOURCES
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import getLocalSourceFor
theLocalSrc='GluGluToH200ToZZTo2L2Nu'
if(len(sys.argv)>2 ): theLocalSrc = sys.argv[2]
ffile=0
if(len(sys.argv)>3 ): ffile = int(sys.argv[3])
fstep=-1
if(len(sys.argv)>4 ): fstep = int(sys.argv[4])
fnames = getLocalSourceFor( theLocalSrc, ffile, fstep ) 
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
    process.load('CMGTools.Common.runInfoCounter_cfi')
    process.genPath = cms.Path( process.runInfoCounterSequence+process.genHiggsFinalState )

### SINGLE LEPTON
process.load('CMGTools.Common.muon_cff')
process.cmgMuon.cfg.inputCollection = cms.InputTag("selectedPatMuonsPFlow")
process.cmgMuon.cuts = cms.PSet( pt = cms.string('pt()>15 && abs(eta())<2.4'),
                                 id = cms.string('sourcePtr().muonID(\"AllGlobalMuons\") && sourcePtr().muonID(\"AllTrackerMuons\")')
                                 )
process.load('CMGTools.Common.electron_cff')
process.cmgElectron.cfg.inputCollection = cms.InputTag("selectedPatElectronsPFlow")
process.cmgElectron.cuts = cms.PSet( pt = cms.string('pt()>15 && abs(eta())<2.5'),
#                                     id = cms.string('sourcePtr().isEcalDriven()')
                                     id = cms.string('')
                                     )

### DI-MUON selection
process.load('CMGTools.Common.diMuon_cff')
process.cmgDiMuon.cuts.zmumu.pt = cms.string('pt() >= 0')
process.cmgDiMuon.cuts.zmumu.mass = cms.string('mass() >= 50')
process.cmgDiMuon.cuts.zmumu.leg1_quality = cms.string('')
process.cmgDiMuon.cuts.zmumu.leg1_kinematics.eta = cms.string('')
process.cmgDiMuon.cuts.zmumu.leg2_quality = process.cmgDiMuon.cuts.zmumu.leg1_quality
process.cmgDiMuon.cuts.zmumu.leg2_kinematics.eta = process.cmgDiMuon.cuts.zmumu.leg1_kinematics.eta
process.cmgDiMuonCount.minNumber = cms.uint32(1)

### DI-ELECTRON selection
process.load('CMGTools.Common.diElectron_cff')
process.cmgDiElectron.cuts.zee.pt = cms.string('pt() >= 0')
process.cmgDiElectron.cuts.zee.mass = cms.string('mass() >= 50')
process.cmgDiElectron.cuts.zee.leg1_quality = cms.string('')
process.cmgDiElectron.cuts.zee.leg1_kinematics.eta = cms.string('')
process.cmgDiElectron.cuts.zee.leg2_quality = process.cmgDiElectron.cuts.zee.leg1_quality
process.cmgDiElectron.cuts.zee.leg2_kinematics.eta = process.cmgDiElectron.cuts.zee.leg1_kinematics.eta
process.cmgDiElectronCount.minNumber = cms.uint32(1)

### E-MU selection
process.load('CMGTools.Common.emuDilepton_cff')
process.cmgEmuDilepton.cuts.emukinematics.pt = cms.string('pt() >= 0')
process.cmgEmuDilepton.cuts.emukinematics.mass = cms.string('mass() >= 0')
process.cmgEmuDilepton.cuts.emukinematics.leg1_quality = process.cmgDiElectron.cuts.zee.leg1_quality
process.cmgEmuDilepton.cuts.emukinematics.leg1_kinematics.eta = process.cmgDiElectron.cuts.zee.leg1_kinematics
process.cmgEmuDilepton.cuts.emukinematics.leg2_quality = process.cmgDiMuon.cuts.zmumu.leg2_quality
process.cmgEmuDilepton.cuts.emukinematics.leg2_kinematics.eta = process.cmgDiMuon.cuts.zmumu.leg2_kinematics.eta
process.cmgEmuDileptonCount.minNumber = cms.uint32(1)

### JET selection
process.load('CMGTools.Common.jet_cff')
myjets=cms.InputTag('selectedPatJetsPFlow')
process.cmgPFJet.cfg.inputCollection = myjets
process.cmgPFJet.cfg.btagType = cms.string('trackCountingHighEffBJetTags')
process.cmgPFBaseJet.cfg.inputCollection = myjets
process.cmgPFBaseJet.cfg.btagType = cms.string('trackCountingHighEffBJetTags')

#### MET
process.load('CMGTools.Common.met_cff')
mymet=cms.InputTag('patMETsPFlow')
process.cmgMHTPFJets.cfg.inputCollection = myjets
process.cmgMHTPFJets30.cfg.inputCollection = myjets
process.cmgMETPFCandidates.cfg.inputCollection = mymet
process.metSequence = cms.Sequence( process.cmgMHTPFJets+
                                    process.cmgMHTPFJets30+
                                    process.cmgMETPFCandidates)

### the sequences
process.leptonPath = cms.Path(process.electronSequence*process.muonSequence)
process.eePath = cms.Path(process.diElectronSequence)
process.mumuPath = cms.Path(process.diMuonSequence)
process.emuPath = cms.Path(process.emuSequence)
process.jetmetPath = cms.Path( process.pfJetSequence*process.metSequence )
### configure the process
if(not isData):
    print '*** MC based analysis'
    process.e = cms.EndPath(process.saveHistosInRunInfo*process.out)
    process.schedule = cms.Schedule(process.genPath,process.leptonPath,process.eePath,process.mumuPath,process.emuPath,process.jetmetPath,process.e)
else :
    print '*** Data based analysis'
    process.e = cms.EndPath(process.out)
    process.schedule = cms.Schedule(process.leptonPath,process.eePath,process.mumuPath,process.emuPath,process.jetmetPath,process.e)

print '*** Scheduling the following sequence: '

process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.suppressWarning=cms.untracked.vstring('patMuonTriggerResults')

### event content and output
from CMGTools.Common.EventContent.particleFlow_cff import particleFlow as particleFlowEventContent  
from CMGTools.Common.EventContent.particleFlow_cff import particleFlowBase as particleFlowEventContentBase
from CMGTools.Common.EventContent.particleFlow_cff import particleFlowMHT as particleFlowMHTContentBase
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( particleFlowEventContent ) 
process.out.outputCommands.extend( particleFlowEventContentBase )
process.out.outputCommands.extend( particleFlowMHTContentBase )
process.out.outputCommands.append( 'keep *_genHiggsFinalState_*_*' )
process.out.outputCommands.append( 'keep *_cmgEmuDilepton*_*_*' )
process.out.outputCommands.append( 'keep GenRunInfoProduct_generator_*_*' )
process.out.outputCommands.append( 'keep double*_*_MEtoEDMConverterRun_*' )
process.out.outputCommands.append( 'drop patMETs_*_*_*' )
process.out.fileName = cms.untracked.string('/tmp/psilva/'+theLocalSrc+'_'+str(ffile)+'_cmgTuple.root')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('eePath','mumuPath','emuPath') )

### output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('/tmp/psilva/'+theLocalSrc+'_'+str(ffile)+'_histograms.root'))
