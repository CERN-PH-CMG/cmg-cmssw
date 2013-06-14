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

from CMGTools.GenStudies.particles import GenJet, Jet, SimJet, PFJet, Electron, Muon



class GenObjectReader( Analyzer ):
    """Read physics objects, apply efficiency and fake parametrization, smear them.
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(GenObjectReader,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(GenObjectReader, self).declareHandles()

        self.handles['genJets'] = AutoHandle( *self.cfg_ana.genJetCol )
        self.handles['genParticles'] = AutoHandle( *self.cfg_ana.genPartCol )

    def beginLoop(self):
        super(GenObjectReader,self).beginLoop()

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )

        event.genJets = []
        for gj in self.handles['genJets'].product():
            event.genJets.append( GenJet(gj.p4()) )

        event.genElectrons = []
        event.genMuons = []
        event.genParticles = []
        event.genParticles3 = []
        for gp in self.handles['genParticles'].product():
            pygp = GenParticle(gp)
            # keep in mind we're reading C++ reco::GenParticles:
            # http://cmslxr.fnal.gov/lxr/source/DataFormats/Candidate/interface/LeafCandidate.h
            event.genParticles.append( pygp )
            #TODO genParticlesPruned doesn't contain status 3???
            # Maybe specific to HZHA?
            if gp.status() == 2: 
                event.genParticles3.append( pygp ) 
            if abs(gp.pdgId()) == 11: 
                event.genElectrons.append( Electron(gp.p4()) )
            elif abs(gp.pdgId()) == 13:
                event.genMuons.append( Muon(gp.p4()) )

        # what you want here is the daughters of the Higgs boson 
        event.higgsdaughters = []

        # create event.cleanGenJets from event.genJets, removing the ones that are close
        # to the higgs daughters
        # to do that, check the code in GenJetAnalyzer.py
        
        return True
    
        
