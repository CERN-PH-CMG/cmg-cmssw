#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class xsec_KF:

    def getQCDHistosFromFile(self, systematics):
        ret = {}
        bosons = {"z":23, "w":24}
        for k,v in bosons.iteritems():
            for s in systematics:
                hnlo = self.tfile.Get("%snlo012/%snlo012_%s" % (k,k,s)).Clone("kfac")
                hlo = self.tfile.Get("%slo/%slo_nominal" % (k,k)).Clone()
                hnlo.Divide(hlo)
                hnlo.SetDirectory(None)
                ret[(v,s)] = hnlo
        return ret

    def getEWKHistosFromFile(self):
        ret = {}
        bosons = {"z":23, "w":24}
        for k,v in bosons.iteritems():
            kFac = self.tfile.Get("%s_ewkcorr/%s_ewkcorr" % (k,k)).Clone("kfac")
            kFac.SetDirectory(None)
            ret[v] = kFac
        return ret

    def getEvKF_QCD(self, pdgId, pt, systematic="nominal"):
        k = (pdgId,systematic)
        histo = self.kfacQCD.get(k)
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

    def getEvKF_EWK(self, pdgId, pt, systematic="nominal"):
        histo = self.kfacEWK.get(pdgId)
        if histo==None:
            print "WARNING! No histograms fetched from file. Returning 1.0"
            return 1.0
        # ptmin = histo.GetXaxis().GetXmin()+1e-3
        # the k-factor for pT < 200 GeV are very large,
        # limited by the stat of the HT binned samples
        ptmin = 200 
        ptmax = histo.GetXaxis().GetXmax()-1e-3
        bin = histo.GetXaxis().FindBin(min(max(pt,ptmin),ptmax))
        # systematics for missing higher order are taken 100% of the correction
        if systematic == "up": return 2*histo.GetBinContent(bin) - 1.0
        elif systematic == "down": return 1.0
        return histo.GetBinContent(bin)

    def __init__(self, file) :
        self.qcdsysts = ['NLO_QCD', 'NLO_QCD_renScaleUp', 'NLO_QCD_renScaleDown', 'NLO_QCD_facScaleUp', 'NLO_QCD_facScaleDown', 'NLO_QCD_pdfUp', 'NLO_QCD_pdfDown']
        self.qcdhists = ['nominal', 'scale2010', 'scale0510', 'scale1020', 'scale1005', 'pdfUp', 'pdfDown']
        self.ewksysts = ["NLO_EWK","NLO_EWK_up","NLO_EWK_down"]
        self.ewkhists = ["nominal","up","down"]
        self.branches = self.qcdsysts + self.ewksysts
        self.tfile = ROOT.TFile(file)
        if not self.tfile or self.tfile.IsZombie():
            raise FileError("Could not open file {0}".format(file))
        self.kfacQCD = self.getQCDHistosFromFile(self.qcdhists)
        self.kfacEWK = self.getEWKHistosFromFile()
        self.tfile.Close()
        
    def listBranches(self):
        return self.branches[:]

    def __call__(self,event):
        ret = {}
        genParts = Collection(event,"GenPart","nGenPart")
        searchedIds = [23,24]
        for k in self.branches:
            ret[k] = 1.0
        qcddic = dict(zip(self.qcdsysts,self.qcdhists))
        ewkdic = dict(zip(self.ewksysts,self.ewkhists))
        for p in genParts:
            if abs(p.pdgId) in searchedIds and p.motherId == -9999: 
                for k,v in qcddic.iteritems():
                    ret[k] = self.getEvKF_QCD(abs(p.pdgId),p.pt,v)
                for k,v in ewkdic.iteritems():
                    ret[k] = self.getEvKF_EWK(abs(p.pdgId),p.pt,v)
                break
        return ret
