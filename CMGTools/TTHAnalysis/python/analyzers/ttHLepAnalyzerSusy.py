from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.TTHAnalysis.tools.EfficiencyCorrector import EfficiencyCorrector

from CMGTools.RootTools.utils.DeltaR import bestMatch
from CMGTools.RootTools.physicsobjects.RochesterCorrections import rochcor
from CMGTools.RootTools.physicsobjects.MuScleFitCorrector   import MuScleFitCorr
from CMGTools.RootTools.physicsobjects.ElectronCalibrator import EmbeddedElectronCalibrator
from CMGTools.TTHAnalysis.electronCalibrator import ElectronCalibrator

from ROOT import CMGMuonCleanerBySegmentsAlgo
cmgMuonCleanerBySegments = CMGMuonCleanerBySegmentsAlgo()

class ttHLepAnalyzerSusy( Analyzer ):

    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepAnalyzerSusy,self).__init__(cfg_ana,cfg_comp,looperName)
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
    #----------------------------------------
    # DECLARATION OF HANDLES OF LEPTONS STUFF   
    #----------------------------------------
        

    def declareHandles(self):
        super(ttHLepAnalyzerSusy, self).declareHandles()

        #leptons
        self.handles['muons'] = AutoHandle(self.cfg_ana.muons,"std::vector<cmg::Muon>")            
        self.handles['electrons'] = AutoHandle(self.cfg_ana.electrons,"std::vector<cmg::Electron>")            
    
        #rho for muons
        self.handles['rhoMu'] = AutoHandle( (self.cfg_ana.rhoMuon, 'rho'), 'double')
        #rho for electrons
        self.handles['rhoEle'] = AutoHandle( (self.cfg_ana.rhoElectron, 'rho'), 'double')

        #photons (a la hzz4l definition)
        self.handles['photons'] = AutoHandle( self.cfg_ana.photons,'std::vector<cmg::Photon>')

    def beginLoop(self):
        super(ttHLepAnalyzerSusy,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')

    #------------------
    # MAKE LEPTON LISTS
    #------------------

    
    def makeLeptons(self, event):
        ### inclusive leptons = all leptons that could be considered somewhere in the analysis, with minimal requirements (used e.g. to match to MC)
        event.inclusiveLeptons = []
        ### selected leptons = subset of inclusive leptons passing some basic id definition and pt requirement
        ### other    leptons = subset of inclusive leptons failing some basic id definition and pt requirement
        event.selectedLeptons = []
        event.selectedMuons = []
        event.selectedElectrons = []
        event.otherLeptons = []

        #muons
        allmuons = self.makeAllMuons(event)

        for mu in allmuons:
            # inclusive, very loose, selection
            if (mu.sourcePtr().track().isNonnull() and mu.muonID(self.cfg_ana.inclusive_muon_id) and 
                    mu.pt()>self.cfg_ana.inclusive_muon_pt and abs(mu.eta())<self.cfg_ana.inclusive_muon_eta and 
                    abs(mu.dxy())<self.cfg_ana.inclusive_muon_dxy and abs(mu.dz())<self.cfg_ana.inclusive_muon_dz):
                event.inclusiveLeptons.append(mu)
                # basic selection
                if (mu.muonID(self.cfg_ana.loose_muon_id) and 
                        mu.pt() > self.cfg_ana.loose_muon_pt and abs(mu.eta()) < self.cfg_ana.loose_muon_eta and 
                        abs(mu.dxy()) < self.cfg_ana.loose_muon_dxy and abs(mu.dz()) < self.cfg_ana.loose_muon_dz and
                        mu.relIso03 < self.cfg_ana.loose_muon_relIso and 
                        mu.absIso03 < (self.cfg_ana.loose_muon_absIso if hasattr(self.cfg_ana,'loose_muon_absIso') else 9e99)):
                    mu.looseIdSusy = True
                    event.selectedLeptons.append(mu)
                    event.selectedMuons.append(mu)
                else:
                    mu.looseIdSusy = False
                    event.otherLeptons.append(mu)

        #electrons        
        allelectrons = self.makeAllElectrons(event)

        looseMuons = event.selectedLeptons[:]
        for ele in allelectrons:
            ## remove muons if muForEleCrossCleaning is not empty
            ## apply selection
            if ( ele.electronID(self.cfg_ana.inclusive_electron_id) and
                    ele.pt()>self.cfg_ana.inclusive_electron_pt and abs(ele.eta())<self.cfg_ana.inclusive_electron_eta and 
                    abs(ele.dxy())<self.cfg_ana.inclusive_electron_dxy and abs(ele.dz())<self.cfg_ana.inclusive_electron_dz and 
                    ele.numberOfHits()<=self.cfg_ana.inclusive_electron_lostHits ):
                event.inclusiveLeptons.append(ele)
                # basic selection
                if (ele.electronID(self.cfg_ana.loose_electron_id) and
                         ele.pt()>self.cfg_ana.loose_electron_pt and abs(ele.eta())<self.cfg_ana.loose_electron_eta and 
                         abs(ele.dxy()) < self.cfg_ana.loose_electron_dxy and abs(ele.dz())<self.cfg_ana.loose_electron_dz and 
                         ele.relIso03 <= self.cfg_ana.loose_electron_relIso and
                         ele.absIso03 < (self.cfg_ana.loose_electron_absIso if hasattr(self.cfg_ana,'loose_electron_absIso') else 9e99) and
                         ele.numberOfHits() <= self.cfg_ana.loose_electron_lostHits and
                         bestMatch(ele, looseMuons)[1] > self.cfg_ana.min_dr_electron_muon ):
                    event.selectedLeptons.append(ele)
                    event.selectedElectrons.append(ele)
                    ele.looseIdSusy = True
                else:
                    event.otherLeptons.append(ele)
                    ele.looseIdSusy = False

        event.otherLeptons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedLeptons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedMuons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedElectrons.sort(key = lambda l : l.pt(), reverse = True)
        event.inclusiveLeptons.sort(key = lambda l : l.pt(), reverse = True)

        for lepton in event.selectedLeptons:
            if hasattr(self,'efficiency'):
                self.efficiency.attachToObject(lepton)

    def makePhotons(self, event):
        event.allphotons = map( Photon, self.handles['photons'].product() )
        event.allphotons.sort(key = lambda l : l.pt(), reverse = True)
       
    def makeAllMuons(self, event):
        """
               make a list of all muons, and apply basic corrections to them
        """
        # Start from all muons
        allmuons = map( Muon, self.handles['muons'].product() )

        # Muon scale and resolution corrections (if enabled)
        if self.cfg_ana.doMuScleFitCorrections:
            for mu in allmuons:
                self.muscleCorr.correct(mu, event.run)
        elif self.cfg_ana.doRochesterCorrections:
            for mu in allmuons:
                corp4 = rochcor.corrected_p4(mu, event.run) 
                mu.setP4( corp4 )

        # Clean up dulicate muons (note: has no effect unless the muon id is removed)
        if self.cfg_ana.doSegmentBasedMuonCleaning:
            isgood = cmgMuonCleanerBySegments.clean( self.handles['muons'].product() )
            newmu = []
            for i,mu in enumerate(allmuons):
                if isgood[i]: newmu.append(mu)
            allmuons = newmu

        # Attach the vertex to them, for dxy/dz calculation
        for mu in allmuons:
            mu.associatedVertex = event.goodVertices[0]

        # Compute relIso in 0.3 and 0.4 cones
        for mu in allmuons:
            mu.absIso03 = (mu.sourcePtr().pfIsolationR03().sumChargedHadronPt + max( mu.sourcePtr().pfIsolationR03().sumNeutralHadronEt +  mu.sourcePtr().pfIsolationR03().sumPhotonEt -  mu.sourcePtr().pfIsolationR03().sumPUPt/2,0.0))
            mu.absIso04 = (mu.sourcePtr().pfIsolationR04().sumChargedHadronPt + max( mu.sourcePtr().pfIsolationR04().sumNeutralHadronEt +  mu.sourcePtr().pfIsolationR04().sumPhotonEt -  mu.sourcePtr().pfIsolationR04().sumPUPt/2,0.0))
            mu.relIso03 = mu.absIso03/mu.pt()
            mu.relIso04 = mu.absIso04/mu.pt()
 
        return allmuons

    def makeAllElectrons(self, event):
        """
               make a list of all electrons, and apply basic corrections to them
        """
        allelectrons = map( Electron, self.handles['electrons'].product() )

        ## Duplicate removal for fast sim (to be checked if still necessary in latest greatest 5.3.X releases)
        allelenodup = []
        for e in allelectrons:
            dup = False
            for e2 in allelenodup:
                if abs(e.pt()-e2.pt()) < 1e-6 and abs(e.eta()-e2.eta()) < 1e-6 and abs(e.phi()-e2.phi()) < 1e-6 and e.charge() == e2.charge():
                    dup = True
                    break
            if not dup: allelenodup.append(e)
        allelectrons = allelenodup

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

        # Electron scale calibrations
        if self.cfg_ana.doElectronScaleCorrections:
            for ele in allelectrons:
                self.electronEnergyCalibrator.correct(ele, event.run)

        # Attach the vertex
        for ele in allelectrons:
            ele.associatedVertex = event.goodVertices[0]

        # Compute relIso with R=0.3 and R=0.4 cones
        for ele in allelectrons:
            ele.absIso03 = (ele.chargedHadronIso(0.3) + max(ele.neutralHadronIso(0.3)+ele.photonIso(0.3)-ele.rho*ele.EffectiveArea,0))
            ele.absIso04 = (ele.chargedHadronIso(0.4) + max(ele.neutralHadronIso(0.4)+ele.photonIso(0.4)-ele.rho*ele.EffectiveArea,0))
            ele.relIso03 = ele.absIso03/ele.pt()
            ele.relIso04 = ele.absIso04/ele.pt()

        # Set tight MVA id
        for ele in allelectrons:
            ele.tightIdResult = ele.electronID("POG_MVA_ID_Trig")
        
        return allelectrons 

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.counters.counter('events').inc('all events')

        #call the leptons/photons functions
        self.makeLeptons(event)
        self.makePhotons(event)

        return True
