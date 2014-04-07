import random
import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection, bestMatch
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2


dRbins = [(0,0.1), (0.1, 0.2), (0.2, 0.3), (0.3, 0.4), (0.4, 0.5)]
pdgIds = [11,13,22,130,211]


class ParticleAnalyzer( Analyzer ):
    """Analyze particles. designed to work for a few gen particles only (gun)
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ParticleAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

     
    def process(self, iEvent, event):
        
        particles = event.recParticles
        genParticles = event.genParticles

        def match(jet, objects, name):
            bm, dr2 = bestMatch(jet, objects)
            if bm:
                bm.dr = math.sqrt(dr2)
            setattr(jet, name, bm)

        # should probably do the following by type

        # find closest sim particle
        for gen in genParticles:            
            for id in pdgIds:
                otherParticles = [p for p in particles if abs(p.pdgId())==id]
                match(gen, otherParticles, 'sim{id}'.format(id=id))
            
        # measure particle energy density in each ring (divide by ring area)
        return True        
    
