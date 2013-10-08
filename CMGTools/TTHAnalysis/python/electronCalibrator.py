import ROOT
import os, types
from ROOT import SimpleElectron, ElectronEnergyCalibrator, ElectronEPcombinator, TRandom3
from math import *

class ElectronCalibrator:
    def __init__(self,isMC,reReco=False,correctionType=1,combinationType=1,verbose=0):
        self.correctionType  = correctionType
        self.combinationType = combinationType
        tag = "Summer12_DR53X_HCP2012" if isMC else "Moriond2013";
        base = "%s/src/" % os.environ['CMSSW_BASE'];
        pathData = base+"EgammaAnalysis/ElectronTools/data/scalesMoriond.csv"
        pathLin  = base+"EgammaAnalysis/ElectronTools/data/linearityNewReg-May2013.csv"
        self.electronEnergyCalibrator = ElectronEnergyCalibrator(
                                            pathData, pathLin,
                                            tag,    ## dataset  
                                            correctionType, 
                                            False,  # apply linearity
                                            0.607,  # smearing ratio
                                            1 if isMC else 0, # isMC
                                            True, # updateEnergyError,
                                            verbose, False, #verbose, sync
                                            TRandom3(0)) # random number generator
        self.myCombinator = ElectronEPcombinator()
        self.isMC = isMC
    def correct(self,cmgelectron,run,verbose=False):
        ele = cmgelectron.sourcePtr().get()
        elClass = ele.classification();
        combinedMomentumError = ele.p4Error(ele.candidateP4Kind())
        # FIXME : p4Error not filled for pure tracker electrons
        # Recompute it using the parametrization implemented in
        # RecoEgamma/EgammaElectronAlgos/src/ElectronEnergyCorrector.cc::simpleParameterizationUncertainty()
        if not ele.ecalDrivenSeed():
                momentum = max(15., ele.p()); #(combinedMomentum<15. ? 15. : combinedMomentum);
                if ele.isEB() :
                    parEB = ( 5.24e-02, 2.01e-01, 1.00e-02 );
                    combinedMomentumError = momentum * sqrt( pow(parEB[0]/sqrt(momentum),2) + pow(parEB[1]/momentum,2) + pow(parEB[2],2) );
                elif ele.isEE():
                    parEE = ( 1.46e-01, 9.21e-01, 1.94e-03 ) ;
                    combinedMomentumError = momentum * sqrt( pow(parEE[0]/sqrt(momentum),2) + pow(parEE[1]/momentum,2) + pow(parEE[2],2) );
        mySimpleElectron = SimpleElectron(
                    run,
                    ele.classification(),
                    ele.r9(),
                    ele.correctedEcalEnergy(),
                    ele.correctedEcalEnergyError(),
                    ele.trackMomentumAtVtx().R(),
                    ele.trackMomentumError(),
                    ele.p(),                            # regressionEnergy,
                    ele.p4Error(ele.candidateP4Kind()), # regressionEnergyError,
                    ele.p(),
                    combinedMomentumError,
                    ele.superCluster().eta(),
                    ele.isEB(),
                    1 if self.isMC else 0,
                    ele.ecalDriven(),
                    ele.trackerDrivenSeed());
        # energy calibration for ecalDriven electrons
        if ele.ecalDrivenSeed() or self.correctionType==2 or self.combinationType==3:
            self.electronEnergyCalibrator.calibrate(mySimpleElectron);
            # E-p combination
            if self.combinationType == 0:
                    if verbose: print "[CalibratedGsfElectronProducer] You choose not to combine."
            elif self.combinationType == 1:
                    if verbose: print "[CalibratedGsfElectronProducer] You choose corrected regression energy for standard combination"
                    self.myCombinator.setCombinationMode(1);
                    self.myCombinator.combine(mySimpleElectron);
            elif self.combinationType == 2:
                    if verbose: print "[CalibratedGsfElectronProducer] You choose uncorrected regression energy for standard combination" 
                    self.myCombinator.setCombinationMode(2);
                    self.myCombinator.combine(mySimpleElectron);
            elif self.combinationType == 3:
                    if verbose: print "[CalibratedGsfElectronProducer] You choose regression combination."
                    #self.myEpCombinationTool.combine(mySimpleElectron);
                    #self.theEnCorrector.correctLinearity(mySimpleElectron);
            else:
                raise RuntimeError, "CalibratedgsfElectronProducer|ConfigError: Unknown combination Type !!!" 
            oldMomentum = ele.p4(ele.P4_COMBINATION);
            newMomentum = ele.p4(ele.P4_COMBINATION);
            scale = mySimpleElectron.getCombinedMomentum()/oldMomentum.t()
            newMomentum.SetPxPyPzE(
               oldMomentum.x()*scale,
               oldMomentum.y()*scale,
               oldMomentum.z()*scale,
               mySimpleElectron.getCombinedMomentum()) 
            ele.correctMomentum(
               newMomentum,
               mySimpleElectron.getTrackerMomentumError(),
               mySimpleElectron.getCombinedMomentumError());
            cmgelectron.setP4(newMomentum)

