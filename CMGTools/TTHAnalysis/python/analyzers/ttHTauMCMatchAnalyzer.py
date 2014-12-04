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
from CMGTools.TTHAnalysis.analyzers.ttHLepMCMatchAnalyzer import matchObjectCollection3

class ttHTauMCMatchAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHTauMCMatchAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHTauMCMatchAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(ttHTauMCMatchAnalyzer,self).beginLoop()

    def matchTaus(self, event):
        match = matchObjectCollection3(event.inclusiveTaus, event.gentaus, deltaRMax = 0.5)
        for lep in event.inclusiveTaus:
            gen = match[lep]
            lep.mcMatchId = 1 if gen else 0

    def process(self, iEvent, event):
        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True

        self.matchTaus(event)
        return True
