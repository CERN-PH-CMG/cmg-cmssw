
class Looper(object):

    def __init__(self):
        # self.analyzers = [ self ]
        pass

    def Loop(self, nEvents=-1 ):
        '''Loop on a given number of events, and call ToEvent for each event.'''
        print 'starting loop'
        # self.InitCounters()
        nEvents = int(nEvents)
        for iEv in range(0, self.events.size() ):
            if iEv == nEvents:
                break
            if iEv%1000 ==0:
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


