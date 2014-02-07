from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

# loadLibs()

gSystem.Load("libCMGToolsH2TauTau")

from ROOT import BTagEfficiency as BTagEfficiency_CC
from ROOT import BTagWeight as BTagWeight_CC

class BTagEfficiency:
    def __init__ (self) :
        self.BTagEfficiencyPy = BTagEfficiency_CC()

class BTagWeight:
    def __init__ (self) :
        self.BTagWeightPy = BTagWeight_CC()
