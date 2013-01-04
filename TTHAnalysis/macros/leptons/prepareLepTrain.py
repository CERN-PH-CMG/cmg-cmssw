#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *

class LepTreeProducer(Module):
    def beginJob(self):
        self.t = PyTree(self.book("TTree","t","t"))
        self.t.branch("dr_in","F")
        self.t.branch("ptf_in","F")
        self.t.branch("CSV_in","F")
        self.t.branch("dr_out","F")
        self.t.branch("ptf_out","F")
        self.t.branch("CSV_out","F")
        self.t.branch("good","I")
        self.t.branch("nJet25","I")
        self.t.branch("nBJetLoose25","I")
        self.t.branch("nBJetMedium25","I")
        self.copyvars = [ 'sip3d','dxy','dz','relIso','chargedIso','mvaId','pt','eta','pdgId','innerHits','tightId',"mva"]
        for C in self.copyvars: self.t.branch(C,"F")
    def analyze(self,event):
        if event.nLepGood < 3: return False
        jet = Collection(event,"Jet")
        lep = Collection(event,"LepGood","nLepGood",8)
        glep = Collection(event,"GenLep")
        for l in lep:
            (dr,ptf) = (l.jetDR,l.jetPtRatio)
            self.t.dr_in = dr
            self.t.ptf_in = ptf
            self.t.CSV_in = l.jetBTagCSV
            (j,dr) = closest(l, jet)
            ptot = j.p4() + l.p4()
            self.t.dr_out = dr
            self.t.ptf_out = l.pt/ptot.Pt()
            self.t.CSV_out = j.btagCSV
            if l.mcMatchId > 0:
                (gmatch,dr) = closest(l,glep)
                if dr < 0.3 and gmatch.pdgId == l.pdgId:
                    self.t.good = 2
                else:
                    self.t.good = 1
                self.t.fill()
            else: 
                self.t.good = -l.mcMatchAny
            for C in self.copyvars: setattr(self.t, C, getattr(l,C))
            self.t.nJet25 = event.nJet25
            self.t.nBJetLoose25 = event.nBJetLoose25
            self.t.nBJetMedium25 = event.nBJetMedium25
            self.t.fill()

from sys import argv
f = ROOT.TFile.Open(argv[1])
t = f.Get("ttHLepTreeProducerBase")
print "Reading %s (%d entries)" % (argv[1], t.GetEntries())

booker = Booker(argv[2] if len(argv) >= 3 else "lepTree.root")
el = EventLoop([ LepTreeProducer("rec",booker), ])
el.loop([t])
booker.done()
