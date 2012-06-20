import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenParticle

from CMGTools.LEP3.analyzers.DiObject import DiObject
from CMGTools.LEP3.analyzers.EMiss import EMiss, EVis

from CMGTools.RootTools.utils.DeltaR import deltaR
from math import pi, sqrt, acos
        
class EMissAnalyzer( Analyzer ):

    def declareHandles(self):
        super(EMissAnalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle ('cmgPFJetSel',
                                           'std::vector<cmg::PFJet>')

        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )



        
    def beginLoop(self):
        super(EMissAnalyzer,self).beginLoop()
        self.counters.addCounter('EMiss')
        self.counters.addCounter('EMissGen')
        self.counters.addCounter('WWHGen')
        counts = []
        counts.append(self.counters.counter('EMiss'))
        counts.append(self.counters.counter('EMissGen'))
        counts.append(self.counters.counter('WWHGen'))
        for count in counts : 
            count.register('All events')
            count.register('ptMiss > 10.')
            count.register('mVis < 180.')
            count.register('ctMiss > 1.00')
            count.register('mMiss > 0.')
            count.register('Two Good Jets')
            count.register('passing')
        
    def buildMiss(self, event):

        self.nunubb = False
        self.wwh = False
        self.neutrinos = []
        self.bquarks = []
        for ptc in self.mchandles['genParticles'].product():
            isNeutrino = abs(ptc.pdgId()) == 12 or abs(ptc.pdgId()) == 14 or abs(ptc.pdgId()) == 16
            isbquark = abs(ptc.pdgId()) == 5
            if isNeutrino : 
                if ptc.numberOfMothers() and ptc.mother(0).pdgId() != 23 : continue
                self.neutrinos.append(GenParticle(ptc))
            elif isbquark : 
                if not(ptc.numberOfMothers()) : continue
                if ptc.mother(0).pdgId() != 25 : continue
                self.bquarks.append(GenParticle(ptc))

        if len(self.neutrinos) == 2 and len(self.bquarks) == 2 :
            self.nunubb = True
            self.wwh = not(self.neutrinos[0].numberOfMothers())
##            for ptc in self.mchandles['genParticles'].product():
##                print GenParticle(ptc)

        self.jets = []
        for ptj in self.handles['jets'].product():
            self.jets.append(Jet(ptj))

        self.eMiss = EMiss(self.jets)
        self.eVis = EVis(self.jets)
        #if self.nunubb : print self.eMiss

        


    def buildJetList(self, event):

        self.jets = []
        for ptj in self.handles['jets'].product():
            self.jets.append(Jet(ptj))
            #print jet, jet.nConstituents(), jet.component(1).number(), Jet(ptj).component(1).fraction()

        

        if len(self.jets) > 2 : 
            self.jets.sort(key=lambda a: a.energy(), reverse = True)
            tmpJets = set(self.jets)
#            print 'Jets Avant : '
#            for jet in tmpJets:
#                print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
            while len(tmpJets) != 2: 
                dijets = self.findPairs(tmpJets)
                dijets.sort(key=lambda a: a.M())
#                print dijets[0],dijets[0].M()
                
                tmpJets.remove(dijets[0].leg1)
                tmpJets.remove(dijets[0].leg2)
                tmpJets.add(dijets[0])

 #           print 'Jets apres : '
            self.jets = []
            for jet in tmpJets:
 #               print jet,jet.nConstituents(), jet.mass(), jet.btag(7)
 #               print jet,jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(), jet.btag(7)
                self.jets.append(jet)
                
        self.jets.sort(key=lambda a: a.btag(7), reverse = True)


    def findPairs(self, jets):
        out = []

        for j1, j2 in itertools.combinations(jets, 2):
            out.append( DiObject(j1, j2) )

        return out
    
 
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        #print 'Event ',eventNumber
        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        
        self.buildMiss( event )

        event.ptMiss = self.eMiss.Pt()
        event.pMiss = self.eMiss.P()
        event.eMiss = self.eMiss.E()
        event.ctMiss = 1.
        if self.eMiss.P() > 0. : 
            event.ctMiss = self.eMiss.Pz()/self.eMiss.P()
        event.mMiss = self.eMiss.M()
        event.ptVis = self.eVis.Pt()
        event.pVis = self.eVis.P()
        event.eVis = self.eVis.E()
        event.ctVis = -1.
        if self.eVis.P() > 0. : 
            event.ctVis = self.eVis.Pz()/self.eVis.P()
        event.mVis = self.eVis.M()
        event.nunubb = self.nunubb
        event.wwh = self.wwh

        event.allJets = self.jets
        event.jetPairs = self.findPairs( event.allJets )


        #for ptc in self.mchandles['genParticles'].product():
        #    print GenParticle(ptc)
        #for jet in self.jets :
        #    print jet, jet.nConstituents(), jet.component(1).number(), jet.component(1).fraction(), jet.mass(),jet.btag(7),jet.btag(0)

        self.counters.counter('EMiss').inc('All events')
        if self.nunubb : self.counters.counter('EMissGen').inc('All events')
        if self.wwh : self.counters.counter('WWHGen').inc('All events')
        event.step = 0

        if self.eMiss.Pt() < self.cfg_ana.ptmiss :
            return 0
        else:
            event.step +=1
        self.counters.counter('EMiss').inc('ptMiss > 10.')
        if self.nunubb : self.counters.counter('EMissGen').inc('ptMiss > 10.')
        if self.wwh : self.counters.counter('WWHGen').inc('ptMiss > 10.')
            
        if self.eVis.M() > self.cfg_ana.mvis :
            return 0
        else:
            event.step +=1
        self.counters.counter('EMiss').inc('mVis < 180.')
        if self.nunubb : self.counters.counter('EMissGen').inc('mVis < 180.')
        if self.wwh : self.counters.counter('WWHGen').inc('mVis < 180.')
            
        if self.eMiss.P() == 0. or abs(self.eMiss.Pz()/self.eMiss.P()) > self.cfg_ana.ctmiss  :
            return 0
        else:
            event.step +=1
        self.counters.counter('EMiss').inc('ctMiss > 1.00')
        if self.nunubb : self.counters.counter('EMissGen').inc('ctMiss > 1.00')
        if self.wwh : self.counters.counter('WWHGen').inc('ctMiss > 1.00')
            
        if self.eMiss.M() < self.cfg_ana.mmiss :
            return 0
        else:
            event.step +=1

        self.counters.counter('EMiss').inc('mMiss > 0.')
        if self.nunubb : self.counters.counter('EMissGen').inc('mMiss > 0.')
        if self.wwh : self.counters.counter('WWHGen').inc('mMiss > 0.')

        # Force the event into two jets.
        self.buildJetList( event )        

        if len(self.jets) != 2 : 
            return 0
        else:
            event.step +=1
        self.counters.counter('EMiss').inc('Two Good Jets')
        if self.nunubb : self.counters.counter('EMissGen').inc('Two Good Jets')
        if self.wwh : self.counters.counter('WWHGen').inc('Two Good Jets')

        acol = self.jets[0].px() * self.jets[1].px() + \
               self.jets[0].py() * self.jets[1].py() + \
               self.jets[0].pz() * self.jets[1].pz()
        acol /= self.jets[0].p() * self.jets[1].p()
        acol = acos(acol)*180./pi

        acop = self.jets[0].px() * self.jets[1].px() + \
               self.jets[0].py() * self.jets[1].py() 
        acop /= self.jets[0].pt() * self.jets[1].pt()
        acop = acos(acop)*180./pi
        event.acol = acol
        event.acop = acop
        



#        self.counters.counter('EMiss').inc('passing')
#        if self.nunubb : self.counters.counter('EMissGen').inc('passing')
#        if self.wwh : self.counters.counter('WWHGen').inc('passing')
#        event.step += 1


        return True
 

 
    
##    def testTwoJets(self, jets) :
    
##          if len(jets) != 2 :
##             #print 'NJets = ', len(jets)
##             return False
        
