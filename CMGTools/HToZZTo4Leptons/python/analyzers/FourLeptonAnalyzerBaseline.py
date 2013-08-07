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

from CMGTools.HToZZTo4Leptons.tools.LineShapeWeights import LineShapeWeights


        
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

        self.lineShapeWeights = LineShapeWeights(self.cfg_comp)

       
    def process(self, iEvent, event):
        super(FourLeptonAnalyzerBaseline,self).process(iEvent,event)
        
        #apply lineshape weights
        self.lineShapeWeights.applyWeight(event)

        #startup counter
        self.counters.counter('FourLepton').inc('all events')

        #create a cut flow
        cutFlow = CutFlowMaker(self.counters.counter("FourLepton"),event,event.leptons1,event.leptons2)
        #Cuts :Apply minimal criteria like sip<100 and min Pt and eta and require at least two leptons 
        passed=cutFlow.applyDoubleCut(self.testLeptonSkim1,self.testLeptonSkim2,'lepton preselection',2,'skimmedLeptons1','skimmedLeptons2')

        #merge the two output collections in one non overlapping
        cutFlow.mergeResults('skimmedLeptons')
        #define loose leptons
        passed = cutFlow.applyCut(self.testLeptonLoose,'loose ID',2,'looseLeptons')
        
        cleanOverlap = OverlapCleaner(event.looseLeptons,0.05,11,13,self.testMuonCleaning)
        #Remove any electrons that are near to tight muons!
        passed = cutFlow.applyCut(cleanOverlap,'electron cross cleaning',2,'cleanLeptons')
        #good leptons
        passed = cutFlow.applyCut(self.testLeptonGood,'Good leptons',2,'goodLeptons')


        self.FSR.setElectronID(self.testLeptonGood)
        self.FSR.setLeptons(cutFlow.obj1)
        self.FSR.attachPhotons(event.photons)

        #Create pair
        event.leptonPairs = self.findPairs(cutFlow.obj1)
        cutFlow.setSource1(event.leptonPairs)

        #require ghost suppresiion
        passed=cutFlow.applyCut(self.testDiLeptonGhostSuppression,'Z1 ghost suppression ',1,'zBosonsGhostSup')

        
        #require that    OS/SF
        passed=cutFlow.applyCut(self.testZSkim,'2l at least one z',1,'zBosons')

        #require tight ID
        passed=cutFlow.applyCut(self.testZTightID,'2l  tight ID',1,'zBosonsTightID')

        if passed:
            event.bestZ = self.bestZBosonByMass(cutFlow.obj1)
            event.leptonsForZ2 = copy.copy(event.goodLeptons)
            event.leptonsForZ2.remove( event.bestZ.leg1)
            event.leptonsForZ2.remove( event.bestZ.leg2)

            cutFlow.setSource1([event.bestZ])
        else:
            cutFlow.setSource1([])
            event.leptonsForZ2 = []
            
        #Apply also M>40 and M<120 cut for comparing with others
        passed=cutFlow.applyCut(self.testZ1Mass,'2l MZ less than 120',1,'zBosonsMass')



        if not passed:    
            event.bestZ = None
            event.leptonsForZ2=[]

        cutFlow.setSource1(event.leptonsForZ2)
        passed=cutFlow.applyCut(self.testLeptonGood,'4 good leptons',2,'looseLeptons2')

            
        #make pairs for Z2
        event.leptonPairs2 = self.findPairs(cutFlow.obj1)
        cutFlow.setSource1(event.leptonPairs2)



        passed=cutFlow.applyCut(self.testZTightID,'tight ID for Z2',1,'zBosons2TightID')
        #Z2 SF 
        passed=cutFlow.applyCut(self.testZSF,'SF for Z2',1,'zBosons2SF')

        #Z2 SF and OS
        passed=cutFlow.applyCut(self.testZOS,'OS for Z2',1,'zBosons2OS')

        #pick the highest pt pair
        sortedZ2ByPt=sorted(cutFlow.obj1,key=lambda x: x.leg1.pt()+x.leg2.pt(),reverse=True)

        if len(sortedZ2ByPt)>0:
            event.bestZ2 = sortedZ2ByPt[0]
            cutFlow.setSource1([event.bestZ2])
        else:
            cutFlow.setSource1([])
            event.bestZ2 = None
        #Z2 mass
        passed=cutFlow.applyCut(self.testZ2Mass,'Mass for Z2',1,'zBosons2Mass')

        if event.bestZ is not None and passed:
            event.fourLeptons = [self.mergePairs(event.bestZ,cutFlow.obj1[0])]
            self.boostFourLeptons(event.fourLeptons,event)
            cutFlow.setSource1(event.fourLeptons)
        else:
            cutFlow.setSource1([])


        #Ghost Suppression
        passed=cutFlow.applyCut(self.testFourLeptonGhostSuppression,'ghost suppression',1,'fourLeptonsGhostSup')
        #Check SF for both pairs 
        passed=cutFlow.applyCut(self.testFourLeptonSF,'4l pair SF',1,'fourLeptonsSFZ2')
        #Pt Cuts (CAREFUL: The correct cut is : Any combination of leptons must be 20/10 not the Z1 ones
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'4l Pt Thresholds',1,'fourLeptonsPt')
        #QCD suppression
        passed=cutFlow.applyCut(self.testFourLeptonMinOSMass,'4l QCD suppression',1,'fourLeptonsQCDSuppression')
        #Z -> 4 l phase space
        passed=cutFlow.applyCut(self.testFourLeptonMassZ,'4l Z phase space',1,'fourLeptonsZPhaseSpace')
        #Z2 Mass Tight Cut
        passed=cutFlow.applyCut(lambda x: x.leg2.mass()>12.,'4l Tight Mass2',1,'fourLeptonsTightZ2')

        if passed:
            event.higgsCand = cutFlow.obj1[0]
            event.otherLeptons=copy.copy(event.cleanLeptons)
            event.otherLeptons.remove(event.higgsCand.leg1.leg1)
            event.otherLeptons.remove(event.higgsCand.leg1.leg2)
            event.otherLeptons.remove(event.higgsCand.leg2.leg1)
            event.otherLeptons.remove(event.higgsCand.leg2.leg2)
            event.otherLeptons = filter(self.testLeptonTight,event.otherLeptons)
            event.otherLeptons = filter(lambda x: x.pt>10.,event.otherLeptons)


            metV = TLorentzVector(event.met.px(),event.met.py(),event.met.pz(),event.met.energy())
            event.recoil = (-metV-event.higgsCand).Pt()
            if len(event.otherLeptons)>0:
                event.otherLeptons =sorted(event.otherLeptons,key=lambda x: x.pt(),reverse=True)
                event.higgsCand.leptonTag = event.otherLeptons[0]
                
        #ZZ phase smace
        passed=cutFlow.applyCut(self.testFourLeptonMass,'4l H phase space',1,'fourLeptonsHPhaseSpace')



        
        passed=cutFlow.applyCut(lambda x: x.KD>0.1,'KD',1,'fourLeptonsMELA')
        passed=cutFlow.applyCut(lambda x: hasattr(x.leg1,'fsrPhoton') or hasattr(x.leg2,'fsrPhoton') ,'FSR',1,'fourLeptonsWithFSR')
        passed=cutFlow.applyCut(lambda x: hasattr(x.leg1,'fsrPhoton') and hasattr(x.leg2,'fsrPhoton') ,'Both FSR',1,'fourLeptonsWithFSR')
        cutFlow.setSource1(event.fourLeptonsHPhaseSpace)
        passed=cutFlow.applyCut(lambda x: x.gg0KD>0.3,'PseudoKD',1,'fourLeptonsPSMELA')
        cutFlow.setSource1(event.fourLeptonsHPhaseSpace)
        passed=cutFlow.applyCut(lambda x: x.qq2PlusKD>0.15,'GraviKD',1,'fourLeptonsSpinTwoMELA')



        cutFlow.setSource1(event.fourLeptonsHPhaseSpace)
        passed=cutFlow.applyCut(lambda x: x.jets['nJets']>=1,'NJEtsaboveone',1,'fourLeptonsOneJet')
        passed=cutFlow.applyCut(lambda x: x.jets['nJets']==2,'NJEtsTwo',1,'fourLeptonsTwoJets')
        passed=cutFlow.applyCut(lambda x: x.jets['nJets']==2 and abs(x.jets['dEta'])>3 and x.jets['Mjj']>300.,'VBF',1,'fourLeptonsVBF')  




        ##################NOW A PATH FOR FAKES
        if event.bestZ is not None:
            event.leptonsForLooseZ2 = copy.copy(event.looseLeptons)
            event.leptonsForLooseZ2.remove( event.bestZ.leg1)
            event.leptonsForLooseZ2.remove( event.bestZ.leg2)

        else:
            event.leptonsForLooseZ2=[]

        self.correctFakeWeights(event)
        self.FSR.setElectronID(self.testLeptonLoose)
        self.FSR.setLeptons(event.leptonsForLooseZ2)
        if hasattr(event.bestZ,'fsrPhoton'):
            event.photons.remove(event.bestZ.fsrPhoton)
        self.FSR.attachPhotons(event.photons)
        event.leptonPairsLoose = self.findPairs(event.leptonsForLooseZ2)
        cutFlow.setSource1(event.leptonPairsLoose)
        #Z2 SF 
        passed=cutFlow.applyCut(self.testZSF,'SF for Z2 Loose',1,'zBosons2SFLoose')
        #Z2 SS 
        passed=cutFlow.applyCut(lambda x: x.charge()!=0,'SF for Z2 Loose',1,'zBosons2SFLoose')
        #Z2 SIP CUT
        passed=cutFlow.applyCut(lambda x: abs(x.leg1.sip3D())<4 and abs(x.leg2.sip3D())<4,'SIP cut for Z2 loose',1,'zBosons2SIP')
        #pick the highest pt pair
        sortedZ2ByPtLoose=sorted(cutFlow.obj1,key=lambda x: x.leg1.pt()+x.leg2.pt(),reverse=True)

        if len(sortedZ2ByPtLoose)>0:
            event.bestZ2Loose = sortedZ2ByPtLoose[0]
            cutFlow.setSource1([event.bestZ2Loose])
        else:
            cutFlow.setSource1([])
            event.bestZ2Loose = None
        #Z2 mass
        passed=cutFlow.applyCut(self.testZ2Mass,'Mass for Z2 Loose',1,'zBosons2MassLoose')

        if event.bestZ is not None and passed:
            event.fourLeptonsLoose = [self.mergePairs(event.bestZ,cutFlow.obj1[0])]
            self.boostFourLeptons(event.fourLeptonsLoose,event)
            cutFlow.setSource1(event.fourLeptonsLoose)
        else:
            cutFlow.setSource1([])
        #Ghost Suppression
        passed=cutFlow.applyCut(self.testFourLeptonGhostSuppression,'ghost suppression Loose',1,'fourLeptonsGhostSupLoose')
        #Check SF for both pairs 
        passed=cutFlow.applyCut(self.testFourLeptonSF,'4l pair SF Loose',1,'fourLeptonsSFZ2Loose')
        #Pt Cuts (CAREFUL: The correct cut is : Any combination of leptons must be 20/10 not the Z1 ones
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'4l Pt Thresholds Loose',1,'fourLeptonsPt Loose')
        #Z -> 4 l phase space
        passed=cutFlow.applyCut(self.testFourLeptonMassZ,'4l Z phase space Loose',1,'fourLeptonsZPhaseSpaceLoose')
        if passed:
            event.higgsCandLoose = cutFlow.obj1[0]



        ##################NOW A SECONDPATH FOR FAKES
        cutFlow.setSource1(event.leptonPairsLoose)
        #Z2 SF 
        passed=cutFlow.applyCut(self.testZSF,'OS for Z2 Loose',1,'zBosons3SFLoose')
        #Z2 OS 
        passed=cutFlow.applyCut(lambda x: x.charge()==0,'SF for Z2 Loose',1,'zBosons3SFLoose')
        #Z2 SIP CUT
        passed=cutFlow.applyCut(lambda x: abs(x.leg1.sip3D())<4 and abs(x.leg2.sip3D())<4,'SIP cut for Z2 loose',1,'zBosons3SIP')
        #pick the highest pt pair
        sortedZ3ByPtLoose=sorted(cutFlow.obj1,key=lambda x: x.leg1.pt()+x.leg2.pt(),reverse=True)

        if len(sortedZ3ByPtLoose)>0:
            event.bestZ3Loose = sortedZ3ByPtLoose[0]
            cutFlow.setSource1([event.bestZ3Loose])
        else:
            cutFlow.setSource1([])
            event.bestZ3Loose = None
        #Z2 mass
        passed=cutFlow.applyCut(self.testZ2Mass,'Mass for Z2 Loose',1,'zBosons3MassLoose')

        if event.bestZ is not None and passed:
            event.fourLeptonsLooseOS = [self.mergePairs(event.bestZ,cutFlow.obj1[0])]
            self.boostFourLeptons(event.fourLeptonsLooseOS,event)
            cutFlow.setSource1(event.fourLeptonsLooseOS)
        else:
            cutFlow.setSource1([])
        #Ghost Suppression
        passed=cutFlow.applyCut(self.testFourLeptonGhostSuppression,'ghost suppression Loose OS',1,'fourLeptonsGhostSupLooseOS')
        #Check SF for both pairs 
        passed=cutFlow.applyCut(self.testFourLeptonSF,'4l pair SF Loose OS',1,'fourLeptonsSFZ2LooseOS')
        #Pt Cuts (CAREFUL: The correct cut is : Any combination of leptons must be 20/10 not the Z1 ones
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'4l Pt Thresholds Loose OS',1,'fourLeptonsPtLooseOS')
        #Z -> 4 l phase space
        passed=cutFlow.applyCut(self.testFourLeptonMassZ,'4l Z phase space LooseOS',1,'fourLeptonsZPhaseSpaceLooseOS')
        if passed:
            event.higgsCandLooseOS = cutFlow.obj1[0]

        if hasattr(event,'higgsCand') or hasattr(event,'higgsCandLoose') or hasattr(event,'higgsCandLooseOS'):
            return True
        
        return  False
    

