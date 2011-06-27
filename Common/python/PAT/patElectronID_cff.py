import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff import *

from RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi import *

electronIDSequence = cms.Sequence(
    eidVeryLoose + 
    eidLoose +                
    eidMedium +
    eidTight +
    eidSuperTight
    )

patElectronIDSequence = cms.Sequence( simpleEleIdSequence + electronIDSequence ) 


