#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *

IVF_PRESEL = lambda sv : sv.pt > 5 and sv.ntracks >= 2 and abs(sv.dxy)<2 and sv.cosTheta > 0.98
class nSVFriend:
    def __init__(self,sel,sort=lambda sv : -sv.pt,label="Sel",allvars=False,indices=True,mc=False):
        self.sel = sel
        self.sort = sort
        self.label = label
        self.indices = indices
        self.allvars = allvars
        self.intvars = ["ntracks"] if allvars else [] 
        self.fltvars = ["mass","pt","eta","phi","ndof","dxy","sip3d","cosTheta","mva","jetPt","jetBTagCSV"] if allvars else [] 
        if allvars and mc:
            self.intvars += [ "mcFlavFirst", "mcFlavHeaviest" ]
            self.fltvars += [ "mcMatchFraction" ]
    def listBranches(self):
        ret = [ ("n%sSV" % self.label,"I") ]
        if self.indices:
            ret.append( ("%sSV_%s" % (self.label,"idx"),"I",20,"n%sSV" % self.label) )
        for i in self.intvars:
            ret.append( ("%sSV_%s" % (self.label,i),"I",20,"n%sSV" % self.label) )
        for i in self.fltvars:
            ret.append( ("%sSV_%s" % (self.label,i),"F",20,"n%sSV" % self.label) )
        return ret
    def __call__(self,event):
        sv = Collection(event,"SV","nSV",20)
        sels = []
        for i,v in enumerate(sv):
            if self.sel(v): sels.append((i,v))
        sels.sort(key = lambda (i,v): self.sort(v))
        ret = {}
        ret["n%sSV" % self.label] = len(sels)
        if self.indices:
            ret["%sSV_idx" % self.label] = [ i for (i,v) in sels ]
        for i in self.intvars + self.fltvars:
            ret["%sSV_%s" % (self.label,i)] = [ getattr(v,i) for (n,v) in sels ]
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = nSVFriend(sel = lambda sv : sv.pt > 5 and sv.ntracks >= 2 and abs(sv.dxy)<2 and sv.cosTheta > 0.98)
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nSV)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        
