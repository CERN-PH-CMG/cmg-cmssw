#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
from CMGTools.MonoXAnalysis.tools.eventVars_monojet import EventVarsMonojet
import re,os

class Lep_SF_one:
    def __init__(self,h2d):
        self.h2d = h2d.Clone("Lep_one_"+h2d.GetName())
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

class Lep_SF_both:
    def __init__(self,hists_el,hists_mu,what):
            self.sf_el = Lep_SF_one(hists_el.Get(what))
            self.sf_mu = Lep_SF_one(hists_mu.Get(what))
    def __call__(self,lepton):
        sf = self.sf_mu if abs(lepton.pdgId) == 13 else self.sf_el
        return sf(lepton)

class Lep_SF_Event:
    def __init__(self,evtSel,hists_el,hists_mu,tight,loose=None):
        self.sf_tight = Lep_SF_both(hists_el,hists_mu,tight)
        self.evtSel = evtSel
        if loose != None: 
            self.sf_loose = Lep_SF_both(hists_el,hists_mu,loose)
    def __call__(self,event,nlep,flatSyst=0.0):
        leps = Collection(event,"LepGood","nLepGood",2)
        sfev = 1.0
        # the following is not completely correct for asymmetric selection, it should be the full combinatorics
        if self.sf_loose != None:
            for i,l in enumerate(leps):
                if i >= nlep: break
                if self.evtSel.lepIdTight(l): sfev *= self.sf_tight(l) * (1+flatSyst)
                elif self.evtSel.lepIdVeto(l): sfev *= self.sf_loose(l) * (1+flatSyst)
                else: sfev *= 1.0
        else:
            for i,l in enumerate(leps):
                if i >= nlep: break
                sfev *= self.sf_tight(l)
        return sfev

class AllLepSFs:
    def __init__(self):
        self.f_el = ROOT.TFile("/afs/cern.ch/work/e/emanuele/public/monox/leptonsf/eff_electron.root")
        self.f_mu = ROOT.TFile("/afs/cern.ch/work/e/emanuele/public/monox/leptonsf/eff_muon.root")
        self.evtSel = EventVarsMonojet()
        self.sf_TightLoose = Lep_SF_Event(self.evtSel,self.f_el,self.f_mu,"LepTightSF1","LepLooseSF1")
        self.sf_Tight = Lep_SF_Event(self.evtSel,self.f_el,self.f_mu,"LepTightSF1")
        self.f_el.Close()
        self.f_mu.Close()
    def listBranches(self):
        return [ 'LepTightLoose', 'LepTight', 'LepTightLooseUp', 'LepTightUp', 'LepTightLooseDown', 'LepTightDown' ]
    def __call__(self,event):
        return {
            'LepTightLoose' : self.sf_TightLoose(event,2),
            'LepTight' : self.sf_TightLoose(event,1),
            'LepTightLooseUp' : self.sf_TightLoose(event,2,0.02),
            'LepTightUp' : self.sf_TightLoose(event,1,0.02),
            'LepTightLooseDown' : self.sf_TightLoose(event,2,-0.02),
            'LepTightDown' : self.sf_TightLoose(event,1,-0.02),
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
