#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class LepMVA_SF_one:
    def __init__(self,h2d):
        self.h2d = h2d.Clone("LepMVA_one_"+h2d.GetName())
        self.h2d.SetDirectory(None)
        self.ptmin = h2d.GetXaxis().GetXmin()+1e-3
        self.ptmax = h2d.GetXaxis().GetXmax()-1e-3
        self.etamin = h2d.GetYaxis().GetXmin()+1e-3
        self.etamax = h2d.GetYaxis().GetXmax()-1e-3
    def fetch(self,pt,eta):
        xbin = self.h2d.GetXaxis().FindBin(min(max(pt,self.ptmin),self.ptmax))
        ybin = self.h2d.GetYaxis().FindBin(min(max(eta,self.etamin),self.etamax))
        return self.h2d.GetBinContent(xbin,ybin)
    def __call__(self,lepton):
        return self.fetch(lepton.pt, lepton.eta)
 
class LepMVA_SF_both:
    def __init__(self,hists_el,hists_mu,what):
        self.sf_el = LepMVA_SF_one(hists_el.Get(what))
        self.sf_mu = LepMVA_SF_one(hists_mu.Get(what))
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
        self.sf_mvaLoose = LepMVA_SF_Event(self.f_el,self.f_mu,"LepMVALooseSF2D")
        self.sf_mvaTight = LepMVA_SF_Event(self.f_el,self.f_mu,"LepMVATightSF2D")
        self.sf_tightCharge = LepMVA_SF_Event(self.f_el,self.f_mu,"TightChargeSF2D")
        self.f_el.Close()
        self.f_mu.Close()
    def listBranches(self):
        return [ 'LepMVALoose_2l', 'LepMVALoose_3l', 'LepMVALoose_4l', 'LepMVATight_2l', 'LepMVATight_3l', 'LepTightCharge_2l', 'LepTightCharge_3l' ]
    def __call__(self,event):
        return {
            'LepMVALoose_2l' : self.sf_mvaLoose(event,2), 
            'LepMVALoose_3l' : self.sf_mvaLoose(event,3), 
            'LepMVALoose_4l' : self.sf_mvaLoose(event,4), 
            'LepMVATight_2l' : self.sf_mvaTight(event,2), 
            'LepMVATight_3l' : self.sf_mvaTight(event,2), 
            'LepTightCharge_2l' : self.sf_tightCharge(event,2),
            'LepTightCharge_3l' : self.sf_tightCharge(event,2),
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
