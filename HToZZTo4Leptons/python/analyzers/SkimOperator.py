from CMGTools.RootTools.fwlite.Analyzer import Analyzer

class SkimOperator( Analyzer ):
    '''Makes the OR of the results from ZSkims, see ZSkim.'''
    
    def beginLoop(self):
        super(SkimOperator,self).beginLoop()
        self.counters.addCounter('Skim')
        self.count = self.counters.counter('Skim')
        self.count.register('all events')
        self.count.register('passing')
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.count.inc('all events')

        theOR = False
        for skimName in self.cfg_ana.skims:
            result = getattr( event, skimName)
            theOR = theOR or result.passing

        if theOR:
            self.count.inc('passing')

        event.skim = theOR
        return True
