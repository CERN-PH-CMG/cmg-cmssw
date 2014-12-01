from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters

class stFilterAnalyzer( Analyzer ):

    def beginLoop(self):
        super(stFilterAnalyzer,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('accepted events')

    def process(self, iEvent, event):
        self.counters.counter('events').inc('all events')
        if(len(event.selectedLeptons)==0):
            return False
        if(event.selectedLeptons[0].pt()+event.met.pt()<125):
            return False
        self.counters.counter('events').inc('accepted events')
        return True
