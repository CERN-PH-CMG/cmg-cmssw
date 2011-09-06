import FWCore.ParameterSet.Config as cms
from CMGTools.H2TauTau.tools import vertexCollection
from CMGTools.Common.skims.cmgCandMerge_cfi import cmgCandMerge

def configureCmgTupleDiTau( process, runOnMC ):

    process.setName_('CMGDiTau')
    
    ######
    process.load('CMGTools.Common.diTau_cff')
    process.ditauFactory.leg1Collection = cms.InputTag("cmgTauSel")
    process.ditauFactory.leg2Collection = cms.InputTag("cmgTauSel")
    process.cmgDiTau.cfg = process.ditauFactory.clone()


    ##Filter events without candidates
    process.EventFilter = cms.EDFilter("CandViewCountFilter",
                                       src = cms.InputTag("cmgDiTauSel"),
                                       minNumber = cms.uint32(1),
                                       )

    #################################################
    process.analysisSequence = cms.Sequence(process.diTauSequence
                                            +process.EventFilter
                                            )
    

    process.p = cms.Path(process.analysisSequence)

    ###########
    process.out.outputCommands = cms.untracked.vstring( 'drop *')
    process.out.outputCommands.append( 'keep *_addPileupInfo_*_HLT')
    process.out.outputCommands.append( "keep *_"+vertexCollection+"_*_RECO" )
    process.out.outputCommands.append( 'keep *_TriggerResults_*_PAT')
    process.out.outputCommands.append( 'keep *_cmgTriggerObjectSel_*_PAT')
    process.out.outputCommands.append( 'keep *_cmgDiTauSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgTauSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgMuonSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgElectronSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgPFJetSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgPFMET_*_*')
    
    if runOnMC: 
        process.out.outputCommands.append( 'keep *_genParticlesStatus3_*_PAT')








##     ##filter events without tau-tau candidates
##     process.cmgDiTauSelMerged = cmgCandMerge.clone(
##         src = cms.VInputTag(cms.InputTag("cmgDiTauSel"),
## #                            cms.InputTag("cmgTauESel"),
## #                            cms.InputTag("cmgTauMuSel"),
## #                            cms.InputTag("cmgMuESel"),
## #                            cms.InputTag("cmgDiESel"),
## #                            cms.InputTag("cmgDiMuSel"),                    
##                             )
##         )


## #                                            +process.tauESequence
## #                                            +process.tauMuSequence
## #                                            +process.muESequence
## #                                            +process.diMuonSequence
## #                                            +process.diElectronSequence
## #                                            +process.cmgDiTauSelMerged 

##     #    process.out.outputCommands.append( 'keep *_cmgTauESel_*_*' )
## #    process.out.outputCommands.append( 'keep *_cmgTauMuSel_*_*' )
## #    process.out.outputCommands.append( 'keep *_cmgMuESel_*_*' )
## #    process.out.outputCommands.append( 'keep *_cmgDiMuonSel_*_*' )
## #    process.out.outputCommands.append( 'keep *_cmgDiElectronSel_*_*' )
