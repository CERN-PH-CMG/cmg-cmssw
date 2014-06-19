import types

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.TTHAnalysis.tools.EfficiencyCorrector import EfficiencyCorrector

from CMGTools.RootTools.utils.DeltaR import bestMatch
from CMGTools.RootTools.physicsobjects.RochesterCorrections import rochcor
from CMGTools.RootTools.physicsobjects.MuScleFitCorrector   import MuScleFitCorr
from CMGTools.RootTools.physicsobjects.ElectronCalibrator import EmbeddedElectronCalibrator
from CMGTools.TTHAnalysis.electronCalibrator import * # load also older one

from ROOT import CMGMuonCleanerBySegmentsAlgo
cmgMuonCleanerBySegments = CMGMuonCleanerBySegmentsAlgo()

from CMGTools.TTHAnalysis.signedSip import *
 
class ttHLepAnalyzerFR( Analyzer ):

    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepAnalyzerFR,self).__init__(cfg_ana,cfg_comp,looperName)
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
 
        super(ttHLepAnalyzerFR, self).declareHandles()

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

        ## dEdX
        #self.handles['dEdX'] = AutoHandle( ('dedxHarmonic2','','RECO'), 'edm::ValueMap<reco::DeDxData>' )
    def beginLoop(self):
        super(ttHLepAnalyzerFR,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('vetoed events')
        count.register('accepted events')


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
        event.inclusiveLeptons = []


        

        #muons
        allmuons = map( Muon, self.handles['muons'].product() )
        
        if self.cfg_ana.doRecomputeSIP3D:
            for mu in allmuons:
                if mu.sourcePtr().innerTrack().isNonnull():
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
            mu.relIso03= (mu.sourcePtr().pfIsolationR03().sumChargedHadronPt + max( mu.sourcePtr().pfIsolationR03().sumNeutralHadronEt +  mu.sourcePtr().pfIsolationR03().sumPhotonEt -  mu.sourcePtr().pfIsolationR03().sumPUPt/2,0.0))/mu.pt()
            
            mu.looseFakeId = ((mu.isGlobal() or mu.isTracker() and mu.numberOfMatches()>0) and mu.sourcePtr().userFloat("isPFMuon")>0.5 and mu.pt()>10 and abs(mu.eta())<2.4 and mu.tightId()>0.5 and abs(mu.dxy())<0.2 and abs(mu.dz())<0.2 and mu.relIso03<1.0)
            mu.tightFakeId = ((mu.isGlobal() or mu.isTracker() and mu.numberOfMatches()>0) and mu.sourcePtr().userFloat("isPFMuon")>0.5 and mu.pt()>10 and abs(mu.eta())<2.4 and mu.tightId()>0.5 and abs(mu.dxy())<0.01 and abs(mu.dz())<0.2 and mu.relIso03<0.1)

            if mu.pt()>5 and abs(mu.eta())<2.4:
                if mu.looseFakeId:
                    event.inclusiveLeptons.append(mu)
                    event.selectedLeptons.append(mu)
                    

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
                if ele.sourcePtr().gsfTrack().isNonnull():
                    ## compute the variable and set it
                    ele._sip3d = abs(signedSip3D(ele, event.goodVertices[0]))
                    ## attach it to the object redefining the sip3D() method
                    ele.sip3D  = types.MethodType(lambda self : self._sip3d, ele, ele.__class__)

        # fill EA for rho-corrected isolation
        for ele in allelectrons:
          ele.rho = float(self.handles['rhoEle'].product()[0])
          SCEta = abs(ele.sourcePtr().superCluster().eta())
          if (abs(SCEta) >= 0.0   and abs(SCEta) < 1.0   ) : ele.EffectiveArea = 0.13 # 0.130;
          if (abs(SCEta) >= 1.0   and abs(SCEta) < 1.479 ) : ele.EffectiveArea = 0.14 # 0.137;
          if (abs(SCEta) >= 1.479 and abs(SCEta) < 2.0   ) : ele.EffectiveArea = 0.07 # 0.067;
          if (abs(SCEta) >= 2.0   and abs(SCEta) < 2.2   ) : ele.EffectiveArea = 0.09 # 0.089;
          if (abs(SCEta) >= 2.2   and abs(SCEta) < 2.3   ) : ele.EffectiveArea = 0.11 # 0.107;
          if (abs(SCEta) >= 2.3   and abs(SCEta) < 2.4   ) : ele.EffectiveArea = 0.11 # 0.110;
          if (abs(SCEta) >= 2.4)                           : ele.EffectiveArea = 0.14 # 0.138;

        if self.cfg_ana.doElectronScaleCorrections:
            for ele in allelectrons:
                self.electronEnergyCalibrator.correct(ele, event.run)

        muForEleCrossCleaning = []
        if self.cfg_ana.doEleMuCrossCleaning:
            for mu in event.selectedLeptons + event.looseLeptons:
                if abs(mu.pdgId()) == 13 and (mu.isGlobal() or mu.sourcePtr().userFloat("isPFMuon")>0.5):
                    muForEleCrossCleaning.append(mu)


        for ele in allelectrons:
            ele.associatedVertex = event.goodVertices[0]
            ## remove muons if muForEleCrossCleaning is not empty
            if bestMatch(ele, muForEleCrossCleaning)[1] < 0.02: continue
            ## fill POG MVA tight Id
            ele.tightIdResult = False
            if ele.pt()>7 and abs(ele.eta())<2.5 and abs(ele.dxy())<0.5 and abs(ele.dz())<1. and ele.numberOfHits()<=1:
                 ## fill tightId:
                 if (ele.pt() > 20):
                    SCEta = abs(ele.sourcePtr().superCluster().eta())
                    if   SCEta < 0.8:   ele.tightIdResult = (ele.mvaTrigV0() > 0.00)
                    elif SCEta < 1.479: ele.tightIdResult = (ele.mvaTrigV0() > 0.10)
                    else:               ele.tightIdResult = (ele.mvaTrigV0() > 0.62)
                 elif (ele.pt() > 10):
                    if   SCEta < 0.8:   ele.tightIdResult = (ele.mvaTrigV0() > 0.94)
                    elif SCEta < 1.479: ele.tightIdResult = (ele.mvaTrigV0() > 0.85)
                    else:               ele.tightIdResult = (ele.mvaTrigV0() > 0.92)
            # Compute isolation
            ele.relIso03 = (ele.chargedHadronIso(0.3) + max(ele.neutralHadronIso(0.3)+ele.photonIso(0.3)-ele.rho*ele.EffectiveArea,0))/ele.pt()
            # Compute electron id
            dEtaIn = abs(ele.sourcePtr().deltaEtaSuperClusterTrackAtVtx());
            dPhiIn = abs(ele.sourcePtr().deltaPhiSuperClusterTrackAtVtx());
            sigmaIEtaIEta = abs(ele.sourcePtr().sigmaIetaIeta());
            hoe = abs(ele.sourcePtr().hadronicOverEm());
            ooemoop = abs(1.0/ele.sourcePtr().ecalEnergy() - ele.sourcePtr().eSuperClusterOverP()/ele.sourcePtr().ecalEnergy());
            vtxFitConversion = ele.passConversionVeto();
            mHits = ele.numberOfHits();
            # Set tight and loose flags
            if abs(ele.sourcePtr().superCluster().eta()) < 1.479:
                ele.looseFakeId = dEtaIn < 0.004 and dPhiIn < 0.06 and sigmaIEtaIEta < 0.01 and hoe < 0.12 and ooemoop < 0.05
            else:
                ele.looseFakeId = dEtaIn < 0.007 and dPhiIn < 0.03 and sigmaIEtaIEta < 0.03 and hoe < 0.10 and ooemoop < 0.05
            ele.looseFakeId = ele.looseFakeId and vtxFitConversion and mHits <= 1 and abs(ele.dz()) < 0.1 and ele.relIso03 < 0.6
            ele.looseFakeId = ele.looseFakeId and mHits <= 1 and ele.sourcePtr().isGsfCtfScPixChargeConsistent()
            ele.tightFakeId = ele.looseFakeId and abs(ele.dxy()) < 0.02 and ele.relIso03 < 0.15
            #print "ele pt = %.3f  eta = %.3f sceta = %.3f detaIn = %.4f dphiIn = %.4f  sieie = %.4f  h/e = %.4f  1/e-1/p = %.4f  dxy = %.4f  dz = %.4f  losthits = %d conveto = %d relIso = %.4f     loose id = %d   tight id = %d" % ( ele.pt(),ele.eta(),ele.sourcePtr().superCluster().eta(),dEtaIn,dPhiIn,sigmaIEtaIEta,hoe,ooemoop,abs(ele.dxy()),abs(ele.dz()),mHits,vtxFitConversion,ele.relIso03, ele.looseFakeId, ele.tightFakeId)
            # add to the list if needed
            if ele.pt()>7 and abs(ele.eta())<2.5:
                if ele.looseFakeId:
                    event.inclusiveLeptons.append(ele)
                    event.selectedLeptons.append(ele)
                 
                    
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
