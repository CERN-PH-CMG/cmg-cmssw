from math import *
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.framework.event import Event
import os
import itertools
        
class FourLeptonEventSkimmer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(FourLeptonEventSkimmer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.required = cfg_ana.required
    def declareHandles(self):
        super(FourLeptonEventSkimmer, self).declareHandles()

    def beginLoop(self, setup):
        super(FourLeptonEventSkimmer,self).beginLoop(setup)
        self.counters.addCounter('events')
        self.count = self.counters.counter('events')
        self.count.register('all events')
        self.count.register('passing events')

    def process(self, event):
        self.readCollections( event.input )
        self.count.inc('all events')

        for col in self.required:
            if hasattr(event,col):
                if len(getattr(event,col))>0:
                    self.count.inc('passing events')
                    return True
        
        return False    
