import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet


def getattrGenerator( process, postfix ):
    def fun(name):
        return getattr(process, name+postfix)
    return fun


def addCmgMuons( process, postfix, inputTag ):
    ganew = getattrGenerator( process, postfix )    
    cloneProcessingSnippet(process, process.muonSequence, postfix)
    ganew('cmgMuon').cfg.inputCollection = inputTag
    return ganew( 'muonSequence' )


def addCmgElectrons( process, postfix, inputTag ):
    ganew = getattrGenerator( process, postfix )    
    cloneProcessingSnippet(process, process.electronSequence, postfix)
    ganew('cmgElectron').cfg.inputCollection = inputTag
    return ganew( 'electronSequence' )



## def addStdMuons( process, postfix, newpostfix, cut, runOnMC):

##     fullpostfix = postfix+newpostfix
##     ga = getattrGenerator( process, postfix )
##     ganew = getattrGenerator( process, fullpostfix )

##     muonSeq = cms.Sequence(
##         ga( 'pfMuonIsolationSequence' ) +
##         ga( 'makePatMuons' )  +
##         ga( 'selectedPatMuons' )
##         # +
##         # addCmgMuons( process, newpostfix, 'selectedPatMuons'+fullpostfix )
##         )
##     setattr( process, 'stdMuonSequence'+postfix, muonSeq)
    
##     muonSource = 'muons'
##     cloneProcessingSnippet(process, ga('stdMuonSequence'), newpostfix)
    
##     ganew("muPFIsoDepositCharged").src = muonSource
##     ganew("muPFIsoDepositChargedAll").src = muonSource
##     ganew("muPFIsoDepositNeutral").src = muonSource
##     ganew("muPFIsoDepositGamma").src = muonSource
##     ganew("muPFIsoDepositPU").src = muonSource
##     ganew("patMuons").useParticleFlow = False
##     ganew("selectedPatMuons").cut = cut
##     if runOnMC:
##         ganew("muonMatch").src = muonSource
##     else:
##         ganew('makePatMuons').remove( ganew("muonMatch") )

##     return ganew( 'stdMuonSequence' )

    


## def addStdElectrons(process, postfix, newpostfix, cut, runOnMC):

##     fullpostfix = postfix+newpostfix
##     ga = getattrGenerator( process, postfix )
##     ganew = getattrGenerator( process, fullpostfix )
    
##     electronSeq = cms.Sequence(
##         ga('pfElectronIsolationSequence') + 
##         ga('makePatElectrons') + 
##         ga('selectedPatElectrons')
##         )
##     setattr( process, 'stdElectronSequence'+postfix, electronSeq)

##     electronSource = 'gsfElectrons'
##     cloneProcessingSnippet(process, ga('stdElectronSequence'), newpostfix)
        
##     ganew("elPFIsoDepositCharged").src = electronSource
##     ganew("elPFIsoDepositChargedAll").src = electronSource
##     ganew("elPFIsoDepositNeutral").src = electronSource
##     ganew("elPFIsoDepositGamma").src = electronSource
##     ganew("elPFIsoDepositPU").src = electronSource
##     ganew("patElectrons").useParticleFlow = False
##     ganew("selectedPatElectrons").cut = cut
##     if runOnMC:
##         ganew("electronMatch").src = electronSource
##     else:
##         ganew('makePatElectrons').remove( ganew("electronMatch") )

##     return ganew('stdElectronSequence')

