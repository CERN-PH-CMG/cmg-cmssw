#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *

import os,itertools, ROOT
if "/smearer_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/smearer.cc+" % os.environ['CMSSW_BASE']);
if "/mcCorrections_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/mcCorrections.cc+" % os.environ['CMSSW_BASE']);


from optparse import OptionParser
parser = OptionParser(usage="%prog [options] <PROCESS> <FILEOUT>")
parser.add_option("-t", "--tree",    dest="tree", default='ttHLepTreeProducerTTH', help="Pattern for tree name");
parser.add_option("-V", "--vector",  dest="vectorTree", action="store_true", default=True, help="Input tree is a vector")
(options, args) = parser.parse_args()

class LepTreeProducer(Module):
    def beginJob(self):
        self.t = PyTree(self.book("TTree","t","t"))
        self.t.branch("good","I")
        self.t.branch("nJet25","I")
        self.t.branch("nBJetLoose25","I")
        self.t.branch("nBJetMedium25","I")
        self.t.branch("puWeight","F")
        self.copyvars = ['relIso03','chargedHadRelIso03','relIso04','chargedHadRelIso04','mvaId','pt','eta','pdgId','lostHits','tightId','nStations','trkKink','caloCompatibility','globalTrackChi2','trackerLayers','pixelLayers','mcMatchId','mcMatchAny', 'mcMatchTau']        
        for C in self.copyvars: self.t.branch(C,"F")
        # these I can't copy since I need to apply corrections
        for C in [ 'sip3d','dxy','dz','jetPtRatio','jetBTagCSV','jetDR']: self.t.branch(C,"F")
        self.first = True
    def analyze(self,event):
        lep = Collection(event,"LepGood","nLepGood",8)
        for l in lep:
            self.t.sip3d = ROOT.scaleSip3dMC(l.sip3d, l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny) if self.corr else l.sip3d
            self.t.dz    = ROOT.scaleDzMC(   l.dz,    l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny) if self.corr else l.dz
            self.t.dxy   = ROOT.scaleDxyMC(  l.dxy,   l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny) if self.corr else l.dxy
            (dr,ptf) = (l.jetDR,l.jetPtRatio)
            self.t.jetDR  = ROOT.correctJetDRMC(dr,l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny)       if self.corr else dr
            self.t.jetPtRatio = ROOT.correctJetPtRatioMC(ptf,l.pdgId,l.pt,l.eta,l.mcMatchId,l.mcMatchAny) if self.corr else ptf
            self.t.jetBTagCSV = l.jetBTagCSV
            for C in self.copyvars: setattr(self.t, C, getattr(l,C))
            self.t.nJet25 = event.nJet25
            self.t.nBJetLoose25 = event.nBJetLoose25
            self.t.nBJetMedium25 = event.nBJetMedium25
            self.t.puWeight = event.puWeight
            self.t.fill()

from sys import argv
f = ROOT.TFile.Open(args[0]+"/"+options.tree+"/"+options.tree+"_tree.root")
t = f.Get(options.tree)
t.vectorTree = options.vectorTree
print "Reading %s (%d entries)" % (args[0], t.GetEntries())

booker = Booker(args[1] if len(argv) >= 3 else "lepTree.root")
prod = LepTreeProducer("rec",booker)
if len(args) >= 3 and args[2] == "NoCorr": 
    print "Will not apply corrections"
    prod.corr = False
else:
    print "Will apply corrections"
    prod.corr = True
el = EventLoop([ prod, ])
maxEv = (int(args[3]) if len(args) >= 4 else -1)
print "max entries: ",maxEv
el.loop([t], maxEvents=maxEv)
booker.done()
