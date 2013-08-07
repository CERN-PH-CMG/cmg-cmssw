
import operator 
import itertools
import copy
from math import fabs
from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Jet import Jet


from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
from CMGTools.HToZZTo4Leptons.analyzers.DiObjectPair import DiObjectPair
from CMGTools.HToZZTo4Leptons.tools.FSRRecovery import FSRRecovery
from CMGTools.HToZZTo4Leptons.tools.FakeRateCalculator import FakeRateCalculator
from CMGTools.HToZZTo4Leptons.tools.EfficiencyCorrector import EfficiencyCorrector
from CMGTools.HToZZTo4Leptons.tools.KDCalculator import KDCalculator
from CMGTools.HToZZTo4Leptons.tools.massErrors import MassErrors

from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,isNewerThan



        
class MultiLeptonAnalyzerBase( Analyzer ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(MultiLeptonAnalyzerBase,self).__init__(cfg_ana,cfg_comp,looperName)

        self.fakeRates=[]

        #check if there is fake rate 
        if hasattr(cfg_comp,'fakeRates'):
            for fr in cfg_comp.fakeRates:
                self.fakeRates.append(FakeRateCalculator(fr))

        #check if there is efficiency        
        if hasattr(cfg_comp,'efficiency'):
            self.efficiency= EfficiencyCorrector(cfg_comp.efficiency)

        #initialize MELA    
        self.kdCalc = KDCalculator()
        #initialize mass errors
        self.massRes = MassErrors(cfg_comp.isData)

        if  hasattr(self.cfg_ana,"FSR"):
            self.FSR=FSRRecovery(self.cfg_ana.FSR)


        self.is44X = cmsswIs44X()
    

    def declareHandles(self):
        ''' Here declare handles of all objects we possibly need
        '''
        
        super(MultiLeptonAnalyzerBase, self).declareHandles()
        #rho for muons
        self.handles['rhoMu'] = AutoHandle( (self.cfg_ana.rhoMuon, 'rho'),
                                          'double')
        #rhjo for electrons
        self.handles['rhoEle'] = AutoHandle( (self.cfg_ana.rhoElectron, 'rho'),
                                          'double')
        #met
        self.handles['met'] = AutoHandle( ('cmgPFMET',''),'std::vector<cmg::BaseMET>')
        #fsr photons
        self.handles['photons'] = AutoHandle( ('cmgPhotonSel',''),'std::vector<cmg::Photon>')
        #electrons? DO I NEED THIS
        self.handles['electrons'] = AutoHandle( ('cmgElectronSel',''),'std::vector<cmg::Electron>')
        #jets
        self.handles['jets'] = AutoHandle( ('cmgPFJetSel',''),'std::vector<cmg::PFJet>')

        #mc information
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

        self.handles['BR'] = AutoHandle( ('genInfo','BR'),'int')
        self.handles['processID'] = AutoHandle( ('genInfo','processID'),'int')


    def beginLoop(self):
        super(MultiLeptonAnalyzerBase,self).beginLoop()

        
    def buildPhotonList(self, event):
        '''Build the list of FSR photons and attach it to the event'''
        event.photons = map( Photon,self.handles['photons'].product() )

    def buildJetList(self, event):
        '''Build the list of jets and attach it to the event
           Also apply basic jet requirements  
        '''
        event.jets = map( Jet,self.handles['jets'].product() )

        #prune jets
        event.prunedJets = filter(lambda x:x.pt()>10 and abs(x.eta())<4.7,event.jets)


        event.selectedJets = filter(self.jetId,event.prunedJets) 
        event.shiftedJetsUp  = self.makeShiftedJets(event.selectedJets,1.0)
        event.shiftedJetsDwn = self.makeShiftedJets(event.selectedJets,-1.0)
        

    def jetId(self,jet):
        pfwp ='full53x'

        if self.is44X:
            pfwp='full'
            
        PFID =   ( abs(jet.eta()) <= 2.4                                             and 
                  (jet.component(1).fraction()                                > 0    and
                   jet.component(2).fraction()                                < 0.99 and 
                   jet.component(4).fraction()                                < 0.99 and 
                   jet.component(5).fraction()                                < 0.99 and 
                   jet.component(1).number()                                  > 0    and
                   jet.nConstituents()                                        > 1   )or
                  abs(jet.eta()) > 2.4                                               and
                              (jet.component(4).fraction()                                < 0.99 and 
                               jet.component(5).fraction()                                < 0.99 and 
                               jet.nConstituents()                                        > 1    ) 
                  ) 

        wp= [{ 'ptMin':0, 'ptMax':10,'etaMin':0,'etaMax':2.5,'mva':-0.95},
             { 'ptMin':0, 'ptMax':10,'etaMin':2.5,'etaMax':2.75,'mva':-0.96},
             { 'ptMin':0, 'ptMax':10,'etaMin':2.75,'etaMax':3.0,'mva':-0.94},
             { 'ptMin':0, 'ptMax':10,'etaMin':3.0,'etaMax':5.0,'mva':-0.95},
             { 'ptMin':10, 'ptMax':20,'etaMin':0,'etaMax':2.5,'mva':-0.95},
             { 'ptMin':10, 'ptMax':20,'etaMin':2.5,'etaMax':2.75,'mva':-0.96},
             { 'ptMin':10, 'ptMax':20,'etaMin':2.75,'etaMax':3.0,'mva':-0.94},
             { 'ptMin':10, 'ptMax':20,'etaMin':3.0,'etaMax':5.0,'mva':-0.95},
             { 'ptMin':20, 'ptMax':30,'etaMin':0,'etaMax':2.5,'mva':-0.63},
             { 'ptMin':20, 'ptMax':30,'etaMin':2.5,'etaMax':2.75,'mva':-0.60},
             { 'ptMin':20, 'ptMax':30,'etaMin':2.75,'etaMax':3.0,'mva':-0.55},
             { 'ptMin':20, 'ptMax':30,'etaMin':3.0,'etaMax':5.0,'mva':-0.45},
             { 'ptMin':30, 'ptMax':9999999,'etaMin':0,'etaMax':2.5,'mva':-0.63},
             { 'ptMin':30, 'ptMax':9999999,'etaMin':2.5,'etaMax':2.75,'mva':-0.60},
             { 'ptMin':30, 'ptMax':9999999,'etaMin':2.75,'etaMax':3.0,'mva':-0.55},
             { 'ptMin':30, 'ptMax':9999999,'etaMin':3.0,'etaMax':5.0,'mva':-0.45}]

        PUID=False
        for element in wp:
            if jet.pt()>=element['ptMin'] and jet.pt()<element['ptMax'] and \
               fabs(jet.eta())>=element['etaMin'] and fabs(jet.eta())<element['etaMax'] and \
               jet.puMva(pfwp)>=element['mva']:
                PUID=True

        return PFID and PUID        
#        return PFID


    def makeShiftedJets(self,jets,sigma):
        shiftedJets=[]
        for jet in jets:

            copiedJet = copy.deepcopy(jet)
            copiedJet.scaleEnergy((1+sigma*jet.uncOnFourVectorScale()))
            shiftedJets.append(copiedJet)
            
        return shiftedJets
    



    def buildLeptonList(self, event):
        '''Build the list of the Leptons
           The event vertex needs to be defined first
        ''' 
        
        
        event.leptons1 = map( self.__class__.LeptonClass1,
                              self.handles['leptons1'].product() )



        for lepton in event.leptons1:
            lepton.associatedVertex = event.vertex

            if hasattr(self,'efficiency'):
                self.efficiency.attachToObject(lepton)


        
        if self.__class__.LeptonClass1 != self.__class__.LeptonClass2: 
            event.leptons2 = map( self.__class__.LeptonClass2,
                                  self.handles['leptons2'].product() )

            for lepton in event.leptons2:
                lepton.associatedVertex = event.vertex
                if hasattr(self,'efficiency'):
                    self.efficiency.attachToObject(lepton)
        else:
            event.leptons2 = event.leptons1




    def doMC(self, iEvent, event):
        event.genParticles=self.mchandles['genParticles'].product()
        event.higgsesGen=filter(lambda x:abs(x.pdgId())==25,event.genParticles)
        if len(event.higgsesGen)>0:
            event.higgsGen =event.higgsesGen[0]

        event.genDecay   = self.handles['BR'].product()[0]        
        event.genProcess = self.handles['processID'].product()[0]        
            

       
    def process(self, iEvent, event):
        self.event = iEvent.eventAuxiliary().id().event()
        event.vertex = event.goodVertices[0]
        self.readCollections( iEvent )

        #basic event quantities
        event.met = self.handles['met'].product()[0]
        event.rhoMu = self.handles['rhoMu'].product()[0]
        event.rhoEle = self.handles['rhoEle'].product()[0]
        self.rhoMu = event.rhoMu
        self.rhoEle = event.rhoEle
        event.step=0
        if self.cfg_comp.isMC:
            self.doMC(iEvent,event)

        #get leptons
        self.buildLeptonList( event )

        #Get photons
        self.buildPhotonList( event )

        #get jets
        self.buildJetList(event)



        return True
    


    def correctFakeWeights(self, event):
        '''correct the fake rates attached to the LooseZ2 leptons,
           using the channel-dependent fake rates.
           needs knowldege of the Z1 decay mode.'''
        if not hasattr(event,'bestZ'):  return
        if not hasattr(event.bestZ, 'leg1'):  return
        z1Flav = event.bestZ.leg1.pdgId()
        if not hasattr(event,'leptonsForLooseZ2'): return
        for lepton in event.leptonsForLooseZ2:
            if hasattr(self,'fakeRates'):
                for fr in self.fakeRates:
                    fr.attachToObject(lepton, z1Flav)

    def correctFakeWeightsComb(self,fourLepton ):
        '''correct the fake rates attached to the LooseZ2 leptons,
           using the channel-dependent fake rates.
           needs knowldege of the Z1 decay mode.'''
        z1Flav = fourLepton.leg1.leg1.pdgId()
        leptons=[fourLepton.leg2.leg1,fourLepton.leg2.leg2]
#        if hasattr(fourLepton,'leptonTag'):
#            leptons.append(fourLepton.leptonTag)
        if hasattr(self,'fakeRates'):
            for lepton in leptons:
                for fr in self.fakeRates:
                    fr.attachToObject(lepton, z1Flav)
                    

    #####################################################################
    #Combinatorial METHODS
    #####################################################################

    def mergePairs(self,pair1,pair2):
        quadObject =DiObjectPair(pair1.leg1, pair1.leg2,pair2.leg1,pair2.leg2)
        if hasattr(pair1,'fsrPhoton'):
            quadObject.leg1.setFSR(pair1.fsrPhoton)
        if hasattr(pair2,'fsrPhoton'):
            quadObject.leg2.setFSR(pair2.fsrPhoton)
        quadObject.updateP4()
        return quadObject    
            
        


    def findPairs(self, leptons):
        '''Make combinatorics and make dilepton pairs
           Include FSR if in cfg file
        '''
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            z = DiObject(l1, l2)
            if hasattr(self,'FSR'):
                self.FSR.recoverZ(z)
            out.append(z)
        return out




    def findQuads(self, leptons,photons):
        '''Make combinatorics and make permulations of four leptons
           Cut the permutation sin half
           Include FSR if in cfg file
        '''
        out = []
        for l1, l2,l3,l4 in itertools.permutations(leptons, 4):
            if l1.pt()>l2.pt() and l3.pt()>l4.pt():
                quadObject =DiObjectPair(l1, l2,l3,l4)
                if abs(quadObject.leg1.M()-91.118)<abs(quadObject.leg2.M()-91.118):
                    if  hasattr(self,"FSR"):
                        self.FSR.setBoson(quadObject)
                        self.FSR.attachPhotons(photons)
                        self.FSR.recoverZ(quadObject.leg1)
                        self.FSR.recoverZ(quadObject.leg2)
                        quadObject.updateP4()
                    out.append(quadObject)
        return out


    ##############################################################
    ######Lepton identification
    ###############################################################

    def testLepton(self, lepton, pt, eta,sel=None):
        if sel is not None and  not lepton.getSelection(sel):
          return False
        if lepton.pt() > pt and abs(lepton.eta()) < eta: 
            return True
        else:
            return False


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


    def testLeptonLoose(self, lepton,sel=None):
        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1.

    def testLeptonGood(self, lepton,sel=None):
        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1. and abs(lepton.sip3D())<4.


    def testLeptonTight(self, lepton,sel=None):
        return abs(lepton.dxy())<0.5 and abs(lepton.dz())<1. and abs(lepton.sip3D())<4.

    def testMuonLoose(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        looseID = (muon.isGlobal() or (muon.isTracker() and muon.numberOfMatches()>0))
        return looseID

    def testMuonPF(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        pfID = muon.sourcePtr().userFloat("isPFMuon")>0.5
        return pfID

    def testMuonCleaning(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.
        This is for cross cleaning muons from electrons
        '''
        looseID = (muon.isGlobal() or self.testMuonPF(muon))
        return looseID

    def testMuonTight(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''

#        iso  = muon.absEffAreaIso(self.rhoMu,self.cfg_ana.effectiveAreas)/muon.pt()<0.4 #warning:you need to set the self.rho !!!
        iso  = muon.absIso(0.5,0.0)/muon.pt()<0.4 #warning:you need to set the self.rho !!!
        return self.testMuonLoose(muon) and self.testMuonPF(muon) and iso

    def testMuonGood(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''

        return self.testMuonLoose(muon) and self.testMuonPF(muon) 


    def testElectronLoose(self, electron):
        looseID = electron.numberOfHits()<=1
        return looseID

    def testElectronGood(self,lepton):
        id = lepton.mvaIDZZ()
        return id
    
    def testElectronTight(self, electron):
        '''Returns True if a electron passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        id = electron.mvaIDZZ()
        iso  = electron.absEffAreaIso(self.rhoEle,self.cfg_ana.effectiveAreas)/electron.pt()<0.4 #warning:you need to set the self.rho !!!
        return iso and id 



    ###################################################
    ############## Di Lepton Selectors
    ###################################################

    def testZSF(self, zCand):
        return  (abs(zCand.leg1.pdgId()) == abs(zCand.leg2.pdgId()) )

    def testZOS(self, zCand):
        return  (zCand.charge() == 0) 

    def testZSkim(self, zCand):
        '''Applies  OS SF and M requirements on a Z'''
        return self.testZSF(zCand) and \
               self.testZOS(zCand)


    def testZ1PtThresholds(self, zCand):
        return zCand.leg1.pt() > self.cfg_ana.z1_pt1 and \
               zCand.leg2.pt() > self.cfg_ana.z1_pt2 

    def testZ1Mass(self, zCand):
        return zCand.M() > self.cfg_ana.z1_m[0] and \
               zCand.M() < self.cfg_ana.z1_m[1] 

    def testZ2Mass(self, zCand):
        return zCand.M() > self.cfg_ana.z2_m[0] and \
               zCand.M() < self.cfg_ana.z2_m[1] 


    def testZLooseID(self,zCand):
        return self.testLeptonLoose(zCand.leg1) and \
               self.testLeptonLoose(zCand.leg2)

    def testZTightID(self,zCand):
        '''Tricky part. if there is FSR check if it within 0.4
           If yes attach a photon to the lepton
           After the isolation cut delete the photon from the leptons
           to be able to do it later again
        '''
        if hasattr(zCand,'fsrPhoton'):
            zCand.leg1.fsrPhotons=[]
            zCand.leg2.fsrPhotons=[]
            if zCand.fsrDR1()<self.cfg_ana.FSR.leptonIsoCone:
                zCand.leg1.fsrPhotons.append(zCand.fsrPhoton)
            if zCand.fsrDR2()<self.cfg_ana.FSR.leptonIsoCone:
                zCand.leg2.fsrPhotons.append(zCand.fsrPhoton)

        decision = self.testLeptonTight(zCand.leg1) and \
               self.testLeptonTight(zCand.leg2)        

        if hasattr(zCand.leg1,'fsrPhotons'):
            del zCand.leg1.fsrPhotons
        if hasattr(zCand.leg2,'fsrPhotons'):
            del zCand.leg2.fsrPhotons

        return decision


    def bestZBosonByMass(self, zBosons):
        if len( zBosons ) == 0:
            return None
        elif len( zBosons ) == 1:
            return zBosons[0]
        else:
            # taking the closest from the Z mass
            zMass = 91.1876
            return min(zBosons, key = lambda x: abs( x.mass() - zMass) )

    def bestZBosonBySumPt(self, zBosons):
        if len( zBosons ) == 0:
            return None
        elif len( zBosons ) == 1:
            return zBosons[0]
        else:
            return max(zBosons, key = lambda x: x.leg1.pt()+x.leg2.pt())





    #######################################################################################################
    #############################Fourlepton selectors
    #######################################################################################################
    def testFourLeptonLooseID(self, fourLepton):
        return self.testZLooseID(fourLepton.leg1) and self.testZLooseID(fourLepton.leg2)

    def testFourLeptonTightID(self, fourLepton):
        return self.testZTightID(fourLepton.leg1) and self.testZTightID(fourLepton.leg2)

    def testFourLeptonTightIDZ1(self, fourLepton):
        return self.testZTightID(fourLepton.leg1) 

    def testFourLeptonTightIDZ2(self, fourLepton):
        return self.testZTightID(fourLepton.leg2) 

    def testFourLeptonTightIDFSRBoth(self, fourLepton):
        photons=[]
        fsrExists=False

        if hasattr(fourLepton.leg1,'fsrPhoton'):
            fsrExists=True
            photons.append(fourLepton.leg1.fsrPhoton)
        if hasattr(fourLepton.leg2,'fsrPhoton'):
            fsrExists=True
            photons.append(fourLepton.leg2.fsrPhoton)

        if fsrExists:    
            fourLepton.leg1.leg1.fsrPhotons=[]
            fourLepton.leg1.leg2.fsrPhotons=[]
            fourLepton.leg2.leg1.fsrPhotons=[]
            fourLepton.leg2.leg2.fsrPhotons=[]
            for gamma in photons:
                if deltaR(fourLepton.leg1.leg1.eta(),fourLepton.leg1.leg1.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg1.leg1.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg1.leg2.eta(),fourLepton.leg1.leg2.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg1.leg2.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg2.leg1.eta(),fourLepton.leg2.leg1.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg2.leg1.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg2.leg2.eta(),fourLepton.leg2.leg2.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg2.leg2.fsrPhotons.append(gamma)


        decision=self.testLeptonTight(fourLepton.leg1.leg1) and \
               self.testLeptonTight(fourLepton.leg1.leg2) and \
               self.testLeptonTight(fourLepton.leg2.leg1) and \
               self.testLeptonTight(fourLepton.leg2.leg2)        

        if hasattr(fourLepton.leg1.leg1,'fsrPhotons'):
            del fourLepton.leg1.leg1.fsrPhotons
        if hasattr(fourLepton.leg1.leg2,'fsrPhotons'):
            del fourLepton.leg1.leg2.fsrPhotons
        if hasattr(fourLepton.leg2.leg1,'fsrPhotons'):
            del fourLepton.leg2.leg1.fsrPhotons
        if hasattr(fourLepton.leg2.leg2,'fsrPhotons'):
            del fourLepton.leg2.leg2.fsrPhotons

        return decision
    


    def testFourLeptonTightIDZ1FSRBoth(self, fourLepton):
        photons=[]
        fsrExists=False

        if hasattr(fourLepton.leg1,'fsrPhoton'):
            fsrExists=True
            photons.append(fourLepton.leg1.fsrPhoton)
        if hasattr(fourLepton.leg2,'fsrPhoton'):
            fsrExists=True
            photons.append(fourLepton.leg2.fsrPhoton)

        if fsrExists:    
            fourLepton.leg1.leg1.fsrPhotons=[]
            fourLepton.leg1.leg2.fsrPhotons=[]
            fourLepton.leg2.leg1.fsrPhotons=[]
            fourLepton.leg2.leg2.fsrPhotons=[]
            for gamma in photons:
                if deltaR(fourLepton.leg1.leg1.eta(),fourLepton.leg1.leg1.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg1.leg1.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg1.leg2.eta(),fourLepton.leg1.leg2.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg1.leg2.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg2.leg1.eta(),fourLepton.leg2.leg1.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg2.leg1.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg2.leg2.eta(),fourLepton.leg2.leg2.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg2.leg2.fsrPhotons.append(gamma)

        decision = self.testLeptonTight(fourLepton.leg1.leg1) and \
               self.testLeptonTight(fourLepton.leg1.leg2)


        if hasattr(fourLepton.leg1.leg1,'fsrPhotons'):
            del fourLepton.leg1.leg1.fsrPhotons
        if hasattr(fourLepton.leg1.leg2,'fsrPhotons'):
            del fourLepton.leg1.leg2.fsrPhotons
        if hasattr(fourLepton.leg2.leg1,'fsrPhotons'):
            del fourLepton.leg2.leg1.fsrPhotons
        if hasattr(fourLepton.leg2.leg2,'fsrPhotons'):
            del fourLepton.leg2.leg2.fsrPhotons


        return decision


    def testFourLeptonTightIDZ2FSRBoth(self, fourLepton):
        photons=[]
        fsrExists=False

        if hasattr(fourLepton.leg1,'fsrPhoton'):
            fsrExists=True
            photons.append(fourLepton.leg1.fsrPhoton)
        if hasattr(fourLepton.leg2,'fsrPhoton'):
            fsrExists=True
            photons.append(fourLepton.leg2.fsrPhoton)

        if fsrExists:    
            fourLepton.leg1.leg1.fsrPhotons=[]
            fourLepton.leg1.leg2.fsrPhotons=[]
            fourLepton.leg2.leg1.fsrPhotons=[]
            fourLepton.leg2.leg2.fsrPhotons=[]
            for gamma in photons:
                if deltaR(fourLepton.leg1.leg1.eta(),fourLepton.leg1.leg1.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg1.leg1.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg1.leg2.eta(),fourLepton.leg1.leg2.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg1.leg2.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg2.leg1.eta(),fourLepton.leg2.leg1.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg2.leg1.fsrPhotons.append(gamma)
                if deltaR(fourLepton.leg2.leg2.eta(),fourLepton.leg2.leg2.phi(),gamma.eta(),gamma.phi())<self.cfg_ana.FSR.leptonIsoCone:
                    fourLepton.leg2.leg2.fsrPhotons.append(gamma)



        decision = self.testLeptonTight(fourLepton.leg2.leg1) and \
               self.testLeptonTight(fourLepton.leg2.leg2)
        

        if hasattr(fourLepton.leg1.leg1,'fsrPhotons'):
            del fourLepton.leg1.leg1.fsrPhotons
        if hasattr(fourLepton.leg1.leg2,'fsrPhotons'):
            del fourLepton.leg1.leg2.fsrPhotons
        if hasattr(fourLepton.leg2.leg1,'fsrPhotons'):
            del fourLepton.leg2.leg1.fsrPhotons
        if hasattr(fourLepton.leg2.leg2,'fsrPhotons'):
            del fourLepton.leg2.leg2.fsrPhotons

        return decision




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

    def testDiLeptonGhostSuppression(self, diLepton):
        leptons = [diLepton.leg1, \
                   diLepton.leg2]

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
    
    
    def getFourLeptonZ2SumPt(self, hBoson):
        sum = hBoson.leg2.leg1.pt()+hBoson.leg2.leg2.pt()
        return sum    



    def sortFourLeptons(self, fourLeptons):
        '''Sorts four lepton candidates by mass
        If there are more than one take the two highest
        Pt ones'''
        #sort the first one by Z mass and if there
        #are more than one take the two highest pt ones
        if len(fourLeptons)>1 :
            sorted1=sorted(fourLeptons,key=self.getFourLeptonZ2SumPt,reverse=True)
            sorted2=sorted(sorted1,key=lambda x: abs(x.leg1.mass()-91.118))
            return sorted2
        else:
            return fourLeptons


    def sortFourLeptonsByKD(self, fourLeptons):
        '''Sorts four lepton candidates by KD
        If there are more than one take the two highest
        Pt ones'''
        if len(fourLeptons)>1 :
            s=sorted(fourLeptons,key=lambda x: x.KD,reverse=True)
            return s
        else:
            return fourLeptons


    def sortFourLeptonsByPt(self, fourLeptons):
        '''Sorts four lepton candidates by mass
        If there are more than one take the two highest
        Pt ones'''
        def comp(x):
            if x.leg2.mass()>12.:
                return x.leg2.mass()
            else:
                return 999999999
            
        #sort the first one by Z mass and if there
        #are more than one take the two highest pt ones
        if len(fourLeptons)>1 :
            s=sorted(fourLeptons,key=lambda x: x.KD,reverse=True)
            return s
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


        



    def boostFourLeptons(self,objColl,event):

        for obj in objColl:
            self.kdCalc.calculate(obj)
            self.massRes.calculate(obj)
            self.calculateJetObservables(obj,event.selectedJets,'jets')
            self.calculateJetObservables(obj,event.shiftedJetsUp,'jetsUp')
            self.calculateJetObservables(obj,event.shiftedJetsDwn,'jetsDwn')





    ##################################################################
    ##High level calculations
    ##################################################################    
        
    def calculateJetObservables(self,object,jets,prefix = 'jets',cleanDR = 0.5):
        #Apply ID
        leptons = object.daughterLeptons()+object.daughterPhotons()

        cleanedJets=[]

        for jet in jets:
            if jet.pt()>30: ###Apply the real cut here to do proper systs on jet counting
#            if (jet.pt()>30 and abs(jet.eta())<4.7) or (jet.pt()>20 and abs(jet.eta())<2.1): ###Apply the real cut here to do proper systs on jet counting
                overlap=False
                for lepton in leptons:
                    if deltaR(jet.eta(),jet.phi(),lepton.eta(),lepton.phi())<cleanDR:
                        overlap=True
                        break
                if not overlap:
                    cleanedJets.append(jet)
                

        jetVars=dict()

        
        jetVars['nJets']=len(cleanedJets)

        #B tagging
        bJets = filter(lambda x: x.bDiscriminator('combinedSecondaryVertexBJetTags')>0.224,cleanedJets)
        jetVars['nBJets']=len(bJets)
        jetVars['nBJetsTight']=len(filter(lambda x: x.bDiscriminator('combinedSecondaryVertexBJetTags')>0.679,cleanedJets))

        #HT
        ht=0
        for jet in cleanedJets:
            ht = ht + jet.pt()
        jetVars['HT'] = ht



        #M bb
        if len(bJets) >=2:
            jetVars['Mbb'] = (bJets[0].p4()+bJets[1].p4()).M()
        else:
            jetVars['Mbb']=-99

        #Mjjj    
        if len(cleanedJets)>2:
            jetVars['Mjjj'] = (cleanedJets[0].p4()+cleanedJets[1].p4()+cleanedJets[2].p4()).M()
        else:    
            jetVars['Mjjj'] = -99.

        #VBF     
        if len(cleanedJets)>1:
            jetVars['dEta'] = cleanedJets[0].eta()-cleanedJets[1].eta()
            jetVars['dPhi'] = deltaPhi(cleanedJets[0].phi(),cleanedJets[1].phi())
            jetVars['Mjj'] = (cleanedJets[0].p4()+cleanedJets[1].p4()).M()
            jetVars['Ptjj'] = (cleanedJets[0].p4()+cleanedJets[1].p4()).Pt()
            jetVars['subleadingPt']=cleanedJets[1].pt()
            jetVars['subleadingEta']=cleanedJets[1].eta()
            jetVars['Fisher']=0.18*fabs(jetVars['dEta'])  +1.92e-4*jetVars['Mjj']
        
        else:
            jetVars['dEta'] = -99
            jetVars['dPhi'] = -99
            jetVars['Mjj'] = -99
            jetVars['Ptjj'] = -99
            jetVars['subleadingPt']=-99.
            jetVars['subleadingEta']=-99.
            jetVars['Fisher']=-99.

        #highest Jet    
        if  len(cleanedJets)>0:
            #Add leading jet variables    
            highestJet = max(cleanedJets,key=lambda x:x.pt())
            jetVars['leadingPt']=highestJet.pt()
            jetVars['leadingEta']=highestJet.eta()
        else:
            jetVars['leadingPt']=-99.
            jetVars['leadingEta']=-99.



        setattr(object,prefix,jetVars)    



       

                                                                                                           



                                                                                                       
