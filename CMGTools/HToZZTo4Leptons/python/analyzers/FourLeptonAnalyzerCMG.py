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


        
class FourLeptonAnalyzerCMG( MultiLeptonAnalyzerBase ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def declareHandles(self):
        super(FourLeptonAnalyzerCMG, self).declareHandles()

    def beginLoop(self):
        super(FourLeptonAnalyzerCMG,self).beginLoop()
        self.counters.addCounter('FourLepton')
        count = self.counters.counter('FourLepton')
        count.register('all events')

        self.lineShapeWeights = LineShapeWeights(self.cfg_comp)

       
    def process(self, iEvent, event):
        super(FourLeptonAnalyzerCMG,self).process(iEvent,event)
        
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

        #clean overlap for muons
        cleanOverlap = OverlapCleaner(event.looseLeptons,0.05,11,13,self.testMuonCleaning)

        #Remove any electrons that are near to tight muons!
        passed = cutFlow.applyCut(cleanOverlap,'electron cross cleaning',2,'cleanLeptons')

        #Make all combinations of leptons
        event.allFourLeptons = self.findQuads(event.cleanLeptons,event.photons)

        #Sort them by Z1 nearest to the Z Z2 highest sum Pt 
        event.sortedFourLeptons = self.sortFourLeptons(event.allFourLeptons)
        cutFlow.setSource1(event.sortedFourLeptons)

        #Now apply cuts

        #require ghost suppresiion(minimum distance) 
        passed=cutFlow.applyCut(self.testFourLeptonGhostSuppression,' ghost suppression ',1,'FourLeptonsGhostSup')

        #require that  both Z1 and Z2 are   SF
        passed=cutFlow.applyCut(self.testFourLeptonSF,'same flavour',1,'fourLeptonsSF')
        #require Z1 tight ID
        passed=cutFlow.applyCut(self.testFourLeptonTightIDZ1FSRBoth,'Z1 tight ID',1,'fourLeptonsZ1TightID')

        #require Z1 OS/SF and mass
        passed=cutFlow.applyCut(self.testFourLeptonZ1,'good Z1',1,'fourLeptonsGoodZ1')

        #Apply Pt Cuts
        passed=cutFlow.applyCut(self.testFourLeptonPtThr,'Pt cuts ',1,'fourLeptonsPtCuts')

        #Check SF for both pairs 
        passed=cutFlow.applyCut(self.testFourLeptonSF,'4l pair SF',1,'fourLeptonsSFZ2')
        #QCD suppression
        passed=cutFlow.applyCut(self.testFourLeptonMinOSMass,'4l QCD suppression',1,'fourLeptonsLoose')
        #Add KD etc
        self.boostFourLeptons(event.fourLeptonsLoose,event)

        #require Z2 OS
        passed=cutFlow.applyCut(self.testFourLeptonOS,'Z2 OS',1,'fourLeptonsOS')



        #require Z2 tight ID
        passed=cutFlow.applyCut(self.testFourLeptonTightIDZ2FSRBoth,'Z2 tight ID',1,'fourLeptonsTight')

        cutFlow.obj1 = self.sortFourLeptonsByKD(event.fourLeptonsTight)


        #pick only the first one
        if passed:
             #if lepton tag sort differently
             tightLeptons=[]
             for fL in event.fourLeptonsTight:
                 tightLeptons.append(fL.leg1.leg1)
                 tightLeptons.append(fL.leg1.leg2)
                 tightLeptons.append(fL.leg2.leg1)
                 tightLeptons.append(fL.leg2.leg2)
             tightLeptonSet =set(tightLeptons)    
             #             if len(tightLeptonSet)>4:
#                 passed2=cutFlow.applyCut(lambda x: x.leg2.M()<60.,'Z2 mass below 60',1,'fourLeptonsZ2offshell')

             cutFlow.setSource1([cutFlow.obj1[0]])

        #Z2 Mass Tight Cut
        passed=cutFlow.applyCut(self.testFourLeptonMassZ2,'4l Tight Mass2',1,'fourLeptonsTightZ2')
        #Z -> 4 l phase space
        passed=cutFlow.applyCut(self.testFourLeptonMassZ,'4l Z phase space',1,'fourLeptonsZPhaseSpace')


        if passed:
            event.higgsCand = cutFlow.obj1[0]
            metV = TLorentzVector(event.met.px(),event.met.py(),event.met.pz(),event.met.energy())
            event.recoil = (-metV-event.higgsCand).Pt()

            #search for lepton tag(Pick the one with smallest M_Z2)
            event.otherLeptonsTag=copy.copy(event.cleanLeptons)
            event.otherLeptonsTag.remove(event.higgsCand.leg1.leg1)
            event.otherLeptonsTag.remove(event.higgsCand.leg1.leg2)
            event.otherLeptonsTag.remove(event.higgsCand.leg2.leg1)
            event.otherLeptonsTag.remove(event.higgsCand.leg2.leg2)
            event.otherLeptonsTag =sorted(event.otherLeptonsTag,key=lambda x: x.pt(),reverse=True)
            event.otherLeptonsTightTag = filter(self.testLeptonTight,event.otherLeptonsTag)

            if len(event.otherLeptonsTightTag)>0:
                if event.otherLeptonsTightTag[0].pt()>10.:
                    event.higgsCand.leptonTag = event.otherLeptonsTightTag[0]
#                    if event.higgsCand.M()>140:
#                        import pdb;pdb.set_trace();



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

        #Paths for FAKES
        cutFlow.setSource1(event.fourLeptonsLoose)
        #require Z2 SS
        passed=cutFlow.applyCut(lambda x: x.leg2.charge()!=0,'Loose SS',1,'fourLeptonsLooseSS')
        #Z2 SIP CUT
        passed=cutFlow.applyCut(lambda x: abs(x.leg2.leg1.sip3D())<4 and abs(x.leg2.leg2.sip3D())<4,'SIP cut for Z2 OS loose',1,'fourLeptonsLooseSIP')

        #pick only the first one
        if passed:
            cutFlow.setSource1([cutFlow.obj1[0]])


        #Z2 Mass Tight Cut
        passed=cutFlow.applyCut(self.testFourLeptonMassZ2,'4l Loose Mass2',1,'fourLeptonsLooseZ2')
        if passed:
            event.higgsCandLoose =cutFlow.obj1[0]
            #create lepton tag
            event.otherLeptonsTagLoose=copy.copy(event.cleanLeptons)
            event.otherLeptonsTagLoose.remove(event.higgsCandLoose.leg1.leg1)
            event.otherLeptonsTagLoose.remove(event.higgsCandLoose.leg1.leg2)
            event.otherLeptonsTagLoose.remove(event.higgsCandLoose.leg2.leg1)
            event.otherLeptonsTagLoose.remove(event.higgsCandLoose.leg2.leg2)
            event.otherLeptonsTagLoose =filter(lambda x: x.sip3D()<4.,event.otherLeptonsTagLoose)
            event.otherLeptonsTagLoose =sorted(event.otherLeptonsTagLoose,key=lambda x: x.pt(),reverse=True)
            if len(event.otherLeptonsTagLoose)>0:
                if event.otherLeptonsTagLoose[0].pt()>10.:
                    event.higgsCandLoose.leptonTagLoose = event.otherLeptonsTagLoose[0]
            self.correctFakeWeightsComb(event.higgsCandLoose)


        #Second Path for FAKES
        cutFlow.setSource1(event.fourLeptonsLoose)
        #require Z2 OS
        passed=cutFlow.applyCut(self.testFourLeptonOS,'Loose OS',1,'fourLeptonsLooseOS')
        #pick only the first one
        #Z2 SIP CUT
        passed=cutFlow.applyCut(lambda x: abs(x.leg2.leg1.sip3D())<4 and abs(x.leg2.leg2.sip3D())<4,'SIP cut for Z2 OS loose',1,'fourLeptonsLooseOSSIP')

        if passed:
            cutFlow.setSource1([cutFlow.obj1[0]])

        #Z2 Mass Tight Cut
        passed=cutFlow.applyCut(self.testFourLeptonMassZ2,'4l Loose OS Mass2',1,'fourLeptonsLooseOSZ2')
        if passed:
            event.higgsCandLooseOS = cutFlow.obj1[0]

            #create lepton tag
            event.otherLeptonsTagLooseOS=copy.copy(event.cleanLeptons)
            event.otherLeptonsTagLooseOS.remove(event.higgsCandLooseOS.leg1.leg1)
            event.otherLeptonsTagLooseOS.remove(event.higgsCandLooseOS.leg1.leg2)
            event.otherLeptonsTagLooseOS.remove(event.higgsCandLooseOS.leg2.leg1)
            event.otherLeptonsTagLooseOS.remove(event.higgsCandLooseOS.leg2.leg2)
            event.otherLeptonsTagLooseOS =filter(lambda x: x.sip3D()<4.,event.otherLeptonsTagLooseOS)
            event.otherLeptonsTagLooseOS =sorted(event.otherLeptonsTagLooseOS,key=lambda x: x.pt(),reverse=True)


            if len(event.otherLeptonsTagLooseOS)>0:
                if event.otherLeptonsTagLooseOS[0].pt()>10:
                    event.higgsCandLooseOS.leptonTagLooseOS = event.otherLeptonsTagLooseOS[0]
                    
            self.correctFakeWeightsComb(event.higgsCandLooseOS)






        if hasattr(event,'higgsCand') or hasattr(event,'higgsCandLoose') or hasattr(event,'higgsCandLooseOS'):
            return True
        
        return  False
    

