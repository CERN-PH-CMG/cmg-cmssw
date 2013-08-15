#!/bin/env python
from CMGTools.RootTools.utils.DeltaR import deltaR
from CMGTools.TTHAnalysis.signedSip import *
import ROOT

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

#-------------------
# GENERIC PARTICLE
#-------------------

def bookParticle( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_energy'.format(pName=pName))

def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_energy'.format(pName=pName), particle.energy() )
    


#----------
# LEPTON
#----------

def bookLepton( tree, pName, isMC=False ):
    bookParticle(tree, pName )
    var(tree, '{pName}_pfpt'.format(pName=pName))
    var(tree, '{pName}_pdgId'.format(pName=pName), int)
    var(tree, '{pName}_charge'.format(pName=pName), int)
    var(tree, '{pName}_sip3d'.format(pName=pName))
    var(tree, '{pName}_sip3ds'.format(pName=pName))
    var(tree, '{pName}_ip3ds'.format(pName=pName))
    var(tree, '{pName}_dxy'.format(pName=pName))
    var(tree, '{pName}_dz'.format(pName=pName))
    var(tree, '{pName}_relIso'.format(pName=pName))
    var(tree, '{pName}_chargedIso'.format(pName=pName))
    var(tree, '{pName}_mvaIso'.format(pName=pName))
    var(tree, '{pName}_jetPtRatio'.format(pName=pName))
    var(tree, '{pName}_jetBTagCSV'.format(pName=pName))
    #var(tree, '{pName}_jetBTagTCHE'.format(pName=pName))
    #var(tree, '{pName}_ptRelJet'.format(pName=pName))
    var(tree, '{pName}_jetDR'.format(pName=pName))
    #var(tree, '{pName}_jetSelf'.format(pName=pName), int)
    #var(tree, '{pName}_jetChHadMultiplicity'.format(pName=pName), int)
    #var(tree, '{pName}_jetChHadEnergyFraction'.format(pName=pName))
    #var(tree, '{pName}_jetPhoMultiplicity'.format(pName=pName), int)
    #var(tree, '{pName}_jetPhoEnergyFraction'.format(pName=pName))
    #var(tree, '{pName}_jetMuEnergyFraction'.format(pName=pName))
    var(tree, '{pName}_innerHits'.format(pName=pName), int)
    var(tree, '{pName}_mvaId'.format(pName=pName))
    var(tree, '{pName}_tightId'.format(pName=pName))
    var(tree, '{pName}_convVeto'.format(pName=pName))
    var(tree, '{pName}_relIso03'.format(pName=pName))
    var(tree, '{pName}_mva'.format(pName=pName))
    var(tree, '{pName}_tightCharge'.format(pName=pName), int)
    if isMC:
        var(tree, '{pName}_mcMatchId'.format(pName=pName), int)
        var(tree, '{pName}_mcMatchAny'.format(pName=pName), int)
        var(tree, '{pName}_mcDeltaRB'.format(pName=pName))
        var(tree, '{pName}_mvaNoCorr'.format(pName=pName))
        var(tree, '{pName}_mvaDoubleCorr'.format(pName=pName))
    # charge misid vars (Jason)
    var(tree, '{pName}_eleQ_gsfCtfQ'.format(pName=pName), int)
    var(tree, '{pName}_eleQ_pRelDiff'.format(pName=pName))
    ## # ele ID vars (Riccardo)
    ## var(tree, '{pName}_eleId_validKF'.format(pName=pName), int)
    ## var(tree, '{pName}_eleId_fbrem'.format(pName=pName))
    ## var(tree, '{pName}_eleId_kfchi2'.format(pName=pName))
    ## var(tree, '{pName}_eleId_kfhits'.format(pName=pName))
    ## var(tree, '{pName}_eleId_gsfchi2'.format(pName=pName))
    ## var(tree, '{pName}_eleId_deta'.format(pName=pName))
    ## var(tree, '{pName}_eleId_dphi'.format(pName=pName))
    ## #var(tree, '{pName}_eleId_etawidth'.format(pName=pName))
    ## #var(tree, '{pName}_eleId_phiwidth'.format(pName=pName))
    ## var(tree, '{pName}_eleId_detacalo'.format(pName=pName))
    ## var(tree, '{pName}_eleId_see'.format(pName=pName))
    ## var(tree, '{pName}_eleId_e1x5e5x5'.format(pName=pName))
    ## #var(tree, '{pName}_eleId_HoE'.format(pName=pName))
    ## var(tree, '{pName}_eleId_EoP'.format(pName=pName))
    ## var(tree, '{pName}_eleId_IoEmIoP'.format(pName=pName))
    ## var(tree, '{pName}_eleId_eleEoPout'.format(pName=pName))
    ## var(tree, '{pName}_eleId_PreShowerOverRaw'.format(pName=pName))
    
def fillLepton( tree, pName, lepton ):
    fillParticle(tree, pName, lepton )
    fill(tree, '{pName}_pdgId'.format(pName=pName), lepton.pdgId() )
    fill(tree, '{pName}_charge'.format(pName=pName), lepton.charge() )
    fill(tree, '{pName}_sip3d'.format(pName=pName), lepton.sip3D() )
    fill(tree, '{pName}_sip3ds'.format(pName=pName), signedSip3D(lepton) )
    fill(tree, '{pName}_ip3ds'.format(pName=pName),  signedIp3D(lepton) )
    fill(tree, '{pName}_dxy'.format(pName=pName), lepton.dxy() )
    fill(tree, '{pName}_dz'.format(pName=pName), lepton.dz() )
    fill(tree, '{pName}_relIso'.format(pName=pName), lepton.relIso(dBetaFactor=0.5))
    fill(tree, '{pName}_chargedIso'.format(pName=pName), lepton.chargedHadronIso())
    fill(tree, '{pName}_mvaIso'.format(pName=pName), lepton.mvaIso())
    if hasattr(lepton, 'jet'):
        #fill(tree, '{pName}_ptRelJet'.format(pName=pName), lepton.ptRelJet)
        fill(tree, '{pName}_jetPtRatio'.format(pName=pName), lepton.pt()/lepton.jet.pt())
        fill(tree, '{pName}_jetBTagCSV'.format(pName=pName), 
            lepton.jet.btag('combinedSecondaryVertexBJetTags') if hasattr(lepton.jet, 'btag') else -99)
        #fill(tree, '{pName}_jetBTagTCHE'.format(pName=pName), 
        #    lepton.jet.btag('trackCountingHighEffBJetTags') if hasattr(lepton.jet, 'btag') else -99)
        fill(tree, '{pName}_jetDR'.format(pName=pName), deltaR(lepton.eta(),lepton.phi(),lepton.jet.eta(),lepton.jet.phi()))
        #fill(tree, '{pName}_jetSelf'.format(pName=pName), lepton.jet.pdgId() == lepton.pdgId())
        #fill(tree, '{pName}_jetChHadMultiplicity'.format(pName=pName), lepton.jet.component(1).number() if hasattr(lepton.jet, 'component') else 0)
        #fill(tree, '{pName}_jetChHadEnergyFraction'.format(pName=pName), lepton.jet.component(1).fraction() if hasattr(lepton.jet, 'component') else 0)
        #fill(tree, '{pName}_jetPhoMultiplicity'.format(pName=pName), lepton.jet.component(4).number() if hasattr(lepton.jet, 'component') else 0)
        #fill(tree, '{pName}_jetPhoEnergyFraction'.format(pName=pName), lepton.jet.component(4).fraction() if hasattr(lepton.jet, 'component') else 0)
        #fill(tree, '{pName}_jetMuEnergyFraction'.format(pName=pName), lepton.jet.component(3).fraction() if hasattr(lepton.jet, 'component') else 0)
    if abs(lepton.pdgId())==11:
        fill(tree, '{pName}_pfpt'.format(pName=pName), lepton.sourcePtr().p4(ROOT.reco.GsfElectron.P4_PFLOW_COMBINATION).Pt())
        fill(tree, '{pName}_innerHits'.format(pName=pName), lepton.numberOfHits())
        fill(tree, '{pName}_mvaId'.format(pName=pName), lepton.mvaNonTrigV0())
        fill(tree, '{pName}_tightId'.format(pName=pName), lepton.mvaTrigV0())
        fill(tree, '{pName}_convVeto'.format(pName=pName), lepton.sourcePtr().passConversionVeto())
        #fill(tree, '{pName}_relIso03'.format(pName=pName), (lepton.chargedHadronIso(0.3) + max(((lepton.neutralHadronIso(0.3)+lepton.photonIso(0.3))-0.5*(lepton.puChargedHadronIso(0.3))),0)))
        fill(tree, '{pName}_relIso03'.format(pName=pName), lepton.chargedHadronIso(0.3) + max(lepton.neutralHadronIso(0.3)+lepton.photonIso(0.3)-lepton.rho*lepton.EffectiveArea,0))
    if abs(lepton.pdgId())==13:
        fill(tree, '{pName}_pfpt'.format(pName=pName), lepton.sourcePtr().pfP4().Pt())
        fill(tree, '{pName}_innerHits'.format(pName=pName), lepton.sourcePtr().innerTrack().numberOfValidHits())
        fill(tree, '{pName}_tightId'.format(pName=pName), lepton.tightId())
    if hasattr(lepton, 'jet'):    
        fill(tree, '{pName}_mva'.format(pName=pName), lepton.mvaValue)
        if hasattr(lepton, 'mvaNoCorr'):
            fill(tree, '{pName}_mvaNoCorr'.format(pName=pName), lepton.mvaNoCorr)
            fill(tree, '{pName}_mvaDoubleCorr'.format(pName=pName), lepton.mvaDoubleCorr)
    if hasattr(lepton, 'mcMatchId'):
        fill(tree, '{pName}_mcMatchId'.format(pName=pName), lepton.mcMatchId)
        fill(tree, '{pName}_mcMatchAny'.format(pName=pName), lepton.mcMatchAny)
        fill(tree, '{pName}_mcDeltaRB'.format(pName=pName), lepton.mcDeltaRB)
    if abs(lepton.pdgId()) == 13:
        fill(tree, '{pName}_tightCharge'.format(pName=pName), 
                lepton.sourcePtr().innerTrack().ptError()/lepton.sourcePtr().innerTrack().pt() < 0.2)
    elif abs(lepton.pdgId()) == 11:
        fill(tree, '{pName}_tightCharge'.format(pName=pName), 
                lepton.sourcePtr().isGsfCtfScPixChargeConsistent() + lepton.sourcePtr().isGsfScPixChargeConsistent())
    # charge misid vars (Jason)
    if abs(lepton.pdgId()) == 11:
        ele = lepton.sourcePtr()
        fill(tree, '{pName}_eleQ_pRelDiff'.format(pName=pName), abs(ele.superCluster().energy()-ele.gsfTrack().p())/ele.gsfTrack().p())
        fill(tree, '{pName}_eleQ_gsfCtfQ'.format(pName=pName), ele.isGsfCtfChargeConsistent())
    ## # ele ID vars (Riccardo)
    ## if abs(lepton.pdgId()) == 11:
        ## ele = lepton.sourcePtr()
        ## fill(tree, '{pName}_eleId_validKF'.format(pName=pName), ele.closestCtfTrackRef().isNonnull())
        ## fill(tree, '{pName}_eleId_fbrem'.format(pName=pName), ele.fbrem())
        ## fill(tree, '{pName}_eleId_kfchi2'.format(pName=pName), ele.closestCtfTrackRef().normalizedChi2() if ele.closestCtfTrackRef().isNonnull() else 0)
        ## fill(tree, '{pName}_eleId_kfhits'.format(pName=pName), ele.closestCtfTrackRef().hitPattern().trackerLayersWithMeasurement() if ele.closestCtfTrackRef().isNonnull() else -1)
        ## fill(tree, '{pName}_eleId_gsfchi2'.format(pName=pName), ele.gsfTrack().normalizedChi2())
        ## fill(tree, '{pName}_eleId_deta'.format(pName=pName), ele.deltaEtaSuperClusterTrackAtVtx())
        ## fill(tree, '{pName}_eleId_dphi'.format(pName=pName), ele.deltaPhiSuperClusterTrackAtVtx())
        ## #fill(tree, '{pName}_eleId_etawidth'.format(pName=pName), ele.superCluster().etaWidth()) #(not used?))
        ## #fill(tree, '{pName}_eleId_phiwidth'.format(pName=pName), ele.superCluster().phiWidth()) #(not used?))
        ## fill(tree, '{pName}_eleId_detacalo'.format(pName=pName), ele.deltaEtaSeedClusterTrackAtCalo())
        ## fill(tree, '{pName}_eleId_see'.format(pName=pName), ele.sigmaIetaIeta())
        ## fill(tree, '{pName}_eleId_e1x5e5x5'.format(pName=pName), 1.-ele.e1x5()/ele.e5x5() if ele.e5x5() != 0. else  -1.)
        ## #fill(tree, '{pName}_eleId_HoE'.format(pName=pName), ele.hadronicOverEm()) #(not used?))
        ## fill(tree, '{pName}_eleId_EoP'.format(pName=pName), ele.eSuperClusterOverP())
        ## #fill(tree, '{pName}_eleId_IoEmIoP'.format(pName=pName), 1.0/ele.superCluster().energy() - 1.0/ele.gsfTrack().p() )
        ## fill(tree, '{pName}_eleId_IoEmIoP'.format(pName=pName), 1.0/ele.ecalEnergy() - 1.0/ele.p() )
        ## fill(tree, '{pName}_eleId_eleEoPout'.format(pName=pName), ele.eEleClusterOverPout())
        ## fill(tree, '{pName}_eleId_PreShowerOverRaw'.format(pName=pName), ele.superCluster().preshowerEnergy()/ele.superCluster().rawEnergy())
   
     

# jet

##    /// particle types
##     enum ParticleType {
##       X=0,     // undefined
##       h,       // charged hadron
##       e,       // electron 
##       mu,      // muon 
##       gamma,   // photon
##       h0,      // neutral hadron
##       h_HF,        // HF tower identified as a hadron
##       egamma_HF    // HF tower identified as an EM particle
##     };

def bookJet( tree, pName, isMC=False, saveID = False ):
    bookParticle(tree, pName )
    var(tree, '{pName}_btagCSV'.format(pName=pName))
#     var(tree, '{pName}_puMvaFull'.format(pName=pName))
#     var(tree, '{pName}_puMvaSimple'.format(pName=pName))
#     var(tree, '{pName}_puMvaCutBased'.format(pName=pName))
    if saveID:
        var(tree, '{pName}_puJetId'.format(pName=pName), int)
        var(tree, '{pName}_looseJetId'.format(pName=pName), int)
#     var(tree, '{pName}_chFrac'.format(pName=pName))
#     var(tree, '{pName}_eFrac'.format(pName=pName))
#     var(tree, '{pName}_muFrac'.format(pName=pName))
#     var(tree, '{pName}_gammaFrac'.format(pName=pName))
#     var(tree, '{pName}_h0Frac'.format(pName=pName))
#     var(tree, '{pName}_hhfFrac'.format(pName=pName))
    var(tree, '{pName}_rawPt'.format(pName=pName))
    bookParticle(tree, '{pName}_leg'.format(pName=pName))
    if isMC:
        var(tree, '{pName}_mcMatchId'.format(pName=pName),   int)
        var(tree, '{pName}_mcMatchFlav'.format(pName=pName), int)
        var(tree, '{pName}_mcFlavour'.format(pName=pName), int)
        #var(tree, '{pName}_mcFlavour2'.format(pName=pName), int)
        var(tree, '{pName}_mcPt'.format(pName=pName))

def fillJet( tree, pName, jet, saveID = False ):
    fillParticle(tree, pName, jet )
    fill(tree, '{pName}_btagCSV'.format(pName=pName), jet.btag('combinedSecondaryVertexBJetTags') )
#     fill(tree, '{pName}_puMvaFull'.format(pName=pName), jet.puMva('full') )
#     fill(tree, '{pName}_puMvaSimple'.format(pName=pName), jet.puMva('simple'))
#     fill(tree, '{pName}_puMvaCutBased'.format(pName=pName), jet.puMva('cut-based'))
    if saveID:
        fill(tree, '{pName}_puJetId'.format(pName=pName), jet.puJetIdPassed)
        fill(tree, '{pName}_looseJetId'.format(pName=pName), jet.pfJetIdPassed)
#     fill(tree, '{pName}_puJetId'.format(pName=pName), jet.puJetIdPassed)
#     fill(tree, '{pName}_looseJetId'.format(pName=pName), jet.pfJetIdPassed)
#     fill(tree, '{pName}_chFrac'.format(pName=pName), jet.component(1).fraction() )
#     fill(tree, '{pName}_eFrac'.format(pName=pName), jet.component(2).fraction() )
#     fill(tree, '{pName}_muFrac'.format(pName=pName), jet.component(3).fraction() )
#     fill(tree, '{pName}_gammaFrac'.format(pName=pName), jet.component(4).fraction() )
#     fill(tree, '{pName}_h0Frac'.format(pName=pName), jet.component(5).fraction() )
#     fill(tree, '{pName}_hhfFrac'.format(pName=pName), jet.component(6).fraction() )
    fill(tree, '{pName}_rawPt'.format(pName=pName), jet.pt() * jet.rawFactor())
    if hasattr(jet, 'leg') and jet.leg:
        fillParticle(tree, '{pName}_leg'.format(pName=pName), jet.leg )
    if hasattr(jet, 'mcMatchId'):
        fill(tree, '{pName}_mcMatchId'.format(pName=pName),   jet.mcMatchId)
        fill(tree, '{pName}_mcMatchFlav'.format(pName=pName), jet.mcMatchFlav)
        fill(tree, '{pName}_mcFlavour'.format(pName=pName), abs(jet.partonFlavour())) ## official definition
        #fill(tree, '{pName}_mcFlavour2'.format(pName=pName), jet.mcFlavour)
        fill(tree, '{pName}_mcPt'.format(pName=pName), jet.mcJet.pt() if jet.mcJet else 0)
    
#----------
# PHOTONS
#----------

def bookPhoton( tree, pName, isMC=False ):
    bookParticle(tree, pName )
    var(tree, '{pName}_chargedIso'.format(pName=pName))
    var(tree, '{pName}_relIso'.format(pName=pName))
    var(tree, '{pName}_relIsoAntiPU'.format(pName=pName))

def fillPhoton( tree, pName, photon ):
    fillParticle(tree, pName, photon )
    fill(tree, '{pName}_chargedIso'.format(pName=pName),   photon.chargedHadronIso()/photon.pt())
    fill(tree, '{pName}_relIso'.format(pName=pName),       photon.relIso(0.5))   # dbeta corr
    fill(tree, '{pName}_relIsoAntiPU'.format(pName=pName), photon.relIso(-1.0))  # include full PU contribution

#----------
# GEN PARTICLE
#----------

def bookGenParticle( tree, pName, withSourceId=False ):
    bookParticle(tree, pName )
    var(tree, '{pName}_pdgId'.format(pName=pName), int)
    var(tree, '{pName}_charge'.format(pName=pName), int)
    if withSourceId:
        var(tree, '{pName}_sourceId'.format(pName=pName), int)
    
def fillGenParticle( tree, pName, lepton, withSourceId=False ):
    fillParticle(tree, pName, lepton )
    fill(tree, '{pName}_pdgId'.format(pName=pName), lepton.pdgId() )
    fill(tree, '{pName}_charge'.format(pName=pName), lepton.charge() )
    if withSourceId:
        fill(tree, '{pName}_sourceId'.format(pName=pName), lepton.sourceId )
 
