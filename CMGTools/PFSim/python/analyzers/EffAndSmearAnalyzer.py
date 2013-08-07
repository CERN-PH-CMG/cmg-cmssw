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

from CMGTools.PFSim.particles import GenJet, Jet, Electron, Muon
from CMGTools.PFSim.detectors.models import CMS


def effAndSmearObjects( objects, detmodel, particleTypeName, output_class):
    results = []
    eff_fun = getattr(detmodel, ''.join([particleTypeName, 'Efficiency']) )
    res_fun = getattr(detmodel, ''.join([particleTypeName, 'Resolution']) )
    
    for iobj in objects:
        eff_val = eff_fun(iobj)
        res_val = res_fun(iobj)
        
        oobj = output_class( iobj.p4() )
        oobj.gen = iobj
        smearing = random.gauss(1, res_val ) 
        oobj.smear( smearing )

        if random.uniform(0,1)<eff_val:
            results.append(oobj)
    return results



class EffAndSmearAnalyzer( Analyzer ):
    """Read physics objects, apply efficiency and fake parametrization, smear them.
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(EffAndSmearAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(EffAndSmearAnalyzer, self).declareHandles()

        self.mchandles['genParticles'] = AutoHandle( *self.cfg_ana.genPartCol )
        self.mchandles['genJets'] = AutoHandle( *self.cfg_ana.genJetCol )

    def beginLoop(self):
        super(EffAndSmearAnalyzer,self).beginLoop()

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )
        
        event.genJets = [GenJet(ptc.p4()) for ptc in self.mchandles['genJets'].product() ]

        event.jets = effAndSmearObjects(event.genJets, CMS, 'jet', Jet)

        event.genElectrons = []
        event.genMuons = []
        event.genParticles = []
        event.genParticles3 = []
        for gp in self.mchandles['genParticles'].product():
            pygp = GenParticle(gp)
            event.genParticles.append( pygp )
            if gp.status() == 3:
                event.genParticles3.append( pygp ) 
            if abs(gp.pdgId()) == 11: 
                event.genElectrons.append( Electron(gp.p4()) )
            elif abs(gp.pdgId()) == 13:
                event.genMuons.append( Muon(gp.p4()) )

        event.electrons = effAndSmearObjects(event.genElectrons, CMS, 'electron', Electron)
        event.muons = effAndSmearObjects(event.genMuons, CMS, 'muon', Muon )

        #TODO how to decide which ones are considered as leptons in the analysis?
        # iso cut? smear and eff full collection of gen particles?
        # or use gen provenance? 
        event.leptons = [] 

        #TODO deal with taus
        
        return True
        
