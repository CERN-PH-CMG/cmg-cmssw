#!/bin/env python

def var( tree, varName ):
    tree.addVar('float', varName)

def fill( tree, varName, value ):
    setattr( tree.s, varName, value )

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

def fillLepton( tree, pName, lepton ):
    fillParticle(tree, pName, lepton )
    fill(tree, '{pName}_relIso05'.format(pName=pName), lepton.relIso(0.5) )


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

# tau 

def bookTau( tree, pName ):
    bookLepton(tree, pName )
    var(tree, '{pName}_looseIso'.format(pName=pName))
    var(tree, '{pName}_mediumIso'.format(pName=pName))
    var(tree, '{pName}_tightId'.format(pName=pName))

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
