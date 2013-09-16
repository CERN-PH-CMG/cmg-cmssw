import FWCore.ParameterSet.Config as cms

from EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi import *

from ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff import *

from RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi import *

##CGIT
mvaIDSequence = cms.Sequence(
     mvaTrigV0 +
     mvaNonTrigV0
     )

electronIDSequence = cms.Sequence(
    eidVeryLoose + 
    eidLoose +              
    eidMedium +
    eidTight +
    eidSuperTight
     )

patElectronIDSequence = cms.Sequence(
    mvaIDSequence +
    simpleEleIdSequence + 
    electronIDSequence
    ) 


