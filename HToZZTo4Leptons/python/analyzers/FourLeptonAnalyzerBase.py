import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton

from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
from CMGTools.HToZZTo4Leptons.analyzers.DiObjectPair import DiObjectPair






        
class FourLeptonAnalyzerBase( Analyzer ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton


    def declareHandles(self):
        super(FourLeptonAnalyzerBase, self).declareHandles()

        self.handles['rho'] = AutoHandle( ('kt6PFJetsForIso', 'rho'),
                                          'double')
        self.handles['met'] = AutoHandle( ('cmgPFMET',''),'std::vector<cmg::BaseMET>')


    def beginLoop(self):
        super(FourLeptonAnalyzerBase,self).beginLoop()
        self.counters.addCounter('FourLepton')
        count = self.counters.counter('FourLepton')
        count.register('all events')
        

    def buildLeptonList(self, event):
        event.leptons1 = map( self.__class__.LeptonClass1,
                              self.handles['leptons1'].product() )
        if self.__class__.LeptonClass1 != self.__class__.LeptonClass2: 
            event.leptons2 = map( self.__class__.LeptonClass2,
                                  self.handles['leptons2'].product() )
        else:
            event.leptons2 = event.leptons1

       
    def process(self, iEvent, event):
        return True
    
    def findPairs(self, leptons):
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            out.append( DiObject(l1, l2) )
        return out


    #this function is different for the ee/mumu
    def findQuads(self, leptons):
        out = []
        for l1, l2,l3,l4 in itertools.permutations(leptons, 4):
            if l1.pt()>l2.pt() and l3.pt()>l4.pt():
                out.append( DiObjectPair(l1, l2,l3,l4))
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
        return True

    def testLeptonLoose2(self, lepton,sel=None):
        return  True

    def testLeptonTight1(self, lepton,sel=None):
        return abs(lepton.sip3D())<4.

    def testLeptonTight2(self, lepton,sel=None):
        return abs(lepton.sip3D())<4.


#    Skimming of leptons 1-2 corresponds to the flavour here 
    def testLeptonSkim1(self, lepton,sel=None):
        if hasattr(self.cfg_ana,"minPt1") and hasattr(self.cfg_ana,"maxEta1"):
            return self.testLepton(lepton,self.cfg_ana.minPt1,self.cfg_ana.maxEta1,sel)  and \
                   abs(lepton.sip3D())<100.
        else:
            return abs(lepton.sip3D())<100.

    def testLeptonSkim2(self, lepton,sel=None):
        if hasattr(self.cfg_ana,"minPt2") and hasattr(self.cfg_ana,"maxEta2"):
            return self.testLepton(lepton,self.cfg_ana.minPt2,self.cfg_ana.maxEta2,sel)  and \
                   abs(lepton.sip3D())<100.
        else:
            return abs(lepton.sip3D())<100.
#####################################################




    #loose muon  requirements  
    def testMuonLoose(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        looseID = (muon.isGlobal() or muon.isTracker())
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

        iso  = muon.absEffAreaIso(self.rho,self.cfg_ana.effectiveAreas)/muon.pt()<0.25 #warning:you need to set the self.rho !!!
        return self.testMuonLoose(muon) and self.testMuonPF(muon) and iso

    def testElectronTight(self, electron):
        '''Returns True if a electron passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        mvaRegions = [{'ptMin':0,'ptMax':10, 'etaMin':0.0, 'etaMax':0.8,'mva':0.47},\
                      {'ptMin':0,'ptMax':10, 'etaMin':0.8 ,'etaMax':1.479,'mva':0.004},\
                      {'ptMin':0,'ptMax':10, 'etaMin':1.479, 'etaMax':2.5,'mva':0.295},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':0.0, 'etaMax':0.8,'mva':0.5},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':0.8, 'etaMax':1.479,'mva':0.12},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':1.479, 'etaMax':2.5,'mva':0.6}]

        ID=False 


        for element in mvaRegions:
            if electron.pt()>= element['ptMin'] and \
               electron.pt()< element['ptMax'] and \
               abs(electron.eta())>=element['etaMin'] and \
               abs(electron.eta())<element['etaMax'] and \
               electron.mvaNonTrigV0()> element['mva']: 
                ID=True


        iso  = electron.absEffAreaIso(self.rho,self.cfg_ana.effectiveAreas)/electron.pt()<0.25 #warning:you need to set the self.rho !!!

        conv = (electron.numberOfHits()<=1)
        
        return iso and ID and conv


        


    def testZSkim(self, zCand):
        '''Applies  OS SF and M requirements on a Z'''
        return zCand.M() > self.cfg_ana.z1_m[0] and \
               self.testZSF(zCand) and \
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
        return self.testLeptonTight1(zCand.leg1) and \
               self.testLeptonTight1(zCand.leg2)

    def testZ2LooseID(self,zCand):
        return self.testLeptonLoose2(zCand.leg1) and \
               self.testLeptonLoose2(zCand.leg2)

    def testZ2TightID(self,zCand):
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
        leading_pt = fourLepton.sortedPtLeg(0) 
        subleading_pt = fourLepton.sortedPtLeg(1) 
        return leading_pt>self.cfg_ana.z1_pt1  and subleading_pt>self.cfg_ana.z1_pt2

    def testFourLeptonZ1(self, fourLepton):
        return self.testZ1Mass(fourLepton.leg1) and self.testZSF(fourLepton.leg1) and \
               self.testZOS(fourLepton.leg1)


    def testFourLeptonMassZ1Z2(self, fourLepton):
        return self.testZ1Mass(fourLepton.leg1) and self.testZ2Mass(fourLepton.leg2)

    def testFourLeptonMassZ1(self, fourLepton):
        return self.testZ1Mass(fourLepton.leg1)

        
    def testFourLeptonMinMass(self, fourLepton):
        return fourLepton.minPairMass()>self.cfg_ana.minMass

    def testFourLeptonMassZ(self, fourLepton):
        return fourLepton.mass()>=70.

    def testFourLeptonMass(self, fourLepton):
        return fourLepton.mass()>=self.cfg_ana.minHMass


        
    def bestZBosonByMass(self, zBosons):
        if len( zBosons ) == 0:
            return None
        elif len( zBosons ) == 1:
            return zBosons[0]
        else:
            # taking the closest from the Z mass
            zMass = 91.2
            return min(zBosons, key = lambda x: abs( x.mass() - zMass) )

    def bestZBosonBySumPt(self, zBosons):
        if len( zBosons ) == 0:
            return None
        elif len( zBosons ) == 1:
            return zBosons[0]
        else:
            return max(zBosons, key = lambda x: x.leg1.pt()+x.leg2.pt())



    def sortFourLeptons(self, fourLeptons):
        #sort the first one by Z mass and if there
        #are more than one take the two highest pt ones
        if len(fourLeptons)>1 :
            fourLeptons=sorted(fourLeptons,key=lambda x: x.leg2.leg1.pt()+x.leg2.leg2.pt(),reverse=True)
            fourLeptons=sorted(fourLeptons,key=lambda x: abs(x.leg1.mass()-91.2))
            
        

