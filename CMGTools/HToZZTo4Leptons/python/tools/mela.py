from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

# loadLibs()

gSystem.Load("libCMGToolsHToZZTo4Leptons")

from ROOT import MELAAngles
# need to make a MELAEstimator on the same model


if __name__ == '__main__':

    melaAngles = MELAAngles()

