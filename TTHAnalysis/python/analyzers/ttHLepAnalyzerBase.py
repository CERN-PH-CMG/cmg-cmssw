import operator 
import itertools
import copy
import types

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
from CMGTools.TTHAnalysis.tools.EfficiencyCorrector import EfficiencyCorrector

from CMGTools.RootTools.utils.DeltaR import deltaR, deltaPhi, bestMatch
from CMGTools.RootTools.physicsobjects.RochesterCorrections import rochcor

from ROOT import CMGMuonCleanerBySegmentsAlgo, ElectronEnergyCalibrator, TRandom3
cmgMuonCleanerBySegments = CMGMuonCleanerBySegmentsAlgo()

from CMGTools.TTHAnalysis.signedSip import *
 
class ttHLepAnalyzerBase( Analyzer ):

    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepAnalyzerBase,self).__init__(cfg_ana,cfg_comp,looperName)

        if self.cfg_ana.doElectronScaleCorrections:
            tag = "Summer12_DR53X_HCP2012" if cfg_comp.isMC else "Moriond2013";
            self.electronEnergyCalibrator = ElectronEnergyCalibrator(
                                                tag,   ## dataset  
                                                True, # isAOD
                                                cfg_comp.isMC, # isMC
                                                True, # updateEnergyError,
                                                999,  # applyCorrections (999 = correct and/or smear for SC-based energy estimation)
                                                0.607, # smearing ratio
                                                False, False, #verbose, sync
                                                TRandom3(0),) # random number generator
                                                

        if hasattr(cfg_comp,'efficiency'):
            self.efficiency= EfficiencyCorrector(cfg_comp.efficiency)

        self.relaxId = cfg_ana.relaxId if hasattr(cfg_ana,'relaxId') else  False
    #----------------------------------------
    # DECLARATION OF HANDLES OF LEPTONS STUFF   
    #----------------------------------------
        

    def declareHandles(self):
 
        super(ttHLepAnalyzerBase, self).declareHandles()

        #leptons
        self.handles['muons'] = AutoHandle(self.cfg_ana.muons,"std::vector<cmg::Muon>")            
        self.handles['electrons'] = AutoHandle(self.cfg_ana.electrons,"std::vector<cmg::Electron>")            
    
        
        #rho for muons
        self.handles['rhoMu'] = AutoHandle( (self.cfg_ana.rhoMuon, 'rho'),
                                          'double')
        #rho for electrons
        self.handles['rhoEle'] = AutoHandle( (self.cfg_ana.rhoElectron, 'rho'),
                                          'double')

        #photons (a la hzz4l definition)
        self.handles['photons'] = AutoHandle( ('cmgPhotonSel',''),'std::vector<cmg::Photon>')


    def beginLoop(self):
        super(ttHLepAnalyzerBase,self).beginLoop()


    #------------------
    # MAKE LEPTON LISTS
    #------------------

    
    # the muons are already corrected with Rochester corrections, are already cleaned with the ghost cleaning
    # the electrons have already the electron energy regression and calibration applied
    # the V5_10_0 cmgTuple, have been corrected with Mike's patch for the SIP computation -> cmgMuons have been remade ->
    # (cvs up -r michalis_sipPatchBranch  CMGTools/Common/src/MuonFactory.cc) 
    # nb: the event vertex needs to be defined first -> using the vertex analyzer
    # nb: in the following dxy and dz are computed with respect to the PV good vertex, sip with respect to the PV

    def makeLeptons(self, event):
        
        event.looseLeptons = []
        event.selectedLeptons = []

        #muons
        allmuons = map( Muon, self.handles['muons'].product() )

        if self.cfg_ana.doRecomputeSIP3D:
            for mu in allmuons:
                if mu.sourcePtr().innerTrack().isNonnull():
                    ## compute the variable and set it
                    mu._sip3d = abs(signedSip3D(mu, event.goodVertices[0]))
                    ## attach it to the object redefining the sip3D() method
                    mu.sip3D  = types.MethodType(lambda self : self._sip3d, mu, mu.__class__)

        if self.cfg_ana.doRochesterCorrections:
            for mu in allmuons:
                corp4 = rochcor.corrected_p4(mu, event.run) 
                mu.setP4( corp4 )

        if self.cfg_ana.doSegmentBasedMuonCleaning:
            isgood = cmgMuonCleanerBySegments.clean( self.handles['muons'].product() )
            newmu = []
            for i,mu in enumerate(allmuons):
                if isgood[i]: newmu.append(mu)
            allmuons = newmu

        for mu in allmuons:
            mu.associatedVertex = event.goodVertices[0]
            if (mu.isGlobal() or mu.isTracker() and mu.numberOfMatches()>0) and mu.pt()>5 and abs(mu.eta())<2.4 and abs(mu.dxy())<0.5 and abs(mu.dz())<1.:
                if mu.sourcePtr().userFloat("isPFMuon")>0.5 and (self.relaxId or mu.sip3D()<10) and mu.relIso(dBetaFactor=0.5)<self.cfg_ana.isolationCut:
                    event.selectedLeptons.append(mu)
                else:
                    event.looseLeptons.append(mu)

        #electrons        
        allelectrons = map( Electron, self.handles['electrons'].product() )

        if self.cfg_ana.doRecomputeSIP3D:
            for ele in allelectrons:
                if ele.sourcePtr().gsfTrack().isNonnull():
                    ## compute the variable and set it
                    ele._sip3d = abs(signedSip3D(ele, event.goodVertices[0]))
                    ## attach it to the object redefining the sip3D() method
                    ele.sip3D  = types.MethodType(lambda self : self._sip3d, ele, ele.__class__)

        for ele in allelectrons:
          ele.rho = float(self.handles['rhoEle'].product()[0])
          SCEta = abs(ele.sourcePtr().superCluster().eta())
          if (abs(SCEta) >= 0.0 and abs(SCEta) < 1.0 ) : ele.EffectiveArea = 0.130;
          if (abs(SCEta) >= 1.0 and abs(SCEta) < 1.479 ) : ele.EffectiveArea = 0.137;
          if (abs(SCEta) >= 1.479 and abs(SCEta) < 2.0 ) : ele.EffectiveArea = 0.067;
          if (abs(SCEta) >= 2.0 and abs(SCEta) < 2.2 ) : ele.EffectiveArea = 0.089;
          if (abs(SCEta) >= 2.2 and abs(SCEta) < 2.3 ) : ele.EffectiveArea = 0.107;
          if (abs(SCEta) >= 2.3 and abs(SCEta) < 2.4 ) : ele.EffectiveArea = 0.110;
          if (abs(SCEta) >= 2.4) : ele.EffectiveArea = 0.138;

        if self.cfg_ana.doElectronScaleCorrections:
            for ele in allelectrons:
                calibratedPatEle = ele.sourcePtr().get()
                self.electronEnergyCalibrator.correctLite(calibratedPatEle, calibratedPatEle.r9(), event.run)
                ele.setP4(calibratedPatEle.p4(calibratedPatEle.P4_COMBINATION))

        muForEleCrossCleaning = []
        if self.cfg_ana.doEleMuCrossCleaning:
            for mu in event.selectedLeptons + event.looseLeptons:
                if abs(mu.pdgId()) == 13 and (mu.isGlobal() or mu.sourcePtr().userFloat("isPFMuon")>0.5):
                    muForEleCrossCleaning.append(mu)


        for ele in allelectrons:
            ele.associatedVertex = event.goodVertices[0]
            ## remove muons if muForEleCrossCleaning is not empty
            if bestMatch(ele, muForEleCrossCleaning)[1] < 0.02: continue
            ## apply selection
            if ele.pt()>7 and abs(ele.eta())<2.5 and abs(ele.dxy())<0.5 and abs(ele.dz())<1. and ele.numberOfHits()<=1:
                 if (self.relaxId or ele.mvaIDZZ() and ele.sip3D()<10) and ele.relIso(dBetaFactor=0.5)<self.cfg_ana.isolationCut:
                    event.selectedLeptons.append(ele)
                 else:
                    event.looseLeptons.append(ele)
                    
        event.looseLeptons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedLeptons.sort(key = lambda l : l.pt(), reverse = True)

        for lepton in event.selectedLeptons:
            if hasattr(self,'efficiency'):
                self.efficiency.attachToObject(lepton)

        #print "Found ",len(event.looseLeptons)," loose leptons"
        #print "Found ",len(event.selectedLeptons)," good leptons"


    def makePhotons(self, event):

        event.allphotons = map( Photon, self.handles['photons'].product() )
        event.allphotons.sort(key = lambda l : l.pt(), reverse = True)
       

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        eventNumber = iEvent.eventAuxiliary().id().event()
        #print 'Event ',eventNumber
        
        #import pdb; pdb.set_trace()

        #call the leptons/photons functions
        self.makeLeptons(event)
        self.makePhotons(event)
        

        ret = False
        if len(event.selectedLeptons) >= self.cfg_ana.minGoodLeptons:
            ret = True
        if hasattr(self.cfg_ana, 'maxGoodLeptons') and len(event.selectedLeptons) > self.cfg_ana.maxGoodLeptons:
            ret = False
        #if self.cfg_ana.doSSLeptons and len(event.selectedLeptons) >= 2:
        #    if event.selectedLeptons[0].charge() == event.selectedLeptons[1].charge():
        #        ret = True

        return ret
