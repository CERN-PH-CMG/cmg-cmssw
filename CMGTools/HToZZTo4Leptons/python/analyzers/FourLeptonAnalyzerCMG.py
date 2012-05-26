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

from CMGTools.HToZZTo4Leptons.analyzers.FSRRecovery import FSRRecovery




        
class FourLeptonAnalyzerCMG( FourLeptonAnalyzerBase ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def declareHandles(self):
        super(FourLeptonAnalyzerCMG, self).declareHandles()

    def beginLoop(self):
        super(FourLeptonAnalyzerCMG,self).beginLoop()


       
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
        self.rho = event.rho

        #Get photons
        self.buildPhotonList( event )

        
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


        #make lepton combinations 
        event.leptonPairs = self.findPairsWithFSR(cutFlow.obj1,event.photons)
        cutFlow.setSource1(event.leptonPairs)
        
        #require that   M>40 and OS/SF
        passed=cutFlow.applyCut(self.testZSkim,'2l at least one z',1,'zBosons')


        #Apply also M<120 cut for comparing with others
        passed=cutFlow.applyCut(self.testZ1Mass,'2l MZ less than 120',1,'zBosonsMass')


        #Now we will do the fake rate thing. We will apply full ID on the Z
        #Then find the best Z . Then find the leptons that dont belong to the Z
        #We will use those leptons for fake rate measurement
        passed=cutFlow.applyCut(self.testZ1TightID,'2l  tight ID',1,'zBosonsTightID')


        #if no zs throw the event
        if not passed: return self.cfg_ana.keep
        event.step += 1


        #Find the best of the Zs
        if passed:
            event.bestZForFakeRate = self.bestZBosonByMass(event.zBosonsTightID)

            
            #####FAKE RATE MEASUREMENT BUSINESS#############
            ###############################################
            
            event.leptonsForFakeRate = copy.copy(event.cleanLeptons)
            event.leptonsForFakeRate.remove( event.bestZForFakeRate.leg1)
            event.leptonsForFakeRate.remove( event.bestZForFakeRate.leg2)
            event.leptonsForFakeRateWithPhoton=copy.copy(event.leptonsForFakeRate)

            #To measure fake rate with FSR we need to measure it with photons
            # So lets make a collection for those
            for lepton in event.leptonsForFakeRateWithPhoton:
                if  hasattr(self.cfg_ana,"FSR"):
                    fsrAlgo=FSRRecovery(self.cfg_ana.FSR)
                    fsrAlgo.setPhotons(event.photons)
                    fsrAlgo.setLeg(lepton)
                    fsrAlgo.recoverLeg()

            event.leptonsForFakeRateWithPhoton=filter(lambda x: hasattr(x,'fsrPhoton'),event.leptonsForFakeRateWithPhoton)        

            
            #sort them by highest Pt
            event.leptonsForFakeRate.sort(key=lambda x: x.pt(),reverse=True)
            event.leptonsForFakeRateWithPhoton.sort(key=lambda x: x.pt(),reverse=True)

            
        #OK in the post analysis process we will use those leptons to measure the fake rate
        #Nothing else to be done for that

        #Now create four Lepton candidtes upstream. Use all permutations and not combinations
        #of leptons so we can pick the best Z1 and Z2
        event.fourLeptons = self.findQuadsWithFSR(event.cleanLeptons,event.photons)
        #Sort them by M1 near Z and My highest Pt sum
        self.sortFourLeptons(event.fourLeptons)
        cutFlow.setSource1(event.fourLeptons)
        #Next Step : Apply Loose Lepton Selection
        passed=cutFlow.applyCut(self.testFourLeptonLooseID,'4l loose lepton id',1,'fourLeptonsLooseID')
        #Require Z1 OS/SF and mass cuts
        passed=cutFlow.applyCut(self.testFourLeptonZ1,'4l pair 1 built',1,'fourLeptonsZ1')
        #Apply Tight ID for Z1
        passed=cutFlow.applyCut(self.testFourLeptonTightIDZ1,'4l  Z1 tightID',1,'fourLeptonsTightZ1')
        #Search for second SF pair
        passed=cutFlow.applyCut(self.testFourLeptonSF,'4l pair 2  SF',1,'fourLeptonsSFZ2')
        #Mass cuts for second lepton
        passed=cutFlow.applyCut(self.testFourLeptonMassZ1Z2,'4l pair 2 mass cut',1,'fourLeptonsMassZ2')
        #Pt Cuts (CAREFUL: The correct cut is : Any combination of leptons must be 20/10 not the Z1 ones
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'4l Pt Thresholds',1,'fourLeptonsFakeRateApp')
        #####THIS IS OUR FAKE RATE APPLICATION REGION
        if passed:
            event.higgsCandLoose = cutFlow.obj1[0]
        #Search for second OS pair
        passed=cutFlow.applyCut(self.testFourLeptonOS,'4l pair 2  OS',1,'fourLeptonsOSZ2')

        #Now apply tight ID in all leptons
        passed=cutFlow.applyCut(self.testFourLeptonTightID,'4l tight lepton tightid',1,'fourLeptonsTightID')
        
        #QCD suppression
        passed=cutFlow.applyCut(self.testFourLeptonMinMass,'4l QCD suppression',1,'fourLeptonsQCDSuppression')


        #The other analyzer has cuts on M>70 or M>100 . I am totally against a cut at M>100 so I am not
        #putting it here . We need to use the Z->4L peak in the fit simultaneously with the Higgs
        #to exploit the correlations
        

        if passed:
            event.higgsCand = cutFlow.obj1[0]
        
        return True
    
