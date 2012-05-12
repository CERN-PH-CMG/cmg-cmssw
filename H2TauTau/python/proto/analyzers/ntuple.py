#!/bin/env python

def var( tree, varName ):
    self.tree.addVar('float', varName)

def fill( tree, varName, value ):
    setattr( self.tree.s, varName, value )

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

def fillLepton( tree, lepton ):
    fillParticle(tree, pName )
    fill(tree, '{pName}_relIso05'.format(pName=pName), lepton.relIso(0.5) )


# muon


def bookMuon( tree, pName ):
    bookLepton(tree, pName )
    var(tree, '{pName}_mvaIso'.format(pName=pName))
    var(tree, '{pName}_looseId'.format(pName=pName))
    var(tree, '{pName}_tightId'.format(pName=pName))

def fillMuon( tree, muon ):
    fillLepton(tree, pName )
    fill(tree, '{pName}_mvaIso'.format(pName=pName), muon.mvaIso() )
    fill(tree, '{pName}_looseId'.format(pName=pName), muon.looseId() )
    fill(tree, '{pName}_tightId'.format(pName=pName), muon.looseId() )

# electron

# tau 

# jet

def bookJet( tree, pName ):
    bookParticle(tree, pName )
    var(tree, '{pName}_puMvaFull'.format(pName=pName))
    var(tree, '{pName}_puMvaSimple'.format(pName=pName))
    var(tree, '{pName}_puMvaCutBased'.format(pName=pName))
    var(tree, '{pName}_looseJetId'.format(pName=pName))

def fillJet( tree, jet ):
    fillParticle(tree, pName )
    fill(tree, '{pName}_puMvaFull'.format(pName=pName), jet.puMva('full') )
    fill(tree, '{pName}_puMvaSimple'.format(pName=pName), jet.puMva('simple'))
    fill(tree, '{pName}_puMvaCutBased'.format(pName=pName), jet.puMva('cut-based'))
    fill(tree, '{pName}_looseJetId'.format(pName=pName), jet.getSelection('cuts_looseJetId'))
