import itertools

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Electron import Electron

from DataFormats.FWLite import Events, Handle,Lumis

class SkimCountAnalyzer( Analyzer ):
    '''This analyzer finds the initial events before the skim
    '''

    def declareHandles(self):
        super(SkimCountAnalyzer, self).declareHandles()
        self.counterHandle = Handle("edm::MergeableCounter")
        
    def beginLoop(self):
        super(SkimCountAnalyzer,self).beginLoop()

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
    

