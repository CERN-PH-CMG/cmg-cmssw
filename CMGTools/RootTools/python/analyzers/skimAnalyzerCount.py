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
    
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(skimAnalyzerCount, self).__init__(cfg_ana, cfg_comp, looperName)
        self.useLumiBlocks = self.cfg_ana.useLumiBlocks if (hasattr(self.cfg_ana,'useLumiBlocks')) else False
 
    def declareHandles(self):
        super(skimAnalyzerCount, self).declareHandles()
        self.counterHandle = Handle("edm::MergeableCounter")
        self.mchandles['GenInfo'] = AutoHandle( ('generator','',''), 'GenEventInfoProduct' )
        
    def beginLoop(self):
        super(skimAnalyzerCount,self).beginLoop()

        self.counters.addCounter('SkimReport')
        self.count = self.counters.counter('SkimReport')
        self.count.register('All Events')
        if self.cfg_comp.isMC: 
            self.count.register('Sum Weights')

        if not self.useLumiBlocks:
            print 'Will actually count events instead of accessing lumi blocks'
            return True

        print 'Counting the total events before the skim by accessing luminosity blocks'
        lumis = Lumis(self.cfg_comp.files)
        totalEvents=0
        
        for lumi in lumis:
            if lumi.getByLabel('prePathCounter',self.counterHandle):
                totalEvents+=self.counterHandle.product().value
            else:
                self.useLumiBlocks = False
                break
            
       
        if self.useLumiBlocks:
            self.count.inc('All Events',totalEvents)
            if self.cfg_comp.isMC: 
                self.count.inc('Sum Weights',totalEvents)
            print 'Done -> proceeding with the analysis' 
        else:
            print 'Failed -> will have to actually count events (this can happen if the input dataset is not a CMG one)'



    def process(self, iEvent, event):
        if not self.useLumiBlocks:
            self.readCollections( iEvent )
            self.count.inc('All Events')
            if self.cfg_comp.isMC: 
                self.count.inc('Sum Weights', self.mchandles['GenInfo'].product().weight())
        return True
