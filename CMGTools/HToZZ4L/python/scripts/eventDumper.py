#!/usr/bin/env python
from math import *
import re, string
from CMGTools.TTHAnalysis.treeReAnalyzer import *

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
parser.add_option("-t", "--tree",          dest="tree", default='tree', help="Pattern for tree name");

(options, args) = parser.parse_args(); sys.argv = []

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
    def preselect(self,ev):
        return True
    def makeVars(self,ev):
        jets = Collection(ev,"Jet")
        ev.Jet1_pt_zs = jets[0].pt if len(jets) > 0 else -1.0
        ev.Jet2_pt_zs = jets[1].pt if len(jets) > 1 else -1.0
    def analyze(self,ev):
        try:
            self.makeVars(ev)
        except:
            pass
        if self.options.fmt: 
            print string.Formatter().vformat(options.fmt.replace("\\t","\t"),[],ev)
            return True
        leps = Collection(ev,"Lep","nLep") 
        jets = Collection(ev,"Jet")
        print "run %6d lumi %4d event %11d " % (ev.run, ev.lumi, ev.evt)
        for i,l in enumerate(leps):
            print "    lepton %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f   tightId %d relIso %5.3f sip3d %5.2f dxy %+4.3f dz %+4.3f " % (
                    i+1, l.pdgId,l.pt,l.eta,l.phi, l.tightId, l.relIso04, l.sip3d, l.dxy, l.dz),
            if self.options.ismc:
                print "   mcMatch id %+3d, any %+2d" % (l.mcMatchId, l.mcMatchAny),
            print ""
        for i,j in enumerate(jets):
            if self.options.ismc:
                print "    jet %d:  pt %5.1f uncorrected pt %5.1f eta %+4.2f phi %+4.2f  btag %4.3f mcMatch %2d mcFlavour %2d mcPt %5.1f" % (i, j.pt, j.rawPt, j.eta, j.phi, min(1.,max(0.,j.btagCSV)), j.mcMatchId, j.mcFlavour, j.mcPt)
            else:
                print "    jet %d:  pt %5.1f uncorrected pt %5.1f eta %+4.2f phi %+4.2f  btag %4.3f" % (i+1, j.pt, j.rawPt, j.eta, j.phi, min(1.,max(0.,j.btagCSV)))
 
        print "    met %6.2f (phi %+4.2f)" % (ev.met_pt, ev.met_phi)
        if self.options.ismc:
            print "    vertices %d" % (ev.nVert)

cut = None
if options.cut:
    cut = options.cut
 
file = ROOT.TFile.Open(args[0])
tree = file.Get(options.tree)
tree.vectorTree = False
dumper = BaseDumper("dump", options)
el = EventLoop([dumper])
el.loop(tree,options.maxEvents,cut=cut)

