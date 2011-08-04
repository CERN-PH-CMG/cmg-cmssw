import FWCore.ParameterSet.Config as cms
from CMGTools.H2TauTau.tools import vertexCollection

def configureCmgTupleDiTauClean( process ):
    
    process.setName_('CMGDiTauClean')

    ######
    process.load('CMGTools.H2TauTau.skims.cmgDiTauSelClean_cff')

    ##filter events without tau-tau candidates
    process.EventFilter = cms.EDFilter("CandViewCountFilter",
                                       src = cms.InputTag("cmgDiTauSelClean"),
                                       minNumber = cms.uint32(1),
                                       )
    
    ######
    process.analysisSequence = cms.Sequence(
        process.cmgDiTauSelCleanSequence
        +process.cmgDiTauSelCleanHistogramsSequence
        +process.EventFilter
    )
    process.p = cms.Path(process.analysisSequence)

    ########
    process.out.outputCommands = cms.untracked.vstring( 'drop *')
    process.out.outputCommands.append( 'keep *_addPileupInfo_*_*')
    process.out.outputCommands.append( "keep *_"+vertexCollection+"_*_*" )    
    process.out.outputCommands.append( 'keep *_cmgDiTauSelClean_*_*' ) 
    process.out.outputCommands.append( 'keep *_cmgPFMET_*_*')










##     process.load('CMGTools.H2TauTau.skims.cmgTauESelClean_cff')
##     process.load('CMGTools.H2TauTau.skims.cmgTauMuSelClean_cff')
##     process.load('CMGTools.H2TauTau.skims.cmgMuESelClean_cff')
##     process.load('CMGTools.H2TauTau.skims.cmgDiESelClean_cff')
##     process.load('CMGTools.H2TauTau.skims.cmgDiMuSelClean_cff')


##         process.cmgDiTauSelMerged = cmgCandMerge.clone(
##         src = cms.VInputTag(cms.InputTag("cmgDiTauSelClean"),
##                             cms.InputTag("cmgTauESelClean"),
##                             cms.InputTag("cmgTauMuSelClean"),
##                             cms.InputTag("cmgMuESelClean"),
##                             cms.InputTag("cmgDiESelClean"),
##                             cms.InputTag("cmgDiMuSelClean"),                    
##                             )
##         )


##         +process.cmgTauESelCleanSequence
##         +process.cmgTauESelCleanHistogramsSequence
##         +process.cmgTauMuSelCleanSequence
##         +process.cmgTauMuSelCleanHistogramsSequence
##         +process.cmgMuESelCleanSequence
##         +process.cmgMuESelCleanHistogramsSequence
##         +process.cmgDiESelCleanSequence
##         +process.cmgDiESelCleanHistogramsSequence
##         +process.cmgDiMuSelCleanSequence
##         +process.cmgDiMuSelCleanHistogramsSequence
        
##         +process.cmgDiTauSelMerged        

##     process.out.outputCommands.append( 'keep *_cmgTauESelClean_*_*' )
##     process.out.outputCommands.append( 'keep *_cmgTauMuSelClean_*_*' )
##     process.out.outputCommands.append( 'keep *_cmgMuESelClean_*_*' )
##     process.out.outputCommands.append( 'keep *_cmgDiESelClean_*_*' )
##     process.out.outputCommands.append( 'keep *_cmgDiMuSelClean_*_*' )
