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
        # count.register('at least 2 good jets')
        # count.register('at least 2 clean jets')

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )

        event.selectedJets = filter(self.testJet, event.jets)
                
        self.counters.counter('jets').inc('all events')

        event.cleanJets, dummy = cleanObjectCollection( event.selectedJets,
                                                        masks = event.leptons,
                                                        deltaRMin = 0.5 )
        

        # associating a genjet to each clean jet
        # pairs = matchObjectCollection( event.cleanJets, event.genJets, 0.5*0.5 )
        for jet in event.cleanJets:
            bm, dr2 = bestMatch(jet, event.genJets)
            jet.genJet = bm
            jet.genJet.dR = math.sqrt(dr2) 

        # associating a lepton to each clean jet
        pairs = matchObjectCollection( event.cleanJets, event.leptons, 0.5*0.5 )
        for jet in event.cleanJets:
            jet.lepton = pairs[jet]

        # event.jets30 = [jet for jet in event.jets if jet.pt()>30]
        # event.cleanJets30 = [jet for jet in event.cleanJets if jet.pt()>30]
        
##         if len( event.jets30 )>=2:
##             self.counters.counter('jets').inc('at least 2 good jets')
               
##         if len( event.cleanJets30 )>=2:
##             self.counters.counter('jets').inc('at least 2 clean jets')

        if len(event.cleanJets)<2:
            return True

        return True
        
    
    def testJet( self, jet ):
        # 2 is loose pile-up jet id
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta 


