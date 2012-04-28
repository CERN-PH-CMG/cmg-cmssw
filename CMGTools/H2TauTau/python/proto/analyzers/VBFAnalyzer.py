import random
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.utils.DeltaR import cleanObjectCollection
from CMGTools.RootTools.physicsobjects.VBF import VBF
from CMGTools.RootTools.statistics.Counter import Counter, Counters

class VBFAnalyzer( Analyzer ):
    '''Analyze jets, and in particular VBF.'''

    def declareHandles(self):
        super(VBFAnalyzer, self).declareHandles()

        self.handles['jets'] = AutoHandle( self.cfg_ana.jetCol,
                                           'std::vector<cmg::PFJet>' )

    def beginLoop(self):
        super(VBFAnalyzer,self).beginLoop()
        self.counters.addCounter('VBF')
        count = self.counters.counter('VBF')
        count.register('all events')
        count.register('at least 2 good jets')
        count.register('at least 2 clean jets')
        count.register('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj))
        count.register('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
        count.register('no central jets')
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        cmgJets = self.handles['jets'].product()
        event.jets = []
        event.cleanJets = []
        for cmgJet in cmgJets:
            jet = Jet( cmgJet )
            if self.cfg_comp.isMC and hasattr( self.cfg_comp, 'jetScale'):
                scale = random.gauss( self.cfg_comp.jetScale,
                                      self.cfg_comp.jetSmear )
                jet.scaleEnergy( scale )
            if not self.testJet( cmgJet ):
                continue
            event.jets.append(jet)
        self.counters.counter('VBF').inc('all events')
        if len( event.jets )<2:
            return True
        self.counters.counter('VBF').inc('at least 2 good jets')
       
        event.cleanJets = cleanObjectCollection( event.jets,
                                                 masks = [event.diLepton.leg1(),
                                                          event.diLepton.leg2() ],
                                                 deltaRMin = 0.5 )
        
        if len( event.cleanJets )<2:
            return True
        self.counters.counter('VBF').inc('at least 2 clean jets')

        event.vbf = VBF( event.cleanJets )
        if event.vbf.mjj > self.cfg_ana.Mjj:
            self.counters.counter('VBF').inc('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj) )
        else:
            return True 
        if abs(event.vbf.deta) > self.cfg_ana.deltaEta:
            self.counters.counter('VBF').inc('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
        else:
            return True 
        if len(event.vbf.centralJets)==0:
            self.counters.counter('VBF').inc('no central jets')
        else:
            return True
        
        return True
        
        
    def testJet( self, jet ):
        if jet.pt() > self.cfg_ana.jetPt and \
               abs( jet.eta() ) < self.cfg_ana.jetEta:
            return True
        else:
            return False

