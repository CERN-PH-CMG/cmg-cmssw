import FWCore.ParameterSet.Config as cms

electronUserIsolation  = cms.PSet(
    user = cms.VPSet(
    cms.PSet( src = cms.InputTag("eleIsoFromDepsTkOptimized5") ),
    cms.PSet( src = cms.InputTag("eleIsoFromDepsTkOptimized7") ),
    )         
)

muonUserIsolation  = cms.PSet(
    user = cms.VPSet(
    cms.PSet( src = cms.InputTag("muIsoFromDepsTkOptimized") )  
    )         
)

def addElectronCustomIsoDeposit( process, sequenceName, postfix ):
    """
    Adds custom isoDeposit to the PAT electron
    """

    userIso = getattr(process, "patElectrons" + postfix).userIsolation
    if not userIso.hasParameter('user') : userIso.user = electronUserIsolation.user
    else : userIso.user += electronUserIsolation.user
#    print userIso ###

    userIsoD = getattr(process, "patElectrons" + postfix).isoDeposits
    userIsoD.tracker = cms.InputTag("eleIsoDepositTk")
#    userIsoD.ecal = cms.InputTag("eleIsoDepositEcalFromHits")
#    userIsoD.hcal = cms.InputTag("eleIsoDepositHcalFromTowers")
#    print userIsoD ###


    process.load('CMGTools.Common.PAT.patLeptModifiedIsoDeposit_cff')
    
    getattr(process, sequenceName + postfix).replace(
            getattr(process, "patElectrons" + postfix),
            process.patElectronIsoDeposit  + getattr(process, "patElectrons" + postfix) 
            )

def addMuonCustomIsoDeposit( process, sequenceName, postfix ):
    """
    Adds custom isoDeposit to the PAT muons
    """

    userIso = getattr(process, "patMuons" + postfix).userIsolation
    if not userIso.hasParameter('user') : userIso.user = muonUserIsolation.user
    else : userIso.user += muonUserIsolation.user
#    print userIso ###

    userIsoD = getattr(process, "patMuons" + postfix).isoDeposits
    userIsoD.tracker = cms.InputTag("muIsoDepositTk")
    userIsoD.ecal    = cms.InputTag("muIsoDepositCalByAssociatorTowers","ecal")
    userIsoD.hcal    = cms.InputTag("muIsoDepositCalByAssociatorTowers","hcal")
## Example of adding user iso
#     if not userIsoD.hasParameter('user') : userIsoD.user = muonIsoDeposits.user
#     else : userIsoD.user += muonIsoDeposits.user
#    print userIsoD ###


    process.load('CMGTools.Common.PAT.patLeptModifiedIsoDeposit_cff')
    
    getattr(process, sequenceName + postfix).replace(
            getattr(process, "patMuons" + postfix),
            process.patMuonIsoDeposit  + getattr(process, "patMuons" + postfix) 
            )
    

    
