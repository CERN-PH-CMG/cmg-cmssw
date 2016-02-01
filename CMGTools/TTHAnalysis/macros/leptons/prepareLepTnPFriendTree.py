#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import sys, os
from math import ceil

class LepTreeProducer(Module):
    def __init__(self,name,booker,
            evSel = lambda event : True, tagSel = lambda tag: True, probeSel = lambda probe: True, pairSel = lambda tag,probe,mass: True,
            probeVars = [ "pt", "eta", "phi", "charge", "abseta",
                          "dxy", "dz", "ip3d", "sip3d", 
                          "jetPtRelv2", "jetPtRatiov2",
                          "miniRelIso", "relIso03", "relIsoAn04",
                          "mvaTTH", 
                          "tightId", "convVeto", "lostHits", "tightCharge", "mediumMuonId", "mvaIdPhys14", "mvaIdSpring15",
                          "mcMatchId"],
            tagVars = [ "pt", "eta", "relIso03", "mcMatchId" ] ):
        Module.__init__(self,name,booker)
        print "Booked ",name
        self.evSel = evSel
        self.probeSel = probeSel
        self.tagSel = tagSel
        self.pairSel = pairSel
        def makeFunc(x):
            if x == "abseta": return lambda l : abs(l.eta)
            return eval("lambda l : getattr(l, '%s', -999.)" % x)
        self.probeVars = [ (n, makeFunc(n)) for n in probeVars ]
        self.tagVars = [ (n, makeFunc(n)) for n in tagVars ]
    def beginJob(self):
        self.t = PyTree(self.book("TTree","fitter_tree","fitter_tree"))
        self.t.branch("mass","F")
        for V,F in self.probeVars: 
            self.t.branch(V,"F")
        for V,F in self.tagVars: 
            self.t.branch("tag_"+V,"F")
        self.t.branch("pair_probeMultiplicity","F")
        self.t.branch("nVert","F")
        self.t.branch("run","F")
    def analyze(self,event):
        if event.nLepGood < 2: return True
        if not self.evSel(event): return True
        lep = Collection(event,"LepGood","nLepGood")
        tags = [ m for m in lep if self.tagSel(m) ]
        probes = [ m for m in lep if self.probeSel(m) ]
        pairs = [ (t,p) for t in tags for p in probes ]
        self.t.run = event.run
        self.t.nVert = event.nVert
        for tag in tags:
            pairs = []
            for probe in probes:
                if tag == probe: continue
                mass = (tag.p4() + probe.p4()).M()
                if not self.pairSel(tag,probe,mass): continue
                pairs.append((tag,probe,mass))
            self.t.pair_probeMultiplicity = len(pairs)
            for tag,probe,mass in pairs:
                for V,F in self.tagVars: 
                    setattr(self.t, 'tag_'+V, F(tag))
                for V,F in self.probeVars: 
                    setattr(self.t, V, F(probe))
                self.t.mass = mass
                self.t.fill()
                
def run(myArgs):
    (infile,outfile,range) = myArgs
    f = ROOT.TFile(infile)
    t = f.Get("tree")
    t.vectorTree = True
    booker = Booker(outfile)
    muTnP = LepTreeProducer("tpTreeMu",booker,
                evSel = lambda ev : ev.HLT_SingleMu,
                tagSel = lambda mu : mu.pt > 25 and abs(mu.eta) < 2.1 and mu.relIso03 < 0.2,
                pairSel = lambda tag, probe, mass : tag.pdgId + probe.pdgId == 0 and mass > 60 and mass < 120)
    elTnP = LepTreeProducer("tpTreeEl",booker,
                evSel = lambda ev : ev.HLT_SingleMu,
                tagSel = lambda mu : mu.pt > 30 and abs(mu.eta) < 2.0 and mu.relIso03 < 0.2,
                pairSel = lambda tag, probe, mass : tag.pdgId + probe.pdgId == 0 and mass > 60 and mass < 120)
    el = EventLoop([ muTnP, elTnP ])
    el.loop([t], eventRange=range)
    booker.done()
    print "Done",outfile
    return True

if __name__ == '__main__':
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] inputTree.root outputTree.root")
    parser.add_option("-c", "--chunk",   dest="chunk",     type="int",    default=-1, help="Process only this chunk");
    parser.add_option("-N", "--events",  dest="chunkSize", type="int",    default=250000, help="Default chunk size when splitting trees");
    parser.add_option("-M", "--maxEntries",  dest="maxEntries", type="int",    default=-1, help="Max entries");
    parser.add_option("-j", "--jobs",    dest="jobs",      type="int",    default=0, help="Use N threads");
    parser.add_option("-p", "--pretend", dest="pretend",   action="store_true", default=False, help="Don't run anything");
    parser.add_option("-q", "--queue",   dest="queue",     type="string", default=None, help="Run jobs on lxbatch instead of locally");
    (options, args) = parser.parse_args()
    if len(args) != 2: 
        print "python prepareLepTnPFriendTree.py inputTree.root outputTree.root "
        exit()
    if options.chunk != -1:
        ic = options.chunk
        start = ic*options.chunkSize
        end   = min(options.maxEntries, (ic+1)*options.chunkSize)
        print "Running %s (entries %d-%d)" % (args[1], start, end)
        run((args[0], args[1], xrange(start,end)))
        exit()
    else:
        f = ROOT.TFile.Open(args[0])
        t = f.Get("tree")
        entries = t.GetEntries()
        f.Close()
        if options.maxEntries != -1: entries = min(entries,options.maxEntries)

        tasks = []
        chunks = int(ceil(entries/float(options.chunkSize)))
        for ic in xrange(chunks):
            start = ic*options.chunkSize
            end   = min(entries, (ic+1)*options.chunkSize)
            oname = "%s.chunk%d.root" % (args[1].replace(".root",""), ic)
            print "  Chunk %s (%d-%d)" % (oname,start,end)
            if options.pretend: continue
            if options.queue:
                print "bsub -q {queue} {dir}/lxbatch_runner.sh {dir} {cmssw} python {self} -N {chunkSize} -M {max} -c {ic} {input} {output}".format(
                    queue = options.queue, dir = os.getcwd(), cmssw = os.environ['CMSSW_BASE'], 
                    self=sys.argv[0], chunkSize=options.chunkSize, max=entries,  ic=ic, input=args[0], output=oname)
                continue
            else:
                tasks.append((args[0],oname,xrange(start,end)))
        if options.pretend: exit()
        if options.jobs > 0:
            print "Running in parallel using %d jobs" % options.jobs
            from multiprocessing import Pool
            Pool(options.jobs).map(run, tasks)
        else:
            print "Running sequentially"
            map(run, tasks)

