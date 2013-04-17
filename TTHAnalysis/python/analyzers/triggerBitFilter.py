import operator 
import itertools
import copy
from math import *

from ROOT import TriggerBitChecker

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
        
class triggerBitFilter( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(triggerBitFilter,self).__init__(cfg_ana,cfg_comp,looperName)
        triggers = cfg_comp.triggers
        vetoTriggers = cfg_comp.vetoTriggers if hasattr(cfg_comp, 'vetoTriggers') else []
        import ROOT
        trigVec = ROOT.vector(ROOT.string)()
        for t in triggers: trigVec.push_back(t)
        self.mainFilter = TriggerBitChecker(trigVec)
        if len(vetoTriggers):
            vetoVec = ROOT.vector(ROOT.string)()
            for t in vetoTriggers: vetoVec.push_back(t)
            self.vetoFilter = TriggerBitChecker(vetoVec)
        else:
            self.vetoFilter = None 
        
    def declareHandles(self):
        super(triggerBitFilter, self).declareHandles()
        self.handles['TriggerResults'] = AutoHandle( ('TriggerResults','','HLT'), 'edm::TriggerResults' )

    def beginLoop(self):
        super(triggerBitFilter,self).beginLoop()

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        if not self.mainFilter.check(iEvent.object(), self.handles['TriggerResults'].product()):
            return False
        if self.vetoFilter != None and self.vetoFilter.check(iEvent.object(), self.handles['TriggerResults'].product()):
            return False
        return True

