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
        totalFourMu=0
        totalFourEle=0
        totalTwoEleTwoMu=0
        totalZH=0
        totalWH=0
        totalTTH=0
        
        for lumi in lumis:
            lumi.getByLabel('prePathCounter',self.counterHandle)
            totalEvents+=self.counterHandle.product().value
            lumi.getByLabel('genInfo','4mu',self.counterHandle)
            totalFourMu+=self.counterHandle.product().value
            lumi.getByLabel('genInfo','4e',self.counterHandle)
            totalFourEle+=self.counterHandle.product().value
            lumi.getByLabel('genInfo','2e2mu',self.counterHandle)
            totalTwoEleTwoMu+=self.counterHandle.product().value
            lumi.getByLabel('genInfo','ZH',self.counterHandle)
            totalZH+=self.counterHandle.product().value
            lumi.getByLabel('genInfo','WH',self.counterHandle)
            totalWH+=self.counterHandle.product().value
            lumi.getByLabel('genInfo','ttH',self.counterHandle)
            totalTTH+=self.counterHandle.product().value
        

        self.counters.addCounter('SkimReport')
        self.count = self.counters.counter('SkimReport')
        self.count.register('All Events')
        self.count.register('4e')
        self.count.register('4mu')
        self.count.register('2e2mu')
        self.count.register('ZH')
        self.count.register('WH')
        self.count.register('ttH')

        self.count.inc('All Events',totalEvents)
        self.count.inc('2e2mu',totalTwoEleTwoMu)
        self.count.inc('4mu',totalFourMu)
        self.count.inc('4e',totalFourEle)
        self.count.inc('ZH',totalZH)
        self.count.inc('WH',totalWH)
        self.count.inc('ttH',totalTTH)

        print 'Done -> proceeding with the analysis' 



    def process(self, iEvent, event):
        return True
    

