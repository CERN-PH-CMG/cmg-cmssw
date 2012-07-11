import operator 
import itertools
import copy
import pprint

from ROOT import TLorentzVector
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
# from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.LEP3.analyzers.DiObject import DiObject

from CMGTools.LEP3.kinfitters import FourJetKinFitter




        
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
#         count.register('>6 jets')
        count.register('At least 1Z')
        count.register('At least 2Z, or 1Z & 1W')
        count.register('Passing')

        self.fitter = FourJetKinFitter('fitter','test for Cris')

 
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.counters.counter('SixJets').inc('All events')

        event.evnum = iEvent.eventAuxiliary().id().event()
        event.jets = map( Jet, self.handles['jets'].product())
        # need to do lepton cleaning
        event.jets.sort(key=lambda x: x.energy(), reverse = True)
        
        if len(event.jets)<6:
            return False
        self.counters.counter('SixJets').inc('Six good jets')

##         if len(event.jets)>6:
##             self.counters.counter('SixJets').inc('>6 jets')

        event.sixjets = self.exclusiveJets(event.jets)
        
        # pair the jets and look for a Z
        # for each good Z, 1 event hypothesis
        pairs = self.findPairs( event.sixjets )
        event.zCands = []
        wCands = []
        for pair in pairs:
            if abs( pair.M() - 91 ) < 5.:
                event.zCands.append(pair)
            if abs( pair.M() - 85. ) < 5:
                wCands.append(pair)
                
        event.wCands = [ w for w in wCands if w not in event.zCands]

        if len(event.zCands)==0:
            return False

        self.counters.counter('SixJets').inc('At least 1Z')

        if len(event.zCands)>=2 or len(event.wCands)>0:
            self.counters.counter('SixJets').inc('At least 2Z, or 1Z & 1W')
        else:
            return False
        
        self.counters.counter('SixJets').inc('Passing')
 
        return True
 

    def findPairs(self, jets):
        out = []
        for j1, j2 in itertools.combinations(jets, 2):
            out.append( DiObject(j1, j2) )
        return out


    def exclusiveJets(self, jets, nJets=6):
        if len(jets)<=nJets:
            return jets
        tmpJets = copy.copy(jets)
        while len(tmpJets) > nJets: 
            dijets = self.findPairs(tmpJets) 
            dijets.sort(key=lambda x: x.M())
            tmpJets.remove(dijets[0].leg1)
            tmpJets.remove(dijets[0].leg2)
            tmpJets.append(dijets[0])
        return sorted(tmpJets, key=lambda x: x.energy(), reverse = True)
    
