import operator 
import itertools
import copy

from ROOT import TLorentzVector
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
# from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet

        
class SixJetAnalyzer( Analyzer ):

    def declareHandles(self):
        super(SixJetAnalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle ('cmgPFJetSel',
                                           'std::vector<cmg::PFJet>')

        
    def beginLoop(self):
        super(SixJetAnalyzer,self).beginLoop()
        self.counters.addCounter('SixJets')
        count = self.counters.counter('SixJets')
        count.register('All events')
        count.register('Six good jets')
        count.register('Passing')

 
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.counters.counter('SixJets').inc('All events')

        event.evnum = iEvent.eventAuxiliary().id().event()
        event.jets = map( Jet, self.handles['jets'].product())

        if len(event.jets)<6:
            return False
        self.counters.counter('SixJets').inc('Six good jets')
        self.counters.counter('SixJets').inc('Passing')
 
        return True
 
