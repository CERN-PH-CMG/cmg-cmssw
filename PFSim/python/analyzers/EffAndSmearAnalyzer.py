import random
import copy
import math

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Particle import Particle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenJet
from CMGTools.RootTools.physicsobjects.LorentzVectors import LorentzVector
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2


class ParticleWithP4(Particle):
    def __init__(self, momentum):
        self.momentum = momentum
    def p4(self):
        return self.momentum
    def pdgId(self):
        return None

class Jet(ParticleWithP4):
    pass
class Electron(ParticleWithP4):
    pass
class Muon(ParticleWithP4):
    pass
class Photon(ParticleWithP4):
    pass
class Tau(ParticleWithP4):
    pass


def effAndSmearObjects( objects, fun ):
    results = []
    for iobj in objects:
        oobj = fun( iobj )
        if oobj:
            oobj.unsmeared = iobj
            results.append(oobj)
    return results


#TODO implement a better smearing and efficiency model.
#TODO implement common smearing and efficiency technique
#TODO set up an efficiency and smearing module


def effAndSmearMomentum(p4, resolution ):
    op4 = copy.copy( p4 )
    resolution = resolution / math.sqrt( p4.energy() ) 
    smearing = random.gauss(1, resolution)
    return op4*smearing  
    
    
def effAndSmear1Electron(iele):
    op4 = effAndSmearMomentum( iele.p4(), 0.02 )
    oele = Electron( op4 )
    oele.gen = iele #TODO will get different results from DR matching...
    return oele
    
def effAndSmear1Muon(imu):
    op4 = effAndSmearMomentum( imu.p4(), 0.002 )
    omu = Muon( op4 )
    omu.gen = imu
    return omu

def effAndSmear1Jet(ijet):
    op4 = effAndSmearMomentum( ijet.p4(), 0.60 )
    ojet = Jet( op4 )
    return ojet




class EffAndSmearAnalyzer( Analyzer ):
    """Read physics objects, apply efficiency and fake parametrization, smear them.
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(EffAndSmearAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(EffAndSmearAnalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle( *self.cfg_ana.jetCol )
        self.mchandles['genParticles'] = AutoHandle( *self.cfg_ana.genPartCol )
        self.mchandles['genJets'] = AutoHandle( *self.cfg_ana.genJetCol )

    def beginLoop(self):
        super(EffAndSmearAnalyzer,self).beginLoop()

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )
        
        event.genJets = map( GenJet, self.mchandles['genJets'].product() ) 

        event.jets = effAndSmearObjects(event.genJets, effAndSmear1Jet )

        event.genElectrons = []
        event.genMuons = []
        for gp in self.mchandles['genParticles'].product():
            if abs(gp.pdgId()) == 11: 
                event.genElectrons.append( Electron(gp.p4()) )
            elif abs(gp.pdgId()) == 13:
                event.genMuons.append( Muon(gp.p4()) )

        event.electrons = effAndSmearObjects(event.genElectrons, effAndSmear1Electron )
        event.muons = effAndSmearObjects(event.genMuons, effAndSmear1Muon )

        #TODO how to decide which ones are considered as leptons in the analysis?
        # iso cut? smear and eff full collection of gen particles?
        # or use gen provenance? 
        event.leptons = [] 

        #TODO deal with taus
        
        return True
        
