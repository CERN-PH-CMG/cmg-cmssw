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
from CMGTools.HToZZTo4Leptons.tools.IsoDepositCreator import IsoDepositCreator

from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Photon import Photon

from CMGTools.RootTools.utils.DeltaR import deltaR


class FourLeptonAnalyzerFSRStudy( FourLeptonAnalyzerBase ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def declareHandles(self):
        super(FourLeptonAnalyzerFSRStudy, self).declareHandles()




    def beginLoop(self):
        super(FourLeptonAnalyzerFSRStudy,self).beginLoop()


       
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
        self.rho = self.handles['rho'].product()[0]

        #Build lepton lists and apply skim
        self.buildLeptonList( event )

        #Get photons
        self.buildPhotonList( event )


        EVN=iEvent.eventAuxiliary().id().event()
        
        #generator
        event.genPhotons = self.getGeneratedFSR(0.5)
        if len(event.genPhotons)>0:
            #find the highest
            event.bestGenPhoton=max(event.genPhotons,key=lambda x: x.pt())

                
        for gamma in event.photons:

#            if EVN==9318 or EVN==9390:
#                print "EVENT=%d\n"  % EVN
#                print gamma.pt()
#                print gamma.eta()
#                print gamma.phi()
            
            match=0
            matched=None
            if len(event.genPhotons)>0:
                dr0=100.
                for mcgamma in event.genPhotons:
                    dr=deltaR(mcgamma.eta(),mcgamma.phi(),gamma.eta(),gamma.phi())
                    if dr < dr0 and dr<0.1:
                        dr0=dr
                        match=1
                        matched=mcgamma
            gamma.match=match
            if match:
                gamma.matchedE = matched.energy()
            else:
                gamma.matchedE=-99
            #add the type
            if abs(gamma.pdgId())==22:
                gamma.type=0
            elif abs(gamma.pdgId())==11:
                gamma.type=1
            elif gamma.charge() != 0:
                gamma.type=2

        if hasattr(event,'bestGenPhoton '):   
            for gamma in event.photons:
                dr=deltaR(gamma.eta(),gamma.phi(),event.bestGenPhoton.eta(),event.bestGenPhoton.phi())
                if dr<0.08:
                    event.matchedPhoton=gamma
                    break


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
            event.leptonsForFakeRate = copy.copy(event.cleanLeptons)
            event.leptonsForFakeRate.remove( event.bestZForFakeRate.leg1)
            event.leptonsForFakeRate.remove( event.bestZForFakeRate.leg2)

            #sort them by highest Pt
            event.leptonsForFakeRate.sort(key=lambda x: x.pt(),reverse=True)
            
        #OK in the post analysis process we will use those leptons to measure the fake rate
        #Nothing else to be done for that

        #Now create four Lepton candidtes upstream. Use all permutations and not combinations
        #of leptons so we can pick the best Z1 and Z2
        event.fourLeptons = self.findQuadsWithFSR(event.cleanLeptons,event.photons)
        #Sort them by M1 near Z and My highest Pt sum
        self.sortFourLeptons(event.fourLeptons)
        cutFlow.setSource1(event.fourLeptons)

        if len(event.fourLeptons)>0:
            event.higgsCandLoose = event.fourLeptons[0]

        
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
    
    def getGeneratedFSR(self,ptmin):
       trueFSR=[]

       for ptc in self.mchandles['genParticles'].product():
           if ptc.status() !=1: continue
           if abs(ptc.pdgId()) != 22 or ptc.pt() <ptmin  or abs(ptc.eta())>2.5: continue
           if not(ptc.numberOfMothers()) : continue
           moth = ptc.mother(0)
           if not ((abs(moth.pdgId()) == 13) or (abs(moth.pdgId()) == 11) or (abs(moth.pdgId()) == 15)): continue  
           trueFSR.append(ptc)

       return trueFSR    

#    def buildPhotonList(self,minpt):
#
#       vertices = self.handles['vertices'].product()
#       xv=0.
#       yv=0.
#       zv=0.
#       
#       if len(vertices)>0:
#           xv=vertices[0].position().x()
#           xy=vertices[0].position().y()
#           xz=vertices[0].position().z()
#
#
#       photonList=[]
#       muonList=[]
#       hadronList=[]
#       neutralHadronList=[]
#       
#       for ptf in self.handles['pf'].product():
#           if (abs(ptf.pdgId())==22 )  and ptf.pt()>minpt:
#               photonList.append(Photon(ptf))
#           elif abs(ptf.pdgId())==13:    
#               muonList.append(ptf)
#           elif abs(ptf.pdgId())==211 and ptf.pt()>0.3:    
#               hadronList.append(ptf)
#           elif abs(ptf.pdgId()) in [111,130,310,2112]:
#               neutralHadronList.append(ptf)
#
#
#       #if a photron is near an electron kill it
#       
#
#       #find the correct muons
#       for muon in muonList:
#
#           if muon.rawEcalEnergy()<2.0 :
#               continue
#
#           #create the photon
#           x=muon.positionAtECALEntrance().x()
#           y=muon.positionAtECALEntrance().y()
#           z=muon.positionAtECALEntrance().z()
#
#           x=(x-xv)/sqrt((x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv))
#           y=(y-yv)/sqrt((x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv))
#           z=(z-zv)/sqrt((x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv))
#
#           vector=Math.XYZTVector(x*muon.rawEcalEnergy(), \
#                                 y*muon.rawEcalEnergy(), \
#                                 z*muon.rawEcalEnergy(),
#                                  muon.rawEcalEnergy())
#
#           vector2=Math.XYZTVector(x*muon.ecalEnergy(), \
#                                 y*muon.ecalEnergy(), \
#                                 z*muon.ecalEnergy(),
#                                 muon.ecalEnergy()  )
#
#           hasOverlap=False
#           #check if there is already a photon
#           for photon in photonList:
#               if deltaR(vector.eta(),vector.phi(),photon.eta(),photon.phi())<0.01 and \
#                  abs(vector.pt()-photon.pt())/photon.pt()<0.05:
#                   hasOverlap=True
#                   break
#
#               if deltaR(vector2.eta(),vector2.phi(),photon.eta(),photon.phi())<0.01 and \
#                  abs(vector2.pt()-photon.pt())/photon.pt()<0.05:
#                   hasOverlap=True
#                   break
#           if not hasOverlap:
#               gamma = Photon(muon)
#               gamma.setP4(vector)
#               photonList.append(gamma)
#               
#       isoCalculator=IsoDepositCreator(vertices,hadronList)
#       #for match
#       fsr = self.getGeneratedFSR(1.0)
#       for photon in photonList:
#           deposits=isoCalculator.getIsolation(photon)
#           photon.isolation=deposits
#           #now add neutral and photons
#           neutralIso=0.0
#           for otherPhoton in photonList:
#               if deltaR(photon.eta(),photon.phi(),otherPhoton.eta(),otherPhoton.phi())<0.4 and \
#                  deltaR(photon.eta(),photon.phi(),otherPhoton.eta(),otherPhoton.phi())>0.01:
#                   neutralIso=neutralIso+otherPhoton.pt()
#
#           for neutral in neutralHadronList:
#               if deltaR(photon.eta(),photon.phi(),neutral.eta(),neutral.phi())<0.4:
#                   neutralIso=neutralIso+neutral.pt()
#           photon.neutralIso = neutralIso        
#           match=False
#           for f in fsr:
#               if deltaR(f.eta(),f.phi(),photon.eta(),photon.phi())<0.08:
#                   match=True
#                   break
#           photon.match=match
#           
#       return photonList
           
