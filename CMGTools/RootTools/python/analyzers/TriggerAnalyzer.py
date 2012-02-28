from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Counter import Counter
from CMGTools.RootTools.utils.TriggerList import TriggerList

class TriggerAnalyzer( Analyzer ):
    '''Analyze vertices, add weight to MC events'''

    def declareHandles(self):
        super(TriggerAnalyzer, self).declareHandles()

        self.handles['cmgTriggerObjectSel'] =  AutoHandle(
            'cmgTriggerObjectSel',
            'std::vector<cmg::TriggerObject>'
            )
 
        self.handles['cmgTriggerObjectListSel'] =  AutoHandle(
            'cmgTriggerObjectListSel',
            'std::vector<cmg::TriggerObject>'
            )
 
    def beginLoop(self):
        super(TriggerAnalyzer,self).beginLoop()
        self.triggerList = TriggerList( self.cfg_comp.triggers )
        self.counters.addCounter('Trigger')
        self.counters.counter('Trigger').register('All events')
        self.counters.counter('Trigger').register('trigger passed')
        

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]
        event.triggerObjects = self.handles['cmgTriggerObjectListSel'].product()
        run = iEvent.eventAuxiliary().id().run()
        
        self.counters.counter('Trigger').inc('All events')
        # import pdb; pdb.set_trace()
        passed, hltPath = self.triggerList.triggerPassed(event.triggerObject,
                                                         run, self.cfg_comp.isData,
                                                         usePrescaled = False)
        if not passed:
            return False
        event.hltPath = hltPath 
        self.counters.counter('Trigger').inc('trigger passed')
        return True

    def write(self):
        print 'writing TriggerAnalyzer'
        super(TriggerAnalyzer, self).write()
        self.triggerList.write( self.dirName )
        if self.cfg_comp.isData:
            self.triggerList.computeLumi( self.cfg_comp.json )
        elif self.cfg_comp.isMC is False:
            print 'cannot compute lumi, json is not present in component configuration.'

    def __str__(self):
        tmp = super(TriggerAnalyzer,self).__str__()
        triglist = str( self.triggerList )
        return '\n'.join( [tmp, triglist ] )
