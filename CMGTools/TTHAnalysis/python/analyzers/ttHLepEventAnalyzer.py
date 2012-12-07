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

from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi


        
class ttHLepEventAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepEventAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.maxLeps = cfg_ana.maxLeps

    def declareHandles(self):
        super(ttHLepEventAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(ttHLepEventAnalyzer,self).beginLoop()

    def makeZs(self, event, maxLeps):
        event.bestZ1 = [ 0., -1,-1 ]
        event.bestZ2 = [ 0., -1,-1, 0. ]
        nlep = len(event.selectedLeptons)
        for i,l1 in enumerate(event.selectedLeptons):
            for j in range(i+1,nlep):
                if j >= maxLeps: break
                l2 = event.selectedLeptons[j]    
                if l1.pdgId() == -l2.pdgId():
                    zmass = (l1.p4() + l2.p4()).M()
                    if abs(zmass - 91.188) < abs(event.bestZ1[0] - 91.188):
                        event.bestZ1 = [ zmass, i, j ]
        if event.bestZ1[0] != 0 and nlep > 3:
            for i,l1 in enumerate(event.selectedLeptons):
                if i == event.bestZ1[1]: continue
                for j in range(i+1,nlep):
                    if j >= maxLeps: break
                    if j == event.bestZ1[2]: continue
                    l2 = event.selectedLeptons[j]    
                    if l1.pdgId() == -l2.pdgId():
                        if l1.pt() + l2.pt() > event.bestZ2[0]:
                            event.bestZ2 = [ l1.pt() + l2.pt(), i, j, (l1.p4() + l2.p4()).M() ]

    def makeMlls(self, event, maxLeps):
        event.minMllSFOS = min(self.mllValues(event,  lambda l1,l2 : l1.pdgId()  == -l2.pdgId(), maxLeps))
        event.minMllAFOS = min(self.mllValues(event,  lambda l1,l2 : l1.charge() == -l2.charge(), maxLeps))
        event.minMllAFAS = min(self.mllValues(event,  lambda l1,l2 : True, maxLeps))

    def mllValues(self, event, pairSelection, maxLeps):
        pairs = []
        nlep = len(event.selectedLeptons)
        for i,l1 in enumerate(event.selectedLeptons):
            for j in range(i+1,nlep):
                if j >= maxLeps: break
                l2 = event.selectedLeptons[j]    
                if pairSelection(l1,l2):
                    pairs.append( (l1.p4() + l2.p4()).M() )
        if pairs == []: pairs.append(-1)
        return pairs
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        event.bjetsLoose  = [ j for j in event.cleanJets if j.getSelection("cuts_csv_loose")  ]
        event.bjetsMedium = [ j for j in event.cleanJets if j.getSelection("cuts_csv_medium") ]

        self.makeZs(event, self.maxLeps)
        self.makeMlls(event, self.maxLeps)

        if self.cfg_ana.verbose:
            print 'Event ',eventNumber
            print 'Selected leptons: ',len(event.selectedLeptons)
            print "\n".join(["\t%s" % p for p in event.selectedLeptons])
            print 'Selected clean jets   : ',len(event.cleanJets)
            print "\n".join(["\t%s" % p for p in event.cleanJets])
            print 'Selected loose  b-jets: ',len(event.bjetsLoose)
            print 'Selected medium b-jets: ',len(event.bjetsMedium)
            print 'Best Z1, if any: mass = %6.1f ' % event.bestZ1[0]
            print 'Best Z2, if any: mass = %6.1f ' % event.bestZ2[3]
            print 'Min m(ll) values: SF/OS = %5.1f, AF/OS = %5.1f, AF/AS = %5.1f' % (event.minMllSFOS, event.minMllAFOS, event.minMllAFAS)

        return True
