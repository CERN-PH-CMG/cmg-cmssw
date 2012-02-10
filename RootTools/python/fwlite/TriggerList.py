import os

from CMGTools.RootTools.statistics.Counter import Counter as Trigger
from CMGTools.RootTools.utils.triggerevo import Menus

class TriggerList( object ):
    '''TODO: is it the right package? need to document this class'''
    def __init__(self, triggerList):
        self.triggerList = map( Trigger, triggerList )
        fileName = '/'.join( [os.environ['CMSSW_BASE'],
                              'src/CMGTools/RootTools/python/utils/triggerEvolution_all.txt'])
        datasets = ['TauPlusX']
        self.menus = Menus( fileName, datasets )
        self.run = -1

    def restrictList(self, run, triggerList ):
        if run == 1:
            return triggerList
        if run != self.run:
            try:
                selMenus = self.menus.findUnprescaledPaths(run, 'TauPlusX')
                self.unprescaledPaths = set( path.name for path in selMenus )
                self.restrictedTriggerList = [trigger for trigger in triggerList if trigger.name in self.unprescaledPaths ]
                print 'restricting list: ', run, [trigger.name for trigger in self.restrictedTriggerList]
            except ValueError:
                print 'no menu with run', run, 'using full trigger list.'
                self.restrictedTriggerList = self.triggerList
            self.run = run
        return self.restrictedTriggerList
        
    def triggerPassed(self, triggerObject, run, usePrescaled=False, triggerList = None):
        '''returns true if at least one of the triggers in self.triggers passes.'''
        if triggerList is None:
            triggerList = self.restrictList( run, self.triggerList ) 
        if not triggerList or len(triggerList)==0:
            return True
        passed = False
        for trigger in triggerList:
            trigger.inc('a: tot   ')
            if triggerObject.getSelectionRegExp( trigger.name ):
                prescaleFactor = triggerObject.getPrescale( trigger.name )
                if usePrescaled or prescaleFactor == 1:
                    trigger.inc('b: passed')
                    passed = True
                # don't break, need to test all triggers in the list
        return passed

    def __str__(self):
        head = 'TriggerList'
        triggers = '\n'.join( map(str, self.triggerList) )
        return ':\n'.join( [head, triggers] )
        
if __name__ == '__main__':
    list = ['HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v[5,6]','HLT_IsoMu15_LooseIsoPFTau15_v9']
    trigList = TriggerList( list )
    print trigList
