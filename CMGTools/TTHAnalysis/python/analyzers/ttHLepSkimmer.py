
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

class ttHLepSkimmer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepSkimmer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.ptCuts = cfg_ana.ptCuts if hasattr(cfg_ana, 'ptCuts') else []
        self.ptCuts += 10*[-1.]

        self.idCut = cfg_ana.idCut if hasattr(cfg_ana, 'idCut') else "True"
        self.idFunc = eval("lambda lepton : "+self.idCut);

    def declareHandles(self):
        super(ttHLepSkimmer, self).declareHandles()

    def beginLoop(self):
        super(ttHLepSkimmer,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('vetoed events')
        count.register('accepted events')


    def process(self, iEvent, event):
        self.readCollections( iEvent )
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

        if ret: self.counters.counter('events').inc('accepted events')
        return ret
