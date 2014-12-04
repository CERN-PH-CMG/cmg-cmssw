
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

class ttHJetMETSkimmer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHJetMETSkimmer,self).__init__(cfg_ana,cfg_comp,looperName)
        self.bjetCut = eval("lambda jet : "+self.cfg_ana.nBJet[2]) if self.cfg_ana.nBJet[2] != "" else (lambda jet : True);

    def declareHandles(self):
        super(ttHJetMETSkimmer, self).declareHandles()

    def beginLoop(self):
        super(ttHJetMETSkimmer,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('pass jetPtCuts')
        count.register('pass jetVeto')
        count.register('pass met')
        count.register('pass HT')
        count.register('pass MHT')
        count.register('pass nBJet')
        count.register('accepted events')


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.counters.counter('events').inc('all events')

        jets = getattr(event, self.cfg_ana.jets)
        for i,ptCut in enumerate(self.cfg_ana.jetPtCuts):
            if len(jets) <= i or jets[i].pt() <= ptCut:
                return False
        self.counters.counter('events').inc('pass jetPtCuts')
        
        if float(self.cfg_ana.jetVetoPt) > 0:
            if len(jets) > len(self.cfg_ana.jetPtCuts):
                for j in jets[len(self.cfg_ana.jetPtCuts):]:
                    if j.pt() > self.cfg_ana.jetVetoPt:
                        return False
        self.counters.counter('events').inc('pass jetVeto')

        if float(self.cfg_ana.metCut) > 0 and event.met.pt() <= self.cfg_ana.metCut:
            return False
        self.counters.counter('events').inc('pass met')

        if float(self.cfg_ana.htCut[1]) > 0 and getattr(event, self.cfg_ana.htCut[0]) <=  self.cfg_ana.htCut[1]:
            return False
        self.counters.counter('events').inc('pass HT')

        if float(self.cfg_ana.mhtCut[1]) > 0 and getattr(event, self.cfg_ana.mhtCut[0]) <=  self.cfg_ana.mhtCut[1]:
            return False
        self.counters.counter('events').inc('pass MHT')

        nB = 0;
        for j in jets:
            if j.btagWP(self.cfg_ana.nBJet[0]) and self.bjetCut(j):
                nB += 1;
        if nB < int(self.cfg_ana.nBJet[1]): 
            return False
        self.counters.counter('events').inc('pass nBJet')

        self.counters.counter('events').inc('accepted events')
        return True
