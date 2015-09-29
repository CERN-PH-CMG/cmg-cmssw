#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from array import array
from glob import glob
import os

from PhysicsTools.Heppy import loadlibs
from CMGTools.TTHAnalysis.leptonMVA  import MVAVar, MVATool, CategorizedMVA

_CommonSpect = [ 
]
_CommonVars = { 
 '2015':[
    MVAVar("LepGood_miniRelIso := min(LepGood_miniRelIso,1.0)",lambda x: min(x.miniRelIso,1.0)),
    MVAVar("LepGood_jetPtRatio := min(LepGood_jetPtRatio,1.5)",lambda x: min(x.jetPtRatio,1.5)),
    MVAVar("LepGood_jetPtRel := min(LepGood_jetPtRel,30)",lambda x: min(x.jetPtRel,30.0)),
 ],
 '2015v2':[
    MVAVar("LepGood_miniRelIso := min(LepGood_miniRelIso,1.0)",lambda x: min(x.miniRelIso,1.0)),
    MVAVar("LepGood_jetPtRatio_LepAwareJECv2 := min(LepGood_jetPtRatio_LepAwareJECv2,1.5)",lambda x: min(x.jetPtRatio_LepAwareJECv2,1.5)),
    MVAVar("LepGood_jetPtRelv2 := min(LepGood_jetPtRelv2,30)",lambda x: min(x.jetPtRelv2,30.0)),
 ],
}

_ElectronVars = {
 '2015':[],
 '2015v2':[],
}

_MuonVars = {
 '2015':[],
 '2015v2':[],
}

class LeptonMVA:
    def __init__(self,kind,basepath):
        global _CommonVars, _CommonSpect, _ElectronVars, _MuonVars
        if type(basepath) == tuple: basepathmu, basepathel  = basepath
        else:                       basepathmu, basepathel  = basepath, basepath
        muVars = _CommonVars[kind] + _MuonVars[kind]
        elVars = _CommonVars[kind] + _ElectronVars[kind]
        muVarsHi = _CommonVars[kind] + _MuonVars[kind]
        elVarsHi = _CommonVars[kind] + _ElectronVars[kind]
        self.mu = CategorizedMVA([
                ( lambda x: True, MVATool("BDTG",basepathmu%"mu",_CommonSpect,muVars) ),
                ])
        self.el = CategorizedMVA([
                ( lambda x: True, MVATool("BDTG",basepathel%"el",_CommonSpect,muVars) ),
                ])
    def __call__(self,lep,ncorr=0):
        if   abs(lep.pdgId) == 11: return self.el(lep,ncorr)
        elif abs(lep.pdgId) == 13: return self.mu(lep,ncorr)
        else: return -99

class MultiIsoMVAFriend:
    def __init__(self,training,path,label=""):
        self.mva = LeptonMVA(training,path+"/weights/%s_BDTG.weights.xml" if type(path) == str else path)
        self.label = label
    def listBranches(self):
        return [ ("nLepGood","I"), ("LepGood_mvaNew"+self.label,"F",8,"nLepGood") ]
    def __call__(self,event):
        lep = Collection(event,"LepGood","nLepGood",8)
        ret = { 'nLepGood' : event.nLepGood }
        ret['LepGood_mvaNew'+self.label] = [ self.mva(l, ncorr=0) for l in lep ] 
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    if len(argv) > 2: tree.AddFriend("sf/t",argv[2])
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf0  = MultiIsoMVAFriend("2015",
                                          (os.environ["CMSSW_BASE"]+"/src/CMGTools/TTHAnalysis/python/plotter/ttH-multilepton/train-MVA-Iso/weights/2015_%s_BDTG.weights.xml",
                                           os.environ["CMSSW_BASE"]+"/src/CMGTools/TTHAnalysis/python/plotter/ttH-multilepton/train-MVA-Iso/weights/2015_%s_BDTG.weights.xml",),
                                          label="2015")
            self.sf1  = MultiIsoMVAFriend("2015v2",
                                          (os.environ["CMSSW_BASE"]+"/src/CMGTools/TTHAnalysis/python/plotter/ttH-multilepton/train-MVA-Iso/weights/2015v2_%s_BDTG.weights.xml",
                                           os.environ["CMSSW_BASE"]+"/src/CMGTools/TTHAnalysis/python/plotter/ttH-multilepton/train-MVA-Iso/weights/2015v2_%s_BDTG.weights.xml",),
                                          label="2015v2")
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf0(ev)
            print self.sf1(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        
