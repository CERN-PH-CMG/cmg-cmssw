from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

gSystem.Load("libCMGToolsH2TauTau")

from ROOT import BtagSF as BtagSF_CC

class BtagSF:
    def __init__ (self, seed) :
        self.BtagSFcalc = BtagSF_CC(seed) 

