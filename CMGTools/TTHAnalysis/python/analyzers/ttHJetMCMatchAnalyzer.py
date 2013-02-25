import operator 
import itertools
import copy
from math import *

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Jet import Jet

from CMGTools.RootTools.utils.DeltaR import *


        
class ttHJetMCMatchAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHJetMCMatchAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHJetMCMatchAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(ttHJetMCMatchAnalyzer,self).beginLoop()

    def jetFlavour(self,event):
        def isFlavour(x,f):
            id = abs(x.pdgId())
            if id > 999: return (id/1000)%10 == f
            if id >  99: return  (id/100)%10 == f
            return id % 100 == f

        event.bqObjects = [ p for p in event.genParticles if (p.status() == 2 and isFlavour(p,5)) ]
        event.cqObjects = [ p for p in event.genParticles if (p.status() == 2 and isFlavour(p,4)) ]
        
        for jet in event.jets:
           (bmatch, dr) = bestMatch(jet, event.bqObjects)
           if dr < 0.5:
               jet.mcFlavour = 5
           else:
               (cmatch, dr) = bestMatch(jet, event.cqObjects) 
               if dr < 0.5:
                   jet.mcFlavour = 4
               else:
                   jet.mcFlavour = 0

        event.heaviestQCDFlavour = 5 if len(event.bqObjects) else (4 if len(event.cqObjects) else 1);
                    
    def matchJets(self, event):
        match = matchObjectCollection2(event.cleanJets,
                                       event.genbquarks + event.genwzquarks,
                                       deltaRMax = 0.5)
        for jet in event.cleanJets:
            gen = match[jet]
            jet.mcMatchId   = (gen.sourceId     if gen != None else 0)
            jet.mcMatchFlav = (abs(gen.pdgId()) if gen != None else 0)

    def process(self, iEvent, event):

        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True

        self.matchJets(event)
        self.jetFlavour(event)

        return True
