#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class LepMVA_SF_one:
    def __init__(self,hists,what,etaEdge):
        #print "initializing %s %s from %s" % (what,etaEdge,hists.GetName())
        self._what = what
        self._etaEdge = etaEdge
        self._th1s = {}
        for X in "data", "mc":
            for B in "ptb", "pte":
                hist = hists.Get("%s_%s%s" % (what,X,B))
                if not bool(hist):
                    hists.ls()
                    raise RuntimeError, "Missing %s_%s%s in %s " % (what,X,B,hists.GetName())
                self._th1s[X+B] = hist.Clone("LepMVA_SF_one_"+hist.GetName())
                #print hists, hists.GetName(), hist
                #print "%s_%s%s(5) = %s = %s\n" % (what,X,B, hist.GetBinContent(5), self._th1s[X+B].GetBinContent(5))
                
    def fetch(self,hname,pt):
        h = self._th1s[hname]
        bin = max(1,min(h.GetNbinsX(), h.GetXaxis().FindBin(pt)))
        #print "fetching bin %d of %s for pt=%s; bin 5 has %s " % (bin,h.GetName(),pt,h.GetBinContent(5))
        return h.GetBinContent(bin)
    def __call__(self,lepton):
        B = "ptb" if abs(lepton.eta) < self._etaEdge else "pte"
        edata = self.fetch("data"+B, lepton.pt)
        emc   = self.fetch("mc"  +B, lepton.pt)
        sf = edata/emc
        #print "%s eff(%s) for id %d pt %.1f: data %.3f, mc %.3f --> sf %.3f %.3f" % (self._what,B,abs(lepton.pdgId),lepton.pt,edata,emc,sf,self._etaEdge)
        return sf
 
class LepMVA_SF_both:
    def __init__(self,hists_el,hists_mu,what):
        #print "reading EL from ",hists_el.GetName()
        #print "reading MU from ",hists_mu.GetName()
        self.sf_el = LepMVA_SF_one(hists_el,what,1.479)
        self.sf_mu = LepMVA_SF_one(hists_mu,what,1.5)
    def __call__(self,lepton):
        sf = self.sf_mu if abs(lepton.pdgId) == 13 else self.sf_el
        return sf(lepton)

class LepMVA_SF_Event:
    def __init__(self,hists_el,hists_mu,what):
        self.sf = LepMVA_SF_both(hists_el,hists_mu,what)
    def __call__(self,event,nlep):
        leps = Collection(event,"LepGood","nLepGood",4)
        sfev = 1.0
        for i,l in enumerate(leps):
            if i >= nlep: break
            sfev *= self.sf(l)
        return sfev

class AllLepSFs:
    def __init__(self):
        self.f_el = ROOT.TFile("%s/src/CMGTools/TTHAnalysis/data/MVAandTigthChargeSF_ele.root" % os.environ['CMSSW_BASE'])
        self.f_mu = ROOT.TFile("%s/src/CMGTools/TTHAnalysis/data/MVAandTigthChargeSF_mu.root"  % os.environ['CMSSW_BASE'])
        self.sf_mvaLoose = LepMVA_SF_Event(self.f_el,self.f_mu,"LepMVALooseEff")
        self.sf_mvaTight = LepMVA_SF_Event(self.f_el,self.f_mu,"LepMVATightEff")
        self.sf_tightCharge = LepMVA_SF_Event(self.f_el,self.f_mu,"TightChargeEff")
    def listBranches(self):
        return [ 'LepMVALoose_2l', 'LepMVALoose_3l', 'LepMVALoose_4l', 'LepMVATight_2l', 'LepTightCharge_2l' ]
    def __call__(self,event):
        return {
            'LepMVALoose_2l' : self.sf_mvaLoose(event,2), 
            'LepMVALoose_3l' : self.sf_mvaLoose(event,3), 
            'LepMVALoose_4l' : self.sf_mvaLoose(event,4), 
            'LepMVATight_2l' : self.sf_mvaTight(event,2), 
            'LepTightCharge_2l' : self.sf_tightCharge(event,2),
        }

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("ttHLepTreeProducerBase")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = AllLepSFs()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 10)
