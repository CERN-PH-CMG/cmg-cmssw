from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.TTHAnalysis.analyzers.ntupleObjects import *
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.fwlite.Event import Event
from ROOT import TriggerBitChecker

class ttHLepTreeProducerNew( TreeAnalyzerNumpy ):

    #-----------------------------------
    # TREE PRODUCER FOR THE TTH ANALYSIS
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ttHLepTreeProducerNew,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Declare what we want to fill
        self.globalVariables = [ 
            NTupleVariable("nVert", lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
        ]
        self.globalObjects = {
        }
        self.collections = {
            "selectedLeptons" : NTupleCollection("LepGood", leptonType, 8, help="Leptons after the preselection"),
        }
        self.scalar = not self.cfg_ana.vectorTree

        ## Now book the variables
        self.initDone = True
        self.declareVariables()

    def declareHandles(self):
        super(ttHLepTreeProducerNew, self).declareHandles()
        self.handles['TriggerResults'] = AutoHandle( ('TriggerResults','','HLT'), 'edm::TriggerResults' )

    def declareCoreVariables(self, tr, isMC):
        """Here we declare the variables that we always want and that are hard-coded"""
            
        tr.var('run', int)
        tr.var('lumi', int)
        tr.var('evt', int)

    def declareVariables(self):
        if not hasattr(self,'initDone'): return
        isMC = self.cfg_comp.isMC 
        tree = self.tree
        self.declareCoreVariables(tree, isMC)

        for v in self.globalVariables:
            v.makeBranch(tree, isMC)
        for o in self.globalObjects.itervalues(): 
            o.makeBranches(tree, isMC)
        for c in self.collections.itervalues():
            if self.scalar:
                c.makeBranchesScalar(tree, isMC)
            else:
                c.makeBranchesVector(tree, isMC)
            
    def fillCoreVariables(self, tr, iEvent, event, isMC):
        """Here we fill the variables that we always want and that are hard-coded"""
        tr.fill('run', event.run) 
        tr.fill('lumi',event.lumi)
        tr.fill('evt', event.eventId)    

    def process(self, iEvent, event):
        self.readCollections( iEvent )
         
        isMC = self.cfg_comp.isMC 
        self.tree.reset()
        self.fillCoreVariables(self.tree, iEvent, event, isMC)

        for v in self.globalVariables:
            v.fillBranch(self.tree, event, isMC)

        for on, o in self.globalObjects.iteritems(): 
            o.fillBranches(self.tree, getattr(event, on), isMC)

        for cn, c in self.collections.iteritems():
            if self.scalar:
                c.fillBranchesScalar(self.tree, getattr(event, cn), isMC)
            else:
                c.fillBranchesVector(self.tree, getattr(event, cn), isMC)

        self.tree.tree.Fill()      

