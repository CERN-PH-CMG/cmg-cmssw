#!/usr/bin/env python
from math import *
import re
from CMGTools.TTHAnalysis.treeReAnalyzer import *

from optparse import OptionParser
import json

parser = OptionParser(usage="usage: %prog [options] rootfile [what] \nrun with --help to get list of options")
parser.add_option("-r", "--run-range",  dest="runrange", default=(0,99999999), type="float", nargs=2, help="Run range")
parser.add_option("-c", "--cut",  dest="cut", default=None, type="string", help="Cut file to apply")
parser.add_option("-T", "--type",  dest="type", default=None, type="string", help="Type of events to select")
parser.add_option("-F", "--fudge",   dest="fudge",  default=False, action="store_true",  help="print -999 for missing variables")
parser.add_option("-m", "--mc",     dest="ismc",  default=False, action="store_true",  help="print MC match info")
parser.add_option("-j", "--json",   dest="json",  default=None, type="string", help="JSON file to apply")
parser.add_option("-n", "--maxEvents",  dest="maxEvents", default=-1, type="int", help="Max events")

(options, args) = parser.parse_args()
what = args[1] if len(args) > 1 else "signal"
if what not in [ "signal", "CRss", "CRos" ]: raise RuntimeError, "Unknown what"

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
        self.lepsAny = [leps[i] for i in xrange(ev.nLepGood)]
        self.lepsMVAL = []
        self.lepsMVAT = []
        for l in self.lepsAny: 
            if l.mva < self.lepMVAL: break
            self.lepsMVAL.append(l)
        for l in self.lepsAny: 
            if l.mva < self.lepMVAT: break
            self.lepsMVAT.append(l)
        jets = Collection(ev,"Jet")
        self.jetsPtSorted = [ jets[i] for i in xrange(ev.nJet25) ]
        self.jetsPtSorted.sort(key = lambda j : -j.pt)
    def analyze(self,ev):
        self.makeVars(ev)
        print "run %6d lumi %4d event %11d : leptons %d (MVA loose %d, MVA tight %d), jets %d (CSV loose %d, CSV medium %d)" % (
                ev.run, ev.lumi, ev.evt, ev.nLepGood, len(self.lepsMVAL), len(self.lepsMVAT), ev.nJet25, ev.nBJetLoose25, ev.nBJetMedium25)
        for i,l in enumerate(self.lepsAny):
            print "    lepton %d: id %+2d pt %5.1f eta %+4.2f phi %+4.2f    MVA %+4.2f    relIso %5.3f sip3d %5.2f dxy %+4.3f dz %+4.3f   jetPtR %4.2f jetDR %4.3f jetBTag %4.3f" % (
                    i+1, l.pdgId,l.pt,l.eta,l.phi, l.mva, l.relIso, l.sip3d, 10*l.dxy, 10*l.dz, l.jetPtRatio, l.jetDR, min(1.,max(0.,l.jetBTagCSV))),
            if self.options.ismc:
                print "   mcMatch %+3d" % (l.mcMatchId if l.mcMatchId > 0 else -l.mcMatchAny),
            if abs(l.pdgId) == 11:
                print "   mvaId %5.3f misHit %d tightCh %d mvaIdTrig %5.3f" % (l.mvaId, l.innerHits, l.tightCharge, l.tightId)
            else:
                print "   tightId %d tkHit %2d tightCh %d" % (l.tightId, l.innerHits, l.tightCharge)
        for i,j in enumerate(self.jetsPtSorted):
            if self.options.ismc:
                print "    jet %d:  pt %5.1f eta %+4.2f phi %+4.2f  btag %4.3f mcMatch %2d mcFlavour %d/%d" % (i, j.pt, j.eta, j.phi, min(1.,max(0.,j.btagCSV)), j.mcMatchId, j.mcMatchFlav,j.mcFlavour)
            else:
                print "    jet %d:  pt %5.1f eta %+4.2f phi %+4.2f  btag %4.3f" % (i+1, j.pt, j.eta, j.phi, min(1.,max(0.,j.btagCSV)))
        
        print "    met %6.2f (phi %+4.2f)     mht %6.2f    met nopu  %6.2f (phi %+4.2f)" % (ev.met, ev.met_phi, ev.mhtJet25, ev.metNoPU, ev.metNoPU_phi)
        if self.options.ismc:
            print "    vertices %d    pu weight %5.2f" % (ev.nVert, ev.puWeight)
        else:
            print "    vertices %d" % (ev.nVert)
        print ""
        print ""
        print ""

        
file = ROOT.TFile.Open(args[0])
treename = "ttHLepTreeProducerBase"
tree = file.Get(treename)
el = EventLoop([BaseDumper("dump", options)])
el.loop(tree,options.maxEvents)

