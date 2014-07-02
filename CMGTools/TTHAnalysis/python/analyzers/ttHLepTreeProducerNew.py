from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.TTHAnalysis.analyzers.ntupleObjects import *
from CMGTools.TTHAnalysis.analyzers.ntupleTypes   import *
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.fwlite.Event import Event
from ROOT import TriggerBitChecker

class ttHLepTreeProducerNew( TreeAnalyzerNumpy ):

    #-----------------------------------
    # TREE PRODUCER FOR THE TTH ANALYSIS
    #-----------------------------------
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(ttHLepTreeProducerNew,self).__init__(cfg_ana, cfg_comp, looperName)

        ## Read whether we want vectors or flat trees
        self.scalar = not self.cfg_ana.vectorTree

        ## Read whether we want 4-vectors 
        if not getattr(self.cfg_ana, 'saveTLorentzVectors', False):
            fourVectorType.removeVariable("p4")

        ## Declare how we store floats by default
        self.tree.setDefaultFloatType("F"); # otherwise it's "D"
        

    def declareHandles(self):
        super(ttHLepTreeProducerNew, self).declareHandles()
        self.handles['TriggerResults'] = AutoHandle( ('TriggerResults','','HLT'), 'edm::TriggerResults' )
        for k,v in self.collections.iteritems():
            if type(v) == tuple and isinstance(v[0], AutoHandle):
                self.handles[k] = v[0]

    def declareCoreVariables(self, tr, isMC):
        """Here we declare the variables that we always want and that are hard-coded"""
            
        tr.var('run', int, storageType="i")
        tr.var('lumi', int, storageType="i")
        tr.var('evt', int, storageType="i")
        tr.var('isData', int)

        self.triggerBitCheckers = []
        if hasattr(self.cfg_ana, 'triggerBits'):
            for T, TL in self.cfg_ana.triggerBits.iteritems():
                trigVec = ROOT.vector(ROOT.string)()
                for TP in TL:
                    trigVec.push_back(TP)
                tr.var( 'HLT_'+T, int )
                self.triggerBitCheckers.append( (T, TriggerBitChecker(trigVec)) )
 
        if isMC:
            ## PU weights
            tr.var("puWeight")
            ## PDF weights
            self.pdfWeights = []
            if hasattr(self.cfg_ana, "PDFWeights") and len(self.cfg_ana.PDFWeights) > 0:
                self.pdfWeights = self.cfg_ana.PDFWeights
                for (pdf,nvals) in self.pdfWeights:
                    if self.scalar:
                        for i in range(nvals): tr.var('pdfWeight_%s_%d' % (pdf,i))
                    else:
                        tr.vector('pdfWeight_%s' % pdf, nvals)

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
            if type(c) == tuple: c = c[-1]
            if self.scalar:
                c.makeBranchesScalar(tree, isMC)
            else:
                c.makeBranchesVector(tree, isMC)
            
    def fillCoreVariables(self, tr, iEvent, event, isMC):
        """Here we fill the variables that we always want and that are hard-coded"""
        tr.fill('run', event.run) 
        tr.fill('lumi',event.lumi)
        tr.fill('evt', event.eventId)    
        tr.fill('isData', 0 if isMC else 1)

        triggerResults = self.handles['TriggerResults'].product()
        for T,TC in self.triggerBitCheckers:
            tr.fill("HLT_"+T, TC.check(iEvent.object(), triggerResults))

        if isMC:
            ## PU weights
            tr.fill("puWeight", event.eventWeight)
            ## PDF weights
            for (pdf,nvals) in self.pdfWeights:
                if len(event.pdfWeights[pdf]) != nvals:
                    raise RuntimeError, "PDF lenght mismatch for %s, declared %d but the event has %d" % (pdf,nvals,event.pdfWeights[pdf])
                if self.scalar:
                    for i,w in enumerate(event.pdfWeights[pdf]):
                        tr.fill('pdfWeight_%s_%d' % (pdf,i), w)
                else:
                    tr.vfill('pdfWeight_%s' % pdf, event.pdfWeights[pdf])

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.fillTree(iEvent, event)
         
    def fillTree(self, iEvent, event, resetFirst=True):
        isMC = self.cfg_comp.isMC 
        if resetFirst: self.tree.reset()

        self.fillCoreVariables(self.tree, iEvent, event, isMC)

        for v in self.globalVariables:
            if not isMC and v.mcOnly: continue
            v.fillBranch(self.tree, event, isMC)

        for on, o in self.globalObjects.iteritems(): 
            if not isMC and o.mcOnly: continue
            o.fillBranches(self.tree, getattr(event, on), isMC)

        for cn, c in self.collections.iteritems():
            if type(c) == tuple and isinstance(c[0], AutoHandle):
                if not isMC and c[-1].mcOnly: continue
                objects = self.handles[cn].product()
                setattr(event, cn, [objects[i] for i in xrange(objects.size())])
                c = c[-1]
            if not isMC and c.mcOnly: continue
            if self.scalar:
                c.fillBranchesScalar(self.tree, getattr(event, cn), isMC)
            else:
                c.fillBranchesVector(self.tree, getattr(event, cn), isMC)

        self.tree.tree.Fill()      

