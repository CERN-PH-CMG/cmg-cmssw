from math import *

from CMGTools.TTHAnalysis.analyzers.ttHLepTreeProducerNew import *

class ttHLepStudyTreeProducer( ttHLepTreeProducerNew ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepStudyTreeProducer,self).__init__(cfg_ana,cfg_comp,looperName) 

        self.globalVariables = [ 
            NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
        ]
        self.globalObjects = {
            "lep"  : NTupleObject("Lep",   leptonTypeFull, help="Probe lepton"),
        }
        self.collections = {}

        ## Now book the variables
        self.initDone = True
        self.declareVariables()

    def declareHandles(self):
        super(ttHLepStudyTreeProducer, self).declareHandles()

    def beginLoop(self):
        super(ttHLepStudyTreeProducer,self).beginLoop()
        self.counters.addCounter('leptons')
        count = self.counters.counter('leptons')
        count.register('electron')
        count.register('prompt electron')
        count.register('non-prompt electron')
        count.register('unmatched electron')
        count.register('muon')
        count.register('prompt muon')
        count.register('non-prompt muon')
        count.register('unmatched muon')

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        for lep in event.inclusiveLeptons:
            ## increment counters
            name = "muon" if abs(lep.pdgId()) == 13 else "electron"
            self.counters.counter('leptons').inc(name)
            if lep.mcMatchId > 0:     self.counters.counter('leptons').inc('prompt '+name)
            elif lep.mcMatchAny != 0: self.counters.counter('leptons').inc('non-prompt '+name)
            else:                     self.counters.counter('leptons').inc('unmatched '+name)
            event.lep = lep
            self.fillTree(iEvent, event)
         
        return True
