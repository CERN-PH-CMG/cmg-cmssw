import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched

from CMGTools.HToZZTo4Leptons.analyzers.DiObjectPair import DiObjectPair
from CMGTools.HToZZTo4Leptons.analyzers.CutFlowMaker import CutFlowMaker

from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzerBase import FourLeptonAnalyzerBase
from CMGTools.HToZZTo4Leptons.analyzers.OverlapCleaner import OverlapCleaner 
        
class FourLeptonAnalyzerBaseline( FourLeptonAnalyzerBase ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def declareHandles(self):
        super(FourLeptonAnalyzerBaseline, self).declareHandles()

    def beginLoop(self):
        super(FourLeptonAnalyzerBaseline,self).beginLoop()


       
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        event.step = 0
        #startup counter
        self.counters.counter('FourLepton').inc('all events')

        #basic event quantities
        event.met = self.handles['met'].product()[0]
        event.rho = self.handles['rho'].product()[0]

        #Add rho in the analyzer so that we can use it in the isolation
        self.rho = event.rho

        
        #Build lepton lists and apply skim
        self.buildLeptonList( event )


        #create a cut flow
        cutFlow = CutFlowMaker(self.counters.counter("FourLepton"),event,event.leptons1,event.leptons2)


        #Cuts :Apply minimal criteria like sip<100 and min Pt and eta and require at least two leptons 
        passed=cutFlow.applyDoubleCut(self.testLeptonSkim1,self.testLeptonSkim2,'lepton preselection',2,'skimmedLeptons1','skimmedLeptons2')
        if passed: event.step += 1


        #merge the two output collections in one non overlapping
        cutFlow.mergeResults('skimmedLeptons')

        #Remove any electrons that are near to tight muons!
        

        cleanOverlap = OverlapCleaner(event.skimmedLeptons,0.05,11,13,self.testMuonPF)
        passed = cutFlow.applyCut(cleanOverlap,'electron cross cleaning',2,'cleanLeptons')


        #make lepton combinations (actually  permutations!) 
        event.leptonPairs = self.findPairs(cutFlow.obj1 )
        cutFlow.setSource1(event.leptonPairs)


        
        #require that muPT>20/10 and M>40 and OS/SF
        passed=cutFlow.applyCut(self.testZSkim,'at least one z',1,'zBosons')

        #if no zs throw the event
        if not passed: return self.cfg_ana.keep
        event.step += 1

        #Apply also M<120 cut for comparing with others
        passed=cutFlow.applyCut(self.testZ1Mass,'MZ less than 120',1,'zBosonsTight')

        #Now create four Lepton candidtes upstrem. Use all permutations and not combinations
        #of leptons so we can pick the best Z1 and Z2
        event.fourLeptons = self.findQuads(event.cleanLeptons)
        #Sort them by M1 near Z and My highest Pt sum
        self.sortFourLeptons(event.fourLeptons)
        cutFlow.setSource1(event.fourLeptons)

        if len(event.fourLeptons)>0:
            event.higgsCandLoose = event.fourLeptons[0]

        
        #Next Step : Apply Tight Muon Selection
        passed=cutFlow.applyCut(self.testFourLeptonTightID,'lepton id',1,'fLTightID')
        #Require Z1 cuts
        passed=cutFlow.applyCut(self.testFourLeptonZ1,'pair 1 built',1,'fLZ1')
        #Search for second SF pair
        passed=cutFlow.applyCut(self.testFourLeptonSF,'pair 2  SF',1,'fLZ2SF')
        #Search for second OS pair
        passed=cutFlow.applyCut(self.testFourLeptonOS,'pair 2 built OS and SF',1,'fLZ2OSSF')
        #Mass cuts for second lepton
        passed=cutFlow.applyCut(self.testFourLeptonMassZ1Z2,'pair 2 mass cut',1,'fLZ2Mass')
        #Pt Cuts
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'Pt Thresholds',1,'fLPt12')
        #QCD suppression
        passed=cutFlow.applyCut(self.testFourLeptonMinMass,'QCD suppression',1,'fLQCDsup')
        #Z->4l phase space
        passed=cutFlow.applyCut(self.testFourLeptonMassZ,'Z 4l phase space',1,'fLZ4l')

        #ZZ->4l phase space
        passed=cutFlow.applyCut(self.testFourLeptonMass,'ZZ 4l phase space',1,'fLZZ4l')

        if passed:
            event.higgsCand = cutFlow.obj1[0]
        
        return True
    


#In this analysis this is loose
    def testLeptonLoose1(self, lepton,sel=None):
        return  abs(lepton.sip3D())<4. 

    def testLeptonLoose2(self, lepton,sel=None):
        return  abs(lepton.sip3D())<4. 




###In the cross check we used 0.25 for isolation
    def testMuonTight(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''

        iso  = muon.absEffAreaIso(self.rho,self.cfg_ana.effectiveAreas)/muon.pt()<0.25 #warning:you need to set the self.rho !!!
        return self.testMuonLoose(muon) and self.testMuonPF(muon) and iso

    def testElectronTight(self, electron):
        '''Returns True if a electron passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        id = electron.mvaIDZZ()
        iso  = electron.absEffAreaIso(self.rho,self.cfg_ana.effectiveAreas)/electron.pt()<0.25 #warning:you need to set the self.rho !!!

        conv = (electron.numberOfHits()<=1)
        
        return iso and id and conv
