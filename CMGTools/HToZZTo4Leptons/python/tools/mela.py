from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

# loadLibs()

gSystem.Load("libCMGToolsHToZZTo4Leptons")

from ROOT import MELAAngles
# need to make a MELAEstimator on the same model

# you can even create a python class for high level stuff
class LvMELAAngles(MELAAngles):
    def __init__(self, lv1, lv2, lv3, lv4):
        print lv1.Pt()
        # here you could pass the variables necessary to the base class,
        # to its constructor.
        super(LvMELAAngles, self).__init__()

if __name__ == '__main__':

    from ROOT import TLorentzVector
    lv1 = TLorentzVector(1,2,3,4)
    melaAngles = LvMELAAngles(lv1, lv1, lv1, lv1)
    
