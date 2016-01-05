#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from array import array
from glob import glob
import os.path

import ROOT

class MVAVar:
    def __init__(self,name,func,corrfunc=None):
        self.name = name
        self.var  = array('f',[0.])
        self.func = func
        self.corrfunc = corrfunc
    def set(self,lep,ncorr): ## apply correction ncorr times
        self.var[0] = self.func(lep)
        if self.corrfunc:
            for i in range(ncorr):
                self.var[0] = self.corrfunc(self.var[0], lep.pdgId,lep.pt,lep.eta,lep.mcMatchId,lep.mcMatchAny)

from CMGTools.TTHAnalysis.leptonMVA import MVATool, CategorizedMVA

_CommonSpect = [ 
]
_CommonVars = {
'WithPtV2': [
    MVAVar("LepGood_pt",lambda x: x.pt),
    MVAVar("LepGood_miniRelIsoCharged",lambda x: x.miniRelIsoCharged),
    MVAVar("LepGood_miniRelIsoNeutral",lambda x: x.miniRelIsoNeutral),
    MVAVar("LepGood_jetPtRelv2",lambda x: x.jetPtRelv2),
    MVAVar("LepGood_jetPtRatio := min(LepGood_jetPtRatio_LepAwareJECv2,1.5)", lambda x : min(x.jetPtRatiov2,1.5)),
    MVAVar("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", lambda x : max(x.jetBTagCSV,0.)),
    MVAVar("LepGood_sip3d",lambda x: x.sip3d),
    MVAVar("LepGood_dxy := log(abs(LepGood_dxy))",lambda x: log(abs(x.dxy))),
    MVAVar("LepGood_dz  := log(abs(LepGood_dz))", lambda x: log(abs(x.dz))),
], 'WithPtV2_oldTrees': [
    MVAVar("LepGood_pt",lambda x: x.pt),
    MVAVar("LepGood_miniRelIsoCharged",lambda x: x.miniRelIsoCharged),
    MVAVar("LepGood_miniRelIsoNeutral",lambda x: x.miniRelIsoNeutral),
    MVAVar("LepGood_jetPtRelv2",lambda x: x.jetPtRelv2),
    MVAVar("LepGood_jetPtRatio := min(LepGood_jetPtRatio_LepAwareJECv2,1.5)", lambda x : min(x.jetPtRatio_LepAwareJECv2,1.5)),
    MVAVar("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", lambda x : max(x.jetBTagCSV,0.)),
    MVAVar("LepGood_sip3d",lambda x: x.sip3d),
    MVAVar("LepGood_dxy := log(abs(LepGood_dxy))",lambda x: log(abs(x.dxy))),
    MVAVar("LepGood_dz  := log(abs(LepGood_dz))", lambda x: log(abs(x.dz))),
] 
}

_ElectronVars = {
 'V2': [
    MVAVar("LepGood_mvaIdPhys14",lambda x: x.mvaIdPhys14)
 ]
}

_MuonVars = [
    MVAVar("LepGood_segmentCompatibility",lambda x: x.segmentCompatibility)
]

class LeptonMVA:
    def __init__(self,basepath,training="WithPtV2"):
        global _CommonVars, _CommonSpect, _ElectronVars, _MuonVars, _SVVars
        if type(basepath) == tuple: basepathmu, basepathel  = basepath
        else:                       basepathmu, basepathel  = basepath, basepath
        print "Booking %s %s" % (training, basepath)
        if "V2" in training:
            muVars = _CommonVars[training][:] + _MuonVars[:]
            elVars = _CommonVars[training][:] + _ElectronVars['V2'][:]
        if not muVars:
            self.mu = lambda mu, ncorr : -37.0;
        else:
            self.mu = CategorizedMVA([
                ( lambda x: abs(x.eta) <  1.5 , MVATool("BDTG",basepathmu%"mu_eta_b",_CommonSpect,muVars) ),
                ( lambda x: abs(x.eta) >= 1.5 , MVATool("BDTG",basepathmu%"mu_eta_e",_CommonSpect,muVars) ),
            ])
        if not elVars:
            self.el = lambda el, ncorr : -37.0;
        else:
            self.el = CategorizedMVA([
                ( lambda x: abs(x.eta) <  0.8                         , MVATool("BDTG",basepathel%"el_eta_cb",_CommonSpect,elVars) ),
                ( lambda x: abs(x.eta) >= 0.8 and abs(x.eta) <  1.479 , MVATool("BDTG",basepathel%"el_eta_fb",_CommonSpect,elVars) ),
                ( lambda x: abs(x.eta) >= 1.479                       , MVATool("BDTG",basepathel%"el_eta_ec",_CommonSpect,elVars) ),
            ])
    def __call__(self,lep,ncorr=0):
        if   abs(lep.pdgId) == 11: return self.el(lep,ncorr)
        elif abs(lep.pdgId) == 13: return self.mu(lep,ncorr)
        else: return -99

class LepMVAFriend:
    def __init__(self,path,training="WithPtV2",label="",fast=True):
        self.mva = LeptonMVA(path+"/%s_BDTG.weights.xml" if type(path) == str else path, training=training)
        self.fast = fast
        self.label = label
    def listBranches(self):
        return [ ("nLepGood","I"), ("LepGood_mva"+self.label,"F",8,"nLepGood") ]
    def __call__(self,event):
        lep = Collection(event,"LepGood","nLepGood",8)
        ret = { 'nLepGood' : event.nLepGood }
        if event.run >= 1: # DATA
            ret['LepGood_mva'+self.label] = [ self.mva(l, ncorr=0) for l in lep ] 
        else:              # MC
            ret['LepGood_mva'+self.label] = [ self.mva(l, ncorr=0) for l in lep ] 
        return ret

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    if len(argv) > 2: tree.AddFriend("sf/t",argv[2])
    tree.vectorTree = True
    class Tester(Module):
        def __init__(self, name, trees="new"):
            Module.__init__(self,name,None)
            self.mvas = {
                'WithPtV2' : LepMVAFriend(("/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_withpt_v2_BDTG.weights.xml",
                                           "/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_withpt_v2_BDTG.weights.xml",),
                                           training=("WithPtV2_oldTrees" if trees=="old" else 'WithPtV2')),
            }
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            lep = Collection(ev,"LepGood","nLepGood",8)
            for l,m in self.mvas.iteritems():
                print "%-10s: %s %s" % (l, m(ev), [ x.mvaTTH for x in lep ] )
    el = EventLoop([ Tester("tester", "new") ])
    el.loop([tree], maxEvents = 50)

        
