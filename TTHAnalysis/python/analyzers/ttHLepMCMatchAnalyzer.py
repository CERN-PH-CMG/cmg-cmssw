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


        
class ttHLepMCMatchAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepMCMatchAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHLepMCMatchAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(ttHLepMCMatchAnalyzer,self).beginLoop()

    def matchLeptons(self, event):
        match = matchObjectCollection2(event.selectedLeptons, 
                                       event.genleps + event.gentauleps, 
                                       deltaRMax = 0.3)
        for lep in event.selectedLeptons:
            gen = match[lep]
            lep.mcMatchId = (gen.sourceId if gen != None else 0)
    
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

        self.matchLeptons(event)

        self.matchJets(event)

        return True
