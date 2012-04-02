from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.statistics.Counter import Counter
from CMGTools.RootTools.utils.TriggerList import TriggerList
from CMGTools.RootTools.utils.TriggerMatching import selTriggerObjects


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
        self.counters.counter('Trigger').register('HLT')
        

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]
        run = iEvent.eventAuxiliary().id().run()
        
        self.counters.counter('Trigger').inc('All events')
        # import pdb; pdb.set_trace()
        passed, hltPath = self.triggerList.triggerPassed(event.triggerObject,
                                                         run, self.cfg_comp.isData,
                                                         usePrescaled = False)
        if not passed:
            return False
        # import pdb; pdb.set_trace()
        event.hltPath = hltPath 
        if hltPath is not None:
            trigObjs = self.handles['cmgTriggerObjectListSel'].product()
            # selecting the trigger objects used in this path
            event.triggerObjects = selTriggerObjects( trigObjs, hltPath )
            
        self.counters.counter('Trigger').inc('HLT')
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
