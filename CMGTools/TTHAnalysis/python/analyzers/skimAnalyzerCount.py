import itertools

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters

from DataFormats.FWLite import Events, Handle,Lumis

class skimAnalyzerCount( Analyzer ):


    #---------------------------------------------
    # TO FINDS THE INITIAL EVENTS BEFORE THE SKIM
    #---------------------------------------------
    

    def declareHandles(self):
        super(skimAnalyzerCount, self).declareHandles()
        self.counterHandle = Handle("edm::MergeableCounter")
        
    def beginLoop(self):
        super(skimAnalyzerCount,self).beginLoop()

        print 'Counting the total events before the skim by accessing luminosity blocks'
        lumis = Lumis(self.cfg_comp.files)
        totalEvents=0
        
        for lumi in lumis:
            lumi.getByLabel('prePathCounter',self.counterHandle)
            totalEvents+=self.counterHandle.product().value
            
        self.counters.addCounter('SkimReport')
        self.count = self.counters.counter('SkimReport')
        self.count.register('All Events')
        
        self.count.inc('All Events',totalEvents)
        

        print 'Done -> proceeding with the analysis' 



    def process(self, iEvent, event):
        return True
