import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection, bestMatch
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2

class GenJetAnalyzer( Analyzer ):
    ''' Analyze jets '''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(GenJetAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(GenJetAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(GenJetAnalyzer,self).beginLoop()
        self.counters.addCounter('jets')
        count = self.counters.counter('jets')
        count.register('all events')

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )
        self.counters.counter('jets').inc('all events')
        event.selectedGenJets = filter(self.testJet, event.genJets)

        # Overlap removal between leptons
        event.cleanGenJets, dummy = cleanObjectCollection( event.selectedGenJets,
                                                           masks = event.genLeptons3,
                                                           deltaRMin = 0.5 )


        def match(jet, objects, name):
            bm, dr2 = bestMatch(jet, objects)
            if bm:
                bm.dr = math.sqrt(dr2)
            setattr(jet, name, bm)
    
        for jet in event.cleanGenJets:
            match(jet, event.genParticles3, 'genPtc3')

        return True        
    
    def testJet( self, jet ):
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta 


