import random
import copy
import math

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
# from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenJet
from CMGTools.RootTools.physicsobjects.LorentzVectors import LorentzVector
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection
from CMGTools.RootTools.statistics.Counter import Counter, Counters
# from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2

from CMGTools.PFSim.particles import GenJet, Jet, SimJet, PFJet, Electron, Muon, SimParticle, GenParticle, RecParticle
from CMGTools.PFSim.detectors.models import CMS



class ObjectReader( Analyzer ):
    """Read physics objects, apply efficiency and fake parametrization, smear them.
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ObjectReader,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(ObjectReader, self).declareHandles()

        if self.cfg_ana.muonCol:
            self.handles['muons'] = AutoHandle( *self.cfg_ana.muonCol )
        if self.cfg_ana.electronCol:
            self.handles['electrons'] = AutoHandle( *self.cfg_ana.electronCol )
        if self.cfg_ana.jetCol:
            self.handles['recJets'] = AutoHandle( *self.cfg_ana.jetCol )
        self.handles['genJets'] = AutoHandle( *self.cfg_ana.genJetCol )
        self.handles['simJets'] = AutoHandle( *self.cfg_ana.simJetCol )
        self.handles['genParticles'] = AutoHandle( *self.cfg_ana.genPartCol )
        if self.cfg_ana.simPartCol:
            self.handles['simParticles'] = AutoHandle( *self.cfg_ana.simPartCol )
        if self.cfg_ana.recPartCol:
            self.handles['recParticles'] = AutoHandle( *self.cfg_ana.recPartCol )
            

    def beginLoop(self):
        super(ObjectReader,self).beginLoop()

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )

        event.genJets = map(GenJet, self.handles['genJets'].product() )

        #TODO duplicated code, see EffAndSmearAnalyzer.
        #
        event.genElectrons = []
        event.genMuons = []
        event.genParticles = []
        event.genParticles3 = []
        for gp in self.handles['genParticles'].product():
            pygp = GenParticle(gp)
            event.genParticles.append( pygp )
            #TODO genParticlesPruned doesn't contain status 3???
            # Maybe specific to HZHA?
            if gp.status() == 2: 
                event.genParticles3.append( pygp ) 
            if abs(gp.pdgId()) == 11: 
                event.genElectrons.append( Electron(gp.p4()) )
            elif abs(gp.pdgId()) == 13:
                event.genMuons.append( Muon(gp.p4()) )

        if self.cfg_ana.muonCol:
            event.muons = map(Muon,self.handles['muons'].product() )
        if self.cfg_ana.electronCol:
            event.electrons = map(Electron, self.handles['electrons'].product() )
        if self.cfg_ana.jetCol:
            event.recJets = map(PFJet, self.handles['recJets'].product() )
        event.simJets = map(SimJet, self.handles['simJets'].product() )
        if self.cfg_ana.simPartCol:
            event.simParticles = map(SimParticle, self.handles['simParticles'].product() )
        if self.cfg_ana.recPartCol:
            event.recParticles = map(RecParticle, self.handles['recParticles'].product() )
            
        event.simMissMass = self.missingMass( event.simParticles )
        event.simVisMass = -99
        # same for visible mass
        
        event.leptons = []
        
        return True


    def missingMass(self, particles):
        '''Here computing missing mass from the list of particles'''
        pass

    
    def visibleMass(self, particles):
        '''Here computing missing mass from the list of particles'''
        pass
