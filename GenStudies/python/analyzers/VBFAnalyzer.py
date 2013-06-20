from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Jet
from CMGTools.RootTools.physicsobjects.VBF import VBF
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.GenStudies.DiLepton import DiLepton

class VBFAnalyzer( Analyzer ):
    """ Analyses the collection of jets stored in event.cleanJets,
    and adds a VBF object to the event, as event.vbf.

    Example :

    vbfAna = cfg.Analyzer(
        'VBFAnalyzer',
        Mjj = 500,
        deltaEta = 3.5,
        cjvPtCut = 30.,
    )
    """


    def beginLoop(self):
        super(VBFAnalyzer,self).beginLoop()
        self.counters.addCounter('VBF')
        count = self.counters.counter('VBF')
        count.register('all events')
        count.register('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj))
        count.register('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
        count.register('no central jets')

        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.counters.counter('VBF').inc('all events')
        event.tCounter[0] = ['VBF_all', 1]

        if len(event.cleanGenJets)<2:
            return True

        
        event.tCounter[1] = ['VBF_2jets', 1]

        event.dilepton = DiLepton(event.Higgs.leg1(), event.Higgs.leg2(), event.Higgs.met())
        event.vbf = VBF( event.cleanGenJets, event.dilepton, None, self.cfg_ana.cjvPtCut )


        if event.vbf.mjj > self.cfg_ana.Mjj:
            self.counters.counter('VBF').inc('M_jj > {cut:3.1f}'.format(cut=self.cfg_ana.Mjj) )
            event.tCounter[2] = ['VBF_Mjj', 1]
        else:
            return True 
        if abs(event.vbf.deta) > self.cfg_ana.deltaEta:
            self.counters.counter('VBF').inc('delta Eta > {cut:3.1f}'.format(cut=self.cfg_ana.deltaEta) )
            event.tCounter[3] = ['VBF_deltaeta', 1]
        else:
            return True 
        if len(event.vbf.centralJets)==0:
            self.counters.counter('VBF').inc('no central jets')
            event.tCounter[4] = ['VBF_cjv', 1]
        else:
            return True
        
        return True
