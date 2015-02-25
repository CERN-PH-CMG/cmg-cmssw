from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi

import operator 
import itertools
import copy
from math import *

#from ROOT import TLorentzVector, TVectorD

from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.framework.event import Event
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

# from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Lepton
# from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Photon
# from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Electron
# from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Muon
# from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Tau
from PhysicsTools.Heppy.physicsobjects.PhysicsObjects import Jet

import ROOT
from ROOT import AlphaT


import os

class ttHAlphaTVarAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHAlphaTVarAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 

    def declareHandles(self):
        super(ttHAlphaTVarAnalyzer, self).declareHandles()
       #genJets                                                                                                                                                                     
        self.handles['genJets'] = AutoHandle( 'slimmedGenJets','std::vector<reco::GenJet>')

    def beginLoop(self,setup):
        super(ttHAlphaTVarAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')


    # Calculate alphaT using jet ET
    def makeAlphaT(self, event):

        if len(event.cleanJets) == 0:
            event.alphaT = 0
            return
        
        px  = ROOT.std.vector('double')()
        py  = ROOT.std.vector('double')()
        et  = ROOT.std.vector('double')()
#Make alphaT from lead 10 jets
	for jet in event.cleanJets[:10]:
            px.push_back(jet.px())
            py.push_back(jet.py())
            et.push_back(jet.et())
            pass

        alphaTCalc   = AlphaT()
        event.alphaT = alphaTCalc.getAlphaT( et, px, py )

        return

    def process(self, event):
        self.readCollections( event.input )

        event.alphaT = -999
        self.makeAlphaT(event)

        return True
