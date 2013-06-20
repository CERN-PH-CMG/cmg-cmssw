import ROOT
from CMGTools.HToZZTo4Leptons.tools.WeightCalculatorFromHistogram import WeightCalculatorFromHistogram as WeightCalculator
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath


class EfficiencyCorrector(object):
    def __init__(self,cfg):
        self.fileMu = ROOT.TFile(getFullPath(cfg.muonFile))
        self.histMu = self.fileMu.Get(cfg.muonHisto)
        self.fileEle = ROOT.TFile(getFullPath(cfg.eleFile))
        self.histEle = self.fileEle.Get(cfg.eleHisto)
        self.name = cfg.name
        self.weightMu     = WeightCalculator(self.histMu)
        self.weightEle    = WeightCalculator(self.histEle)


    def attachToObject(self,object):
            w = self.weightMu.getWeight(object.pt(),(object.eta()))
            wU =w+ self.weightMu.getWeightErr(object.pt(),(object.eta()))
            wD =w- self.weightMu.getWeightErr(object.pt(),(object.eta()))
            setattr(object,self.name,w)
            setattr(object,self.name+"Up",wU)
            setattr(object,self.name+"Dwn",wD)
