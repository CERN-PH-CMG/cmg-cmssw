import FWCore.ParameterSet.Config as cms
from CMGTools.H2TauTau.tools import vertexCollection

def configureCmgTupleTauMuClean( process, runOnMC ):
    
    process.setName_('CMGDiTauClean')

    ######
    process.load('CMGTools.H2TauTau.skims.cmgTauMuSelClean_cff')

    ##filter events without tau-tau candidates
    process.EventFilter = cms.EDFilter("CandViewCountFilter",
                                       src = cms.InputTag("cmgTauMuSelClean"),
                                       minNumber = cms.uint32(1),
                                       )
    
    ######
    process.analysisSequence = cms.Sequence(
        process.cmgTauMuSelCleanSequence
        +process.cmgTauMuSelCleanHistogramsSequence
        +process.EventFilter
    )
    process.p = cms.Path(process.analysisSequence)

    ########
    process.out.outputCommands = cms.untracked.vstring( 'drop *')
    process.out.outputCommands.append( 'keep *_addPileupInfo_*_HLT')
    process.out.outputCommands.append( "keep *_"+vertexCollection+"_*_RECO" )
    process.out.outputCommands.append( 'keep *_TriggerResults_*_PAT')
    process.out.outputCommands.append( 'keep *_cmgTriggerObjectSel_*_PAT')
    process.out.outputCommands.append( 'keep *_cmgTauMuSelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgTauSel_*_*' ) 
    process.out.outputCommands.append( 'keep *_cmgMuonSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgElectronSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgPFJetSel_*_*' )
    process.out.outputCommands.append( 'keep *_cmgPFMET_*_*')



    if runOnMC: 
        process.out.outputCommands.append( 'keep *_genParticlesStatus3_*_PAT')
        



