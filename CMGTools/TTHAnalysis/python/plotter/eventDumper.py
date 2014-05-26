#!/usr/bin/env python
from math import *
import re, string
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.TTHAnalysis.plotter.tree2yield import CutsFile, scalarToVector

from optparse import OptionParser
import json

parser = OptionParser(usage="usage: %prog [options] rootfile [what] \nrun with --help to get list of options")
parser.add_option("-r", "--run-range",  dest="runrange", default=(0,99999999), type="float", nargs=2, help="Run range")
parser.add_option("-c", "--cut-file",  dest="cutfile", default=None, type="string", help="Cut file to apply")
parser.add_option("-C", "--cut",  dest="cut", default=None, type="string", help="Cut to apply")
parser.add_option("-T", "--type",  dest="type", default=None, type="string", help="Type of events to select")
parser.add_option("-F", "--fudge",   dest="fudge",  default=False, action="store_true",  help="print -999 for missing variables")
parser.add_option("-m", "--mc",     dest="ismc",  default=False, action="store_true",  help="print MC match info")
parser.add_option("--mm", "--more-mc",     dest="moremc",  default=False, action="store_true",  help="print more MC match info")
parser.add_option("--tau", dest="tau",  default=False, action="store_true",  help="print Taus")
parser.add_option("-j", "--json",   dest="json",  default=None, type="string", help="JSON file to apply")
parser.add_option("-n", "--maxEvents",  dest="maxEvents", default=-1, type="int", help="Max events")
parser.add_option("-f", "--format",   dest="fmt",  default=None, type="string",  help="Print this format string")
parser.add_option("-t", "--tree",          dest="tree", default='ttHLepTreeProducerTTH', help="Pattern for tree name");

### CUT-file options
parser.add_option("-S", "--start-at-cut",   dest="startCut",   type="string", help="Run selection starting at the cut matched by this regexp, included.") 
parser.add_option("-U", "--up-to-cut",      dest="upToCut",   type="string", help="Run selection only up to the cut matched by this regexp, included.") 
parser.add_option("-X", "--exclude-cut", dest="cutsToExclude", action="append", default=[], help="Cuts to exclude (regexp matching cut name), can specify multiple times.") 
parser.add_option("-I", "--invert-cut",  dest="cutsToInvert",  action="append", default=[], help="Cuts to invert (regexp matching cut name), can specify multiple times.") 
parser.add_option("-R", "--replace-cut", dest="cutsToReplace", action="append", default=[], nargs=3, help="Cuts to invert (regexp of old cut name, new name, new cut); can specify multiple times.") 
parser.add_option("-A", "--add-cut",     dest="cutsToAdd",     action="append", default=[], nargs=3, help="Cuts to insert (regexp of cut name after which this cut should go, new name, new cut); can specify multiple times.") 
parser.add_option("--s2v", "--scalar2vector",     dest="doS2V",    action="store_true", default=False, help="Do scalar to vector conversion") 
 
(options, args) = parser.parse_args()
what = args[1] if len(args) > 1 else "signal"
if what not in [ "signal", "CRss", "CRos" ]: raise RuntimeError, "Unknown what"

if options.cut and options.cutfile: raise RuntimeError, "You can't specify both a cut and a cutfile"

jsonmap = {}
if options.json:
    J = json.load(open(options.json, 'r'))
    for r,l in J.iteritems():
        jsonmap[long(r)] = l
    stderr.write("Loaded JSON %s with %d runs\n" % (options.json, len(jsonmap)))

def testJson(ev):
    try:
        lumilist = jsonmap[ev.run]
        for (start,end) in lumilist:
            if start <= ev.lumi and ev.lumi <= end:
                return True
        return False
    except KeyError:
        return False

class BaseDumper(Module):
    def __init__(self,name,options=None,booker=None):
        Module.__init__(self,name,booker)
        self.options = options
        self.lepMVAL = 0.35
        self.lepMVAT = 0.70
    def preselect(self,ev):
        return True
    def makeVars(self,ev):
        leps = Collection(ev,"LepGood","nLepGood") 
        self.lepsAny = [leps[i] for i in xrange(min(8,ev.nLepGood))]
        self.lepsMVAL = []
        self.lepsMVAT = []
        for l in self.lepsAny: 
            if l.mva < self.lepMVAL: break
            self.lepsMVAL.append(l)
        for l in self.lepsAny: 
            if l.mva < self.lepMVAT: break
            self.lepsMVAT.append(l)
        jets = Collection(ev,"Jet")
        self.jetsPtSorted = [ jets[i] for i in xrange(min(8,ev.nJet25)) ]
        self.jetsPtSorted.sort(key = lambda j : -j.pt)
        #jetsBad = Collection(ev,"JetFailId")
        #self.jetsBadPtSorted = [ jetsBad[i] for i in xrange(len(jetsBad)) if jetsBad[i].pt > 0 ]
        #self.jetsBadPtSorted.sort(key = lambda j : -j.pt)
    def analyze(self,ev):
        if self.options.tree in ["ttHLepTreeProducerBase", "ttHLepTreeProducerTTH"]: self.makeVars(ev)
        if self.options.fmt: 
            print string.Formatter().vformat(options.fmt.replace("\\t","\t"),[],ev)
            return True
        print "run %6d lumi %4d event %11d : leptons %d (MVA loose %d, MVA tight %d), jets %d (CSV loose %d, CSV medium %d)" % (
                ev.run, ev.lumi, ev.evt, ev.nLepGood, len(self.lepsMVAL), len(self.lepsMVAT), ev.nJet25, ev.nBJetLoose25, ev.nBJetMedium25)
        for i,l in enumerate(self.lepsAny):
            print "    lepton %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f    MVA %+4.2f    relIso %5.3f sip3d %5.2f dxy %+4.3f dz %+4.3f   jetPtR %4.2f jetDR %4.3f jetBTag %4.3f" % (
                    i+1, l.pdgId,l.pt,l.eta,l.phi, l.mva, l.relIso, l.sip3d, 10*l.dxy, 10*l.dz, l.jetPtRatio, l.jetDR, min(1.,max(0.,l.jetBTagCSV))),
            if self.options.ismc:
                print "   mcMatch %+3d" % (l.mcMatchId if l.mcMatchId > 0 else -l.mcMatchAny),
                if self.options.moremc:
                    print "   mcDRB %4.2f" % (l.mcDeltaRB if l.mcMatchAny == 2 and l.mcDeltaRB < 99 else 9.99),
            if abs(l.pdgId) == 11:
                print "   tightId %d mvaId %5.3f misHit %d conVeto %d tightCh %d mvaIdTrig %5.3f" % (l.tightId, l.mvaId, l.lostHits, l.convVeto, l.tightCharge, l.mvaIdTrig) #, l.relIso03/(l.pfpt if l.pfpt else l.pt), l.pfpt)
            else:
                print "   tightId %d lostHits %2d tightCh %d" % (l.tightId, l.lostHits, l.tightCharge)
        if self.options.tau: 
            taus =  [g for g in Collection(ev,"Tau") if g.pt > 0 ]
            for i,l in enumerate(taus):
                print "    tau    %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f    rawIMVA2 %+.3f  CI3hit WP %d IMVA2 WP %d  dxy %+4.3f dz %+4.3f " % (
                        i+1, l.pdgId,l.pt,l.eta,l.phi, l.isoMVA2, l.idCI3hit, l.idMVA2, 10*l.dxy, 10*l.dz),
                if self.options.ismc:
                    print "   mcMatch %+3d" % l.mcMatchId
                else:
                    print ""
        for i,j in enumerate(self.jetsPtSorted):
            if self.options.ismc:
                print "    jet %d:  pt %5.1f uncorrected pt %5.1f eta %+4.2f phi %+4.2f  btag %4.3f mcMatch %2d mcFlavour %d mcPt %5.1f" % (i, j.pt, j.rawPt, j.eta, j.phi, min(1.,max(0.,j.btagCSV)), j.mcMatchId, j.mcFlavour, j.mcPt)
            else:
                print "    jet %d:  pt %5.1f uncorrected pt %5.1f eta %+4.2f phi %+4.2f  btag %4.3f" % (i+1, j.pt, j.rawPt, j.eta, j.phi, min(1.,max(0.,j.btagCSV)))
        #for i,j in enumerate(self.jetsBadPtSorted):
        #    if self.options.ismc:
        #        print "    bad jet %d:  pt %5.1f uncorrected pt %5.1f eta %+4.2f phi %+4.2f  btag %4.3f mcMatch %2d mcFlavour %d/%d mcPt %5.1f  jetId %1d puId %1d" % (i, j.pt, j.rawPt, j.eta, j.phi, min(1.,max(0.,j.btagCSV)), j.mcMatchId, j.mcMatchFlav,j.mcFlavour, j.mcPt, j.looseJetId, j.puJetId)
        #    else:
        #        print "    bad jet %d:  pt %5.1f uncorrected pt %5.1f eta %+4.2f phi %+4.2f  btag %4.3f  jetId %1d puId %1d" % (i+1, j.pt, j.rawPt, j.eta, j.phi, min(1.,max(0.,j.btagCSV)), j.looseJetId, j.puJetId)
        
 
        print "    met %6.2f (phi %+4.2f)     mht %6.2f" % (ev.met_pt, ev.met_phi, ev.mhtJet25)
        if self.options.ismc:
            print "    vertices %d    pu weight %5.2f" % (ev.nVert, ev.puWeight)
        else:
            print "    vertices %d" % (ev.nVert)
        if self.options.moremc:
            gleps = [g for g in Collection(ev,"GenLep") if g.pt > 0 ]
            for i,l in enumerate(gleps):
                print "    gen lep %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f sourceId %2d" % (i+1, l.pdgId,l.pt,l.eta,l.phi, l.sourceId)
            gtaus = [g for g in Collection(ev,"GenLepFromTau","nGenLepsFromTau") if g.pt > 0 ]
            for i,l in enumerate(gtaus):
                print "    gen lep %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f sourceId %2d " % (i+1, l.pdgId,l.pt,l.eta,l.phi, l.sourceId)
            gbq = Collection(ev,"GenBQuark","nGenBQuarks",2)
            for i,l in enumerate([gbq[i] for i in 0,1]):
                print "    gen bq  %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f " % (i+1, l.pdgId,l.pt,l.eta,l.phi)
            glq = Collection(ev,"GenQuark","nGenQuarks",6)
            for i,l in enumerate([q for q in glq if q.pt > 0]):
                print "    gen lq  %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f sourceId %2d " % (i+1, l.pdgId,l.pt,l.eta,l.phi, l.sourceId)
            for i,l in enumerate(self.lepsAny):
                nq = None; nj = None
                for q in [q for q in gbq if q.pt > 0] + [q for q in glq if q.pt > 0]:
                    dr = deltaR(q,l)
                    if nq == None or dr < deltaR(nq,l): nq = q
                for q in self.jetsPtSorted:
                    dr = deltaR(q,l)
                    if nj == None or dr < deltaR(nj,l): nj = q
                print "    lepton %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f    MVA %+4.2f     mcMatch %+3d " % (
                        i+1, l.pdgId,l.pt,l.eta,l.phi, l.mva,  (l.mcMatchId if l.mcMatchId > 0 else -l.mcMatchAny),  )
                if nq != None:
                    print "      nearest quark: id %+2d pt %5.1f eta %+4.2f phi %+4.2f   (deltaR %6.3f)" % (nq.pdgId,nq.pt,nq.eta,nq.phi, deltaR(nq,l))
                if nj != None:
                    print "      nearest jet: pt %5.1f eta %+4.2f phi %+4.2f btag %4.3f mcMatch %2d mcFlavour %d/%d   (deltaR %6.3f)" % (nj.pt,nj.eta,nj.phi, min(1.,max(0.,nj.btagCSV)), nj.mcMatchId, nj.mcMatchFlav, nj.mcFlavour, deltaR(nj,l))
        print ""
        print ""
        print ""


cut = None
if options.cutfile:
    cut = CutsFile(options.cutfile,options).allCuts()
elif options.cut:
    cut = options.cut
if options.doS2V:
    cut = scalarToVector(cut)
 
file = ROOT.TFile.Open(args[0])
treename = options.tree
tree = file.Get(treename)
tree.vectorTree = (options.tree == 'ttHLepTreeProducerTTH')
el = EventLoop([BaseDumper("dump", options)])
el.loop(tree,options.maxEvents,cut=cut)

