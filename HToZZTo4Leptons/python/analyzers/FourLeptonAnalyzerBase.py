import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton,Photon,Electron,Jet

from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
from CMGTools.HToZZTo4Leptons.analyzers.DiObjectPair import DiObjectPair
from CMGTools.HToZZTo4Leptons.tools.FSRRecovery import FSRRecovery
from CMGTools.HToZZTo4Leptons.tools.FakeRateCalculator import FakeRateCalculator
from CMGTools.HToZZTo4Leptons.tools.EfficiencyCorrector import EfficiencyCorrector
from CMGTools.HToZZTo4Leptons.tools.mela import MELACalculator

from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi



        
class FourLeptonAnalyzerBase( Analyzer ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(FourLeptonAnalyzerBase,self).__init__(cfg_ana,cfg_comp,looperName)

        self.fakeRates=[]
        if hasattr(cfg_comp,'fakeRates'):
            for fr in cfg_comp.fakeRates:
                self.fakeRates.append(FakeRateCalculator(fr))

        if hasattr(cfg_comp,'efficiency'):
            self.efficiency= EfficiencyCorrector(cfg_comp.efficiency)

        self.mela = MELACalculator()

    def declareHandles(self):
        super(FourLeptonAnalyzerBase, self).declareHandles()

        self.handles['rhoMu'] = AutoHandle( (self.cfg_ana.rhoMuon, 'rho'),
                                          'double')

        self.handles['rhoEle'] = AutoHandle( (self.cfg_ana.rhoElectron, 'rho'),
                                          'double')

        self.handles['met'] = AutoHandle( ('cmgPFMET',''),'std::vector<cmg::BaseMET>')

        self.handles['photons'] = AutoHandle( ('cmgPhotonSel',''),'std::vector<cmg::Photon>')
        self.handles['electrons'] = AutoHandle( ('cmgElectronSel',''),'std::vector<cmg::Electron>')

        self.handles['jets'] = AutoHandle( ('cmgPFJetSel',''),'std::vector<cmg::PFJet>')



    def beginLoop(self):
        super(FourLeptonAnalyzerBase,self).beginLoop()
        self.counters.addCounter('FourLepton')
        count = self.counters.counter('FourLepton')
        count.register('all events')


        
    def buildPhotonList(self, event):

        event.photons = map( Photon,self.handles['photons'].product() )


    def buildJetList(self, event):
        event.jets = map( Jet,self.handles['jets'].product() )


    def buildLeptonList(self, event):
        #We need the vertices first!
        
        
        event.leptons1 = map( self.__class__.LeptonClass1,
                              self.handles['leptons1'].product() )



        for lepton in event.leptons1:
            lepton.associatedVertex = event.vertex
            if hasattr(self,'fakeRates'):
                for fr in self.fakeRates:
                    fr.attachToObject(lepton)

            if hasattr(self,'efficiency'):
                self.efficiency.attachToObject(lepton)


        
        if self.__class__.LeptonClass1 != self.__class__.LeptonClass2: 
            event.leptons2 = map( self.__class__.LeptonClass2,
                                  self.handles['leptons2'].product() )

            for lepton in event.leptons2:
                lepton.associatedVertex = event.vertex
                if hasattr(self,'fakeRates'):
                    for fr in self.fakeRates:
                        fr.attachToObject(lepton)
                if hasattr(self,'efficiency'):
                    self.efficiency.attachToObject(lepton)

        else:
            event.leptons2 = event.leptons1




       
    def process(self, iEvent, event):
        self.event = iEvent.eventAuxiliary().id().event()
        return True
    
    def findPairs(self, leptons):
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            out.append( DiObject(l1, l2) )
        return out


    def findPairsWithFSR(self, leptons,photons):
        #if the pairs have already been rerun clean the fsr from them
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            z = DiObject(l1, l2)
            if not hasattr(self.cfg_ana,"FSR"):
                out.append(z)
            else:    

                fsrAlgo=FSRRecovery(self.cfg_ana.FSR)

                fsrAlgo.setPhotons(photons)

                fsrAlgo.setZ(z)

                fsrAlgo.recoverZ()

                out.append(z)
        return out




    def findQuadsWithFSR(self, leptons,photons):
        out = []
        for l1, l2,l3,l4 in itertools.permutations(leptons, 4):
            if l1.pt()>l2.pt() and l3.pt()>l4.pt():
                quadObject =DiObjectPair(l1, l2,l3,l4)
                if not hasattr(self.cfg_ana,"FSR"):
                    out.append(quadObject)
                else:    
                    fsrAlgo=FSRRecovery(self.cfg_ana.FSR)
                    fsrAlgo.setPhotons(photons)
                    fsrAlgo.setZZ(quadObject)
                    #recover FSR photons
                    fsrAlgo.recoverZZ()
                    #Now Z 2
                    quadObject.updateP4()
                    out.append(quadObject)
        return out



    #Common Lepton Selection
    def testLepton(self, lepton, pt, eta,sel=None):
        if sel is not None and  not lepton.getSelection(sel):
          return False
        if lepton.pt() > pt and abs(lepton.eta()) < eta: 
            return True
        else:
            return False

        
        
    def testLeptonLoose1(self, lepton,sel=None):
#        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1. and abs(lepton.sip3D())<4.
        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1.

    def testLeptonLoose2(self, lepton,sel=None):
#        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1. and abs(lepton.sip3D())<4.
        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1.

    def testLeptonTight1(self, lepton,sel=None):
        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1. and abs(lepton.sip3D())<4.

    def testLeptonTight2(self, lepton,sel=None):
        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1. and abs(lepton.sip3D())<4.


#    Skimming of leptons 1-2 corresponds to the flavour here not Z1/Z2
    def testLeptonSkim1(self, lepton,sel=None):
        if hasattr(self.cfg_ana,"minPt1") and hasattr(self.cfg_ana,"maxEta1"):
            return self.testLepton(lepton,self.cfg_ana.minPt1,self.cfg_ana.maxEta1,sel)  and \
                   abs(lepton.dxy())<0.5 and abs(lepton.dz())<1.
        else:
            return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1.

    def testLeptonSkim2(self, lepton,sel=None):
        if hasattr(self.cfg_ana,"minPt2") and hasattr(self.cfg_ana,"maxEta2"):
            return self.testLepton(lepton,self.cfg_ana.minPt2,self.cfg_ana.maxEta2,sel)  and \
                   abs(lepton.dxy())<0.5 and abs(lepton.dz())<1.
        else:
            return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1.

#####################################################




    #loose muon  requirements  
    def testMuonLoose(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        looseID = (muon.isGlobal() or (muon.isTracker() and muon.numberOfMatches()>0))
        return looseID

    def testMuonCleaning(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        looseID = (muon.isGlobal() or self.testMuonPF(muon))
        return looseID

    def testElectronLoose(self, electron):
        looseID = electron.numberOfHits()<=1
        return looseID


    def testMuonPF(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''

        pfID = muon.sourcePtr().userFloat("isPFMuon")>0.5
        return pfID




    #tight muonn  requirements 
    def testMuonTight(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''

        iso  = muon.absEffAreaIso(self.rhoMu,self.cfg_ana.effectiveAreas)/muon.pt()<0.4 #warning:you need to set the self.rho !!!
        return self.testMuonLoose(muon) and self.testMuonPF(muon) and iso

    def testElectronTight(self, electron):
        '''Returns True if a electron passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        id = electron.mvaIDZZ()
        iso  = electron.absEffAreaIso(self.rhoEle,self.cfg_ana.effectiveAreas)/electron.pt()<0.4 #warning:you need to set the self.rho !!!
        return iso and id 


        


    def testZSkim(self, zCand):
        '''Applies  OS SF and M requirements on a Z'''
        return self.testZSF(zCand) and \
               self.testZOS(zCand)

     
    def testZSF(self, zCand):
        return  (abs(zCand.leg1.pdgId()) == abs(zCand.leg2.pdgId()) )

    def testZOS(self, zCand):
        return  (zCand.charge() == 0) 

    def testZ1PtThresholds(self, zCand):
        return zCand.leg1.pt() > self.cfg_ana.z1_pt1 and \
               zCand.leg2.pt() > self.cfg_ana.z1_pt2 

    def testZ1Mass(self, zCand):
        return zCand.M() > self.cfg_ana.z1_m[0] and \
               zCand.M() < self.cfg_ana.z1_m[1] 

    def testZ2Mass(self, zCand):
        return zCand.M() > self.cfg_ana.z2_m[0] and \
               zCand.M() < self.cfg_ana.z2_m[1] 


    def testZ1LooseID(self,zCand):
        return self.testLeptonLoose1(zCand.leg1) and \
               self.testLeptonLoose1(zCand.leg2)

    def testZ1TightID(self,zCand):
        #Attach/Detach here the FSR photons
        if hasattr(zCand,'fsrPhoton'):
            if zCand.fsrDR1()<self.cfg_ana.FSR.leptonIsoCone:
                zCand.leg1.fsrPhoton = zCand.fsrPhoton
            if zCand.fsrDR2()<self.cfg_ana.FSR.leptonIsoCone:
                zCand.leg2.fsrPhoton = zCand.fsrPhoton
        else:
            if hasattr(zCand.leg1,'fsrPhoton'):
                del zCand.leg1.fsrPhoton
            if hasattr(zCand.leg2,'fsrPhoton'):
                del zCand.leg2.fsrPhoton

        return self.testLeptonTight1(zCand.leg1) and \
               self.testLeptonTight1(zCand.leg2)

    def testZ2LooseID(self,zCand):
        return self.testLeptonLoose2(zCand.leg1) and \
               self.testLeptonLoose2(zCand.leg2)

    def testZ2TightID(self,zCand):
        #Attach/Detach here the FSR photons
        if hasattr(zCand,'fsrPhoton'):
            if zCand.fsrDR1()<self.cfg_ana.FSR.leptonIsoCone:
                zCand.leg1.fsrPhoton = zCand.fsrPhoton
            if zCand.fsrDR2()<self.cfg_ana.FSR.leptonIsoCone:
                zCand.leg2.fsrPhoton = zCand.fsrPhoton
        else:
            if hasattr(zCand.leg1,'fsrPhoton'):
                del zCand.leg1.fsrPhoton
            if hasattr(zCand.leg2,'fsrPhoton'):
                del zCand.leg2.fsrPhoton
            


        return self.testLeptonTight2(zCand.leg1) and \
               self.testLeptonTight2(zCand.leg2)


    def testFourLeptonLooseID(self, fourLepton):
        return self.testZ1LooseID(fourLepton.leg1) and self.testZ2LooseID(fourLepton.leg2)

    def testFourLeptonTightID(self, fourLepton):
        return self.testZ1TightID(fourLepton.leg1) and self.testZ2TightID(fourLepton.leg2)

    def testFourLeptonTightIDZ1(self, fourLepton):
        return self.testZ1TightID(fourLepton.leg1) 

    def testFourLeptonTightIDZ2(self, fourLepton):
        return self.testZ2TightID(fourLepton.leg2) 



    def testFourLeptonSF(self, fourLepton):
        return self.testZSF(fourLepton.leg1) and self.testZSF(fourLepton.leg2)


    def testFourLeptonOS(self, fourLepton):
        return self.testZOS(fourLepton.leg1) and self.testZOS(fourLepton.leg2)

    def testFourLeptonPtThr(self, fourLepton):
        leading_pt = fourLepton.sortedPtLeg(0).pt() 
        subleading_pt = fourLepton.sortedPtLeg(1).pt() 
        return leading_pt>self.cfg_ana.z1_pt1  and subleading_pt>self.cfg_ana.z1_pt2

    def testFourLeptonZ1(self, fourLepton):
        return self.testZ1Mass(fourLepton.leg1) and self.testZSF(fourLepton.leg1) and \
               self.testZOS(fourLepton.leg1)


    def testFourLeptonGhostSuppression(self, fourLepton):
        leptons = [fourLepton.leg1.leg1, \
                   fourLepton.leg1.leg2, \
                   fourLepton.leg2.leg1, \
                   fourLepton.leg2.leg2]
        for l1,l2 in itertools.combinations(leptons,2):
            if deltaR(l1.eta(),l1.phi(),l2.eta(),l2.phi())<0.02:
                return False
        return True    

    def testFourLeptonMassZ2(self, fourLepton):
        return  self.testZ2Mass(fourLepton.leg2)

    def testFourLeptonMassZ1(self, fourLepton):
        return self.testZ1Mass(fourLepton.leg1)

        
    def testFourLeptonMinOSMass(self, fourLepton):
        return fourLepton.minOSPairMass()>self.cfg_ana.minMass

    def testFourLeptonMinMass(self, fourLepton):
        return fourLepton.minPairMass()>self.cfg_ana.minMass

    def testFourLeptonMassZ(self, fourLepton):
        return fourLepton.mass()>=70

    def testFourLeptonMass(self, fourLepton):
        return fourLepton.mass()>=self.cfg_ana.minHMass and fourLepton.leg2.mass()>self.cfg_ana.minHMassZ2


        
    def bestZBosonByMass(self, zBosons):
        if len( zBosons ) == 0:
            return None
        elif len( zBosons ) == 1:
            return zBosons[0]
        else:
            # taking the closest from the Z mass
            zMass = 91.118
            return min(zBosons, key = lambda x: abs( x.mass() - zMass) )

    def bestZBosonBySumPt(self, zBosons):
        if len( zBosons ) == 0:
            return None
        elif len( zBosons ) == 1:
            return zBosons[0]
        else:
            return max(zBosons, key = lambda x: x.leg1.pt()+x.leg2.pt())


    def getFourLeptonZ2SumPt(self, hBoson):
        sum = hBoson.leg2.leg1.pt()+hBoson.leg2.leg2.pt()
        return sum    



    def sortFourLeptons(self, fourLeptons):
        #sort the first one by Z mass and if there
        #are more than one take the two highest pt ones
        if len(fourLeptons)>1 :
            sorted1=sorted(fourLeptons,key=self.getFourLeptonZ2SumPt,reverse=True)
            sorted2=sorted(sorted1,key=lambda x: abs(x.leg1.mass()-91.118))
            return sorted2
        else:
            return fourLeptons

    def pruneFourLeptonsForZ1(self, fourLeptons):
        '''From all the objects in the collection pick the ones that has exactly the same
           Z1 as the best'''
        
        if len(fourLeptons)>0:
            referenceObj = fourLeptons[0]
            filteredFourLeptons = filter(lambda x: abs(x.leg1.mass()-referenceObj.leg1.mass())<0.0001,fourLeptons)
            return filteredFourLeptons
        else:
            return []
        
    def calculateJetVars(self,fourLepton,jets,cleanDR = 0.5):
        #Apply ID
        idJets = filter(lambda x:x.looseJetId() and  x.passPuJetId('full', 2),jets) 

        #Cross clean from leptons
        cleanedJets = filter(lambda x: deltaR(x.eta(),x.phi(),fourLepton.leg1.leg1.eta(),fourLepton.leg1.leg1.phi())>cleanDR and \
                             deltaR(x.eta(),x.phi(),fourLepton.leg1.leg2.eta(),fourLepton.leg1.leg2.phi())>cleanDR and \
                             deltaR(x.eta(),x.phi(),fourLepton.leg2.leg1.eta(),fourLepton.leg2.leg1.phi())>cleanDR and \
                             deltaR(x.eta(),x.phi(),fourLepton.leg2.leg2.eta(),fourLepton.leg2.leg2.phi())>cleanDR,idJets)


        bJets = filter(lambda x: x.bDiscriminator('combinedSecondaryVertexBJetTags')>0.224,cleanedJets)
        jetVars=dict()
        jetVars['nJets']=len(cleanedJets)

        jetVars['nBJets']=len(bJets)
        jetVars['nBJetsTight']=len(filter(lambda x: x.bDiscriminator('combinedSecondaryVertexBJetTags')>0.679,cleanedJets))
        ht=0
        if len(bJets) >=2:
            jetVars['Mbb'] = (bJets[0].p4()+bJets[1].p4()).M()
        else:
            jetVars['Mbb']=-99
            
        for jet in cleanedJets:
            ht = ht + jet.pt()
        jetVars['HT'] = ht
        if len(cleanedJets)>2:
            jetVars['Mjjj'] = (cleanedJets[0].p4()+cleanedJets[1].p4()+cleanedJets[2].p4()).M()
        else:    
            jetVars['Mjjj'] = -99.

        if len(cleanedJets)>1:
            jetVars['dEta'] = cleanedJets[0].eta()-cleanedJets[1].eta()
            jetVars['dPhi'] = deltaPhi(cleanedJets[0].phi(),cleanedJets[1].phi())
            jetVars['Mjj'] = (cleanedJets[0].p4()+cleanedJets[1].p4()).M()
        else:
            jetVars['dEta'] = -99
            jetVars['dPhi'] = -99
            jetVars['Mjj'] = -99

            
            
        fourLepton.jets = jetVars


       

                                                                                                           



                                                                                                       
