import numpy as my_n
import math
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.analyzers.ntuple import *
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton, Muon, Electron
from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.RootTools.physicsobjects.PileUpSummaryInfo import PileUpSummaryInfo


def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

##--------------------------------------------------

def bookW( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def fillW( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.Pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.Phi() )

def bookZ( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_rap'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))

def fillZ( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.Pt() )
    fill(tree, '{pName}_rap'.format(pName=pName), particle.Rapidity() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.Phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.M() )

##--------------------------------------------------

def bookFourVector( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))

def fillFourVector( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    
def bookParticle( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_charge'.format(pName=pName))

def fillParticle( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )

def bookMuon( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_mass'.format(pName=pName))
    var(tree, '{pName}_charge'.format(pName=pName))
    var(tree, '{pName}RelIso'.format(pName=pName))

def fillMuon( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_mass'.format(pName=pName), particle.mass() )
    fill(tree, '{pName}_charge'.format(pName=pName), particle.charge() )
    fill(tree, '{pName}RelIso'.format(pName=pName), particle.relIso(0.5) )


def bookJet( tree, pName ):
    var(tree, '{pName}_pt'.format(pName=pName))
    var(tree, '{pName}_eta'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def fillJet( tree, pName, particle ):
    fill(tree, '{pName}_pt'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_eta'.format(pName=pName), particle.eta() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )

##--------------------------------------------------
    
def bookLHE_weight( tree, pName ):
    # tree.vars['{pName}_weight'.format(pName=pName)]= my_n.zeros(400, dtype=float)
    # tree.tree.Branch('{pName}_weight'.format(pName=pName),tree.vars['{pName}_weight'.format(pName=pName)] ,'{pName}_weight'.format(pName=pName)+'[400]/D' )
    tree.vector('{pName}_weight'.format(pName=pName), 466)

def fillLHE_weight( tree, pName, LHE_weight,event ):
    # for i in range(0,min(len(LHE_weight),400)):
        # # print 'filling ',i,'with ',LHE_weight[i]
        # tree.vars['{pName}_weight'.format(pName=pName)][i] = LHE_weight[i]
    tree.vfill('{pName}_weight'.format(pName=pName), LHE_weight)

##--------------------------------------------------

def bookMuonCovMatrix( tree, pName ):
    # tree.vars['{pName}CovMatrix'.format(pName=pName)]= my_n.zeros(9, dtype=float)
    # tree.tree.Branch('{pName}CovMatrix'.format(pName=pName),tree.vars['{pName}CovMatrix'.format(pName=pName)] ,'{pName}CovMatrix'.format(pName=pName)+'[9]/D' )
    tree.vector('{pName}CovMatrix'.format(pName=pName), 9)

def fillMuonCovMatrix( tree, pName, covMatrix,event ):
    # vcovMatrix=[]
    # for i in range(0,9):
        # # tree.vars['{pName}CovMatrix'.format(pName=pName)][i] = covMatrix[i]
      # vcovMatrix.append(covMatrix[i])
    tree.vfill('{pName}CovMatrix'.format(pName=pName), covMatrix)

##--------------------------------------------------

def bookMET( tree, pName ):
    var(tree, '{pName}'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))

def bookCustomMET( tree, pName ):
    var(tree, '{pName}'.format(pName=pName))
    var(tree, '{pName}_phi'.format(pName=pName))
    var(tree, '{pName}_sumEt'.format(pName=pName))

def fillMET( tree, pName, particle ):
    fill(tree, '{pName}'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )

def fillCustomMET( tree, pName, particle ):
    fill(tree, '{pName}'.format(pName=pName), particle.pt() )
    fill(tree, '{pName}_phi'.format(pName=pName), particle.phi() )
    fill(tree, '{pName}_sumEt'.format(pName=pName), particle.sumEt() )    ### this was in the W analyzer
