import math

class Looper(object):

    def __init__(self, fraction):
        self.fraction = fraction
        
    def Loop( self, nEvents=None ):
        '''Loop on a given number of events, and call ToEvent for each event.'''
        print 'starting loop'
        if nEvents is None:
            nEvents = self.events.size()
        else:
            nEvents = int(nEvents)
        eventSize = nEvents
        for iEv in range(0, eventSize ):
            # print iEv, nEvents
            if iEv == nEvents:
                break
            if iEv%100 ==0:
                print 'event', iEv
            try:
                self.process( iEv )
            except ValueError:
                #COLIN should not be a value error
                break 
        self.logger.warning( str(self) )

    def process(self, iEv ):
        # for analyzer in self.analyzers:
        #    analyzer.process( iEv )
        pass

    def Write(self):
        # for analyzer in self.analyzers:
        #    analyzer.Write()
        pass 


