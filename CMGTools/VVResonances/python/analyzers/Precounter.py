from math import *
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.framework.event import Event
import os

        
class Precounter( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(Precounter,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(Precounter, self).declareHandles()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')

    def beginLoop(self, setup):
        super(Precounter,self).beginLoop(setup)

    def process(self, event):
        self.counters.counter('events').inc('all events')
        return True
