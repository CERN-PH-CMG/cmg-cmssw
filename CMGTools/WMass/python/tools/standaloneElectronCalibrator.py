import ROOT
import os, types
from ROOT import TRandom3
from math import *

class ElectronCalibrator:
    def __init__(self,isMC,correctionsFile):
        self.electronEnergyCalibrator = ROOT.ElectronEnergyCalibratorRun2(1 if isMC else 0, # isMC
                                                                          True, #sync: trick to apply only scale corrections to data
                                                                          correctionsFile)
        self.isMC = isMC
    def correct(self,electron,run):
        oldMomentum = ROOT.TLorentzVector()
        oldMomentum.SetPtEtaPhiM(electron.pt,electron.eta,electron.phi,0.51e-3)
        p = oldMomentum.E()
        momentum = max(15., p); #(combinedMomentum<15. ? 15. : combinedMomentum);
        if abs(electron.eta)<1.479 :
            parEB = ( 5.24e-02, 2.01e-01, 1.00e-02 );
            combinedMomentumError = momentum * sqrt( pow(parEB[0]/sqrt(momentum),2) + pow(parEB[1]/momentum,2) + pow(parEB[2],2) );
        else:
            parEE = ( 1.46e-01, 9.21e-01, 1.94e-03 ) ;
            combinedMomentumError = momentum * sqrt( pow(parEE[0]/sqrt(momentum),2) + pow(parEE[1]/momentum,2) + pow(parEE[2],2) );
        mySimpleElectron = ROOT.SimpleElectron(run, 
                                               electron.classification,
                                               electron.r9,
                                               electron.ecalEnergy,
                                               electron.correctedEcalEnergyError,
                                               electron.superCluster_energy / electron.eSuperClusterOverP, # trackMomentumAtVtx,
                                               electron.correctedEcalEnergyError, # dummy, should be trk momentum error, no combination done here
                                               p,
                                               electron.regressionEnergyError,
                                               p,
                                               combinedMomentumError,
                                               electron.scEta,
                                               abs(electron.eta)<1.479,
                                               1 if run==1 else 0,
                                               1, # dummy, ecalDrivenSeed
                                               0) # dummy trackerDrivenSeed
        self.electronEnergyCalibrator.calibrate(mySimpleElectron)
        scale = mySimpleElectron.getNewEnergy()/p
        electron.pt = electron.pt * scale
        electron.pterr = mySimpleElectron.getNewEnergyError()/cosh(abs(electron.eta))

