#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class xsec_KF:
    def getHistosFromFile(self, systematic):
        hnloQCD = self.tfile.Get("znlo012/znlo012_%s" % systematic).Clone("znlo")
        hloQCD = self.tfile.Get("zlo/zlo_%s" % systematic).Clone("zlo")
        hcorrEWK = self.tfile.Get("z_ewkcorr/z_ewkcorr").Clone("zcorrEWK")
        self.histosQCD[23] = [hnloQCD,hloQCD]
        self.histoEWK[23] = hcorrEWK
        hnloQCD = self.tfile.Get("wnlo012/wnlo012_%s" % systematic).Clone("wnlo")
        hloQCD = self.tfile.Get("wlo/wlo_%s" % systematic).Clone("wlo")
        hcorrEWK = self.tfile.Get("w_ewkcorr/w_ewkcorr").Clone("wcorrEWK")
        self.histosQCD[24] = [hnloQCD,hloQCD]
        self.histoEWK[24] = hcorrEWK
        hnloQCD = self.tfile.Get("anlo1/anlo1_%s" % systematic).Clone("anlo")
        hloQCD = self.tfile.Get("zlo/zlo_%s" % systematic).Clone("zlo")
        hcorrEWK = self.tfile.Get("a_ewkcorr/a_ewkcorr").Clone("acorrEWK")
        self.histosQCD[22] = [hnloQCD,hloQCD]
        self.histoEWK[22] = hcorrEWK
        for k,v in self.histosQCD.iteritems():
            for h in v: h.SetDirectory(None)
        for k,v in self.histoEWK.iteritems():
            v.SetDirectory(None)
        self.tfile.Close()

    def getEvKF_QCD(self, pdgId, pt, systematic="nominal"):
        histos = self.histosQCD.get(pdgId)
        if histos==None:
            print "WARNING! No histograms fetched from file. Returning 1.0"
            return 1.0
        hnlo = histos[0] ; hlo = histos[1]
        # ptmin = hnlo.GetXaxis().GetXmin()+1e-3
        # the k-factor for pT < 200 GeV are very large,
        # limited by the stat of the HT binned samples
        ptmin = 200 
        ptmax = hnlo.GetXaxis().GetXmax()-1e-3
        bin = hnlo.GetXaxis().FindBin(min(max(pt,ptmin),ptmax))
        return hnlo.GetBinContent(bin) / hlo.GetBinContent(bin)

    def getEvKF_EWK(self, pdgId, pt):
        histo = self.histoEWK.get(pdgId)
        if histo==None:
            print "WARNING! No histograms fetched from file. Returning 1.0"
            return 1.0
        # ptmin = histo.GetXaxis().GetXmin()+1e-3
        # the k-factor for pT < 200 GeV are very large,
        # limited by the stat of the HT binned samples
        ptmin = 200 
        ptmax = histo.GetXaxis().GetXmax()-1e-3
        bin = histo.GetXaxis().FindBin(min(max(pt,ptmin),ptmax))
        return histo.GetBinContent(bin)

    def __init__(self, file, systematic) :
        self.tfile = ROOT.TFile(file)
        if not self.tfile or self.tfile.IsZombie():
            raise FileError("Could not open file {0}".format(file))
        self.systematic = systematic
        self.histosQCD = {}
        self.histoEWK = {}
        self.getHistosFromFile(systematic)
        
    def listBranches(self):
        return [ 'NLO_QCD', 'NLO_EWK' ]
    def __call__(self,event):
        ret = {}
        genParts = Collection(event,"GenPart","nGenPart")
        searchedIds = [23,24]
        kfqcd = 1.0
        kfewk = 1.0
        for p in genParts:
            if abs(p.pdgId) in searchedIds and p.motherId == -9999: 
                kfqcd = self.getEvKF_QCD(abs(p.pdgId),p.pt,self.systematic)
                kfewk = self.getEvKF_EWK(abs(p.pdgId),p.pt)
        ret['NLO_QCD'] = kfqcd
        ret['NLO_EWK'] = kfewk
        return ret
