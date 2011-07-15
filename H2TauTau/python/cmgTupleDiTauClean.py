import FWCore.ParameterSet.Config as cms
from CMGTools.H2TauTau.cmgTuple import vertexCollection

def configureCmgTupleDiTauClean( process ):
    
    process.setName_('CMGDiTauClean')

    ######
    process.load('CMGTools.H2TauTau.skims.cmgDiTauSelClean_cff')
    process.load('CMGTools.H2TauTau.skims.cmgTauESelClean_cff')
    process.load('CMGTools.H2TauTau.skims.cmgTauMuSelClean_cff')
    process.load('CMGTools.H2TauTau.skims.cmgMuESelClean_cff')
    process.load('CMGTools.H2TauTau.skims.cmgDiESelClean_cff')
    process.load('CMGTools.H2TauTau.skims.cmgDiMuSelClean_cff')

    ##filter events without tau-tau candidates
    process.cmgDiTauSelMerged = cmgCandMerge.clone(
        src = cms.VInputTag(cms.InputTag("cmgDiTauSelClean"),
                            cms.InputTag("cmgTauESelClean"),
                            cms.InputTag("cmgTauMuSelClean"),
                            cms.InputTag("cmgMuESelClean"),
                            cms.InputTag("cmgDiESelClean"),
                            cms.InputTag("cmgDiMuSelClean"),                    
                            )
        )
    process.cmgDiTauSelMergedEventFilter = cms.EDFilter("CandViewCountFilter",
                                               src = cms.InputTag("cmgDiTauSelMerged"),
                                               minNumber = cms.uint32(1),
                                               )

    ######
    process.analysisSequence = cms.Sequence(
        process.cmgDiTauSelCleanSequence
        +process.cmgDiTauSelCleanHistogramsSequence
        +process.cmgTauESelCleanSequence
        +process.cmgTauESelCleanHistogramsSequence
        +process.cmgTauMuSelCleanSequence
        +process.cmgTauMuSelCleanHistogramsSequence
        +process.cmgMuESelCleanSequence
        +process.cmgMuESelCleanHistogramsSequence
        +process.cmgDiESelCleanSequence
        +process.cmgDiESelCleanHistogramsSequence
        +process.cmgDiMuSelCleanSequence
        +process.cmgDiMuSelCleanHistogramsSequence
        
        +process.cmgDiTauSelMerged
        +process.cmgDiTauSelMergedEventFilter
    )


    process.p = cms.Path(process.analysisSequence)

    ########
    process.out.outputCommands = cms.untracked.vstring( 'drop *')
    process.out.outputCommands.append("keep recoVertexs_"+vertexCollection+"_*_*")
    process.out.outputCommands.append( 'keep *_cmgDiTauSelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgTauESelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgTauMuSelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgMuESelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgDiESelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgDiMuSelClean_*_*' )
    process.out.outputCommands.append( 'keep *_cmgPFMET_*_*')
