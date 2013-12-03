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

        ## Declare how we store floats by default
        self.tree.setDefaultFloatType("F"); # otherwise it's "D"

        ## Declare what we want to fill
        self.globalVariables = [ 
            NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"),
            NTupleVariable("nJet25", lambda ev: len(ev.cleanJets), int, help="Number of jets with pt > 25"),
            NTupleVariable("nBJetLoose25", lambda ev: len(ev.bjetsLoose), int, help="Number of jets with pt > 25 passing CSV loose"),
            NTupleVariable("nBJetMedium25", lambda ev: len(ev.bjetsMedium), int, help="Number of jets with pt > 25 passing CVS medium"),
            NTupleVariable("nBJetTight25", lambda ev: sum([j.getSelection("cuts_csv_tight") for j in ev.bjetsMedium]), int, help="Number of jets with pt > 25 passing CVS tight"),
            NTupleVariable("htJet25", lambda ev : ev.htJet25, help="H_{T} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("mhtJet25", lambda ev : ev.mhtJet25, help="H_{T}^{miss} computed from leptons and jets (with |eta|<2.4, pt > 25 GeV)"),
            NTupleVariable("mZ1", lambda ev : ev.bestZ1[0], help="Best m(ll) SF/OS"),
            NTupleVariable("mZ2", lambda ev : ev.bestZ2[3], help="m(ll) of second SF/OS pair, for ZZ reco."),
            NTupleVariable("mZ1SFSS", lambda ev : ev.bestZ1sfss[0], help="Best m(ll) SF/SS"),
            NTupleVariable("minMllSFOS", lambda ev: ev.minMllSFOS, help="min m(ll), SF/OS"),
            NTupleVariable("maxMllSFOS", lambda ev: ev.maxMllSFOS, help="max m(ll), SF/OS"),
            NTupleVariable("minMllAFOS", lambda ev: ev.minMllAFOS, help="min m(ll), AF/OS"),
            NTupleVariable("maxMllAFOS", lambda ev: ev.maxMllAFOS, help="max m(ll), AF/OS"),
            NTupleVariable("minMllAFSS", lambda ev: ev.minMllAFSS, help="min m(ll), AF/SS"),
            NTupleVariable("maxMllAFSS", lambda ev: ev.maxMllAFSS, help="max m(ll), AF/SS"),
            NTupleVariable("minMllAFAS", lambda ev: ev.minMllAFAS, help="min m(ll), AF/AS"),
            NTupleVariable("maxMllAFAS", lambda ev: ev.maxMllAFAS, help="max m(ll), AF/AS"),
            NTupleVariable("m2l", lambda ev: ev.m2l, help="m(ll)"),
            NTupleVariable("m3l", lambda ev: ev.m3l, help="m(3l)"),
            NTupleVariable("m4l", lambda ev: ev.m4l, help="m(4l)"),
            NTupleVariable("pt2l", lambda ev: ev.pt2l, help="p_{T}(ll)"),
            NTupleVariable("pt3l", lambda ev: ev.pt3l, help="p_{T}(3l)"),
            NTupleVariable("pt4l", lambda ev: ev.pt4l, help="p_{T}(4l)"),
            NTupleVariable("ht3l", lambda ev: ev.ht3l, help="H_{T}(3l)"),
            NTupleVariable("ht4l", lambda ev: ev.ht4l, help="H_{T}(4l)"),
            NTupleVariable("q3l", lambda ev: ev.q3l, int, help="q(3l)"),
            NTupleVariable("q4l", lambda ev: ev.q4l, int, help="q(4l)"),
            ##--------------------------------------------------
            NTupleVariable("GenHiggsDecayMode", lambda ev : ev.genHiggsDecayMode, int, mcOnly=True, help="H decay mode (15 = tau, 23/24 = W/Z)"),
            NTupleVariable("GenHeaviestQCDFlavour", lambda ev : ev.heaviestQCDFlavour, int, mcOnly=True, help="pdgId of heaviest parton in the event (after shower)"),
        ]
        self.globalObjects = {
            "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after type 1 corrections"),
        }
        self.collections = {
            "selectedLeptons" : NTupleCollection("LepGood", leptonTypeTTH, 8, help="Leptons after the preselection"),
            "selectedTaus"    : NTupleCollection("TauGood", tauTypeTTH, 3, help="Taus after the preselection"),
            "cleanJets"       : NTupleCollection("Jet",     jetTypeTTH, 8, sortDescendingBy = lambda jet : j.btag('combinedSecondaryVertexBJetTags'), help="Cental jets after full selection and cleaning, sorted by b-tag"),
            ##--------------------------------------------------
            "gentopquarks"    : NTupleCollection("GenTop",     genParticleType, 2, help="Generated top quarks from hard scattering"),
            "genbquarks"      : NTupleCollection("GenBQuark",  genParticleType, 2, help="Generated bottom quarks from top quark decays"),
            "genwzquarks"     : NTupleCollection("GenQuark",   genParticleWithSourceType, 6, help="Generated quarks from W/Z decays"),
            "genleps"         : NTupleCollection("GenLep",     genParticleWithSourceType, 6, help="Generated leptons from W/Z decays"),
            "gentauleps"      : NTupleCollection("GenLepFromTau", genParticleWithSourceType, 6, help="Generated leptons from decays of taus from W/Z/h decays"),
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
            if self.scalar:
                c.makeBranchesScalar(tree, isMC)
            else:
                c.makeBranchesVector(tree, isMC)
            
    def fillCoreVariables(self, tr, iEvent, event, isMC):
        """Here we fill the variables that we always want and that are hard-coded"""
        tr.fill('run', event.run) 
        tr.fill('lumi',event.lumi)
        tr.fill('evt', event.eventId)    

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

