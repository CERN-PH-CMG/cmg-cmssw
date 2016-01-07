from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.Tau import Tau

import PhysicsTools.HeppyCore.framework.config as cfg

 
class MonoXTauAnalyzer( Analyzer ):

    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(MonoXTauAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def beginLoop(self, setup):
        super(MonoXTauAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('has >=1 selected taus')
        count.register('has >=1 monox selected taus')
        count.register('has >=1 monox other taus')

    #------------------
    # MAKE LEPTON LISTS
    #------------------
    def makeTaus(self, event):
        event.monoxSelectedTaus = []
        event.monoxOtherTaus = []

        #get all the selected taus from TauAnalyzer
        inputtaus = event.selectedTaus
        for tau in inputtaus:
            if tau.tauID(self.cfg_ana.tauIsolation) < self.cfg_ana.isolationMax:
                event.monoxSelectedTaus.append(tau)
            else:
                event.monoxOtherTaus.append(tau)

        event.monoxSelectedTaus.sort(key = lambda l : l.pt(), reverse = True)
        event.monoxOtherTaus.sort(key = lambda l : l.pt(), reverse = True)
        self.counters.counter('events').inc('all events')
        if len(event.selectedTaus): self.counters.counter('events').inc('has >=1 selected taus')
        if len(event.monoxSelectedTaus): self.counters.counter('events').inc('has >=1 monox selected taus')
        if len(event.monoxOtherTaus): self.counters.counter('events').inc('has >=1 monox other taus')

    def process(self, event):
        self.readCollections( event.input )

        self.makeTaus(event)

        return True

setattr(MonoXTauAnalyzer,"defaultConfig",cfg.Analyzer(
    class_object = MonoXTauAnalyzer,
    # monox hadronic tau selection
    tauIsolation = "byCombinedIsolationDeltaBetaCorrRaw3Hits",
    isolationMax = 5.0 # GeV
  )
)

