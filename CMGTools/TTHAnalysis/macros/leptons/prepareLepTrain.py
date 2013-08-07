#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *

import os, ROOT
if "/smearer_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/smearer.cc+" % os.environ['CMSSW_BASE']);
if "/mcCorrections_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/mcCorrections.cc+" % os.environ['CMSSW_BASE']);

def plausible(rec,gen):
    dr = deltaR(rec,gen)
    if abs(rec.pdgId) == 11 and abs(gen.pdgId) != 11:   return False
    if abs(rec.pdgId) == 13 and abs(gen.pdgId) != 13:   return False
    if dr < 0.3: return True
    if gen.pt < abs(rec.pdgId) == 13 and gen.pdgId != rec.pdgId: return False
    if dr < 0.7: return True
    if min(rec.pt,gen.pt)/max(rec.pt,gen.pt) < 0.3: return False
    return True

class LepTreeProducer(Module):
    def beginJob(self):
        self.t = PyTree(self.book("TTree","t","t"))
        self.t.branch("dr_in","F")
        self.t.branch("ptf_in","F")
        self.t.branch("CSV_in","F")
        #self.t.branch("dr_out","F")
        #self.t.branch("ptf_out","F")
        #self.t.branch("CSV_out","F")
        self.t.branch("good","I")
        self.t.branch("nJet25","I")
        self.t.branch("nBJetLoose25","I")
        self.t.branch("nBJetMedium25","I")
        self.t.branch("puWeight","F")
        #self.t.branch("nVert","F")
        self.copyvars = [ 'relIso','chargedIso','mvaId','pt','eta','pdgId','innerHits','tightId',"mva"]
        for C in self.copyvars: self.t.branch(C,"F")
        # these I can't copy since I need
        for C in [ 'sip3d','dxy','dz' ]: self.t.branch(C,"F")
        self.first = True
    def analyze(self,event):
        #if event.nLepGood < 3: return False
        #jet = Collection(event,"Jet")
        lep = Collection(event,"LepGood","nLepGood",8)
        glep = Collection(event,"GenLep")
        gtau = Collection(event,"GenLepFromTau")
        for l in lep:
            self.t.sip3d = ROOT.scaleSip3dMC(l.sip3d, l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny) if self.corr else l.sip3d
            self.t.dz    = ROOT.scaleDzMC(   l.dz,    l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny) if self.corr else l.dz
            self.t.dxy   = ROOT.scaleDxyMC(  l.dxy,   l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny) if self.corr else l.dxy
            (dr,ptf) = (l.jetDR,l.jetPtRatio)
            self.t.dr_in  = ROOT.correctJetDRMC(dr,l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny)       if self.corr else dr
            self.t.ptf_in = ROOT.correctJetPtRatioMC(ptf,l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny) if self.corr else ptf
            self.t.CSV_in = l.jetBTagCSV
            #(j,dr) = closest(l, jet)
            #ptot = j.p4() + l.p4()
            #self.t.dr_out = dr
            #self.t.ptf_out = l.pt/ptot.Pt()
            #self.t.CSV_out = j.btagCSV
            (gmatch,dr) = closest(l,glep,presel=plausible)
            if dr < 1.5 and abs(gmatch.pdgId) == abs(l.pdgId):
                self.t.good = 20 if dr < 0.5 else 2
            else:
                (gmatch,dr) = closest(l,gtau,presel=plausible)
                if dr < 1.5 and abs(gmatch.pdgId) == abs(l.pdgId):
                    self.t.good = 10 if dr < 0.5 else 1
                else: 
                    self.t.good = -l.mcMatchAny
            self.t.fill()
            for C in self.copyvars: setattr(self.t, C, getattr(l,C))
            self.t.nJet25 = event.nJet25
            self.t.nBJetLoose25 = event.nBJetLoose25
            self.t.nBJetMedium25 = event.nBJetMedium25
            self.t.puWeight = event.puWeight
            #self.t.nVert = event.nVert
            self.t.fill()

from sys import argv
f = ROOT.TFile.Open(argv[1])
t = f.Get("ttHLepTreeProducerBase")
print "Reading %s (%d entries)" % (argv[1], t.GetEntries())

booker = Booker(argv[2] if len(argv) >= 3 else "lepTree.root")
prod = LepTreeProducer("rec",booker)
if len(argv) >= 4 and argv[3] == "NoCorr": 
    print "Will not apply corrections"
    prod.corr = False
else:
    print "Will apply corrections"
    prod.corr = True
el = EventLoop([ prod, ])
maxEv = (int(argv[4]) if len(argv) >= 5 else -1)
print "max entries: ",maxEv
el.loop([t], maxEvents=maxEv)
booker.done()
