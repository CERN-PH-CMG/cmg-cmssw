from CMGTools.RootTools.statistics.Counter import Counter as Trigger

class TriggerList( object ):
    '''TODO: is it the right package? need to document this class'''
    def __init__(self, triggerList):
        self.triggerList = map( Trigger, triggerList )

##     def decodeTriggerList(self, triggerList):
##         raise ValueError('this function is obsolete.')
##         if triggerList is None:
##             return None
##         triglist = map( Trigger, triggerList.split('|') )
##         return triglist

    def triggerPassed(self, triggerObject, triggerList = None):
        '''returns true if at least one of the triggers in self.triggers passes.'''
        if triggerList is None:
            triggerList = self.triggerList
        if not triggerList or len(triggerList)==0:
            return True
        passed = False
        for trigger in triggerList:
            trigger.inc('a: tot   ')
            if triggerObject.getSelectionRegExp( trigger.name ):
                trigger.inc('b: passed')
                passed = True
                # don't break, need to test all triggers in the list
        return passed
        # just doing an OR of all triggers: 
        #return reduce( lambda x,y : x or y,
        #               [triggerObject.getSelectionRegExp( trigger ) for trigger in triggerList])

    def __str__(self):
        head = 'TriggerList'
        triggers = '\n'.join( map(str, self.triggerList) )
        return ':\n'.join( [head, triggers] )
        
if __name__ == '__main__':
    list = ['HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v[5,6]','HLT_IsoMu15_LooseIsoPFTau15_v9']
    trigList = TriggerList( list )
    print trigList
