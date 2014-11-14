import operator 
import itertools
import copy
from math import *

from ROOT import TLorentzVector, TVectorD

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Tau import Tau
from CMGTools.RootTools.physicsobjects.Jet import Jet

from CMGTools.RootTools.utils.DeltaR import * 

import ROOT

from ROOT import ReclusterJets

import os


class ttHReclusterJetsAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHReclusterJetsAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 

    def declareHandles(self):
        super(ttHReclusterJetsAnalyzer, self).declareHandles()
       #genJets                                                                                                                                                                     
        self.handles['genJets'] = AutoHandle( 'slimmedGenJets','std::vector<reco::GenJet>')

    def beginLoop(self):
        super(ttHReclusterJetsAnalyzer,self).beginLoop()
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')

    def makeFatJets(self, event):
        import array
        import numpy
        objects40jc = [ j for j in event.cleanJets if j.pt() > 40.0 and abs(j.eta())<2.4 ]
        
        if len(objects40jc)>=1:
            
            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()
            
            for jet in objects40jc:
                pxvec.push_back(jet.px())
                pyvec.push_back(jet.py())
                pzvec.push_back(jet.pz())
                Evec.push_back(jet.energy())
                

            reclusterJets = ReclusterJets(pxvec, pyvec, pzvec, Evec, 1.,1.2)
            groupingViaKt = reclusterJets.getGrouping()

            pseudoJet1px = 0
            pseudoJet1py = 0
            pseudoJet1pz = 0
            pseudoJet1energy = 0
            
            pseudoJet2px = 0
            pseudoJet2py = 0
            pseudoJet2pz = 0
            pseudoJet2energy = 0

            pseudoJetpx = 0
            pseudoJetpy = 0
            pseudoJetpz = 0
            pseudoJetenergy = 0

            event.FatJetTest = []            

            for index in range(0, len(groupingViaKt[0])):
                pseudoJetpx = groupingViaKt[0][index]
                pseudoJetpy = groupingViaKt[1][index]
                pseudoJetpz = groupingViaKt[2][index]
                pseudoJetenergy = groupingViaKt[3][index]
                event.FatJets.append(ROOT.reco.Particle.LorentzVector( pseudoJetpx, pseudoJetpy, pseudoJetpz, pseudoJetenergy))





    def process(self, iEvent, event):
        self.readCollections( iEvent )

        event.FatJets = []
        self.makeFatJets(event)

        return True
