import random
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet, GenJet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection, matchObjectCollection
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.utils.DeltaR import deltaR2

class PFSimAnalyzer( Analyzer ):
    """Analyze jets ;-)
    """

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(PFSimAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)

    def declareHandles(self):
        super(PFSimAnalyzer, self).declareHandles()

        #TODO read jets from the event, use another analyzer to smear them.
        self.handles['jets'] = AutoHandle( *self.cfg_ana.jetCol )
        
        if self.cfg_comp.isMC:
            # and ("BB" in self.cfg_comp.name):
            # self.mchandles['genParticles'] = AutoHandle( *self.cfg_ana.genPartCol )
            self.mchandles['genJets'] = AutoHandle( *self.cfg_ana.genJetCol )

    def beginLoop(self):
        super(PFSimAnalyzer,self).beginLoop()
        self.counters.addCounter('jets')
        count = self.counters.counter('jets')
        count.register('all events')
        count.register('at least 2 good jets')
        count.register('at least 2 clean jets')

        
    def process(self, iEvent, event):
        
        self.readCollections( iEvent )
        cmgJets = self.handles['jets'].product()

        allJets = []
        event.jets = []
        event.cleanJets = []

        #TODO: remove leptons
        # leptons = event.selectedLeptons
        leptons = []
        
        genJets = None
        if self.cfg_comp.isMC:
            genJets = map( GenJet, self.mchandles['genJets'].product() ) 
            
        for cmgJet in cmgJets:
            jet = Jet( cmgJet )
            allJets.append( jet )
            if self.cfg_comp.isMC and hasattr( self.cfg_comp, 'jetScale'):
                scale = random.gauss( self.cfg_comp.jetScale,
                                      self.cfg_comp.jetSmear )
                jet.scaleEnergy( scale )
            if self.testJet( jet ):
                event.jets.append(jet)
            if genJets:
                pairs = matchObjectCollection( [jet], genJets, 0.5*0.5)
                if pairs[jet] is None:
                    jet.genJet = None
                else:
                    jet.genJet = pairs[jet] 
                # print jet, jet.genJet
                
        self.counters.counter('jets').inc('all events')

        event.cleanJets, dummy = cleanObjectCollection( event.jets,
                                                        masks = leptons,
                                                        deltaRMin = 0.5 )
        

        pairs = matchObjectCollection( leptons, allJets, 0.5*0.5)


        # associating a jet to each lepton
        for lepton in leptons:
            jet = pairs[lepton]
            if jet is None:
                lepton.jet = lepton
            else:
                lepton.jet = jet

        # associating a leg to each clean jet
        invpairs = matchObjectCollection( event.cleanJets, leptons, 99999. )
        for jet in event.cleanJets:
            leg = invpairs[jet]
            jet.leg = leg

        event.jets30 = [jet for jet in event.jets if jet.pt()>30]
        event.cleanJets30 = [jet for jet in event.cleanJets if jet.pt()>30]
        
        if len( event.jets30 )>=2:
            self.counters.counter('jets').inc('at least 2 good jets')
               
        if len( event.cleanJets30 )>=2:
            self.counters.counter('jets').inc('at least 2 clean jets')

        if len(event.cleanJets)<2:
            return True

        return True
        
    
    def testJet( self, jet ):
        # 2 is loose pile-up jet id
        return jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta 


