#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class xsec_KF:
    def getHistosFromFile(self, systematic):
        hnlo = self.tfile.Get("znlo012/znlo012_%s" % systematic).Clone("znlo")
        hlo = self.tfile.Get("zlo/zlo_%s" % systematic).Clone("zlo")
        self.histos[23] = [hnlo,hlo]
        hnlo = self.tfile.Get("wnlo012/wnlo012_%s" % systematic).Clone("vnlo")
        hlo = self.tfile.Get("wlo/wlo_%s" % systematic).Clone("vlo")
        self.histos[24] = [hnlo,hlo]
        hnlo = self.tfile.Get("anlo1/anlo1_%s" % systematic).Clone("vnlo")
        hlo = self.tfile.Get("zlo/zlo_%s" % systematic).Clone("vlo")
        self.histos[22] = [hnlo,hlo]
        for k,v in self.histos.iteritems():
            for h in v: h.SetDirectory(None)
        self.tfile.Close()

    def getEvKF(self, pdgId, pt, systematic="nominal"):
        histos = self.histos.get(pdgId)
        if histos==None:
            print "WARNING! No histograms fetched from file. Returning 1.0"
            return 1.0
        hnlo = histos[0] ; hlo = histos[1]
        ptmin = hnlo.GetXaxis().GetXmin()+1e-3
        ptmax = hnlo.GetXaxis().GetXmax()-1e-3
        bin = hnlo.GetXaxis().FindBin(min(max(pt,ptmin),ptmax))
        return hnlo.GetBinContent(bin) / hlo.GetBinContent(bin)

    def __init__(self, file, systematic) :
        self.tfile = ROOT.TFile(file)
        if not self.tfile or self.tfile.IsZombie():
            raise FileError("Could not open file {0}".format(file))
        self.systematic = systematic
        self.histos = {}
        self.getHistosFromFile(systematic)
        
    def listBranches(self):
        return [ 'NLO' ]
    def __call__(self,event):
        ret = {}
        genParts = Collection(event,"GenPart","nGenPart")
        searchedIds = [23,24]
        kf = 1.0
        for p in genParts:
            if abs(p.pdgId) in searchedIds and p.motherId == -9999: 
                kf = self.getEvKF(abs(p.pdgId),p.pt,self.systematic)
        ret['NLO'] = kf
        return ret
