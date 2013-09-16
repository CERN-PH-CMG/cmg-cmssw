import FWCore.ParameterSet.Config as cms

##CGIT from EGamma.EGammaAnalysisTools.electronIdMVAProducer_cfi import *

from ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff import *

from RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi import *

##CGIT
## mvaIDSequence = cms.Sequence(
##     CGIT mvaTrigV0 +
##     mvaNonTrigV0
## )

electronIDSequence = cms.Sequence(
    eidVeryLoose + 
    eidLoose +              
    eidMedium +
    eidTight +
    eidSuperTight
     )

patElectronIDSequence = cms.Sequence(
    ##CGIT mvaIDSequence +
    simpleEleIdSequence + 
    electronIDSequence
    ) 


