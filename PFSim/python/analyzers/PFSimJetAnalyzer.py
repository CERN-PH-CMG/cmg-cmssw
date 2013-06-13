import random
import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection, bestMatch
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2

class PFSimJetAnalyzer( Analyzer ):
    """Analyze jets ;-)
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(PFSimJetAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(PFSimJetAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(PFSimJetAnalyzer,self).beginLoop()
        self.counters.addCounter('jets')
        count = self.counters.counter('jets')
        count.register('all events')

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )

        self.counters.counter('jets').inc('all events')

        event.selectedGenJets = filter(self.testJet, event.genJets)

        # Could have a better lepton rejection at gen level..
        event.cleanGenJets, dummy = cleanObjectCollection( event.selectedGenJets,
                                                           masks = event.leptons,
                                                           deltaRMin = 0.5 )
        


        def match(jet, objects, name):
            bm, dr2 = bestMatch(jet, objects)
            if bm:
                bm.dr = math.sqrt(dr2)
            setattr(jet, name, bm)
    
        for jet in event.cleanGenJets:
            match(jet, event.recJets, 'rec')
            match(jet, event.simJets, 'sim')
            match(jet, event.genParticles3, 'genPtc3')

        return True        
    
    def testJet( self, jet ):
        # 2 is loose pile-up jet id
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta 


