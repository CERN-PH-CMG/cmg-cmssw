import os

from CMGTools.RootTools.statistics.Counter import Counter as Trigger
from CMGTools.RootTools.utils.triggerevo import Menus
from CMGTools.RootTools.utils.TriggerJSON import TriggerJSON



class TriggerList( object ):
    '''Holds a list of HLT trigger paths. Can be asked if, for a given triggerObject in a given event, one of the triggers in the list is passed.'''
    def __init__(self, triggerList):
        '''triggerlist is a list of HLT trigger paths.

        Internally, each trigger in the list will be kept as a Counter, which allows to count how many
        events have been tested against, and have passed each trigger.'''
        self.triggerList = map( Trigger, triggerList )
        for trig in self.triggerList:
            trig.register('events tested')
            trig.register('events passed')
        fileName = '/'.join( [os.environ['CMSSW_BASE'],
                              'src/CMGTools/RootTools/python/utils/triggerEvolution_all.txt'])
        datasets = ['TauPlusX']
        self.menus = Menus( fileName, datasets )
        self.run = -1
        self.triggerJSON = TriggerJSON()

    def restrictList(self, run, triggerList ):
        '''Restrict the trigger list to the list of unprescaled triggers in this run.

        Seriously speeds up the code.'''
        # import pdb; pdb.set_trace()
        # if run == 1:
        #    return triggerList
        if run != self.run:
            try:
                selMenus = self.menus.findUnprescaledPaths(run, 'TauPlusX')
                self.unprescaledPaths = set( path.name for path in selMenus )
                self.restrictedTriggerList = [trigger \
                                              for trigger in triggerList \
                                              if trigger.name in self.unprescaledPaths ]
                # print 'restricting list: ', run, [trigger.name for trigger in self.restrictedTriggerList]
            except ValueError:
                print 'no menu with run', run, 'using full trigger list.'
                self.restrictedTriggerList = self.triggerList
            self.run = run
        if len(self.restrictedTriggerList) == 0:
            print 'run', run, ': no path from the user list found in the list of unprescaled paths from the trigger DB. The latter could be wrong, using the user trigger list.'
            self.restrictedTriggerList = self.triggerList
        return self.restrictedTriggerList
        
    def triggerPassed(self, triggerObject, run, isData, usePrescaled=False):
        '''returns true if at least one of the triggers in the triggerlist passes.

        run is provided to call restrictList.
        if usePrescaled is False (DEFAULT), only the unprescaled triggers are considered.
        if triggerList is None (DEFAULT), oneself triggerlist is used. '''

        # import pdb; pdb.set_trace()
        triggerList = self.triggerList
        if isData:
            triggerList = self.restrictList( run, self.triggerList ) 
        # if not triggerList or len(triggerList)==0:
        #    return False
        if len(triggerList)==0:
            # no trigger specified, accepting all events
            return True, None
        passed = False
        firstTrigger = None
        for trigger in triggerList:
            trigger.inc('events tested')
            if triggerObject.getSelectionRegExp( trigger.name ):
                prescaleFactor = triggerObject.getPrescale( trigger.name )
                if usePrescaled or prescaleFactor == 1 or not isData:
                    # prescales are set to 0 in MC
                    trigger.inc('events passed')
                    passed = True
                    if firstTrigger is None:
                        firstTrigger = trigger.name
                        self.triggerJSON.setdefault(trigger.name, set()).add( run )
                # don't break, need to test all triggers in the list
                # break
        return passed, firstTrigger


    def write(self, dirName ):
        self.triggerJSON.write( dirName )
        map( lambda x: x.write(dirName), self.triggerList)


    def computeLumi(self, json):
        self.triggerJSON.computeLumi( json )

    def __str__(self):
        head = 'TriggerList'
        triggers = '\n'.join( map(str, self.triggerList) )
        triggerJSON = str( self.triggerJSON)
        return ':\n'.join( [head, triggers, triggerJSON] )
        
if __name__ == '__main__':
    list = ['HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v[5,6]','HLT_IsoMu15_LooseIsoPFTau15_v9']
    trigList = TriggerList( list )
    print trigList
