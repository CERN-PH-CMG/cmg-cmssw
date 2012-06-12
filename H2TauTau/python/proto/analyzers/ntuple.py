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

# lepton

def bookLepton( tree, pName ):
    bookParticle(tree, pName )
    var(tree, '{pName}_relIso05'.format(pName=pName))
    var(tree, '{pName}_dxy'.format(pName=pName))
    var(tree, '{pName}_dz'.format(pName=pName))
    var(tree, '{pName}_weight'.format(pName=pName))
    var(tree, '{pName}_triggerWeight'.format(pName=pName))
    var(tree, '{pName}_triggerEffData'.format(pName=pName))
    var(tree, '{pName}_triggerEffMC'.format(pName=pName))
    var(tree, '{pName}_recEffWeight'.format(pName=pName))

def fillLepton( tree, pName, lepton ):
    fillParticle(tree, pName, lepton )
    fill(tree, '{pName}_relIso05'.format(pName=pName), lepton.relIsoAllChargedDB05() )
    fill(tree, '{pName}_dxy'.format(pName=pName), lepton.dxy() )
    fill(tree, '{pName}_dz'.format(pName=pName), lepton.dz() )
    fill(tree, '{pName}_weight'.format(pName=pName), lepton.weight )
    fill(tree, '{pName}_triggerWeight'.format(pName=pName), lepton.triggerWeight )
    fill(tree, '{pName}_triggerEffData'.format(pName=pName), lepton.triggerEffData )
    fill(tree, '{pName}_triggerEffMC'.format(pName=pName), lepton.triggerEffMC )
    fill(tree, '{pName}_recEffWeight'.format(pName=pName), lepton.recEffWeight )


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

def fillEle( tree, pName, ele ):
    fillLepton(tree, pName, ele)
    fill(tree, '{pName}_mvaIso'.format(pName=pName), ele.mvaIso() )
    fill(tree, '{pName}_mvaTrigV0'.format(pName=pName), ele.sourcePtr().electronID("mvaTrigV0") )
    fill(tree, '{pName}_mvaNonTrigV0'.format(pName=pName), ele.sourcePtr().electronID("mvaNonTrigV0") )


# tau 

def bookTau( tree, pName ):
    bookLepton(tree, pName )
    var(tree, '{pName}_veryLooseIso'.format(pName=pName))
    var(tree, '{pName}_looseIso'.format(pName=pName))
    var(tree, '{pName}_mediumIso'.format(pName=pName))
    var(tree, '{pName}_tightIso'.format(pName=pName))

    var(tree, '{pName}_againstMuonTight'.format(pName=pName))    
    var(tree, '{pName}_againstElectronLoose'.format(pName=pName))    

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

def bookJet( tree, pName ):
    bookParticle(tree, pName )
    var(tree, '{pName}_puMvaFull'.format(pName=pName))
    var(tree, '{pName}_puMvaSimple'.format(pName=pName))
    var(tree, '{pName}_puMvaCutBased'.format(pName=pName))
    var(tree, '{pName}_looseJetId'.format(pName=pName))

def fillJet( tree, pName, jet ):
    fillParticle(tree, pName, jet )
    fill(tree, '{pName}_puMvaFull'.format(pName=pName), jet.puMva('full') )
    fill(tree, '{pName}_puMvaSimple'.format(pName=pName), jet.puMva('simple'))
    fill(tree, '{pName}_puMvaCutBased'.format(pName=pName), jet.puMva('cut-based'))
    fill(tree, '{pName}_looseJetId'.format(pName=pName), jet.getSelection('cuts_looseJetId'))


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
    
