import random
import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.LorentzVectors import LorentzVector
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection, bestMatch
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2


def missMass( comEnergy, particles):
    p4 = LorentzVector( 0, 0, 0, comEnergy)
    hp4 = LorentzVector( 0, 0, 0, 0)
    for p in particles:
        p4 -= p.p4()
        if hp4:
            hp4 += p.p4()
        else:
            hp4 = p.p4()
    return p4, hp4


class MissMassAnalyzer( Analyzer ):
    """Analyze jets ;-)
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(MissMassAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(MissMassAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(MissMassAnalyzer,self).beginLoop()
        self.counters.addCounter('missmass')
        count = self.counters.counter('missmass')
        count.register('all events')

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )
        self.counters.counter('missmass').inc('all events')
        
        mp4, vp4 = missMass( 350., event.simParticles )
        event.simMissMass = mp4.mass()
        event.simVisMass = vp4.mass()
        return True            
 
