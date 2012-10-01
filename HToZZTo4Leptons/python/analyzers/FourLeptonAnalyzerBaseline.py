
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



        
class FourLeptonAnalyzerBaseline( MultiLeptonAnalyzerBase ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def declareHandles(self):
        super(FourLeptonAnalyzerBaseline, self).declareHandles()

    def beginLoop(self):
        super(FourLeptonAnalyzerBaseline,self).beginLoop()
        self.counters.addCounter('FourLepton')
        count = self.counters.counter('FourLepton')
        count.register('all events')

        

       
    def process(self, iEvent, event):
        super(FourLeptonAnalyzerBaseline,self).process(iEvent,event)
        
        keepThisEvent=False

        #startup counter
        self.counters.counter('FourLepton').inc('all events')


        
        #Get photons
        self.buildPhotonList( event )
        #get leptons
        self.buildLeptonList( event )
        #get jets
        self.buildJetList(event)

        


        #create a cut flow
        cutFlow = CutFlowMaker(self.counters.counter("FourLepton"),event,event.leptons1,event.leptons2)
        #Cuts :Apply minimal criteria like sip<100 and min Pt and eta and require at least two leptons 
        passed=cutFlow.applyDoubleCut(self.testLeptonSkim1,self.testLeptonSkim2,'lepton preselection',2,'skimmedLeptons1','skimmedLeptons2')

        #merge the two output collections in one non overlapping
        cutFlow.mergeResults('skimmedLeptons')

        #Remove any electrons that are near to tight muons!
        cleanOverlap = OverlapCleaner(event.skimmedLeptons,0.05,11,13,self.testMuonCleaning)
        passed = cutFlow.applyCut(cleanOverlap,'electron cross cleaning',4,'cleanLeptons')



        #Now create four Lepton candidtes upstream. Use all permutations and not combinations
        #of leptons so we can pick the best Z1 and Z2
        event.fourLeptons = self.findQuads(event.cleanLeptons,event.photons)

        #Sort them by M1 near Z and My highest Pt sum
        event.sortedFourLeptons = self.sortFourLeptons(event.fourLeptons)

        #Give the sorted four leptons in our cut flow 
        cutFlow.setSource1(event.sortedFourLeptons)

        #Ghost Suppression
        passed=cutFlow.applyCut(self.testFourLeptonGhostSuppression,'ghost suppression',1,'fourLeptonsGhostSup')


        #tight ID for Z1
        passed=cutFlow.applyCut(self.testFourLeptonTightIDZ1,'4l z1 tight ID',1,'fourLeptonsTightIDZ1')


        #Next Step : Apply Loose Lepton Selection for Z1 
        passed=cutFlow.applyCut(self.testFourLeptonLooseID,'4l loose lepton id',1,'fourLeptonsLooseID')



        #Require Z1 OS/SF and mass cuts
        passed=cutFlow.applyCut(self.testFourLeptonZ1,'4l pair 1 built',1,'fourLeptonsZ1')



        #Now remove all combinations that have different Z1 as the first
        
        event.prunedZ1 = self.pruneFourLeptonsForZ1(event.fourLeptonsZ1)
        cutFlow.setSource1(event.prunedZ1)

        #Check SF for both pairs 
        passed=cutFlow.applyCut(self.testFourLeptonSF,'4l pair 2  SF',1,'fourLeptonsSFZ2')


        #Pt Cuts (CAREFUL: The correct cut is : Any combination of leptons must be 20/10 not the Z1 ones
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'4l Pt Thresholds',1,'fourLeptonsFakeRateApp')



        if passed:
            #calculate mela massErrors and vbf
            self.boostFourLeptons(event.fourLeptonsFakeRateApp,event)
            event.higgsCandLoose = cutFlow.obj1[0]
            keepThisEvent=True
        #OS Z2    
        passed=cutFlow.applyCut(self.testFourLeptonOS,'4l pair 2  OS',1,'fourLeptonsOSZ2')


        #tight ID    
        passed=cutFlow.applyCut(self.testFourLeptonTightID,'4l tight lepton id',1,'fourLeptonsTightID')



        #HERE PICK ONLY ONE
        bestZ2=[]
        if passed:
            bestZ2 = [cutFlow.obj1[0]]

        cutFlow.setSource1(bestZ2)    
        #Mass cuts for second lepton
        passed=cutFlow.applyCut(self.testFourLeptonMassZ2,'4l pair 2 mass cut',1,'fourLeptonsMassZ2')

#        Pt Cuts (CAREFUL: The correct cut is : Any combination of leptons must be 20/10 not the Z1 ones
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'4l Pt Thresholds2',1,'fourLeptonsPtThres2')

        #QCD suppression
        passed=cutFlow.applyCut(self.testFourLeptonMinOSMass,'4l QCD suppression',1,'fourLeptonsQCDSuppression')

        #Z -> 4 l phase space
        passed=cutFlow.applyCut(self.testFourLeptonMassZ,'4l Z phase space',1,'fourLeptonsZPhaseSpace')


        #Z2 Mass Tight Cut
        passed=cutFlow.applyCut(lambda x: x.leg2.mass()>12.,'4l Tight Mass2',1,'fourLeptonsTightZ2')

        event.otherLeptons=[]
        event.otherTightLeptons=[]
        if passed:
            event.higgsCand = cutFlow.obj1[0]
            event.otherLeptons=copy.copy(event.cleanLeptons)
            event.otherLeptons.remove(event.higgsCand.leg1.leg1)
            event.otherLeptons.remove(event.higgsCand.leg1.leg2)
            event.otherLeptons.remove(event.higgsCand.leg2.leg1)
            event.otherLeptons.remove(event.higgsCand.leg2.leg2)
            event.otherLeptons = filter(lambda x:x.pt()>10,event.otherLeptons)
            event.otherTightLeptons = filter(self.testLeptonTight,event.otherLeptons)
            metV = TLorentzVector(event.met.px(),event.met.py(),event.met.pz(),event.met.energy())

            event.recoil = (-metV-event.higgsCand).Pt()

        #ZZ phase smace
        passed=cutFlow.applyCut(self.testFourLeptonMass,'4l H phase space',1,'fourLeptonsHPhaseSpace')


        passed=cutFlow.applyCut(lambda x: x.mela>0.1,'MELA',1,'fourLeptonsMELA')

        passed=cutFlow.applyCut(lambda x: hasattr(x.leg1,'fsrPhoton') or hasattr(x.leg2,'fsrPhoton') ,'FSR',1,'fourLeptonsWithFSR')



        
        return  keepThisEvent
    

