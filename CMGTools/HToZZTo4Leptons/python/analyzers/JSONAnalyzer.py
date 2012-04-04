import json
from CMGTools.RootTools.fwlite.Analyzer import Analyzer

#FIXME put in root tools

class JSONAnalyzer( Analyzer ):
    '''Apply a json filter.

    example:
    
    jsonFilter = cfg.Analyzer(
      "JSONAnalyzer",
      json = json  # this is the path to the json file 
      )
    '''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(JSONAnalyzer, self).__init__(cfg_ana, cfg_comp, looperName)
        jsonFile = open( self.cfg_ana.json )
        self.json = json.loads( jsonFile.read() )


    def beginLoop(self):
        super(JSONAnalyzer,self).beginLoop()
        self.counters.addCounter('JSON')
        self.count = self.counters.counter('JSON')
        self.count.register('All events')
        self.count.register('Passed')


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.count.inc('All events')
        run = iEvent.eventAuxiliary().id().run()
        lumi = iEvent.eventAuxiliary().id().luminosityBlock()
        # print run, lumi
        # print self.json
        lumis = self.json.get( str(run), None)
        if lumis is None:
            return False
        # print lumis 
        for range in lumis:
            min = range[0]
            max = range[1]
            if lumi>=min and lumi<=max:
                self.count.inc('Passed')
                return True
        return False 
