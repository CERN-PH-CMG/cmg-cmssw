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
    #MVAVar("minMWjj := min(minMWjj,200)"),
    #MVAVar("minMWjjPt"),
    #MVAVar("bestMWjj"),
    MVAVar("bestMTopHad"),
    MVAVar("nJet25"),
    #MVAVar("q3l := LepGood1_charge+LepGood2_charge+LepGood3_charge"),
    #MVAVar("m3l"),
    MVAVar("Jet1_pt"),
    #MVAVar("Jet2_pt"),
    #MVAVar("Jet2_btagCSV:= max(0,Jet2_btagCSV)"),
    #MVAVar("nBJetLoose25"),
    #MVAVar("nBJetMedium25"),
    #MVAVar("nBJetTight25 := (Jet1_btagCSV > 0.898)+(Jet2_btagCSV > 0.898)+(Jet3_btagCSV > 0.898)"),
    MVAVar("max_Lep_eta := max(max(abs(LepGood1_eta),abs(LepGood2_eta)),abs(LepGood3_eta))"),
    #MVAVar("minMllAFOS"),
    MVAVar("minDrllAFOS"),
    MVAVar("htJet25",'F'),
    #MVAVar("htJet25Lep := (LepGood1_pt + LepGood2_pt + LepGood3_pt + htJet25)"),
    #MVAVar("htJet25LepMet := (met + LepGood1_pt + LepGood2_pt + LepGood3_pt + htJet25)"),
    #MVAVar("htJet25ratio1224 := (Jet1_pt*(abs(Jet1_eta) < 1.2) + Jet2_pt*(abs(Jet2_eta) < 1.2) + Jet3_pt*(abs(Jet3_eta) < 1.2) + Jet4_pt*(abs(Jet4_eta) < 1.2) + Jet5_pt*(abs(Jet5_eta) < 1.2) + Jet6_pt*(abs(Jet6_eta) < 1.2) + Jet7_pt*(abs(Jet7_eta) < 1.2) + Jet8_pt*(abs(Jet8_eta) < 1.2))/ (Jet1_pt*(abs(Jet1_eta) < 2.4) + Jet2_pt*(abs(Jet2_eta) < 2.4) + Jet3_pt*(abs(Jet3_eta) < 2.4) + Jet4_pt*(abs(Jet4_eta) < 2.4) + Jet5_pt*(abs(Jet5_eta) < 2.4) + Jet6_pt*(abs(Jet6_eta) < 2.4) + Jet7_pt*(abs(Jet7_eta) < 2.4) + Jet8_pt*(abs(Jet8_eta) < 2.4))"),
    MVAVar("htJet25ratio1224Lep := (LepGood1_pt*(abs(LepGood1_eta)<1.2) + LepGood2_pt*(abs(LepGood2_eta)<1.2) + LepGood3_pt*(abs(LepGood3_eta)<1.2) + Jet1_pt*(abs(Jet1_eta) < 1.2) + Jet2_pt*(abs(Jet2_eta) < 1.2) + Jet3_pt*(abs(Jet3_eta) < 1.2) + Jet4_pt*(abs(Jet4_eta) < 1.2) + Jet5_pt*(abs(Jet5_eta) < 1.2) + Jet6_pt*(abs(Jet6_eta) < 1.2) + Jet7_pt*(abs(Jet7_eta) < 1.2) + Jet8_pt*(abs(Jet8_eta) < 1.2))/ (LepGood1_pt + LepGood2_pt + LepGood3_pt + Jet1_pt*(abs(Jet1_eta) < 2.4) + Jet2_pt*(abs(Jet2_eta) < 2.4) + Jet3_pt*(abs(Jet3_eta) < 2.4) + Jet4_pt*(abs(Jet4_eta) < 2.4) + Jet5_pt*(abs(Jet5_eta) < 2.4) + Jet6_pt*(abs(Jet6_eta) < 2.4) + Jet7_pt*(abs(Jet7_eta) < 2.4) + Jet8_pt*(abs(Jet8_eta) < 2.4))"),

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

class FinalMVA3L:
    def __init__(self,basepath,cat="LJ",algo="BDTG"):
        global _CommonVars, _MultiJetVars
        self.mva = MVATool(algo, basepath, _CommonVars)
    def __call__(self,event):
        return self.mva(event)


class FinalMVA3LTreeProducer(Module):
    def __init__(self,name, booker, path):
        Module.__init__(self,name,booker)
        self.bdtg = FinalMVA3L(path+"/weights/3l_mix_BDTG.weights.xml", cat="L", algo="BDTG")
        
    def beginJob(self):
        self.t = PyTree(self.book("TTree","t","t"))
        self.t.branch("FinalMVA_3L_BDTG","F")

    def analyze(self,event):
        setattr(self.t, "FinalMVA_3L_BDTG", self.bdtg(event))

        self.t.fill()


import os, itertools
from sys import argv
if len(argv) < 3: print "Usage: %s  [--fr]  [-e] <TREE_DIR> <TRAINING>" % argv[0]
jobs = []
base = "finalMVA_3L"
if argv[1] == "-e": # use existing training
    argv = [argv[0]] + argv[2:]
else: # new, make directory training
    if not os.path.exists(argv[2]):            os.mkdir(argv[2])
    if not os.path.exists(argv[2]+"/weights"): os.mkdir(argv[2]+"/weights")
    if not os.path.exists(argv[2]+"/trainings"): os.mkdir(argv[2]+"/trainings")
    os.system("cp weights/3l_mix_BDTG.weights.xml   %s/weights -v" % argv[2])
    os.system("cp 3l_mix.root   %s/trainings -v" % argv[2])

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
    if base == "finalMVA_3L":
        el = EventLoop([ FinalMVA3LTreeProducer("finalMVA",booker,argv[2]), ])
    el.loop([tb], eventRange=range)
    booker.done()
    fb.Close()
    time = timer.RealTime()
    print "=== %s done (%d entries, %.0f s, %.0f e/s) ====" % ( name, nev, time,(nev/time) )
    return (name,(nev,time))

from multiprocessing import Pool
pool = Pool(8)
ret  = dict(pool.map(_runIt, jobs))
fulltime = maintimer.RealTime()
totev   = sum([ev   for (ev,time) in ret.itervalues()])
tottime = sum([time for (ev,time) in ret.itervalues()])
print "Done %d tasks in %.1f min (%d entries, %.1f min)" % (len(jobs),fulltime/60.,totev,tottime/60.)

