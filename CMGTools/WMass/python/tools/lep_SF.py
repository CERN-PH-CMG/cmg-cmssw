#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class Lep_SF_one:
    def __init__(self,hists):
        self.hists = []
        for h in hists:
            h2d = h.Clone("Lep_one_"+h.GetName())
            h2d.SetDirectory(None)
            self.hists.append(h2d)
    def fetch(self,pt,eta,pdgid):
        sfone = 1.0
        for h2d in self.hists:
            xmin = h2d.GetXaxis().GetXmin()+1e-3
            xmax = h2d.GetXaxis().GetXmax()-1e-3
            ymin = h2d.GetYaxis().GetXmin()+1e-3
            ymax = h2d.GetYaxis().GetXmax()-1e-3
            ybin = h2d.GetYaxis().FindBin(min(max(pt,ymin),ymax))
            if abs(pdgid)==11:
                xbin = h2d.GetXaxis().FindBin(min(max(eta,xmin),xmax))
            else:
                xbin = h2d.GetXaxis().FindBin(min(max(abs(eta),xmin),xmax))
            sfone *= h2d.GetBinContent(xbin,ybin)
        return sfone
    def __call__(self,lepton):        
        return self.fetch(lepton.pt, lepton.eta,lepton.pdgId)

class Lep_SF_both:
    def __init__(self,hists_el,hists_mu):
        thists_el = [hists_el[0].Get("EGamma_SF2D")]
        thists_mu = []
        thists_mu.append(hists_mu[0].Get("Lep_eff_IsoMu24_OR_IsoTkMu24_PtEtaBins_abseta_pt_ratio"))
        thists_mu.append(hists_mu[1].Get("Lep_eff_MC_NUM_TightID_DEN_genTracks_PAR_pt_eta_abseta_pt_ratio"))
        thists_mu.append(hists_mu[2].Get("Lep_eff_TightISO_TightID_pt_eta_abseta_pt_ratio"))
        self.sf_el = Lep_SF_one(thists_el)
        self.sf_mu = Lep_SF_one(thists_mu)
    def __call__(self,lepton):
        sf = self.sf_mu if abs(lepton.pdgId) == 13 else self.sf_el
        return sf(lepton)

class Lep_SF_Event:
    def __init__(self,hists_el,hists_mu):
        self.sf = Lep_SF_both(hists_el,hists_mu)
    def __call__(self,event,nlep,flatSyst=0.0):
        leps = Collection(event,"LepGood","nLepGood",2)
        sfev = 1.0
        for i,l in enumerate(leps):
            if i >= nlep: break
            sfev *= self.sf(l)
        return sfev

class AllLepSFs:
    def __init__(self):
        self.f_el = [ROOT.TFile("/afs/cern.ch/work/e/emanuele/public/wmass/leptonsf/eff_eleID_CutsID_Loose_Moriond17.root")]
        self.f_mu = [ROOT.TFile("/afs/cern.ch/work/e/emanuele/public/wmass/leptonsf/MuonTriggerEfficienciesAndSF_RunBtoH_Weighted.root"),
                     ROOT.TFile("/afs/cern.ch/work/e/emanuele/public/wmass/leptonsf/MuonIDEfficienciesAndSF_BCDEFGH_Weighted.root"),
                     ROOT.TFile("/afs/cern.ch/work/e/emanuele/public/wmass/leptonsf/MuonIsoEfficienciesAndSF_BCDEFGH_Weighted.root") ] 
        self.sf_Tight = Lep_SF_Event(self.f_el,self.f_mu)
        f_lep = self.f_el + self.f_mu
        for f in f_lep: f.Close()
    def listBranches(self):
        return [ 'LepTight_1l', 'LepTight_2l' ]
    def __call__(self,event):
        return {
            'LepTight_1l' : self.sf_Tight(event,1),
            'LepTight_2l' : self.sf_Tight(event,2),
        }

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf = AllLepSFs()
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: leps %d" % (ev.run, ev.lumi, ev.evt, ev.nLepGood)
            print self.sf(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 10)
