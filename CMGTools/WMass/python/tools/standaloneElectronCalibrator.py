import ROOT
import os, types
import numpy as np
from ROOT import TRandom3
from math import *

class ElectronCalibrator:
    def __init__(self,isMC,correctionsFile,scaleSystFile=None):
        self.electronEnergyCalibrator = ROOT.ElectronEnergyCalibratorRun2(1 if isMC else 0, # isMC
                                                                          True, #sync: trick to apply only scale corrections to data
                                                                          correctionsFile)
        self.isMC = isMC
        if scaleSystFile!=None: 
            self.setSystFile(scaleSystFile)

    def getBin(self, bvec, val):
        return int(bvec.searchsorted(val, side="right")) - 1

    def setSystFile(self,filename):
        if not os.path.exists(filename):
            print "WARNING! standaloneElectronCalibrator systematic file ",filename," does not exist!"
            return
        lines = [line.rstrip('\n') for line in open(filename)]
        pts=[]; etas=[]
        for l in lines:
            fields=l.split()
            if float(fields[1]) not in pts: pts.append(float(fields[1]))
            if float(fields[2]) not in pts: pts.append(float(fields[2]))
            if float(fields[4]) not in etas: etas.append(float(fields[4]))
            if float(fields[5]) not in etas: etas.append(float(fields[5]))
        self.pt_bins=np.array(pts)
        self.eta_bins=np.array(etas)
        self.scaleSystMap={}
        for l in lines:
            fields=l.split()
            ptbin=self.getBin(self.pt_bins,float(fields[1]))
            etabin=self.getBin(self.eta_bins,float(fields[4]))
            self.scaleSystMap[(ptbin,etabin)]=float(fields[6])

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

    def getOneSigmaScaleFromClosure(self,electron):
        if not hasattr(self,'scaleSystMap'): 
            print "ERROR! Scale systematics not set! Return 0!"
            return 0
        k = (self.getBin(self.pt_bins,electron.pt),self.getBin(self.eta_bins,abs(electron.eta)))
        if k in self.scaleSystMap: return self.scaleSystMap[k]
        else: 
            print "electron with pT,eta = ",electron.pt,electron.eta," do not fit in the systematics map. Return 0 systematic"
            return 0.
