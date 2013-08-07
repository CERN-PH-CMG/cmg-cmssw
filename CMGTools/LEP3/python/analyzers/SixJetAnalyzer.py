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

# from CMGTools.LEP3.kinfitters import FourJetEpKinFitter
# from CMGTools.LEP3.kinfitters import DiJetMKinFitter
from CMGTools.LEP3.analyzers.beta4 import beta4
from CMGTools.LEP3.analyzers.findVV import findWW, findZZ


class LEPJet(Jet):
    def __str__(self):
        tmp = '{className} : {pdgId:>3}, E = {energy:5.1f}, eta = {eta:5.2f}, theta = {theta:5.2f},  phi = {phi:5.2f}'
        return tmp.format( className = self.__class__.__name__,
                           pdgId = self.pdgId(),
                           energy = self.energy(),
                           eta = self.eta(),
                           theta = self.theta(),
                           phi = self.phi() )

     
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

        # self.diJetfitter = DiJetMKinFitter('fitter','test for Colin',91.1876)
        # self.fourJetfitter = FourJetEpKinFitter('fitter','test for Colin',240)
        

 
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.counters.counter('SixJets').inc('All events')

        event.evnum = iEvent.eventAuxiliary().id().event()
        event.jets = map( LEPJet, self.handles['jets'].product())
        # need to do lepton cleaning
        event.jets.sort(key=lambda x: x.energy(), reverse = True)
        
        if len(event.jets)<6:
            return False
        self.counters.counter('SixJets').inc('Six good jets')

##         if len(event.jets)>6:
##             self.counters.counter('SixJets').inc('>6 jets')

        event.sixjets = self.exclusiveJets(event.jets)
        event.sixjets.sort(key=lambda x: x.energy(), reverse = True)
        
##         self.finalDiJets = self.diJetfitter.fit( event.sixjets[0].p4(), event.sixjets[1].p4())
               
##         self.finalFourJets = self.fourJetfitter.fit( event.sixjets[0].p4(), event.sixjets[1].p4(),
##                                                      event.sixjets[2].p4(), event.sixjets[3].p4())


##         # Temporary cout to check if the fitters work
##         print 'This is the energy for the first jet in the event before the kinfit constraint'
##         print event.sixjets[0].p4().energy()
##         print 'This is the energy for the first jet in the event after the kinfit Ep constraint'
##         print self.finalFourJets[0].Energy()
##         print 'This is the energy for the first jet in the event after the kinfit M constraint'
##         print self.finalDiJets.first.Energy()
    
        # pair the jets and look for a Z
        # for each good Z, 1 event hypothesis
        pairs = self.findPairs( event.sixjets )
        
        event.zCands = []      
        for pair in pairs:
            if abs( pair.M() - 91 ) < 30.:
                event.zCands.append(pair)

        if len(event.zCands)==0:
            return False

        self.counters.counter('SixJets').inc('At least 1Z')

        event.zCands.sort( key=lambda x: abs(x.M()-91.2) )
        event.Z1 = event.zCands[0]
        if len(event.zCands)>1:
            event.Z2 = event.zCands[0]

        event.higgsjets = [jet for jet in event.sixjets if jet!=event.Z1.leg1 and jet!=event.Z1.leg2]
        event.higgs = self.exclusiveJets(event.higgsjets, 1)[0] # yeah!

        event.fourjets = self.exclusiveJets(event.jets, 4)
        event.chi2_4jet = beta4(event.fourjets, 120.)

        jetPairs = self.findPairs(event.fourjets)
        event.delta_WW = findWW( jetPairs )[1]
        event.delta_ZZ = findZZ( jetPairs )[1]

        # print event.delta_WW, event.delta_ZZ

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
    
