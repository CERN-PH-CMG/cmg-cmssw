import random
import copy
import math

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
# from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenJet
from CMGTools.RootTools.physicsobjects.LorentzVectors import LorentzVector
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2

from CMGTools.PFSim.particles import GenJet, Jet, PFJet, Electron, Muon
from CMGTools.PFSim.detectors.models import CMS



class ObjectReader( Analyzer ):
    """Read physics objects, apply efficiency and fake parametrization, smear them.
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ObjectReader,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(ObjectReader, self).declareHandles()

        self.handles['muons'] = AutoHandle( *self.cfg_ana.muonCol )
        self.handles['electrons'] = AutoHandle( *self.cfg_ana.electronCol )
        self.handles['jets'] = AutoHandle( *self.cfg_ana.jetCol )
        self.handles['genJets'] = AutoHandle( *self.cfg_ana.genJetCol )
        self.handles['genParticles'] = AutoHandle( *self.cfg_ana.genPartCol )

    def beginLoop(self):
        super(ObjectReader,self).beginLoop()

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )

        event.genJets = map(GenJet, self.handles['genJets'].product() )

        event.genElectrons = []
        event.genMuons = []
        for gp in self.handles['genParticles'].product():
            if abs(gp.pdgId()) == 11: 
                event.genElectrons.append( Electron(gp.p4()) )
            elif abs(gp.pdgId()) == 13:
                event.genMuons.append( Muon(gp.p4()) )

        event.muons = map(Muon, self.handles['muons'].product() )
        event.electrons = map(Electron, self.handles['electrons'].product() )
        event.jets = map(PFJet, self.handles['jets'].product() )
        random.shuffle(event.jets)
 
        event.leptons = []
        
        return True
        
