#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class FR2D_one:
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

class FR2D:
    def __init__(self,frEle,frMu):
        self.frEle = FR2D_one(frEle)
        self.frMu = FR2D_one(frMu)
    def __call__(self,pdgId,pt,eta):
        fr = self.frEle if abs(pdgId) == 11 else self.frMu
        return fr.fetch(pt, eta)
 

class FRWeightBase(object):
    def __init__(self,label,histFile,frName,frProcess):
        self.label = label
        tfile = ROOT.TFile.Open(os.path.expandvars(histFile.replace("$DATA","$CMSSW_BASE/src/CMGTools/HToZZ4L/data")))
        frMu = tfile.Get(frName.format(l="mu", proc=frProcess))
        frEl = tfile.Get(frName.format(l="el", proc=frProcess))
        self._FR2D = FR2D(frEl,frMu)
        tfile.Close()
    def listBranches(self):
        return [ 'FRWeight_'+self.label ]
    def __call__(self,event):
        return { 'FRWeight_'+self.label : self.weight(event) }
 
    

class FRWeight1_ZSS(FRWeightBase):
    def __init__(self,label,histFile,frName,frProcess):      
        FRWeightBase.__init__(self,label,histFile,frName,frProcess)
    def weight(self,event):
        if event.nzzSS == 0: return 0  
        zzSS1 = Object(event, "zzSS1")
        fr1 = self._FR2D(zzSS1.z2_l1_pdgId, zzSS1.z2_l1_pt, zzSS1.z2_l1_eta)
        fr2 = self._FR2D(zzSS1.z2_l2_pdgId, zzSS1.z2_l2_pt, zzSS1.z2_l2_eta)
        return fr1*fr2

def _zzLepPass(lep):
       return lep.tightId and lep.relIsoAfterFSR < (0.4 if abs(lep.pdgId)==13 else 0.5)

class FRWeight1_Z2P2F(FRWeightBase):
    def __init__(self,label,histFile,frName,frProcess):      
        FRWeightBase.__init__(self,label,histFile,frName,frProcess)
    def weight(self,event):
        if event.nzz2P2F > 0: 
            zz2P2F1 = Object(event, "zz2P2F1")
            fr1 = self._FR2D(zz2P2F1.z2_l1_pdgId, zz2P2F1.z2_l1_pt, zz2P2F1.z2_l1_eta)
            fr2 = self._FR2D(zz2P2F1.z2_l2_pdgId, zz2P2F1.z2_l2_pt, zz2P2F1.z2_l2_eta)
            return -fr1*fr2/((1-fr1)*(1-fr2))
        else:
            return 0

class FRWeight1_Z3P1F(FRWeightBase):
    def __init__(self,label,histFile,frName,frProcess):      
        FRWeightBase.__init__(self,label,histFile,frName,frProcess)
    def weight(self,event):
        if event.nzz3P1F > 0: 
            zz3P1F1 = Object(event, "zz3P1F1")
            leps = [ zz3P1F1.subObj("z2_l1"), zz3P1F1.subObj("z2_l2") ]
            fail = leps[0] if _zzLepPass(leps[1]) else leps[1]
            fr = self._FR2D(fail.pdgId, fail.pt, fail.eta)
            return fr/(1.0-fr)
        else:
            return 0


MODULES = [
  ( 'FR_ZSS_DY',   lambda : FRWeight1_ZSS("DYSS", "$DATA/FR_zl_10GeV.root", "FR_{l}_{proc}", "DY") ),
  ( 'FR_ZSS_data', lambda : FRWeight1_ZSS("SS",   "$DATA/FR_zl_10GeV.root", "FR_{l}_{proc}", "data") ),
  ( 'FR_Z2P2F_DY',   lambda : FRWeight1_Z2P2F("DY2P2F", "$DATA/FR_zl_10GeV.root", "FR_{l}_{proc}", "DY") ),
  ( 'FR_Z3P1F_DY',   lambda : FRWeight1_Z3P1F("DY3P1F", "$DATA/FR_zl_10GeV.root", "FR_{l}_{proc}", "DY") ),
  ( 'FR_Z2P2F_data', lambda : FRWeight1_Z2P2F("2P2F", "$DATA/FR_zl_10GeV.root", "FR_{l}_{proc}", "data") ),
  ( 'FR_Z3P1F_data', lambda : FRWeight1_Z3P1F("3P1F", "$DATA/FR_zl_10GeV.root", "FR_{l}_{proc}", "data") ),
] 

if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("tree")
    class Tester(Module):
        def __init__(self, name):
            Module.__init__(self,name,None)
            self.sf1 = FRWeight1_ZSS("DYSS", "$DATA/FR_zl_10GeV.root", "FR_{l}_{proc}", "DY")
            self.sf2 = FRWeight1_ZPF("DYPF", "$DATA/FR_zl_10GeV.root", "FR_{l}_{proc}", "DY")
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: 2P2F %d 3P1F %d SS %d " % (ev.run, ev.lumi, ev.evt, ev.nzz2P2F, ev.nzz3P1F, ev.nzzSS)
            print self.sf1(ev)
            print self.sf2(ev)
    el = EventLoop([ Tester("tester") ])
    el.loop([tree], maxEvents = 10)
