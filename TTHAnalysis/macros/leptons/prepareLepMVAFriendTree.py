#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from array import array
from glob import glob
import os.path

class MVAVar:
    def __init__(self,name,func):
        self.name = name
        self.var  = array('f',[0.])
        self.func = func
    def set(self,lep):
        self.var[0] = self.func(lep)
class MVATool:
    def __init__(self,name,xml,specs,vars):
        self.name = name
        self.reader = ROOT.TMVA.Reader("Silent")
        self.specs = specs
        self.vars  = vars
        for s in specs: self.reader.AddSpectator(s.name,s.var)
        for v in vars:  self.reader.AddVariable(v.name,v.var)
        #print "Would like to load %s from %s! " % (name,xml)
        self.reader.BookMVA(name,xml)
    def __call__(self,lep):
        for s in self.specs: s.set(lep)
        for s in self.vars:  s.set(lep)
        return self.reader.EvaluateMVA(self.name)
class CategorizedMVA:
    def __init__(self,catMvaPairs):
        self.catMvaPairs = catMvaPairs
    def __call__(self,lep):
        for c,m in self.catMvaPairs:
            if c(lep): return m(lep)
        return -99.

_CommonSpect = [ 
]
_CommonVars = [ 
    MVAVar("neuRelIso := relIso - chargedIso/pt",lambda x: x.relIso - x.chargedIso/x.pt),
    MVAVar("chRelIso := chargedIso/pt",lambda x: x.chargedIso/x.pt),
    MVAVar("jetDR_in := min(dr_in,0.5)", lambda x : min(x.jetDR,0.5)),
    MVAVar("jetPtRatio_in := min(ptf_in,1.5)", lambda x : min(x.jetPtRatio,1.5)),
    MVAVar("jetBTagCSV_in := max(CSV_in,0)", lambda x : max(x.jetBTagCSV,0.)),
    #MVAVar("jetDR_out := min(dr_out,5)", lambda x : min(x.dr_out,5.)),
    #MVAVar("jetPtRatio_out := min(ptf_out,1.5)", lambda x : min(x.ptf_out,1.5)),
    #MVAVar("jetBTagCSV_out := max(CSV_out,0)", lambda x : max(x.CSV_out,0.)),
    MVAVar("sip3d",lambda x: x.sip3d),
    MVAVar("dxy := log(abs(dxy))",lambda x: log(abs(x.dxy))),
    MVAVar("dz  := log(abs(dz))", lambda x: log(abs(x.dz))),
]
_ElectronVars = [
    MVAVar("mvaId",lambda x: x.mvaId),
    MVAVar("innerHits",lambda x: x.innerHits),
]
class LeptonMVA:
    def __init__(self,basepath):
        global _CommonVars, _CommonSpect, _ElectronVars
        self.mu = CategorizedMVA([
            ( lambda x: x.pt <= 15 and abs(x.eta) <  1.5 , MVATool("BDTG",basepath%"mu_pteta_low_b", _CommonSpect,_CommonVars) ),
            ( lambda x: x.pt <= 15 and abs(x.eta) >= 1.5 , MVATool("BDTG",basepath%"mu_pteta_low_e", _CommonSpect,_CommonVars) ),
            ( lambda x: x.pt >  15 and abs(x.eta) <  1.5 , MVATool("BDTG",basepath%"mu_pteta_high_b",_CommonSpect,_CommonVars) ),
            ( lambda x: x.pt >  15 and abs(x.eta) >= 1.5 , MVATool("BDTG",basepath%"mu_pteta_high_e",_CommonSpect,_CommonVars) ),
        ])
        self.el = CategorizedMVA([
            ( lambda x: x.pt <= 10 and abs(x.eta) <  0.8                         , MVATool("BDTG",basepath%"el_pteta_low_cb", _CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt <= 10 and abs(x.eta) >= 0.8 and abs(x.eta) <  1.479 , MVATool("BDTG",basepath%"el_pteta_low_fb", _CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt <= 10 and abs(x.eta) >= 1.479                       , MVATool("BDTG",basepath%"el_pteta_low_ec", _CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt >  10 and abs(x.eta) <  0.8                         , MVATool("BDTG",basepath%"el_pteta_high_cb",_CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt >  10 and abs(x.eta) >= 0.8 and abs(x.eta) <  1.479 , MVATool("BDTG",basepath%"el_pteta_high_fb",_CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt >  10 and abs(x.eta) >= 1.479                       , MVATool("BDTG",basepath%"el_pteta_high_ec",_CommonSpect,_CommonVars+_ElectronVars) ),
        ])
    def __call__(self,lep):
        if   abs(lep.pdgId) == 11: return self.el(lep)
        elif abs(lep.pdgId) == 13: return self.mu(lep)
        else: return -99

class LepMVATreeProducer(Module):
    def __init__(self,name,booker,path):
        Module.__init__(self,name,booker)
        self.mva = LeptonMVA(path+"/weights/%s_BDTG.weights.xml")
    def beginJob(self):
        self.t = PyTree(self.book("TTree","t","t"))
        for i in range(8):
            self.t.branch("LepGood%d_mvaNew" % (i+1),"F")
    def analyze(self,event):
        lep = Collection(event,"LepGood","nLepGood",8)
        for i,l in enumerate(lep):
            setattr(self.t, "LepGood%d_mvaNew" % (i+1), self.mva(l))
        for i in xrange(len(lep),8):
            setattr(self.t, "LepGood%d_mvaNew" % (i+1), -99.)
        self.t.fill()

import os, itertools
from sys import argv
if len(argv) < 3: print "Usage: %s <TREE_DIR> <TRAINING>" % argv[0]
jobs = []
if not os.path.exists(argv[2]):            os.mkdir(argv[2])
if not os.path.exists(argv[2]+"/weights"): os.mkdir(argv[2]+"/weights")
for X,Y in itertools.product(["high","low"],["b","e"]):
    os.system("cp weights/mu_pteta_%s_%s_BDTG.weights.xml %s/weights -v" % (X,Y,argv[2]))
for X,Y in itertools.product(["high","low"],["cb","fb","ec"]):
    os.system("cp weights/el_pteta_%s_%s_BDTG.weights.xml %s/weights -v" % (X,Y,argv[2]))
for D in glob(argv[1]+"/*"):
    fname = D+"/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root"
    if os.path.exists(fname):
        short = os.path.basename(D)
        print "  ",os.path.basename(D)
        jobs.append((short,fname,"%s/lepMVAFriend_%s.root" % (argv[2],short)))
print 4*"\n"
print "I have %d taks to process" % len(jobs)

maintimer = ROOT.TStopwatch()
def _runIt(args):
    (name,fin,fout) = args
    timer = ROOT.TStopwatch()
    fb = ROOT.TFile(fin)
    tb = fb.Get("ttHLepTreeProducerBase")
    nev = tb.GetEntries()
    print "==== %s starting (%d entries) ====" % (name, nev)
    booker = Booker(fout)
    el = EventLoop([ LepMVATreeProducer("newMVA",booker,argv[2]), ])
    el.loop([tb])
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

