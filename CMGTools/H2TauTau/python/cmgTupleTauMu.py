import FWCore.ParameterSet.Config as cms
from CMGTools.H2TauTau.tools import vertexCollection


process.setName_('CMGDiTau')

######
process.load('CMGTools.Common.tauMu_cff')
process.tauMuFactory.leg1Collection = cms.InputTag("cmgTauSel")
process.tauMuFactory.leg2Collection = cms.InputTag("cmgMuonSel")
process.cmgTauMu.cfg = process.tauMuFactory.clone()

######
process.load('CMGTools.H2TauTau.skims.cmgTauMuSelClean_cff')
process.load('CMGTools.H2TauTau.cmgDiJetVBF_cff')


##Filter events without candidates
process.EventFilter = cms.EDFilter("CandViewCountFilter",
                                   src = cms.InputTag("cmgTauMuSel"),
                                   minNumber = cms.uint32(1),
                                   )

###########
process.analysisSequence = cms.Sequence(process.tauMuSequence
                                        +process.cmgTauMuSelCleanSequence
                                        +process.cmgTauMuSelCleanHistogramsSequence
                                        +process.cmgDiJetVBFSequence
                                        +process.EventFilter
                                        )

process.p = cms.Path(process.analysisSequence)

###########
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.append( 'keep *_addPileupInfo_*_HLT')
process.out.outputCommands.append( "keep *_"+vertexCollection+"_*_RECO" )
process.out.outputCommands.append( 'keep *_TriggerResults_*_PAT')
process.out.outputCommands.append( 'keep *_cmgTriggerObjectSel_*_PAT')
process.out.outputCommands.append( 'keep *_cmgTauMuSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgTauSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgMuonSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgElectronSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgPFJetSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgPFMET_*_*')
process.out.outputCommands.append( 'keep *_cmgDiJetVBF*_*_*')


if runOnMC: 
    process.out.outputCommands.append( 'keep *_genParticlesStatus3_*_PAT')






