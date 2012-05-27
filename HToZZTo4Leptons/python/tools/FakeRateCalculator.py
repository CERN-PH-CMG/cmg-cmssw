import ROOT
from CMGTools.HToZZTo4Leptons.tools.WeightCalculatorFromHistogram import WeightCalculatorFromHistogram as WeightCalculator
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath


class FakeRateCalculator(object):
    def __init__(self,cfg):
        print getFullPath(cfg.muon)
        self.fileMu = ROOT.TFile(getFullPath(cfg.muon))
        self.histMu = self.fileMu.Get("eff")
        self.histMuUp = self.fileMu.Get("effUp")
        self.histMuDown = self.fileMu.Get("effDwn")
        self.weightMu    = WeightCalculator(self.histMu)
        self.weightMuUp  = WeightCalculator(self.histMuUp)
        self.weightMuDwn = WeightCalculator(self.histMuDown)

        self.fileEle = ROOT.TFile(getFullPath(cfg.electron))
        self.histEle = self.fileEle.Get("eff")
        self.histEleUp = self.fileEle.Get("effUp")
        self.histEleDown = self.fileEle.Get("effDwn")
        self.weightEle    = WeightCalculator(self.histEle)
        self.weightEleUp  = WeightCalculator(self.histEleUp)
        self.weightEleDwn = WeightCalculator(self.histEleDown)

        self.fileMuFSR = ROOT.TFile(getFullPath(cfg.muonFSR))
        self.histMuFSR = self.fileMuFSR.Get("eff")
        self.histMuFSRUp = self.fileMuFSR.Get("effUp")
        self.histMuFSRDown = self.fileMuFSR.Get("effDwn")
        self.weightMuFSR    = WeightCalculator(self.histMuFSR)
        self.weightMuFSRUp  = WeightCalculator(self.histMuFSRUp)
        self.weightMuFSRDwn = WeightCalculator(self.histMuFSRDown)

        self.fileEleFSR = ROOT.TFile(getFullPath(cfg.electronFSR))
        self.histEleFSR = self.fileEleFSR.Get("eff")
        self.histEleFSRUp = self.fileEleFSR.Get("effUp")
        self.histEleFSRDown = self.fileEleFSR.Get("effDwn")
        self.weightEleFSR    = WeightCalculator(self.histEleFSR)
        self.weightEleFSRUp  = WeightCalculator(self.histEleFSRUp)
        self.weightEleFSRDwn = WeightCalculator(self.histEleFSRDown)

        self.name = cfg.name

    def attachToObject(self,object):
        if abs(object.pdgId()) ==11:
            if hasattr(object,'fsrPhoton'):
                w = self.weightEleFSR.getWeight(object.pt(),abs(object.eta()))
                wU = self.weightEleFSRUp.getWeight(object.pt(),abs(object.eta()))
                wD = self.weightEleFSRDwn.getWeight(object.pt(),abs(object.eta()))
                setattr(object,self.name,w)
                setattr(object,self.name+"Up",wU)
                setattr(object,self.name+"Dwn",wD)
            else:   
                w = self.weightEle.getWeight(object.pt(),abs(object.eta()))
                wU = self.weightEleUp.getWeight(object.pt(),abs(object.eta()))
                wD = self.weightEleDwn.getWeight(object.pt(),abs(object.eta()))
                setattr(object,self.name,w)
                setattr(object,self.name+"Up",wU)
                setattr(object,self.name+"Dwn",wD)

        if abs(object.pdgId()) ==13:
            if hasattr(object,'fsrPhoton'):
                w = self.weightMuFSR.getWeight(object.pt(),abs(object.eta()))
                wU = self.weightMuFSRUp.getWeight(object.pt(),abs(object.eta()))
                wD = self.weightMuFSRDwn.getWeight(object.pt(),abs(object.eta()))
                setattr(object,self.name,w)
                setattr(object,self.name+"Up",wU)
                setattr(object,self.name+"Dwn",wD)
            else:   
                w = self.weightMu.getWeight(object.pt(),abs(object.eta()))
                wU = self.weightMuUp.getWeight(object.pt(),abs(object.eta()))
                wD = self.weightMuDwn.getWeight(object.pt(),abs(object.eta()))
                setattr(object,self.name,w)
                setattr(object,self.name+"Up",wU)
                setattr(object,self.name+"Dwn",wD)
