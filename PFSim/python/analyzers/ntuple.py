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
    
def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )

def bookGenParticle(tree, pName):
    bookParticle(tree, pName)
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_pdgId'.format(pName=pName))
    
def fillGenParticle( tree, pName, particle ):
    fillParticle( tree, pName, particle )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_pdgId'.format(pName=pName), particle.pdgId() )

def bookGenJet(tree, pName):
    bookParticle(tree, pName)
    var(tree, '{pName}_dR'.format(pName=pName))
    
def fillGenJet( tree, pName, genjet ):
    fillParticle( tree, pName, genjet )
    fill(tree, '{pName}_dR'.format(pName=pName), genjet.dR )


def bookJet( tree, pName ):
    bookParticle(tree, pName )
    var(tree, '{pName}_chFrac'.format(pName=pName))
    var(tree, '{pName}_eFrac'.format(pName=pName))
    var(tree, '{pName}_muFrac'.format(pName=pName))
    var(tree, '{pName}_gammaFrac'.format(pName=pName))
    var(tree, '{pName}_h0Frac'.format(pName=pName))
    var(tree, '{pName}_hhfFrac'.format(pName=pName))
    var(tree, '{pName}_ehfFrac'.format(pName=pName))

def fillJet( tree, pName, jet ):
    fillParticle(tree, pName, jet )
    fill(tree, '{pName}_chFrac'.format(pName=pName), jet.component(1).fraction() )
    fill(tree, '{pName}_eFrac'.format(pName=pName), jet.component(2).fraction() )
    fill(tree, '{pName}_muFrac'.format(pName=pName), jet.component(3).fraction() )
    fill(tree, '{pName}_gammaFrac'.format(pName=pName), jet.component(4).fraction() )
    fill(tree, '{pName}_h0Frac'.format(pName=pName), jet.component(5).fraction() )
    fill(tree, '{pName}_hhfFrac'.format(pName=pName), jet.component(6).fraction() )
    fill(tree, '{pName}_ehfFrac'.format(pName=pName), jet.component(7).fraction() )
    
