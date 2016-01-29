from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

class ttHFastLepSkimmer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHFastLepSkimmer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.muIdCut = self.cfg_ana.muCut
        self.eleIdCut = self.cfg_ana.eleCut

    def declareHandles(self):
        super(ttHFastLepSkimmer, self).declareHandles()
        self.handles['muons'] = AutoHandle(self.cfg_ana.muons,"std::vector<pat::Muon>")            
        self.handles['electrons'] = AutoHandle(self.cfg_ana.electrons,"std::vector<pat::Electron>")            

    def beginLoop(self, setup):
        super(ttHFastLepSkimmer,self).beginLoop(setup)
        self.counters.addCounter('events')
        self.count = self.counters.counter('events')
        self.count.register('all events')
        self.count.register('accepted events')


    def process(self, event):
        self.readCollections( event.input )
        self.count.inc('all events')
        
        leptons = 0

        for el in self.handles['electrons'].product():
            if self.eleIdCut(el): leptons += 1

        for mu in self.handles['muons'].product():
            if self.muIdCut(mu): leptons += 1

        if leptons >= self.cfg_ana.minLeptons:
             self.count.inc('accepted events')
             return True

        return False
