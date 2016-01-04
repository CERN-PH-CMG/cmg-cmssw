from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
import itertools

class ttHLepSkimmer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepSkimmer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.ptCuts = cfg_ana.ptCuts if hasattr(cfg_ana, 'ptCuts') else []
        self.ptCuts += 10*[-1.]

        self.idCut = cfg_ana.idCut if (getattr(cfg_ana, 'idCut', '') != '') else "True"
        self.idFunc = eval("lambda lepton : "+self.idCut);

        self.requireSameSignPair = getattr(cfg_ana,"requireSameSignPair",False)

    def declareHandles(self):
        super(ttHLepSkimmer, self).declareHandles()

    def beginLoop(self, setup):
        super(ttHLepSkimmer,self).beginLoop(setup)
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('vetoed events')
        count.register('accepted events')


    def process(self, event):
        self.readCollections( event.input )
        self.counters.counter('events').inc('all events')

        
        leptons = []
        for lep, ptCut in zip(event.selectedLeptons, self.ptCuts):
            if not self.idFunc(lep):
                continue
            if lep.pt() > ptCut: 
                leptons.append(lep)

        ret = False 
        if len(leptons) >= self.cfg_ana.minLeptons:
            ret = True
        if len(leptons) > self.cfg_ana.maxLeptons:
            if ret: self.counters.counter('events').inc('vetoed events')
            ret = False
        if ret and self.requireSameSignPair:
            ret = any([l1.charge()==l2.charge() for l1,l2 in itertools.combinations(leptons,2)])

        if ret: self.counters.counter('events').inc('accepted events')
        return ret
