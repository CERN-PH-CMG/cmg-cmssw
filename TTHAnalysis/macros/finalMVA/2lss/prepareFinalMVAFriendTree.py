#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from array import array
from glob import glob
import os.path

import os, ROOT
if "/smearer_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/smearer.cc+" % os.environ['CMSSW_BASE']);
if "/mcCorrections_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/mcCorrections.cc+" % os.environ['CMSSW_BASE']);


class MVAVar:
    def __init__(self,name,type='f'):
        self.name = name
        if ":=" in self.name:
            self.expr = self.name.split(":=")[1]
            if "LepGood1_charge+LepGood2_charge" in self.expr:
                self.expr = "2*LepGood1_charge"
            self.func = lambda ev : ev.eval(self.expr)
        else:
            self.func = lambda ev : ev[self.name]
        self.type = type
        self.var  = array('f',[0.]) #TMVA wants ints as floats! #array(type,[0 if type == 'i' else 0.])
    def set(self,lep): ## apply correction ncorr times
        self.var[0] = self.func(lep)
class MVATool:
    def __init__(self,name,xml,vars):
        self.name = name
        self.reader = ROOT.TMVA.Reader("Silent")
        self.vars  = vars
        for v in vars:  self.reader.AddVariable(v.name,v.var)
        #print "Would like to load %s from %s! " % (name,xml)
        self.reader.BookMVA(name,xml)
    def __call__(self,lep): ## apply correction ncorr times
        for s in self.vars:  s.set(lep)
        return self.reader.EvaluateMVA(self.name)   
class CategorizedMVA:
    def __init__(self,catMvaPairs):
        self.catMvaPairs = catMvaPairs
    def __call__(self,lep):
        for c,m in self.catMvaPairs:
            if c(lep): return m(lep)
        return -99.

_CommonVars = [ 
    MVAVar("htJet25 := min(htJet25,2000)"),
    MVAVar("min_Lep_eta := min(abs(LepGood1_eta),abs(LepGood2_eta))"),
    MVAVar("max_Lep_eta := max(abs(LepGood1_eta),abs(LepGood2_eta))"),
    MVAVar("wavg_eta := (abs(Jet1_eta)*Jet1_pt+abs(Jet2_eta)*Jet2_pt+abs(LepGood1_eta)*LepGood1_pt+abs(LepGood2_eta)*LepGood2_pt)/(Jet1_pt+Jet2_pt+LepGood1_pt+LepGood2_pt)"),
    MVAVar("minMllAFSS"),
    MVAVar("q2l := LepGood1_charge+LepGood2_charge"),
    MVAVar("Jet_pt1 := max(Jet1_pt,Jet2_pt)"),
    MVAVar("Jet_pt2 := min(Jet1_pt,Jet2_pt)"),
    MVAVar("Jet2_btagCSV"),
    MVAVar("nJet25"),
    MVAVar("minMWjj := min(minMWjj,200)"),
    MVAVar("minMWjjPt"),
    MVAVar("bestMWjj"),
    MVAVar("bestMTopHad"),
]
_MultiJetVars = [
]

class Debugger:
    def __init__(self): 
        self.n = 0;
        pass
    def __call__(self,event): 
        self.n += 1;
        if self.n < 50:
            print "untagged event with %d jets, leptons %d, %d" % (event.nJet25, event.LepGood1_pdgId, event.LepGood2_pdgId)
        return -90
class DileptonJetBin:
    def __init__(self,j,fsn,fsl):
        self.j = j;
        self.fsn = fsn
        self.fsl = fsl
    def __call__(self,event):
        return ((event.nJet25 >= self.j) and self.fsl(event))
class FinalMVA2LSS:
    def __init__(self,basepath,cat="LJ",algo="BDTG"):
        global _CommonVars, _MultiJetVars
        cats = []
        cats.append( ((lambda x: x.nJet25   < 2), (lambda x: -1)) ) ## error condition 1
        cats.append( ((lambda x: x.nLepGood < 2), (lambda x: -2)) ) ## error condition 2
        fslambdas = ( ('ee', lambda x: abs(x.LepGood1_pdgId) == 11 and abs(x.LepGood2_pdgId) == 11),
                      ('em', lambda x: abs(x.LepGood1_pdgId) == 13 and abs(x.LepGood2_pdgId) == 13),
                      ('mm', lambda x: abs(x.LepGood1_pdgId) != abs(x.LepGood2_pdgId)) )
        if cat == "LJ":
            for j in range(6,1,-1): ## decreasing, so I can always use >=
                for fsn,fsl in fslambdas:
                    vars = _CommonVars[:] # make a copy
                    if j > 2: vars += _MultiJetVars[:] 
                    cats.append( ( DileptonJetBin(j,fsn,fsl), MVATool(algo,basepath % ("%s_%djet" % (fsn,j)), vars) ) )
        elif cat == "L":
            for fsn,fsl in fslambdas:
                vars = _CommonVars[:] # make a copy
                cats.append( (fsl, MVATool(algo,basepath % fsn, vars)) )

        #for j in range(6,1,-1): ## decreasing, so I can always use >=
        #    cats.append( ((lambda x: x.nJet25 >= j), (lambda x: -j)) )
        cats.append( ((lambda x: True), Debugger()) )
        self.algo = algo
        self.mva = CategorizedMVA(cats)
    def __call__(self,event):
        return self.mva(event)

class FinalMVA2LSSTreeProducer(Module):
    def __init__(self,name, booker, path):
        Module.__init__(self,name,booker)
        #self.bdtg = FinalMVA2LSS(path+"/weights/2lss_mix_%s_BDTG.weights.xml", "BDTG")
        self.bdtg = FinalMVA2LSS(path+"/weights/2lss_mix_%s_BDTG.weights.xml", cat="L", algo="BDTG")
        #self.cld  = FinalMVA2LSS(path+"/weights/2lss_mix_%s_LD.weights.xml"  , cat="L", algo="LD")
    def beginJob(self):
        self.t = PyTree(self.book("TTree","t","t"))
        self.t.branch("FinalMVA_2LSS_BDTG","F")
        #self.t.branch("FinalMVA_2LSS_CLD" ,"F")
    def analyze(self,event):
        setattr(self.t, "FinalMVA_2LSS_BDTG", self.bdtg(event))
        #setattr(self.t, "FinalMVA_2LSS_CLD",  self.cld(event) )
        self.t.fill()


import os, itertools
from sys import argv
if len(argv) < 3: print "Usage: %s  [--fr]  [-e] <TREE_DIR> <TRAINING>" % argv[0]
jobs = []
base = "finalMVA_2LSS"
if argv[1] == "-e": # use existing training
    argv = [argv[0]] + argv[2:]
else: # new, make directory training
    if not os.path.exists(argv[2]):            os.mkdir(argv[2])
    if not os.path.exists(argv[2]+"/weights"): os.mkdir(argv[2]+"/weights")
    if not os.path.exists(argv[2]+"/trainings"): os.mkdir(argv[2]+"/trainings")
    for Y,X in itertools.product(range(6,1,-1),"mm em ee".split()):
        os.system("cp weights/2lss_mix_%s_%djet_BDTG.weights.xml %s/weights -v" % (X,Y,argv[2]))
        os.system("cp weights/2lss_mix_%s_%djet_LD.weights.xml %s/weights -v" % (X,Y,argv[2]))
        os.system("cp 2lss_mix_%s_%djet.root %s/trainings -v" % (X,Y,argv[2]))
    for X in "mm em ee".split():
        os.system("cp weights/2lss_mix_%s_BDTG.weights.xml %s/weights -v" % (X,argv[2]))
        os.system("cp weights/2lss_mix_%s_LD.weights.xml %s/weights -v" % (X,argv[2]))
        os.system("cp 2lss_mix_%s.root %s/trainings -v" % (X,argv[2]))
for D in glob(argv[1]+"/*"):
    fname = D+"/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root"
    if os.path.exists(fname):
        short = os.path.basename(D)
        f = ROOT.TFile.Open(fname);
        t = f.Get("ttHLepTreeProducerBase")
        entries = t.GetEntries()
        f.Close()
        chunk = 2000000.
        if entries < chunk:
            print "  ",os.path.basename(D)
            jobs.append((short,fname,"%s/%s_%s.root" % (argv[2],base,short),xrange(entries)))
        else:
            nchunk = int(ceil(entries/chunk))
            print "  ",os.path.basename(D)," %d chunks" % nchunk
            for i in xrange(nchunk):
                r = xrange(int(i*chunk),min(int((i+1)*chunk),entries))
                jobs.append((short,fname,"%s/%s_%s.chunk%d.root" % (argv[2],base,short,i),r))
print 4*"\n"
print "I have %d taks to process" % len(jobs)

maintimer = ROOT.TStopwatch()
def _runIt(args):
    (name,fin,fout,range) = args
    timer = ROOT.TStopwatch()
    fb = ROOT.TFile(fin)
    tb = fb.Get("ttHLepTreeProducerBase")
    nev = tb.GetEntries()
    print "==== %s starting (%d entries) ====" % (name, nev)
    booker = Booker(fout)
    el = None
    if base == "finalMVA_2LSS":
        el = EventLoop([ FinalMVA2LSSTreeProducer("finalMVA",booker,argv[2]), ])
    el.loop([tb], eventRange=range)
    booker.done()
    fb.Close()
    time = timer.RealTime()
    print "=== %s done (%d entries, %.0f s, %.0f e/s) ====" % ( name, nev, time,(nev/time) )
    return (name,(nev,time))

from multiprocessing import Pool
pool = Pool(4)
ret  = dict(pool.map(_runIt, jobs))
fulltime = maintimer.RealTime()
totev   = sum([ev   for (ev,time) in ret.itervalues()])
tottime = sum([time for (ev,time) in ret.itervalues()])
print "Done %d tasks in %.1f min (%d entries, %.1f min)" % (len(jobs),fulltime/60.,totev,tottime/60.)

