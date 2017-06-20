#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from glob import glob
import os.path, re, pickle

MODULES = []

from CMGTools.WMass.tools.eventVars_wmass import EventVarsWmass
MODULES.append( ('vars_w', EventVarsWmass()) )

#from CMGTools.TTHAnalysis.tools.vertexWeightFriend import VertexWeightFriend
#pufile_mc="/afs/cern.ch/work/e/emanuele/public/wmass/pileup/pileup_profile_Spring16.root"
#pufile_data="/afs/cern.ch/work/e/emanuele/public/wmass/pileup/PileupData_GoldenJSON_Full2016.root"
#MODULES.append ( ('puWeights', VertexWeightFriend(pufile_mc,pufile_data,"pu_mc","pileup",name="puw",verbose=True,vtx_coll_to_reweight="nTrueInt") ) )

#from CMGTools.MonoXAnalysis.tools.eventVetoListChecker import EventVetoListChecker
#MODULES.append( ('eventVetoChecker', EventVetoListChecker(pathvetolists,vetoLists)) )

class VariableProducer(Module):
    def __init__(self,name,booker,sample_nevt,dataset,modules):
        Module.__init__(self,name,booker)
        self._modules = modules
        self._sample_nevt = sample_nevt
        self.dataset = dataset
    def beginJob(self):
        self.t = PyTree(self.book("TTree","t","t"))
        self.branches = {}
        for name,mod in self._modules:
            if name == "vars_w": mod.initSample(self._sample_nevt,self.dataset)
            for B in mod.listBranches():
                # don't add the same branch twice
                if B in self.branches: 
                    print "Will not add branch %s twice" % (B,)
                    continue
                self.branches[B] = True
                if type(B) == tuple:
                    if len(B) == 2:
                        self.t.branch(B[0],B[1])
                    elif len(B) == 4:
                        self.t.branch(B[0],B[1],n=B[2],lenVar=B[3])
                else:
                    self.t.branch(B ,"F")
    def analyze(self,event):
        for name,mod in self._modules:
            keyvals = mod(event)
            for B,V in keyvals.iteritems():
                setattr(self.t, B, V)
                setattr(event,  B, V)
        self.t.fill()

import os, itertools

from optparse import OptionParser
parser = OptionParser(usage="%prog [options] <TREE_DIR> <OUT>")
parser.add_option("-m", "--modules", dest="modules",  type="string", default=[], action="append", help="Run these modules");
parser.add_option("-d", "--dataset", dest="datasets",  type="string", default=[], action="append", help="Process only this dataset (or dataset if specified multiple times)");
parser.add_option("-D", "--dm", "--dataset-match", dest="datasetMatches",  type="string", default=[], action="append", help="Process only this dataset (or dataset if specified multiple times): REGEXP");
parser.add_option("-c", "--chunk",   dest="chunks",    type="int",    default=[], action="append", help="Process only these chunks (works only if a single dataset is selected with -d)");
parser.add_option("-N", "--events",  dest="chunkSize", type="int",    default=500000, help="Default chunk size when splitting trees");
parser.add_option("-j", "--jobs",    dest="jobs",      type="int",    default=1, help="Use N threads");
parser.add_option("-p", "--pretend", dest="pretend",   action="store_true", default=False, help="Don't run anything");
parser.add_option("-T", "--tree-dir",   dest="treeDir",     type="string", default="mjvars", help="Directory of the friend tree in the file (default: 'mjvars')");
parser.add_option("-q", "--queue",   dest="queue",     type="string", default=None, help="Run jobs on lxbatch instead of locally");
parser.add_option("-t", "--tree",    dest="tree",      default='treeProducerWMassEle', help="Pattern for tree name");
parser.add_option("-V", "--vector",  dest="vectorTree", action="store_true", default=True, help="Input tree is a vector");
parser.add_option("-F", "--add-friend",    dest="friendTrees",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename). Can use {name}, {cname} patterns in the treename") 
parser.add_option("--FMC", "--add-friend-mc",    dest="friendTreesMC",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename) to MC only. Can use {name}, {cname} patterns in the treename") 
parser.add_option("--FD", "--add-friend-data",    dest="friendTreesData",  action="append", default=[], nargs=2, help="Add a friend tree (treename, filename) to data trees only. Can use {name}, {cname} patterns in the treename") 
parser.add_option("-L", "--list-modules",  dest="listModules", action="store_true", default=False, help="just list the configured modules");
parser.add_option("-n", "--new",  dest="newOnly", action="store_true", default=False, help="Make only missing trees");
(options, args) = parser.parse_args()

if options.listModules:
    print "List of modules"
    for (n,x) in MODULES:
        print "   '%s': %s" % (n,x)
    exit()

if len(args) != 2 or not os.path.isdir(args[0]) or not os.path.isdir(args[1]): 
    print "Usage: program <TREE_DIR> <OUT>"
    exit()
if len(options.chunks) != 0 and len(options.datasets) != 1:
    print "must specify a single dataset with -d if using -c to select chunks"
    exit()

jobs = []
for D in glob(args[0]+"/*"):
    treename = options.tree
    fname    = "%s/%s/%s_tree.root" % (D,options.tree,options.tree)
    pckfile  = "%s/skimAnalyzerCount/SkimReport.pck" % (D)
    if (not os.path.exists(fname)) and os.path.exists("%s/%s/%s_tree.root" % (D,options.tree,options.tree)):
        treename = options.tree
        fname    = "%s/%s/%s_tree.root" % (D,options.tree,options.tree)

    if (not os.path.exists(fname)) and (os.path.exists("%s/%s/%s_tree.root.url" % (D,options.tree,options.tree)) ):
        treename = options.tree
        fname    = "%s/%s/%s_tree.root" % (D,options.tree,options.tree)
        fname    = open(fname+".url","r").readline().strip()

    if os.path.exists(fname) or (os.path.exists("%s/%s/%s_tree.root.url" % (D,options.tree,options.tree))):
        short = os.path.basename(D)
        if options.datasets != []:
            if short not in options.datasets: continue
        if options.datasetMatches != []:
            found = False
            for dm in  options.datasetMatches:
                if re.match(dm,short): found = True
            if not found: continue
        data = any(x in short for x in "DoubleMu DoubleEl DoubleEG MuEG MuonEG SingleMu SingleEl SinglePhoton MET".split())
        pckobj  = pickle.load(open(pckfile,'r'))
        counters = dict(pckobj)
        if ('Sum Weights' in counters):
            sample_nevt = counters['Sum Weights']
        else:
            sample_nevt = counters['All Events']
        f = ROOT.TFile.Open(fname);
        t = f.Get(treename)
        entries = t.GetEntries()
        f.Close()
        if options.newOnly:
            fout = "%s/evVarFriend_%s.root" % (args[1],short)
            if os.path.exists(fout):
                f = ROOT.TFile.Open(fname);
                t = f.Get(treename)
                if t.GetEntries() != entries:
                    print "Component %s has to be remade, mismatching number of entries (%d vs %d)" % (short, entries, t.GetEntries()) 
                    f.Close()
                else:
                    print "Component %s exists already and has matching number of entries (%d)" % (short, entries) 
                    continue 
        chunk = options.chunkSize
        if entries < chunk:
            print "  ",os.path.basename(D),("  DATA" if data else "  MC")," single chunk"
            jobs.append((short,fname,sample_nevt,"%s/evVarFriend_%s.root" % (args[1],short),data,xrange(entries),-1))
        else:
            nchunk = int(ceil(entries/float(chunk)))
            print "  ",os.path.basename(D),("  DATA" if data else "  MC")," %d chunks" % nchunk
            for i in xrange(nchunk):
                if options.chunks != []:
                    if i not in options.chunks: continue
                r = xrange(int(i*chunk),min(int((i+1)*chunk),entries))
                jobs.append((short,fname,sample_nevt,"%s/evVarFriend_%s.chunk%d.root" % (args[1],short,i),data,r,i))
print "\n"
print "I have %d taks to process" % len(jobs)

if options.queue:
    import os, sys
    basecmd = "bsub -q {queue} {dir}/lxbatch_runner.sh {dir} {cmssw} python {self} -N {chunkSize} -T '{tdir}' -t {tree} {data} {output}".format(
                queue = options.queue, dir = os.getcwd(), cmssw = os.environ['CMSSW_BASE'], 
                self=sys.argv[0], chunkSize=options.chunkSize, tdir=options.treeDir, tree=options.tree, data=args[0], output=args[1]
            )
    if options.vectorTree: basecmd += " --vector "
    friendPost =  "".join(["  -F  %s %s " % (fn,ft) for fn,ft in options.friendTrees])
    friendPost += "".join([" --FM %s %s " % (fn,ft) for fn,ft in options.friendTreesMC])
    friendPost += "".join([" --FD %s %s " % (fn,ft) for fn,ft in options.friendTreesData])
    friendPost += "".join(["  -m  '%s'  " % m for m in options.modules])
    for (name,fin,sample_nevt,fout,data,range,chunk) in jobs:
        if chunk != -1:
            print "{base} -d {data} -c {chunk} {post}".format(base=basecmd, data=name, chunk=chunk, post=friendPost)
        else:
            print "{base} -d {data} {post}".format(base=basecmd, data=name, chunk=chunk, post=friendPost)
        
    exit()

maintimer = ROOT.TStopwatch()
def _runIt(myargs):
    (name,fin,sample_nevt,fout,data,range,chunk) = myargs
    timer = ROOT.TStopwatch()

    fetchedfile = None
    if 'LSB_JOBID' in os.environ or 'LSF_JOBID' in os.environ:
        if fin.startswith("root://"):
            try:
                tmpdir = os.environ['TMPDIR'] if 'TMPDIR' in os.environ else "/tmp"
                tmpfile =  "%s/%s" % (tmpdir, os.path.basename(fin))
                print "xrdcp %s %s" % (fin, tmpfile)
                os.system("xrdcp %s %s" % (fin, tmpfile))
                if os.path.exists(tmpfile):
                    fin = tmpfile 
                    fetchedfile = fin
                    print "success :-)"
            except:
                pass
        fb = ROOT.TFile.Open(fin)
    elif "root://" in fin:        
        ROOT.gEnv.SetValue("TFile.AsyncReading", 1);
        ROOT.gEnv.SetValue("XNet.Debug", 0); # suppress output about opening connections
        ROOT.gEnv.SetValue("XrdClientDebug.kUSERDEBUG", 0); # suppress output about opening connections
        fb   = ROOT.TXNetFile(fin+"?readaheadsz=65535&DebugLevel=0")
        os.environ["XRD_DEBUGLEVEL"]="0"
        os.environ["XRD_DebugLevel"]="0"
        os.environ["DEBUGLEVEL"]="0"
        os.environ["DebugLevel"]="0"
    else:
        fb = ROOT.TFile.Open(fin)
        print fb

    print "getting tree.."
    tb = fb.Get(options.tree)

    if not tb: tb = fb.Get("tree") # new trees
    if options.vectorTree:
        tb.vectorTree = True
    else:
        tb.vectorTree = False
    friends = options.friendTrees[:]
    friends += (options.friendTreesData if data else options.friendTreesMC)
    friends_ = [] # to make sure pyroot does not delete them
    for tf_tree,tf_file in friends:
        tf = tb.AddFriend(tf_tree, tf_file.format(name=name, cname=name)),
        friends_.append(tf) # to make sure pyroot does not delete them
    nev = tb.GetEntries()
    if options.pretend:
        print "==== pretending to run %s (%d entries, %s) ====" % (name, nev, fout)
        return (name,(nev,0))
    print "==== %s starting (%d entries) ====" % (name, nev)
    booker = Booker(fout)
    modulesToRun = MODULES
    if options.modules != []:
        toRun = {}
        for m,v in MODULES:
            for pat in options.modules:
                if re.match(pat,m):
                    toRun[m] = True 
        modulesToRun = [ (m,v) for (m,v) in MODULES if m in toRun ]
    el = EventLoop([ VariableProducer(options.treeDir,booker,sample_nevt,name,modulesToRun), ])
    el.loop([tb], eventRange=range)
    booker.done()
    fb.Close()
    time = timer.RealTime()
    print "=== %s done (%d entries, %.0f s, %.0f e/s) ====" % ( name, nev, time,(nev/time) )
    return (name,(nev,time))

if options.jobs > 0:
    from multiprocessing import Pool
    pool = Pool(options.jobs)
    ret  = dict(pool.map(_runIt, jobs)) if options.jobs > 0 else dict([_runIt(j) for j in jobs])
else:
    ret = dict(map(_runIt, jobs))
fulltime = maintimer.RealTime()
totev   = sum([ev   for (ev,time) in ret.itervalues()])
tottime = sum([time for (ev,time) in ret.itervalues()])
print "Done %d tasks in %.1f min (%d entries, %.1f min)" % (len(jobs),fulltime/60.,totev,tottime/60.)
