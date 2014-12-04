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
from CMGTools.RootTools.physicsobjects.MuScleFitCorrector   import MuScleFitCorr
from CMGTools.RootTools.physicsobjects.ElectronCalibrator import EmbeddedElectronCalibrator
from CMGTools.TTHAnalysis.electronCalibrator import * # load also older one

from ROOT import CMGMuonCleanerBySegmentsAlgo, TRandom3
cmgMuonCleanerBySegments = CMGMuonCleanerBySegmentsAlgo()

from CMGTools.TTHAnalysis.signedSip import *
 
class ttHLepAnalyzerBase( Analyzer ):

    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepAnalyzerBase,self).__init__(cfg_ana,cfg_comp,looperName)
        if self.cfg_ana.doMuScleFitCorrections and self.cfg_ana.doMuScleFitCorrections != "none":
            if self.cfg_ana.doMuScleFitCorrections not in [ "none", "prompt", "prompt-sync", "rereco", "rereco-sync" ]:
                raise RuntimeError, 'doMuScleFitCorrections must be one of "none", "prompt", "prompt-sync", "rereco", "rereco-sync"'
            rereco = ("prompt" not in self.cfg_ana.doMuScleFitCorrections)
            sync   = ("sync"       in self.cfg_ana.doMuScleFitCorrections)
            self.muscleCorr = MuScleFitCorr(cfg_comp.isMC, rereco, sync)
            if hasattr(self.cfg_ana, "doRochesterCorrections") and self.cfg_ana.doRochesterCorrections:
                raise RuntimeError, "You can't run both Rochester and MuScleFit corrections!"
        else:
            self.cfg_ana.doMuScleFitCorrections = False
        if self.cfg_ana.doElectronScaleCorrections == "embedded":
            self.electronEnergyCalibrator = EmbeddedElectronCalibrator()
        else:
            self.electronEnergyCalibrator = ElectronCalibrator(cfg_comp.isMC)
        if hasattr(cfg_comp,'efficiency'):
            self.efficiency= EfficiencyCorrector(cfg_comp.efficiency)

        self.relaxId = cfg_ana.relaxId if hasattr(cfg_ana,'relaxId') else  False
    #----------------------------------------
    # DECLARATION OF HANDLES OF LEPTONS STUFF   
    #----------------------------------------
        

    def declareHandles(self):
 
        super(ttHLepAnalyzerBase, self).declareHandles()

        #leptons
        self.handles['muons'] = AutoHandle(self.cfg_ana.muons,"std::vector<pat::Muon>")            
        self.handles['electrons'] = AutoHandle(self.cfg_ana.electrons,"std::vector<pat::Electron>")            
    
        
        #rho for muons
        self.handles['rhoMu'] = AutoHandle( (self.cfg_ana.rhoMuon, ''),
                                          'double')
        #rho for electrons
        self.handles['rhoEle'] = AutoHandle( (self.cfg_ana.rhoElectron, ''),
                                          'double')

        #photons (a la hzz4l definition)
        self.handles['photons'] = AutoHandle( self.cfg_ana.photons, 'std::vector<pat::Photon>')

        ## dEdX
        #self.handles['dEdX'] = AutoHandle( ('dedxHarmonic2','','RECO'), 'edm::ValueMap<reco::DeDxData>' )
    def beginLoop(self):
        super(ttHLepAnalyzerBase,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('vetoed events')
        count.register('accepted events')


    #------------------
    # MAKE LEPTON LISTS
    #------------------

    
    def makeLeptons(self, event):
        
        event.looseLeptons = []
        event.selectedLeptons = []
        event.inclusiveLeptons = []

        #muons
        allmuons = map( Muon, self.handles['muons'].product() )

        if self.cfg_ana.doRecomputeSIP3D:
            for mu in allmuons:
                if mu.innerTrack().isNonnull():
                    ## compute the variable and set it
                    mu._sip3d = abs(signedSip3D(mu, event.goodVertices[0]))
                    ## attach it to the object redefining the sip3D() method
                    mu.sip3D  = types.MethodType(lambda self : self._sip3d, mu, mu.__class__)

        if self.cfg_ana.doMuScleFitCorrections:
            for mu in allmuons:
                self.muscleCorr.correct(mu, event.run)
        elif self.cfg_ana.doRochesterCorrections:
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
            if (mu.isGlobalMuon() or mu.isTrackerMuon() and mu.numberOfMatches()>0) and mu.pt()>5 and abs(mu.eta())<2.4 and abs(mu.dxy())<0.5 and abs(mu.dz())<1.:
                #pid = mu.originalObject().track().id()
                #key = mu.originalObject().track().key()
                #mu.dEdX = self.handles['dEdX'].product().get(pid,key)
                if mu.isPFMuon() and mu.sip3D() < self.cfg_ana.sip3dCut and mu.relIso(dBetaFactor=0.5)<self.cfg_ana.isolationCut:
                    event.selectedLeptons.append(mu)
                else:
                    event.looseLeptons.append(mu)
                if mu.isPFMuon() and mu.sip3D() < self.cfg_ana.sip3dCutVeryLoose:
                    event.inclusiveLeptons.append(mu)

        #electrons        
        allelectrons = map( Electron, self.handles['electrons'].product() )

        ## duplicate removal for fast sim (to be checked if still necessary in 5_3_12+)
        allelenodup = []
        for e in allelectrons:
            dup = False
            for e2 in allelenodup:
                if abs(e.pt()-e2.pt()) < 1e-6 and abs(e.eta()-e2.eta()) < 1e-6 and abs(e.phi()-e2.phi()) < 1e-6 and e.charge() == e2.charge():
                    dup = True
                    break
            if not dup: allelenodup.append(e)
        allelectrons = allelenodup

        if self.cfg_ana.doRecomputeSIP3D:
            for ele in allelectrons:
                if ele.pt() < 5: continue # we don't even have any info for these
                if ele.gsfTrack().isNonnull():
                    ## compute the variable and set it
                    ele._sip3d = abs(signedSip3D(ele, event.goodVertices[0]))
                    ## attach it to the object redefining the sip3D() method
                    ele.sip3D  = types.MethodType(lambda self : self._sip3d, ele, ele.__class__)

        # fill EA for rho-corrected isolation
        for ele in allelectrons:
          ele.rho = float(self.handles['rhoEle'].product()[0])
          if ele.pt() < 5: continue # we don't even have any info for these
          SCEta = abs(ele.superCluster().eta())
          if (abs(SCEta) >= 0.0 and abs(SCEta) < 1.0 ) : ele.EffectiveArea = 0.130;
          if (abs(SCEta) >= 1.0 and abs(SCEta) < 1.479 ) : ele.EffectiveArea = 0.137;
          if (abs(SCEta) >= 1.479 and abs(SCEta) < 2.0 ) : ele.EffectiveArea = 0.067;
          if (abs(SCEta) >= 2.0 and abs(SCEta) < 2.2 ) : ele.EffectiveArea = 0.089;
          if (abs(SCEta) >= 2.2 and abs(SCEta) < 2.3 ) : ele.EffectiveArea = 0.107;
          if (abs(SCEta) >= 2.3 and abs(SCEta) < 2.4 ) : ele.EffectiveArea = 0.110;
          if (abs(SCEta) >= 2.4) : ele.EffectiveArea = 0.138;

        if self.cfg_ana.doElectronScaleCorrections:
            for ele in allelectrons:
                self.electronEnergyCalibrator.correct(ele, event.run)

        muForEleCrossCleaning = []
        if self.cfg_ana.doEleMuCrossCleaning:
            for mu in event.selectedLeptons + event.looseLeptons:
                if abs(mu.pdgId()) == 13 and (mu.isGlobalMuon() or mu.isPFMuon()):
                    muForEleCrossCleaning.append(mu)


        for ele in allelectrons:
            ele.associatedVertex = event.goodVertices[0]
            ## remove muons if muForEleCrossCleaning is not empty
            if bestMatch(ele, muForEleCrossCleaning)[1] < 0.02: continue
            ## apply selection
            if ele.pt()>7 and abs(ele.eta())<2.5 and abs(ele.dxy())<0.5 and abs(ele.dz())<1. and ele.gsfTrack().trackerExpectedHitsInner().numberOfLostHits()<=1:
                 ## fill tightId:
                 if (ele.pt() > 20):
                    SCEta = abs(ele.superCluster().eta())
                    if   SCEta < 0.8:   ele.tightIdResult = (ele.mvaTrigV0() > 0.00)
                    elif SCEta < 1.479: ele.tightIdResult = (ele.mvaTrigV0() > 0.10)
                    else:               ele.tightIdResult = (ele.mvaTrigV0() > 0.62)
                 elif (ele.pt() > 10):
                    if   SCEta < 0.8:   ele.tightIdResult = (ele.mvaTrigV0() > 0.94)
                    elif SCEta < 1.479: ele.tightIdResult = (ele.mvaTrigV0() > 0.85)
                    else:               ele.tightIdResult = (ele.mvaTrigV0() > 0.92)
                 else:
                    ele.tightIdResult = False
                 if (self.relaxId or ele.mvaIDZZ() and ele.sip3D() < self.cfg_ana.sip3dCut) and ele.relIso(dBetaFactor=0.5)<self.cfg_ana.isolationCut:
                    event.selectedLeptons.append(ele)
                 else:
                    event.looseLeptons.append(ele)
                 if (self.relaxId or ele.mvaIDZZ()) and ele.sip3D() < self.cfg_ana.sip3dCutVeryLoose:
                    event.inclusiveLeptons.append(ele)
                 
                    
        event.looseLeptons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedLeptons.sort(key = lambda l : l.pt(), reverse = True)
        event.inclusiveLeptons.sort(key = lambda l : l.pt(), reverse = True)

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
        self.counters.counter('events').inc('all events')

        eventNumber = iEvent.eventAuxiliary().id().event()
        #print 'Event ',eventNumber
        
        #import pdb; pdb.set_trace()

        #call the leptons/photons functions
        self.makeLeptons(event)
        self.makePhotons(event)
        

        ret = False
        if len(event.selectedLeptons) >= self.cfg_ana.minGoodLeptons:
            ret = True
        if hasattr(self.cfg_ana, 'minInclusiveLeptons') and len(event.inclusiveLeptons) < self.cfg_ana.minInclusiveLeptons:
            if ret: self.counters.counter('events').inc('vetoed events')
            ret = False
        if hasattr(self.cfg_ana, 'maxGoodLeptons') and len(event.selectedLeptons) > self.cfg_ana.maxGoodLeptons:
            if ret: self.counters.counter('events').inc('vetoed events')
            ret = False
        #if self.cfg_ana.doSSLeptons and len(event.selectedLeptons) >= 2:
        #    if event.selectedLeptons[0].charge() == event.selectedLeptons[1].charge():
        #        ret = True

        if ret: self.counters.counter('events').inc('accepted events')
        return ret
