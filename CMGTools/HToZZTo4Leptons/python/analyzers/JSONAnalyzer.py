import json
from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer


from FWCore.PythonUtilities.LumiList import LumiList

#FIXME put in root tools

class JSONAnalyzer( TreeAnalyzer ):
    '''Apply a json filter.

    example:
    
    jsonFilter = cfg.Analyzer(
      "JSONAnalyzer",
      json = json  # this is the path to the json file 
      )
    '''

    def __init__(self, cfg_ana, cfg_comp, looperName):

        if not cfg_comp.isMC:
            self.lumiList = LumiList(cfg_ana.json)
        else:
            self.lumiList = None
        
        self.run=0
        self.lumi=0
        super(JSONAnalyzer, self).__init__(cfg_ana, cfg_comp, looperName)

    def declareVariables(self):
        self.tree.addVar('int', 'RUN')
        self.tree.addVar('int', 'LUMI')
        self.tree.addVar('int', 'Passed')
        self.tree.book()

    def beginLoop(self):
        super(JSONAnalyzer,self).beginLoop()
        self.counters.addCounter('JSON')
        self.count = self.counters.counter('JSON')
        self.count.register('All Lumis')
        self.count.register('Passed Lumis')


    def fill(self, varName, value ):
        setattr( self.tree.s, varName, value )

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        run = iEvent.eventAuxiliary().id().run()
        lumi = iEvent.eventAuxiliary().id().luminosityBlock()


        if self.cfg_comp.isMC:
            return True

        if self.lumiList is None:
            return True

        #Do the job only if 
        # run or lumi changed 

        if run != self.run or lumi !=self.lumi:

            self.run=run
            self.lumi=lumi
            #increase the counter for all Lumis
            self.count.inc('All Lumis')

            #Fill the tree
            self.fill('RUN',run)
            self.fill('LUMI',lumi)
            #check the Json file
            if self.lumiList.contains(run,lumi):

                self.fill('Passed',1)
                self.tree.fill()
                self.count.inc('Passed Lumis')
                return True
            else:
                self.fill('Passed',0)
                self.tree.fill()
                return False


