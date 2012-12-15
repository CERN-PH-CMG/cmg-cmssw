#!/bin/env python
from CMGTools.RootTools.utils.DeltaR import deltaR
from CMGTools.TTHAnalysis.signedSip import *

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
    var(tree, '{pName}_pdgId'.format(pName=pName), int)
    var(tree, '{pName}_charge'.format(pName=pName), int)
    var(tree, '{pName}_sip3d'.format(pName=pName))
    var(tree, '{pName}_sip3ds'.format(pName=pName))
    var(tree, '{pName}_ip3ds'.format(pName=pName))
    var(tree, '{pName}_dxy'.format(pName=pName))
    var(tree, '{pName}_dz'.format(pName=pName))
    var(tree, '{pName}_relIso'.format(pName=pName))
    var(tree, '{pName}_chargedIso'.format(pName=pName))
    var(tree, '{pName}_drBJet'.format(pName=pName))
    var(tree, '{pName}_jetPtRatio'.format(pName=pName))
    var(tree, '{pName}_jetBTagCSV'.format(pName=pName))
    var(tree, '{pName}_jetBTagTCHE'.format(pName=pName))
    var(tree, '{pName}_ptRelJet'.format(pName=pName))
    var(tree, '{pName}_jetDR'.format(pName=pName))
    var(tree, '{pName}_jetSelf'.format(pName=pName), int)
    var(tree, '{pName}_innerHits'.format(pName=pName), int)
    var(tree, '{pName}_mvaId'.format(pName=pName))
    var(tree, '{pName}_tightId'.format(pName=pName))
    var(tree, '{pName}_mva'.format(pName=pName))
    if isMC:
        var(tree, '{pName}_mcMatchId'.format(pName=pName), int)
        var(tree, '{pName}_mcMatchAny'.format(pName=pName), int)
        var(tree, '{pName}_mcDeltaRB'.format(pName=pName))
    
def fillLepton( tree, pName, lepton ):
    fillParticle(tree, pName, lepton )
    fill(tree, '{pName}_pdgId'.format(pName=pName), lepton.pdgId() )
    fill(tree, '{pName}_charge'.format(pName=pName), lepton.charge() )
    fill(tree, '{pName}_sip3d'.format(pName=pName), lepton.sip3D() )
    fill(tree, '{pName}_sip3ds'.format(pName=pName), signedSip3D(lepton) )
    fill(tree, '{pName}_ip3ds'.format(pName=pName),  signedIp3D(lepton) )
    fill(tree, '{pName}_dxy'.format(pName=pName), abs(lepton.dxy()) )
    fill(tree, '{pName}_dz'.format(pName=pName), abs(lepton.dz()) )
    fill(tree, '{pName}_relIso'.format(pName=pName), lepton.relIso(dBetaFactor=0.5))
    fill(tree, '{pName}_chargedIso'.format(pName=pName), lepton.chargedHadronIso())
    fill(tree, '{pName}_drBJet'.format(pName=pName), lepton.drBJet)
    if hasattr(lepton, 'jet'):
        fill(tree, '{pName}_ptRelJet'.format(pName=pName), lepton.ptRelJet)
        fill(tree, '{pName}_jetPtRatio'.format(pName=pName), lepton.pt()/lepton.jet.pt())
        fill(tree, '{pName}_jetBTagCSV'.format(pName=pName), 
            lepton.jet.btag('combinedSecondaryVertexBJetTags') if hasattr(lepton.jet, 'btag') else -99)
        fill(tree, '{pName}_jetBTagTCHE'.format(pName=pName), 
            lepton.jet.btag('trackCountingHighEffBJetTags') if hasattr(lepton.jet, 'btag') else -99)
        fill(tree, '{pName}_jetDR'.format(pName=pName), deltaR(lepton.eta(),lepton.phi(),lepton.jet.eta(),lepton.jet.phi()))
        fill(tree, '{pName}_jetSelf'.format(pName=pName), lepton.jet.pdgId() == lepton.pdgId())
    if abs(lepton.pdgId())==11:
        fill(tree, '{pName}_innerHits'.format(pName=pName), lepton.numberOfHits())
        fill(tree, '{pName}_mvaId'.format(pName=pName), lepton.mvaNonTrigV0())
    if abs(lepton.pdgId())==13:
        fill(tree, '{pName}_innerHits'.format(pName=pName), lepton.sourcePtr().innerTrack().hitPattern().numberOfValidPixelHits())
    if hasattr(lepton, 'jet'):    
        fill(tree, '{pName}_mva'.format(pName=pName), lepton.mvaValue)
        fill(tree, '{pName}_tightId'.format(pName=pName), lepton.tightId())
    if hasattr(lepton, 'mcMatchId'):
        fill(tree, '{pName}_mcMatchId'.format(pName=pName), lepton.mcMatchId)
        fill(tree, '{pName}_mcMatchAny'.format(pName=pName), lepton.mcMatchAny)
        fill(tree, '{pName}_mcDeltaRB'.format(pName=pName), lepton.mcDeltaRB)
   
     

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

def bookJet( tree, pName, isMC=False ):
    bookParticle(tree, pName )
    var(tree, '{pName}_btagCSV'.format(pName=pName))
#     var(tree, '{pName}_puMvaFull'.format(pName=pName))
#     var(tree, '{pName}_puMvaSimple'.format(pName=pName))
#     var(tree, '{pName}_puMvaCutBased'.format(pName=pName))
#     var(tree, '{pName}_puJetId'.format(pName=pName))
#     var(tree, '{pName}_looseJetId'.format(pName=pName))
#     var(tree, '{pName}_chFrac'.format(pName=pName))
#     var(tree, '{pName}_eFrac'.format(pName=pName))
#     var(tree, '{pName}_muFrac'.format(pName=pName))
#     var(tree, '{pName}_gammaFrac'.format(pName=pName))
#     var(tree, '{pName}_h0Frac'.format(pName=pName))
#     var(tree, '{pName}_hhfFrac'.format(pName=pName))
#     var(tree, '{pName}_ehfFrac'.format(pName=pName))
    bookParticle(tree, '{pName}_leg'.format(pName=pName))
    if isMC:
        var(tree, '{pName}_mcMatchId'.format(pName=pName),   int)
        var(tree, '{pName}_mcMatchFlav'.format(pName=pName), int)

def fillJet( tree, pName, jet ):
    fillParticle(tree, pName, jet )
    fill(tree, '{pName}_btagCSV'.format(pName=pName), jet.btag('combinedSecondaryVertexBJetTags') )
#     fill(tree, '{pName}_puMvaFull'.format(pName=pName), jet.puMva('full') )
#     fill(tree, '{pName}_puMvaSimple'.format(pName=pName), jet.puMva('simple'))
#     fill(tree, '{pName}_puMvaCutBased'.format(pName=pName), jet.puMva('cut-based'))
#     fill(tree, '{pName}_puJetId'.format(pName=pName), jet.puJetIdPassed)
#     fill(tree, '{pName}_looseJetId'.format(pName=pName), jet.pfJetIdPassed)
#     fill(tree, '{pName}_chFrac'.format(pName=pName), jet.component(1).fraction() )
#     fill(tree, '{pName}_eFrac'.format(pName=pName), jet.component(2).fraction() )
#     fill(tree, '{pName}_muFrac'.format(pName=pName), jet.component(3).fraction() )
#     fill(tree, '{pName}_gammaFrac'.format(pName=pName), jet.component(4).fraction() )
#     fill(tree, '{pName}_h0Frac'.format(pName=pName), jet.component(5).fraction() )
#     fill(tree, '{pName}_hhfFrac'.format(pName=pName), jet.component(6).fraction() )
#     fill(tree, '{pName}_ehfFrac'.format(pName=pName), jet.component(7).fraction() )
    if hasattr(jet, 'leg') and jet.leg:
        fillParticle(tree, '{pName}_leg'.format(pName=pName), jet.leg )
    if hasattr(jet, 'mcMatchId'):
        fill(tree, '{pName}_mcMatchId'.format(pName=pName),   jet.mcMatchId)
        fill(tree, '{pName}_mcMatchFlav'.format(pName=pName), jet.mcMatchFlav)
    
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
 
