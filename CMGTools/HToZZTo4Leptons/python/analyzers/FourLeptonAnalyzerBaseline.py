
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

from CMGTools.HToZZTo4Leptons.analyzers.FourLeptonAnalyzerBase import FourLeptonAnalyzerBase
from CMGTools.HToZZTo4Leptons.tools.OverlapCleaner import OverlapCleaner 

from CMGTools.HToZZTo4Leptons.tools.FSRRecovery import FSRRecovery




        
class FourLeptonAnalyzerBaseline( FourLeptonAnalyzerBase ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def declareHandles(self):
        super(FourLeptonAnalyzerBaseline, self).declareHandles()

    def beginLoop(self):
        super(FourLeptonAnalyzerBaseline,self).beginLoop()
        

       
    def process(self, iEvent, event):
        super(FourLeptonAnalyzerBaseline,self).process(iEvent,event)
        
        self.readCollections( iEvent )
        vertex = event.goodVertices[0]

        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        event.step = 0

        #set the vertex
        event.vertex=vertex


        #startup counter
        self.counters.counter('FourLepton').inc('all events')

        #basic event quantities
        event.met = self.handles['met'].product()[0]
        event.rhoMu = self.handles['rhoMu'].product()[0]
        event.rhoEle = self.handles['rhoEle'].product()[0]
        self.rhoMu = event.rhoMu
        self.rhoEle = event.rhoEle

        
        #Get photons
        self.buildPhotonList( event )

        self.buildLeptonList( event )

        #create a cut flow
        cutFlow = CutFlowMaker(self.counters.counter("FourLepton"),event,event.leptons1,event.leptons2)
        #Cuts :Apply minimal criteria like sip<100 and min Pt and eta and require at least two leptons 
        passed=cutFlow.applyDoubleCut(self.testLeptonSkim1,self.testLeptonSkim2,'lepton preselection',2,'skimmedLeptons1','skimmedLeptons2')
        if passed: event.step += 1


        #merge the two output collections in one non overlapping
        cutFlow.mergeResults('skimmedLeptons')

        #Remove any electrons that are near to tight muons!
        cleanOverlap = OverlapCleaner(event.skimmedLeptons,0.05,11,13,self.testMuonCleaning)
        passed = cutFlow.applyCut(cleanOverlap,'electron cross cleaning',2,'cleanLeptons')


        #make lepton combinations 
        event.leptonPairs = self.findPairsWithFSR(cutFlow.obj1,event.photons)
        cutFlow.setSource1(event.leptonPairs)
        
        #require that    OS/SF
        passed=cutFlow.applyCut(self.testZSkim,'2l at least one z',1,'zBosons')

        #require tight ID
        passed=cutFlow.applyCut(self.testZ1TightID,'2l  tight ID',1,'zBosonsTightID')

        #Apply also M>40 and M<120 cut for comparing with others
        passed=cutFlow.applyCut(self.testZ1Mass,'2l MZ less than 120',1,'zBosonsMass')

        #if no zs throw the event
        if not passed: return self.cfg_ana.keep
        event.step += 1

        #Find the best of the Zs
        if passed:
            event.bestZForFakeRate = self.bestZBosonByMass(cutFlow.obj1)

            #####FAKE RATE MEASUREMENT BUSINESS#############
            ###############################################
            
            event.leptonsForFakeRate = copy.copy(event.cleanLeptons)
            event.leptonsForFakeRate.remove( event.bestZForFakeRate.leg1)
            event.leptonsForFakeRate.remove( event.bestZForFakeRate.leg2)

            #Require 1 lepton !

            # So lets make a collection for those
            if len(event.leptonsForFakeRate)==1:
                for lepton in event.leptonsForFakeRate:
                    if  hasattr(self.cfg_ana,"FSR"):
                        fsrAlgo=FSRRecovery(self.cfg_ana.FSR)
                        fsrAlgo.setPhotons(event.photons)
                        fsrAlgo.setLeg(lepton)
                        fsrAlgo.recoverLeg()
            else:
                event.leptonsForFakeRate=[]




        #Now create four Lepton candidtes upstream. Use all permutations and not combinations
        #of leptons so we can pick the best Z1 and Z2
        event.fourLeptons = self.findQuadsWithFSR(event.cleanLeptons,event.photons)


        #Sort them by M1 near Z and My highest Pt sum
        event.sortedFourLeptons = self.sortFourLeptons(event.fourLeptons)

        cutFlow.setSource1(event.sortedFourLeptons)


        #tight ID for Z1
        passed=cutFlow.applyCut(self.testFourLeptonTightIDZ1,'4l z1 tight ID',1,'fourLeptonsTightIDZ1')

        #Next Step : Apply Tight Lepton Selection for Z1 
        passed=cutFlow.applyCut(self.testFourLeptonLooseID,'4l loose lepton id',1,'fourLeptonsLooseID')
#        passed=cutFlow.applyCut(self.testFourLeptonTightID,'4l tighter lepton id',1,'fourLeptonsTightIDPre')

        #Require Z1 OS/SF and mass cuts
        passed=cutFlow.applyCut(self.testFourLeptonZ1,'4l pair 1 built',1,'fourLeptonsZ1')


        #Now remove all combinations that have different Z1 as the first
        if passed:
            event.prunedZ1 = self.pruneFourLeptonsForZ1(event.fourLeptonsZ1)
            cutFlow.setSource1(event.prunedZ1)
            

        
        passed=cutFlow.applyCut(self.testFourLeptonSF,'4l pair 2  SF',1,'fourLeptonsSFZ2')


        #Pt Cuts (CAREFUL: The correct cut is : Any combination of leptons must be 20/10 not the Z1 ones
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'4l Pt Thresholds',1,'fourLeptonsFakeRateApp')


        if passed:
            event.higgsCandLoose = cutFlow.obj1[0]

        #OS Z2    
        passed=cutFlow.applyCut(self.testFourLeptonOS,'4l pair 2  OS',1,'fourLeptonsOSZ2')


        #tight ID    
        passed=cutFlow.applyCut(self.testFourLeptonTightID,'4l tight lepton id',1,'fourLeptonsTightID')


        #Mass cuts for second lepton
        passed=cutFlow.applyCut(self.testFourLeptonMassZ2,'4l pair 2 mass cut',1,'fourLeptonsMassZ2')

        #Pt Cuts (CAREFUL: The correct cut is : Any combination of leptons must be 20/10 not the Z1 ones
#        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'4l Pt Thresholds',1,'fourLeptonsFakeRateApp')

        #QCD suppression
        passed=cutFlow.applyCut(self.testFourLeptonMinOSMass,'4l QCD suppression',1,'fourLeptonsQCDSuppression')

        #Z -> 4 l phase space
        passed=cutFlow.applyCut(self.testFourLeptonMassZ,'4l Z phase space',1,'fourLeptonsZPhaseSpace')


        #Z2 Mass Tight Cut
        passed=cutFlow.applyCut(lambda x: x.leg2.mass()>12.,'4l Tight Mass2',1,'fourLeptonsTightZ2')

           

        #calculate mela
        for fl in event.fourLeptonsTightZ2:
            fl.mela=self.mela.calculate(fl)

        if passed:
            event.higgsCand = cutFlow.obj1[0]

        #ZZ phase smace
        passed=cutFlow.applyCut(self.testFourLeptonMass,'4l H phase space',1,'fourLeptonsHPhaseSpace')


        passed=cutFlow.applyCut(lambda x: x.mela>0.1,'MELA',1,'fourLeptonsMELA')

        passed=cutFlow.applyCut(lambda x: hasattr(x.leg1,'fsrPhoton') or hasattr(x.leg2,'fsrPhoton') ,'FSR',1,'fourLeptonsWithFSR')



        
        return True
    
