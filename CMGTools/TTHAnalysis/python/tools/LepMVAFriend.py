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
    def __call__(self,lep,ncorr): ## apply correction ncorr times
        for s in self.specs: s.set(lep,ncorr)
        for s in self.vars:  s.set(lep,ncorr)
        return self.reader.EvaluateMVA(self.name)   
class CategorizedMVA:
    def __init__(self,catMvaPairs):
        self.catMvaPairs = catMvaPairs
    def __call__(self,lep,ncorr):
        for c,m in self.catMvaPairs:
            if c(lep): return m(lep,ncorr)
        return -99.

_CommonSpect = [ 
]
_CommonVars = {
 'old': [ 
    MVAVar("LepGood_neuRelIso03 := LepGood_relIso03 - LepGood_chargedHadRelIso03",lambda x: x.relIso03 - x.chargedHadRelIso03),  
    MVAVar("LepGood_chRelIso03 := LepGood_chargedHadRelIso03",lambda x: x.chargedHadRelIso03),
    MVAVar("LepGood_jetDR := min(LepGood_jetDR,0.5)", lambda x : min(x.jetDR,0.5)),
    MVAVar("LepGood_jetPtRatio := min(LepGood_jetPtRatio,1.5)", lambda x : min(x.jetPtRatio,1.5)),
    MVAVar("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", lambda x : max(x.jetBTagCSV,0.)),
    MVAVar("LepGood_sip3d",lambda x: x.sip3d),
    MVAVar("LepGood_dxy := log(abs(LepGood_dxy))",lambda x: log(abs(x.dxy))),
    MVAVar("LepGood_dz  := log(abs(LepGood_dz))", lambda x: log(abs(x.dz))),
], 'new':[
    MVAVar("LepGood_miniRelIsoCharged",lambda x: x.miniRelIsoCharged),
    MVAVar("LepGood_miniRelIsoNeutral",lambda x: x.miniRelIsoNeutral),
    MVAVar("LepGood_jetPtRel",lambda x: x.jetPtRel),
    MVAVar("LepGood_jetPtRatio := min(LepGood_jetPtRatio,1.5)", lambda x : min(x.jetPtRatio,1.5)),
    MVAVar("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", lambda x : max(x.jetBTagCSV,0.)),
    MVAVar("LepGood_sip3d",lambda x: x.sip3d),
    MVAVar("LepGood_dxy := log(abs(LepGood_dxy))",lambda x: log(abs(x.dxy))),
    MVAVar("LepGood_dz  := log(abs(LepGood_dz))", lambda x: log(abs(x.dz))),
], 'v2':[
    MVAVar("LepGood_miniRelIsoCharged",lambda x: x.miniRelIsoCharged),
    MVAVar("LepGood_miniRelIsoNeutral",lambda x: x.miniRelIsoNeutral),
    MVAVar("LepGood_jetPtRelv2",lambda x: x.jetPtRelv2),
    MVAVar("LepGood_jetPtRatio := min(LepGood_jetPtRatio_LepAwareJECv2,1.5)", lambda x : min(x.jetPtRatio,1.5)),
    MVAVar("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", lambda x : max(x.jetBTagCSV,0.)),
    MVAVar("LepGood_sip3d",lambda x: x.sip3d),
    MVAVar("LepGood_dxy := log(abs(LepGood_dxy))",lambda x: log(abs(x.dxy))),
    MVAVar("LepGood_dz  := log(abs(LepGood_dz))", lambda x: log(abs(x.dz))),
], 'withpt': [
    MVAVar("LepGood_pt",lambda x: x.pt),
    MVAVar("LepGood_miniRelIsoCharged",lambda x: x.miniRelIsoCharged),
    MVAVar("LepGood_miniRelIsoNeutral",lambda x: x.miniRelIsoNeutral),
    MVAVar("LepGood_jetPtRel",lambda x: x.jetPtRel),
    MVAVar("LepGood_jetPtRatio := min(LepGood_jetPtRatio,1.5)", lambda x : min(x.jetPtRatio,1.5)),
    MVAVar("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", lambda x : max(x.jetBTagCSV,0.)),
    MVAVar("LepGood_sip3d",lambda x: x.sip3d),
    MVAVar("LepGood_dxy := log(abs(LepGood_dxy))",lambda x: log(abs(x.dxy))),
    MVAVar("LepGood_dz  := log(abs(LepGood_dz))", lambda x: log(abs(x.dz))),
], 'withpt_v2': [
    MVAVar("LepGood_pt",lambda x: x.pt),
    MVAVar("LepGood_miniRelIsoCharged",lambda x: x.miniRelIsoCharged),
    MVAVar("LepGood_miniRelIsoNeutral",lambda x: x.miniRelIsoNeutral),
    MVAVar("LepGood_jetPtRelv2",lambda x: x.jetPtRelv2),
    MVAVar("LepGood_jetPtRatio := min(LepGood_jetPtRatio_LepAwareJECv2,1.5)", lambda x : min(x.jetPtRatio,1.5)),
    MVAVar("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", lambda x : max(x.jetBTagCSV,0.)),
    MVAVar("LepGood_sip3d",lambda x: x.sip3d),
    MVAVar("LepGood_dxy := log(abs(LepGood_dxy))",lambda x: log(abs(x.dxy))),
    MVAVar("LepGood_dz  := log(abs(LepGood_dz))", lambda x: log(abs(x.dz))),
]
}

_ElectronVars = {
'old':[
    MVAVar("LepGood_mvaIdPhys14",lambda x: x.mvaIdPhys14)
], 'new':[
    MVAVar("LepGood_mvaIdPhys14",lambda x: x.mvaIdPhys14)
]
}

_MuonVars = [
    MVAVar("LepGood_segmentCompatibility",lambda x: x.segmentCompatibility)
]

_SVVars = [
    MVAVar("LepGood_hasSV", lambda x: x.hasSV),
    MVAVar("LepGood_svRedPt := min(max( LepGood_svRedPt, -10),100)", lambda x : min(max( x.svRedPt, -10),100)),
    MVAVar("LepGood_svRedM := min(max( LepGood_svRedM, -1),6)", lambda x : min(max( x.svRedM, -1),6)),
    MVAVar("LepGood_svM := min(max( LepGood_svM, -1),6)", lambda x : min(max( x.svM, -1),6)),
    MVAVar("LepGood_svPt := min(max( LepGood_svPt, -10),100)", lambda x : min(max( x.svPt, -10),100)),
    MVAVar("LepGood_svSip3d := min(max( LepGood_svSip3d, -10),100)", lambda x : min(max( x.svSip3d, -10),100)),
    MVAVar("LepGood_svLepSip3d := min(max( LepGood_svLepSip3d, -10),5)", lambda x : min(max( x.svLepSip3d, -10),5)),
    MVAVar("LepGood_svNTracks := min(max( LepGood_svNTracks, -1),10)", lambda x : min(max( x.svNTracks, -1),10)),
    MVAVar("LepGood_svChi2n := min(max( LepGood_svChi2n, -1),10)", lambda x : min(max( x.svChi2n, -1),10)),
    MVAVar("LepGood_svDxy := min(max( LepGood_svDxy, 0),4)", lambda x : min(max( x.svDxy, 0),4)),
]
class LeptonMVA:
    def __init__(self,basepath,training="susyBase"):
        global _CommonVars, _CommonSpect, _ElectronVars, _MuonVars, _SVVars
        if type(basepath) == tuple: basepathmu, basepathel  = basepath
        else:                       basepathmu, basepathel  = basepath, basepath
        print "Booking %s %s" % (training, basepath)
        if training == "muMVAId":
            muVars = _CommonVars['old'][:] + [ MVAVar("mvaId",lambda x: x.muonMVAIdFull) ]
            elVars = _CommonVars['old'][:] + _ElectronVars['old'][:]
        elif training == "muMVAId_SV":
            muVars = _CommonVars['old'][:] + [ MVAVar("mvaId",lambda x: x.muonMVAIdFull) ] + _SVVars
            elVars = _CommonVars['old'][:] + _ElectronVars['old'][:] + _SVVars
        elif training in [ 'new', 'v2', 'withpt', 'withpt_v2' ]: 
            muVars = _CommonVars[training][:] + _MuonVars[:]
            elVars = _CommonVars[training][:] + _ElectronVars['new'][:]
        else:
            muVars = _CommonVars['old'][:] + _MuonVars[:]
            elVars = _CommonVars['old'][:] + _ElectronVars['old'][:]
        if 'withpt' not in training:
            if not muVars:
                self.mu = lambda mu, ncorr : -37.0;
            else:
                self.mu = CategorizedMVA([
                    ( lambda x: x.pt <= 10, MVATool("BDTG",basepathmu%"mu_pteta_low", _CommonSpect,muVars) ),
                    ( lambda x: x.pt > 10 and x.pt <= 25 and abs(x.eta) <  1.5 , MVATool("BDTG",basepathmu%"mu_pteta_medium_b", _CommonSpect,muVars) ),
                    ( lambda x: x.pt > 10 and x.pt <= 25 and abs(x.eta) >= 1.5 , MVATool("BDTG",basepathmu%"mu_pteta_medium_e", _CommonSpect,muVars) ),
                    ( lambda x: x.pt >  25 and abs(x.eta) <  1.5 , MVATool("BDTG",basepathmu%"mu_pteta_high_b",_CommonSpect,muVars) ),
                    ( lambda x: x.pt >  25 and abs(x.eta) >= 1.5 , MVATool("BDTG",basepathmu%"mu_pteta_high_e",_CommonSpect,muVars) ),
                ])
            if not elVars:
                self.el = lambda el, ncorr : -37.0;
            else:
                self.el = CategorizedMVA([
                    ( lambda x: x.pt <= 10                          , MVATool("BDTG",basepathel%"el_pteta_low", _CommonSpect,elVars) ),    
                    ( lambda x: x.pt >  10 and x.pt <= 25 and abs(x.eta) <  0.8                         , MVATool("BDTG",basepathel%"el_pteta_medium_cb", _CommonSpect,elVars) ),
                    ( lambda x: x.pt >  10 and x.pt <= 25 and abs(x.eta) >= 0.8 and abs(x.eta) <  1.479 , MVATool("BDTG",basepathel%"el_pteta_medium_fb", _CommonSpect,elVars) ),
                    ( lambda x: x.pt >  10 and x.pt <= 25 and abs(x.eta) >= 1.479                       , MVATool("BDTG",basepathel%"el_pteta_medium_ec", _CommonSpect,elVars) ),
                    ( lambda x: x.pt >  25 and abs(x.eta) <  0.8                         , MVATool("BDTG",basepathel%"el_pteta_high_cb",_CommonSpect,elVars) ),
                    ( lambda x: x.pt >  25 and abs(x.eta) >= 0.8 and abs(x.eta) <  1.479 , MVATool("BDTG",basepathel%"el_pteta_high_fb",_CommonSpect,elVars) ),
                    ( lambda x: x.pt >  25 and abs(x.eta) >= 1.479                       , MVATool("BDTG",basepathel%"el_pteta_high_ec",_CommonSpect,elVars) ),
                ])
        else:
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
    def __init__(self,path,training="susyBase",label="",fast=True):
        self.mva = LeptonMVA(path+"/weights/%s_BDTG.weights.xml" if type(path) == str else path, training=training)
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
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.mvas = {
                'old': LepMVAFriend(("/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/data/leptonMVA/tth/%s_BDTG.weights.xml",
                                     "/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/data/leptonMVA/tth/%s_BDTG.weights.xml",)),
                'v2' : LepMVAFriend(("/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_v2_BDTG.weights.xml",
                                     "/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_v2_BDTG.weights.xml",),
                                    training="v2"),
                'withpt' : LepMVAFriend(("/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_withpt_BDTG.weights.xml",
                                         "/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_withpt_BDTG.weights.xml",),
                                        training="withpt"),
                'withpt_v2' : LepMVAFriend(("/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_withpt_v2_BDTG.weights.xml",
                                            "/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_withpt_v2_BDTG.weights.xml",),
                                           training="withpt_v2"),
                'withpt_v2_w05' : LepMVAFriend(("/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_withpt_v2_w05_BDTG.weights.xml",
                                                "/afs/cern.ch/work/g/gpetrucc/CMSSW_7_4_7/src/CMGTools/TTHAnalysis/macros/leptons/weights/%s_withpt_v2_w05_BDTG.weights.xml",),
                                                training="withpt_v2"),
            }
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            for l,m in self.mvas.iteritems():
                print "%-10s: %s" % (l, m(ev))
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 50)

        
