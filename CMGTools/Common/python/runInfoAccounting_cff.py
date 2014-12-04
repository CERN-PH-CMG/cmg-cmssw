import FWCore.ParameterSet.Config as cms


from GeneratorInterface.Core.genFilterEfficiencyProducer_cfi import *
preSelFilterEfficiencyProducer = genFilterEfficiencyProducer.clone()
preSelFilterEfficiencyProducer.filterPath = 'p'

from CMGTools.Common.runInfoAccounting.runInfoAccounting_cfi import runInfoAccounting

runInfoAccountingSequence = cms.Sequence(
    preSelFilterEfficiencyProducer + 
    runInfoAccounting
    )

from CMGTools.Common.runInfoAccounting.runInfoAccountingData_cfi import runInfoAccountingData

runInfoAccountingDataSequence = cms.Sequence(
    preSelFilterEfficiencyProducer + 
    runInfoAccountingData   
    )
