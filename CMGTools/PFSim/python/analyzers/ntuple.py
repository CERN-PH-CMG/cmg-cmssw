#!/bin/env python
from CMGTools.PFSim.analyzers.ParticleAnalyzer import pdgIds


def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

# simple particle

def bookParticle( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_energy'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_dr'.format(pName=pName))
    
def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_energy'.format(pName=pName), particle.p4().energy() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    if hasattr( particle, 'dr'):
        fill(tree, '{pName}_dr'.format(pName=pName), particle.dr )

def bookRecJet(tree, pName):
    bookParticle(tree, pName)
    var(tree, '{pName}_n'.format(pName=pName))
    

def fillRecJet( tree, pName, recjet):
    fillParticle(tree, pName, recjet)
    fill(tree, '{pName}_n'.format(pName=pName), recjet.nConstituents() )
    
def bookGenParticle(tree, pName):
    bookParticle(tree, pName)
    for id in pdgIds:
        bookParticle(tree, '{pName}_sim{id}'.format(pName=pName,id=id))
    var(tree, '{pName}_pdgId'.format(pName=pName))
    
def fillGenParticle( tree, pName, particle ):
    fillParticle( tree, pName, particle )
    for id in pdgIds:
        if hasattr(particle, 'sim{id}'.format(id=id)):
            matched = getattr(particle, 'sim{id}'.format(id=id))
            if matched:
                fillParticle(tree, '{pName}_sim{id}'.format(pName=pName,
                                                            id=id), matched) 
    fill(tree, '{pName}_pdgId'.format(pName=pName), particle.pdgId() )

def bookGenJet(tree, pName):
    bookParticle(tree, pName)
    bookRecJet(tree, '{pName}_rec'.format(pName=pName))
    bookParticle(tree, '{pName}_sim'.format(pName=pName))
    bookParticle(tree, '{pName}_genPtc3'.format(pName=pName))
        
def fillGenJet( tree, pName, genjet ):
    fillParticle( tree, pName, genjet )
    if hasattr(genjet, 'rec') and genjet.rec:
        fillRecJet(tree, '{pName}_rec'.format(pName=pName), genjet.rec )
    if genjet.sim:
        fillParticle(tree, '{pName}_sim'.format(pName=pName), genjet.sim )
    if genjet.genPtc3:
        fillParticle(tree, '{pName}_genPtc3'.format(pName=pName), genjet.genPtc3 )


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
    
