
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
from CMGTools.HToZZTo4Leptons.tools.CutFlowMaker import CutFlowMaker

from CMGTools.HToZZTo4Leptons.analyzers.MultiLeptonAnalyzerBase import MultiLeptonAnalyzerBase
from CMGTools.HToZZTo4Leptons.tools.OverlapCleaner import OverlapCleaner 

from CMGTools.HToZZTo4Leptons.tools.FSRRecovery import FSRRecovery

from CMGTools.RootTools.utils.DeltaR import deltaR

from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon


        
class FakeRateAnalyzer( MultiLeptonAnalyzerBase ):

    LeptonClass1 = Muon
    LeptonClass2 = Electron

    def declareHandles(self):
        super(FakeRateAnalyzer, self).declareHandles()
        muons = 'cmgMuonSel'
        self.handles['leptons1'] = AutoHandle(
            muons,
            'std::vector<cmg::Muon>'
            )

        self.handles['leptons2'] = AutoHandle(
            'cmgElectronSel',
            'std::vector<cmg::Electron>'
            )

    def beginLoop(self):
        super(FakeRateAnalyzer,self).beginLoop()
        self.counters.addCounter('TriLepton')
        count = self.counters.counter('TriLepton')
        count.register('all events')
        

       
    def process(self, iEvent, event):
        super(FakeRateAnalyzer,self).process(iEvent,event)

        #startup counter
        self.counters.counter('TriLepton').inc('all events')
        
        #Get photons
        self.buildPhotonList( event )
        #get leptons
        self.buildLeptonList( event )
        #get jets
        self.buildJetList(event)

        


        #create a cut flow
        cutFlow = CutFlowMaker(self.counters.counter("TriLepton"),event,event.leptons1,event.leptons2)
        #Cuts :Apply minimal criteria like sip<100 and min Pt and eta and require at least two leptons 
        passed=cutFlow.applyDoubleCut(self.testLeptonSkim1,self.testLeptonSkim2,'lepton preselection',3,'skimmedLeptons1','skimmedLeptons2')
        if not passed: return False

        #merge the two output collections in one non overlapping
        cutFlow.mergeResults('skimmedLeptons')


        passed = cutFlow.applyCut(self.testLeptonLoose,'looseID',3,'looseLeptons')


        #Remove any electrons that are near to tight muons!
        cleanOverlap = OverlapCleaner(event.skimmedLeptons,0.05,11,13,self.testMuonCleaning)
        passed = cutFlow.applyCut(cleanOverlap,'electron cross cleaning',3,'cleanLeptons')



        if not passed: return False

        #ask exactly three leptons
        if len(event.cleanLeptons) !=3:
            return False

        event.leptonPairs = self.findPairs(cutFlow.obj1,event.photons)
        cutFlow.setSource1(event.leptonPairs)

        
        #require that    OS/SF
        passed=cutFlow.applyCut(self.testZSkim,'2l at least one z',1,'zBosons')

        #require tight ID
        passed=cutFlow.applyCut(self.testZTightID,'2l  tight ID',1,'zBosonsTightID')

        #Apply also M>40 and M<120 cut for comparing with others
        passed=cutFlow.applyCut(self.testZ1Mass,'2l MZ less than 120',1,'zBosonsMass')

        if not passed: return False
        
        event.bestZForFakeRate = self.bestZBosonByMass(cutFlow.obj1)
                
        #find the additional lepton
        event.leptonsForFakeRate = copy.copy(event.cleanLeptons)
        event.leptonsForFakeRate.remove( event.bestZForFakeRate.leg1)
        event.leptonsForFakeRate.remove( event.bestZForFakeRate.leg2)



        if deltaR(event.leptonsForFakeRate[0].eta(),event.leptonsForFakeRate[0].phi(), \
                  event.bestZForFakeRate.leg1.eta(),event.bestZForFakeRate.leg1.phi())>0.02 and \
                  deltaR(event.leptonsForFakeRate[0].eta(),event.leptonsForFakeRate[0].phi(), \
                         event.bestZForFakeRate.leg2.eta(),event.bestZForFakeRate.leg2.phi())>0.02:
            minmass=True
            if event.leptonsForFakeRate[0].charge()+event.bestZForFakeRate.leg1.charge()==0 and \
                   (event.leptonsForFakeRate[0].p4()+event.bestZForFakeRate.leg1.p4()).M()<self.cfg_ana.minMass:
                minmass=False
            if event.leptonsForFakeRate[0].charge()+event.bestZForFakeRate.leg2.charge()==0 and \
                   (event.leptonsForFakeRate[0].p4()+event.bestZForFakeRate.leg2.p4()).M()<self.cfg_ana.minMass:
                minmass=False


            if  hasattr(self.cfg_ana,"FSR"):
                fsrAlgo=FSRRecovery(self.cfg_ana.FSR)
                fsrAlgo.setPhotons(event.photons)
                fsrAlgo.setLeg(event.leptonsForFakeRate[0])
                fsrAlgo.recoverLeg()
                if not minmass:
                    event.leptonsForFakeRate=[]

                    

        
        
        return True
    

    def testLeptonSkim1(self, leg,sel = None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
                   super( FakeRateAnalyzer, self).testLeptonSkim1( leg,sel )
        else:
            return super( FakeRateAnalyzer, self).testLeptonSkim2( leg,sel )

    def testLeptonSkim2(self, leg,sel = None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
                   super( FakeRateAnalyzer, self).testLeptonSkim1( leg,sel )
        else:
            return super( FakeRateAnalyzer, self).testLeptonSkim2( leg,sel )



    def testLeptonLoose(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonLoose(leg) and \
               super( FakeRateAnalyzer, self).testLeptonLoose( leg,sel )
        else:
            return self.testElectronLoose(leg) and \
            super( FakeRateAnalyzer, self).testLeptonLoose( leg,sel )
                
    def testLeptonTight(self, leg,sel=None):
        if abs(leg.pdgId()) == 13:
            return self.testMuonTight(leg) and \
               super( FakeRateAnalyzer, self).testLeptonTight( leg,sel )
        if abs(leg.pdgId()) == 11:
            return self.testElectronTight(leg) and \
               super( FakeRateAnalyzer, self).testLeptonTight( leg,sel )
