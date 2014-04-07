from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

# loadLibs()

gSystem.Load("libCMGToolsH2TauTau")

from ROOT import VBFMVA as VBFMVA_CC
from ROOT import VBFMVA2012 as VBFMVA2012_CC

class VBFMVA:
    def __init__ (self, weightFile) :
        self.vbfMvaCalc = VBFMVA_CC(weightFile) 

class VBFMVA2012:
    def __init__ (self, weightFile) :
        self.vbfMvaCalc = VBFMVA2012_CC(weightFile) 
