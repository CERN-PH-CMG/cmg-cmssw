import ROOT
from CMGTools.HToZZTo4Leptons.tools.WeightCalculatorFromHistogram import WeightCalculatorFromHistogram as WeightCalculator
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath


class FakeRateCalculator(object):
    def __init__(self,cfg):
        self.fileMu = ROOT.TFile(getFullPath(cfg.muon))
        self.histMuZ1ee = self.fileMu.Get("eff_Z1ee_plus_muon")
        self.histMuUpZ1ee = self.fileMu.Get("effUp_Z1ee_plus_muon")
        self.histMuDownZ1ee = self.fileMu.Get("effDwn_Z1ee_plus_muon")
        self.weightMuZ1ee    = WeightCalculator(self.histMuZ1ee)
        self.weightMuUpZ1ee  = WeightCalculator(self.histMuUpZ1ee)
        self.weightMuDwnZ1ee = WeightCalculator(self.histMuDownZ1ee)
        self.histMuZ1mm = self.fileMu.Get("eff_Z1mumu_plus_muon")
        self.histMuUpZ1mm = self.fileMu.Get("effUp_Z1mumu_plus_muon")
        self.histMuDownZ1mm = self.fileMu.Get("effDwn_Z1mumu_plus_muon")
        self.weightMuZ1mm    = WeightCalculator(self.histMuZ1mm)
        self.weightMuUpZ1mm  = WeightCalculator(self.histMuUpZ1mm)
        self.weightMuDwnZ1mm = WeightCalculator(self.histMuDownZ1mm)

        self.fileEle = ROOT.TFile(getFullPath(cfg.electron))
        self.histEleZ1ee = self.fileEle.Get("eff_Z1ee_plus_electron")
        self.histEleUpZ1ee = self.fileEle.Get("effUp_Z1ee_plus_electron")
        self.histEleDownZ1ee = self.fileEle.Get("effDwn_Z1ee_plus_electron")
        self.weightEleZ1ee    = WeightCalculator(self.histEleZ1ee)
        self.weightEleUpZ1ee  = WeightCalculator(self.histEleUpZ1ee)
        self.weightEleDwnZ1ee = WeightCalculator(self.histEleDownZ1ee)
        self.histEleZ1mm = self.fileEle.Get("eff_Z1mumu_plus_electron")
        self.histEleUpZ1mm = self.fileEle.Get("effUp_Z1mumu_plus_electron")
        self.histEleDownZ1mm = self.fileEle.Get("effDwn_Z1mumu_plus_electron")
        self.weightEleZ1mm    = WeightCalculator(self.histEleZ1mm)
        self.weightEleUpZ1mm  = WeightCalculator(self.histEleUpZ1mm)
        self.weightEleDwnZ1mm = WeightCalculator(self.histEleDownZ1mm)
        self.name = cfg.name

    def attachToObject(self,object, iZ1flavour):
        if abs(object.pdgId()) ==11:
		if abs(iZ1flavour) == 11:	# Z1 to ee
                    w = self.weightEleZ1ee.getWeight(object.pt(),abs(object.eta()))
                    wU = self.weightEleUpZ1ee.getWeight(object.pt(),abs(object.eta()))
                    wD = self.weightEleDwnZ1ee.getWeight(object.pt(),abs(object.eta()))
		elif abs(iZ1flavour) == 13:	# Z1 to mumu
                    w = self.weightEleZ1mm.getWeight(object.pt(),abs(object.eta()))
                    wU = self.weightEleUpZ1mm.getWeight(object.pt(),abs(object.eta()))
                    wD = self.weightEleDwnZ1mm.getWeight(object.pt(),abs(object.eta()))
		else:	
		    return
                setattr(object,self.name,w)
                setattr(object,self.name+"Up",wU)
                setattr(object,self.name+"Dwn",wD)

        if abs(object.pdgId()) ==13:
		if abs(iZ1flavour) == 11:    # Z1 to ee
                    w = self.weightMuZ1ee.getWeight(object.pt(),abs(object.eta()))
                    wU = self.weightMuUpZ1ee.getWeight(object.pt(),abs(object.eta()))
                    wD = self.weightMuDwnZ1ee.getWeight(object.pt(),abs(object.eta()))
		elif abs(iZ1flavour) == 13:
                    w = self.weightMuZ1mm.getWeight(object.pt(),abs(object.eta()))
                    wU = self.weightMuUpZ1mm.getWeight(object.pt(),abs(object.eta()))
                    wD = self.weightMuDwnZ1mm.getWeight(object.pt(),abs(object.eta()))
		else:
		    return
                setattr(object,self.name,w)
                setattr(object,self.name+"Up",wU)
                setattr(object,self.name+"Dwn",wD)
