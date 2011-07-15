import FWCore.ParameterSet.Config as cms
from CMGTools.H2TauTau.cmgTuple import vertexCollection

def configureCmgTupleDiTau( process ):

    process.setName_('CMGDiTau')
    
    ######
    process.load('CMGTools.Common.diTau_cff')
    process.ditauFactory.leg1Collection = cms.InputTag("cmgTauSelClean")
    process.ditauFactory.leg2Collection = cms.InputTag("cmgTauSelClean")
    process.cmgDiTau.cfg = process.ditauFactory.clone()
    ######
    process.load('CMGTools.Common.tauE_cff')
    process.tauEFactory.leg1Collection = cms.InputTag("cmgTauSelClean")
    process.tauEFactory.leg2Collection = cms.InputTag("cmgElectronSelClean")
    process.cmgTauE.cfg = process.tauEFactory.clone()
    ######
    process.load('CMGTools.Common.tauMu_cff')
    process.tauMuFactory.leg1Collection = cms.InputTag("cmgTauSelClean")
    process.tauMuFactory.leg2Collection = cms.InputTag("cmgMuonSelClean")
    process.cmgTauMu.cfg = process.tauMuFactory.clone()
    ######
    process.load('CMGTools.Common.muE_cff')
    process.muEFactory.leg1Collection = cms.InputTag("cmgMuonSelClean")
    process.muEFactory.leg2Collection = cms.InputTag("cmgElectronSelClean")
    process.cmgMuE.cfg = process.muEFactory.clone()
    ######
    process.load('CMGTools.Common.diMuon_cff')
    process.dimuonFactory.leg1Collection = cms.InputTag("cmgMuonSelClean")
    process.dimuonFactory.leg2Collection = cms.InputTag("cmgMuonSelClean")
    process.cmgDiMuon.cfg = process.dimuonFactory.clone()
    process.cmgDiMuon.cuts = cms.PSet()
    ######
    process.load('CMGTools.Common.diElectron_cff')
    process.diElectronFactory.leg1Collection = cms.InputTag("cmgElectronSelClean")
    process.diElectronFactory.leg2Collection = cms.InputTag("cmgElectronSelClean")
    process.cmgDiElectron.cfg = process.diElectronFactory.clone()
    process.cmgDiElectron.cuts = cms.PSet()

    ##filter events without tau-tau candidates
    process.cmgDiTauSelMerged = cmgCandMerge.clone(
        src = cms.VInputTag(cms.InputTag("cmgDiTauSel"),
                            cms.InputTag("cmgTauESel"),
                            cms.InputTag("cmgTauMuSel"),
                            cms.InputTag("cmgMuESel"),
                            cms.InputTag("cmgDiESel"),
                            cms.InputTag("cmgDiMuSel"),                    
                            )
        )
    
    process.cmgDiTauSelMergedEventFilter = cms.EDFilter("CandViewCountFilter",
                                                  src = cms.InputTag("cmgDiTauSelMerged"),
                                                  minNumber = cms.uint32(1),
                                                  )

    #################################################
    process.analysisSequence = cms.Sequence(process.diTauSequence
                                            +process.tauESequence
                                            +process.tauMuSequence
                                            +process.muESequence
                                            +process.diMuonSequence
                                            +process.diElectronSequence
                                            +process.cmgDiTauSelMerged 
                                            +process.cmgDiTauSelMergedEventFilter                                        
                                            )


    process.p = cms.Path(process.analysisSequence)

    ###########
    process.out.outputCommands = cms.untracked.vstring( 'drop *')
    process.out.outputCommands.append("keep recoVertexs_"+vertexCollection+"_*_*")
    process.out.outputCommands.append( 'keep *_cmgDiTauSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgTauESel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgTauMuSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgMuESel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgDiMuonSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgDiElectronSel_*_*' )

