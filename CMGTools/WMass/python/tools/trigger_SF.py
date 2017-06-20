#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.MonoXAnalysis.tools.eventVars_monojet import EventVarsMonojet
import re,os

class LepTriggerSF_Event:
    def __init__(self,file="/afs/cern.ch/work/e/emanuele/public/monox/leptonsf/leptonTrigsfs.root"):
        tfile = ROOT.TFile(file)
        self.hm = tfile.Get("hltmu20_SF").Clone("LepTriggerSF_SingleMuonTriggerSF")
        self.he = tfile.Get("hltel27_SF").Clone("LepTriggerSF_SingleEleTriggerSF")
        self.hm.SetDirectory(None)
        self.he.SetDirectory(None)
        tfile.Close()
        self.evtSel = EventVarsMonojet()
    def __call__(self,event):
        leps = Collection(event,"LepGood","nLepGood",2)
        # single-lepton triggers have tight ID
        tightLeps = [ l for l in leps if self.evtSel.lepIdTight(l) ]
        if len(tightLeps) < 1: return 1.0
        id1 = abs(leps[0].pdgId)
        h = self.hm if id1 == 13 else self.he
        return h.GetBinContent(h.FindBin(leps[0].pt,abs(leps[0].eta)))
 
class MetNoMuTriggerSF:
    def __init__(self,file="/afs/cern.ch/work/e/emanuele/public/monox/leptonsf/mettrigSF.root"):
        tfile = ROOT.TFile(file)
        self.h = tfile.Get("mettrigSF").Clone("MetTriggerSF")
        self.h.SetDirectory(None)
        tfile.Close()
    def __call__(self,event):
        return self.h.GetBinContent(self.h.FindBin(event.metNoMu_pt))

class MetNoMuTriggerSF_Spline:
    def __init__(self,file="/afs/cern.ch/work/e/emanuele/public/monox/leptonsf/met_trig_spline.root"):
        tfile = ROOT.TFile(file)
        self.s = tfile.Get("met_trig_spline3").Clone("MetTriggerSF")
        tfile.Close()
    def __call__(self,event):
        if(event.metNoMu_pt > 500): return 1.0
        return self.s.Eval(event.metNoMu_pt)

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("ttHLepTreeProducerBase")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = LepTriggerSF_Event()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 100)
