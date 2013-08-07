from array import array
from glob import glob
from math import *
import os.path
from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi

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
                self.var[0] = self.corrfunc(self.var[0], lep.pdgId(),lep.pt(),lep.eta(),lep.mcMatchId,lep.mcMatchAny)
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
_CommonVars = [ 
    MVAVar("neuRelIso := relIso - chargedIso/pt",lambda x: x.relIso(dBetaFactor=0.5) - x.chargedHadronIso()/x.pt()),  
    MVAVar("chRelIso := chargedIso/pt",lambda x: x.chargedHadronIso()/x.pt()),
    MVAVar("jetDR_in := min(dr_in,0.5)", lambda x : min(deltaR(x.eta(),x.phi(),x.jet.eta(),x.jet.phi()),0.5), corrfunc=ROOT.correctJetDRMC),
    MVAVar("jetPtRatio_in := min(ptf_in,1.5)", lambda x : min(x.pt()/x.jet.pt(),1.5), corrfunc=ROOT.correctJetPtRatioMC),
    MVAVar("jetBTagCSV_in := max(CSV_in,0)", lambda x : max( (x.jet.btag('combinedSecondaryVertexBJetTags') if hasattr(x.jet, 'btag') else -99) ,0.)),
    #MVAVar("jetDR_out := min(dr_out,5)", lambda x : min(x.dr_out,5.)),
    #MVAVar("jetPtRatio_out := min(ptf_out,1.5)", lambda x : min(x.ptf_out,1.5)),
    #MVAVar("jetBTagCSV_out := max(CSV_out,0)", lambda x : max(x.CSV_out,0.)),
    MVAVar("sip3d",lambda x: x.sip3D(), corrfunc=ROOT.scaleSip3dMC),
]

_MuonVars = [
    MVAVar("dxy := log(abs(dxy))",lambda x: log(abs(x.dxy())), corrfunc=ROOT.scaleDxyMC),
    MVAVar("dz  := log(abs(dz))", lambda x: log(abs(x.dz())), corrfunc=ROOT.scaleDzMC),
]

_ElectronVars = [
    MVAVar("mvaId",lambda x: x.mvaNonTrigV0()),
    MVAVar("innerHits",lambda x: x.numberOfHits()),
]


class LeptonMVA:
    def __init__(self, basepath, isMC):
        global _CommonVars, _CommonSpect, _ElectronVars
        self._isMC = isMC
        self.mu = CategorizedMVA([
            ( lambda x: x.pt() <= 15 and abs(x.eta()) <  1.5 , MVATool("BDTG",basepath%"mu_pteta_low_b", _CommonSpect,_CommonVars+_MuonVars) ),
            ( lambda x: x.pt() <= 15 and abs(x.eta()) >= 1.5 , MVATool("BDTG",basepath%"mu_pteta_low_e", _CommonSpect,_CommonVars+_MuonVars) ),
            ( lambda x: x.pt() >  15 and abs(x.eta()) <  1.5 , MVATool("BDTG",basepath%"mu_pteta_high_b",_CommonSpect,_CommonVars+_MuonVars) ),
            ( lambda x: x.pt() >  15 and abs(x.eta()) >= 1.5 , MVATool("BDTG",basepath%"mu_pteta_high_e",_CommonSpect,_CommonVars+_MuonVars) ),
        ])
        self.el = CategorizedMVA([
            ( lambda x: x.pt() <= 10 and abs(x.eta()) <  0.8                           , MVATool("BDTG",basepath%"el_pteta_low_cb", _CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt() <= 10 and abs(x.eta()) >= 0.8 and abs(x.eta()) <  1.479 , MVATool("BDTG",basepath%"el_pteta_low_fb", _CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt() <= 10 and abs(x.eta()) >= 1.479                         , MVATool("BDTG",basepath%"el_pteta_low_ec", _CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt() >  10 and abs(x.eta()) <  0.8                           , MVATool("BDTG",basepath%"el_pteta_high_cb",_CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt() >  10 and abs(x.eta()) >= 0.8 and abs(x.eta()) <  1.479 , MVATool("BDTG",basepath%"el_pteta_high_fb",_CommonSpect,_CommonVars+_ElectronVars) ),
            ( lambda x: x.pt() >  10 and abs(x.eta()) >= 1.479                         , MVATool("BDTG",basepath%"el_pteta_high_ec",_CommonSpect,_CommonVars+_ElectronVars) ),
        ])
    def __call__(self,lep,ncorr=0):
        if   abs(lep.pdgId()) == 11: return self.el(lep,ncorr)
        elif abs(lep.pdgId()) == 13: return self.mu(lep,ncorr)
        else: return -99
    def addMVA(self,lep):
        if self._isMC:
            lep.mvaValue      = self(lep,1)
            lep.mvaNoCorr     = self(lep,0)
            lep.mvaDoubleCorr = self(lep,2)
        else:
            lep.mvaValue = self(lep,0)

