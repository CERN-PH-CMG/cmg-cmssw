from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

# loadLibs()

gSystem.Load("libCMGToolsH2TauTau")

from ROOT import VBFMVA as VBFMVA_CC

class VBFMVA:
    def __init__ (self, weightFile) :
        self.vbfMvaCalc = VBFMVA_CC(weightFile) 
