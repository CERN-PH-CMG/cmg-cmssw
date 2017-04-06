import ROOT
import os, types
from ROOT import TRandom3
from math import *

class ElectronCalibrator:
    def __init__(self,isMC,reReco=False,correctionType=1,combinationType=1,verbose=0):
        self.correctionType  = correctionType
        self.combinationType = combinationType
        tag = "Summer12_DR53X_HCP2012" if isMC else "Moriond2013";
        base = "%s/src/" % os.environ['CMSSW_BASE'];
        pathData = base+"EgammaAnalysis/ElectronTools/data/scalesMoriond.csv"
        pathLin  = base+"EgammaAnalysis/ElectronTools/data/linearityNewReg-May2013.csv"
        self.electronEnergyCalibrator = ROOT.ElectronEnergyCalibrator(pathData, pathLin,
                                                                      tag,    ## dataset  
                                                                      correctionType, 
                                                                      False,  # apply linearity
                                                                      0.607,  # smearing ratio
                                                                      1 if isMC else 0, # isMC
                                                                      True, # updateEnergyError,
                                                                      verbose, False, #verbose, sync
                                                                      TRandom3(0)) # random number generator
        self.isMC = isMC
    def getCorrectedP4(self,electron,p4,run,verbose=False):
        originalEle = electron
        # energy calibration for ecalDriven electrons
        self.electronEnergyCalibrator.calibrate(electron);
        oldMomentum = p4
        newMomentum = p4
        scale = electron.getNewEnergy()/oldMomentum.E()
        newMomentum.SetPxPyPzE(
            oldMomentum.Px()*scale,
            oldMomentum.Py()*scale,
            oldMomentum.Pz()*scale,
            electron.getCombinedMomentum()) 
        return newMomentum

