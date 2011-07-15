import FWCore.ParameterSet.Config as cms
from CMGTools.H2TauTau.cmgTuple import vertexCollection


def configureCmgTupleClean( process , runOnMC ):

    #######
    process.setName_('CMGClean')

    ######
    process.load('CMGTools.H2TauTau.skims.cmgTauSelClean_cff')
    process.load('CMGTools.H2TauTau.skims.cmgElectronSelClean_cff')
    process.load('CMGTools.H2TauTau.skims.cmgMuonSelClean_cff')


    ######
    process.analysisSequence = cms.Sequence(
        process.tauSelCleanSequence

        +process.electronSelCleanSequence
        +process.electronSelCleanHistogramsSequence
        
        +process.muonSelCleanSequence
        +process.muonSelCleanHistogramsSequence
    )
    
    ##create the truth-matched sequence for histograms
    if runOnMC:
        process.analysisSequence += process.cmgTauSelTrue
        from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
        postfixTrue = 'True'
        cloneProcessingSnippet(process, getattr(process, 'tauSelCleanSequence'), postfixTrue)
        process.cmgTauSelHistogramsTrue.inputCollection = cms.InputTag("cmgTauSelTrue") #first module doesnt get postfix
        process.cmgTauSelPtTrue.src = cms.InputTag("cmgTauSelTrue") #first module doesnt get postfix
        process.analysisSequence +=  getattr(process,"tauSelCleanSequenceTrue") 


    process.p = cms.Path(process.analysisSequence)

    
    ####Remove events without any reconstructed leptons
    from CMGTools.Common.skims.cmgCandMerge_cfi import *
    process.cmgLeptonSelCleanMerged = cmgCandMerge.clone(
        src = cms.VInputTag(cms.InputTag("cmgTauSelClean"),
                            cms.InputTag("cmgElectronSelClean"),
                            cms.InputTag("cmgMuonSelClean"),                    
                            )
        )
    
    process.cmgLeptonSelCleanMergedFilter = cms.EDFilter("CandViewCountFilter",
                                                    src = cms.InputTag("cmgLeptonSelCleanMerged"),
                                                    minNumber = cms.uint32(1),
                                                    )

    process.p += process.cmgLeptonSelCleanMerged
    process.p += process.cmgLeptonSelCleanMergedFilter 
        
    ########
    process.out.outputCommands = cms.untracked.vstring( 'drop *')
    process.out.outputCommands.append("keep recoVertexs_"+vertexCollection+"_*_*")
    process.out.outputCommands.append( 'keep *_cmgTauSelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgElectronSelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgMuonSelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgPFMET_*_*')

