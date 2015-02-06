#!/bin/env python

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

# simple particle

def bookParticle( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_charge'.format(pName=pName))

def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )

def bookGenParticle(tree, pName):
    bookParticle(tree, pName)
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_pdgId'.format(pName=pName))
    
def fillGenParticle( tree, pName, particle ):
    fillParticle( tree, pName, particle )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_pdgId'.format(pName=pName), particle.pdgId() )

# di-tau

def bookDiLepton(tree):
    var( tree, 'visMass')
    var( tree, 'svfitMass')
    var( tree, 'pZetaMET')
    var( tree, 'pZetaVis')
    var( tree, 'pZetaDisc')
    var( tree, 'mt')
    var( tree, 'met')


def fillDiLepton(tree, diLepton):
    fill(tree, 'visMass', diLepton.mass())
    fill(tree, 'svfitMass', diLepton.massSVFit())
    fill(tree, 'pZetaMET', diLepton.pZetaMET())
    fill(tree, 'pZetaVis', diLepton.pZetaVis())
    fill(tree, 'pZetaDisc', diLepton.pZetaDisc())
    fill(tree, 'mt', diLepton.mTLeg2())
    fill(tree, 'met', diLepton.met().pt())

    
# lepton

def bookLepton( tree, pName ):
    bookParticle(tree, pName )
    var(tree, '{pName}_relIso05'.format(pName=pName))
    var(tree, '{pName}_dxy'.format(pName=pName))
    var(tree, '{pName}_dz'.format(pName=pName))
##     var(tree, '{pName}_weight'.format(pName=pName))
##     var(tree, '{pName}_triggerWeight'.format(pName=pName))
##     var(tree, '{pName}_triggerEffData'.format(pName=pName))
##     var(tree, '{pName}_triggerEffMC'.format(pName=pName))
##     var(tree, '{pName}_recEffWeight'.format(pName=pName))

def fillLepton( tree, pName, lepton ):
    fillParticle(tree, pName, lepton )
    fill(tree, '{pName}_relIso05'.format(pName=pName), lepton.relIsoAllChargedDB05() )
    fill(tree, '{pName}_dxy'.format(pName=pName), lepton.dxy() )
    fill(tree, '{pName}_dz'.format(pName=pName), lepton.dz() )
##     fill(tree, '{pName}_weight'.format(pName=pName), lepton.weight )
##     fill(tree, '{pName}_triggerWeight'.format(pName=pName), lepton.triggerWeight )
##     fill(tree, '{pName}_triggerEffData'.format(pName=pName), lepton.triggerEffData )
##     fill(tree, '{pName}_triggerEffMC'.format(pName=pName), lepton.triggerEffMC )
##     fill(tree, '{pName}_recEffWeight'.format(pName=pName), lepton.recEffWeight )


# muon


def bookMuon( tree, pName ):
    bookLepton(tree, pName )
    var(tree, '{pName}_mvaIso'.format(pName=pName))
    var(tree, '{pName}_looseId'.format(pName=pName))
    var(tree, '{pName}_tightId'.format(pName=pName))

def fillMuon( tree, pName, muon ):
    fillLepton(tree, pName, muon)
    fill(tree, '{pName}_mvaIso'.format(pName=pName), muon.mvaIso() )
    fill(tree, '{pName}_looseId'.format(pName=pName), muon.looseId() )
    fill(tree, '{pName}_tightId'.format(pName=pName), muon.tightId() )


# electron


def bookEle( tree, pName ):
    bookLepton(tree, pName )
    var(tree, '{pName}_mvaIso'.format(pName=pName))
    var(tree, '{pName}_mvaTrigV0'.format(pName=pName))
    var(tree, '{pName}_mvaNonTrigV0'.format(pName=pName))
    var(tree, '{pName}_looseId'.format(pName=pName))
    var(tree, '{pName}_tightId'.format(pName=pName))

def fillEle( tree, pName, ele ):
    fillLepton(tree, pName, ele)
    fill(tree, '{pName}_mvaIso'.format(pName=pName), ele.mvaIso() )
    fill(tree, '{pName}_mvaTrigV0'.format(pName=pName), ele.sourcePtr().electronID("mvaTrigV0") )
    fill(tree, '{pName}_mvaNonTrigV0'.format(pName=pName), ele.sourcePtr().electronID("mvaNonTrigV0") )
    fill(tree, '{pName}_looseId'.format(pName=pName), ele.looseIdForEleTau() )
    fill(tree, '{pName}_tightId'.format(pName=pName), ele.tightIdForEleTau() )


# tau 

def bookTau( tree, pName ):
    bookLepton(tree, pName )
    var(tree, '{pName}_veryLooseIso'.format(pName=pName))
    var(tree, '{pName}_looseIso'.format(pName=pName))
    var(tree, '{pName}_mediumIso'.format(pName=pName))
    var(tree, '{pName}_tightIso'.format(pName=pName))

    var(tree, '{pName}_againstMuonTight'.format(pName=pName))    
    var(tree, '{pName}_againstElectronLoose'.format(pName=pName))    

    var(tree, '{pName}_byLooseIsoMVA'.format(pName=pName))    
    var(tree, '{pName}_againstElectronMVA'.format(pName=pName))    
    var(tree, '{pName}_againstElectronTightMVA2'.format(pName=pName))
    var(tree, '{pName}_againstElectronMedium'.format(pName=pName))    
    var(tree, '{pName}_againstMuonLoose'.format(pName=pName))    

    var(tree, '{pName}_rawMvaIso'.format(pName=pName))
    var(tree, '{pName}_looseMvaIso'.format(pName=pName))
    var(tree, '{pName}_mediumMvaIso'.format(pName=pName))
    var(tree, '{pName}_tightMvaIso'.format(pName=pName))
   
    var(tree, '{pName}_EOverp'.format(pName=pName))
    var(tree, '{pName}_decayMode'.format(pName=pName))

def fillTau( tree, pName, tau ):
    fillLepton(tree, pName, tau)
    fill(tree, '{pName}_veryLooseIso'.format(pName=pName),
         tau.tauID("byVLooseCombinedIsolationDeltaBetaCorr"))
    fill(tree, '{pName}_looseIso'.format(pName=pName),
         tau.tauID("byLooseCombinedIsolationDeltaBetaCorr"))
    fill(tree, '{pName}_mediumIso'.format(pName=pName),
         tau.tauID("byMediumCombinedIsolationDeltaBetaCorr"))
    fill(tree, '{pName}_tightIso'.format(pName=pName),
         tau.tauID("byTightCombinedIsolationDeltaBetaCorr"))

    fill(tree, '{pName}_againstMuonTight'.format(pName=pName),
         tau.tauID("againstMuonTight"))
    fill(tree, '{pName}_againstElectronLoose'.format(pName=pName),
         tau.tauID("againstElectronLoose"))

    fill(tree, '{pName}_byLooseIsoMVA'.format(pName=pName),
         tau.tauID("byLooseIsoMVA"))
    fill(tree, '{pName}_againstElectronMVA'.format(pName=pName),
         tau.tauID("againstElectronMVA"))
    fill(tree, '{pName}_againstElectronTightMVA2'.format(pName=pName),
         tau.tauID("againstElectronTightMVA2"))
    fill(tree, '{pName}_againstElectronMedium'.format(pName=pName),
         tau.tauID("againstElectronMedium"))
    fill(tree, '{pName}_againstMuonLoose'.format(pName=pName),
         tau.tauID("againstMuonLoose"))

    fill(tree, '{pName}_rawMvaIso'.format(pName=pName),
         tau.tauID("byRawIsoMVA"))
    fill(tree, '{pName}_looseMvaIso'.format(pName=pName),
         tau.tauID("byLooseIsoMVA"))
    fill(tree, '{pName}_mediumMvaIso'.format(pName=pName),
         tau.tauID("byMediumIsoMVA"))
    fill(tree, '{pName}_tightMvaIso'.format(pName=pName),
         tau.tauID("byTightIsoMVA"))

    fill(tree, '{pName}_rawMvaIso'.format(pName=pName),
         tau.tauID("byRawIsoMVA"))
    fill(tree, '{pName}_EOverp'.format(pName=pName),
         tau.calcEOverP())
    fill(tree, '{pName}_decayMode'.format(pName=pName),
         tau.decayMode())


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

def bookJet( tree, pName ):
    bookParticle(tree, pName )
    var(tree, '{pName}_puMvaFull'.format(pName=pName))
    var(tree, '{pName}_puMvaSimple'.format(pName=pName))
    var(tree, '{pName}_puMvaCutBased'.format(pName=pName))
    var(tree, '{pName}_puJetId'.format(pName=pName))
    var(tree, '{pName}_looseJetId'.format(pName=pName))
    var(tree, '{pName}_chFrac'.format(pName=pName))
    var(tree, '{pName}_eFrac'.format(pName=pName))
    var(tree, '{pName}_muFrac'.format(pName=pName))
    var(tree, '{pName}_gammaFrac'.format(pName=pName))
    var(tree, '{pName}_h0Frac'.format(pName=pName))
    var(tree, '{pName}_hhfFrac'.format(pName=pName))
    var(tree, '{pName}_ehfFrac'.format(pName=pName))
    bookParticle(tree, '{pName}_leg'.format(pName=pName))

def fillJet( tree, pName, jet ):
    fillParticle(tree, pName, jet )
    fill(tree, '{pName}_puMvaFull'.format(pName=pName), jet.puMva('full') )
    fill(tree, '{pName}_puMvaSimple'.format(pName=pName), jet.puMva('simple'))
    fill(tree, '{pName}_puMvaCutBased'.format(pName=pName), jet.puMva('cut-based'))
    fill(tree, '{pName}_puJetId'.format(pName=pName), jet.puJetIdPassed)
    fill(tree, '{pName}_looseJetId'.format(pName=pName), jet.pfJetIdPassed)
    fill(tree, '{pName}_chFrac'.format(pName=pName), jet.component(1).fraction() )
    fill(tree, '{pName}_eFrac'.format(pName=pName), jet.component(2).fraction() )
    fill(tree, '{pName}_muFrac'.format(pName=pName), jet.component(3).fraction() )
    fill(tree, '{pName}_gammaFrac'.format(pName=pName), jet.component(4).fraction() )
    fill(tree, '{pName}_h0Frac'.format(pName=pName), jet.component(5).fraction() )
    fill(tree, '{pName}_hhfFrac'.format(pName=pName), jet.component(6).fraction() )
    fill(tree, '{pName}_ehfFrac'.format(pName=pName), jet.component(7).fraction() )
    if hasattr(jet, 'leg') and jet.leg:
        fillParticle(tree, '{pName}_leg'.format(pName=pName), jet.leg )
    
# vbf

def bookVBF( tree, pName ):
    var(tree, '{pName}_mjj'.format(pName=pName))
    var(tree, '{pName}_deta'.format(pName=pName))
    var(tree, '{pName}_nCentral'.format(pName=pName))
    var(tree, '{pName}_mva'.format(pName=pName))
    var(tree, '{pName}_jdphi'.format(pName=pName))
    var(tree, '{pName}_dijetpt'.format(pName=pName))
    var(tree, '{pName}_dijetphi'.format(pName=pName))
    var(tree, '{pName}_hdijetphi'.format(pName=pName))
    var(tree, '{pName}_visjeteta'.format(pName=pName))
    var(tree, '{pName}_ptvis'.format(pName=pName))
    
def fillVBF( tree, pName, vbf ):
    fill(tree, '{pName}_mjj'.format(pName=pName), vbf.mjj )
    fill(tree, '{pName}_deta'.format(pName=pName), vbf.deta )
    fill(tree, '{pName}_nCentral'.format(pName=pName), len(vbf.centralJets) )
    fill(tree, '{pName}_mva'.format(pName=pName), vbf.mva )
    fill(tree, '{pName}_jdphi'.format(pName=pName), vbf.dphi )
    fill(tree, '{pName}_dijetpt'.format(pName=pName), vbf.dijetpt )
    fill(tree, '{pName}_dijetphi'.format(pName=pName), vbf.dijetphi )
    fill(tree, '{pName}_hdijetphi'.format(pName=pName), vbf.dphidijethiggs )
    fill(tree, '{pName}_visjeteta'.format(pName=pName), vbf.visjeteta )
    fill(tree, '{pName}_ptvis'.format(pName=pName), vbf.ptvis )
    
