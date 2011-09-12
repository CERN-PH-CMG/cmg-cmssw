import FWCore.ParameterSet.Config as cms

from CMGTools.Common.runInfoAccounting_cfi import runInfoAccounting

from GeneratorInterface.Core.genFilterEfficiencyProducer_cfi import *
preSelFilterEfficiencyProducer = genFilterEfficiencyProducer.clone()
preSelFilterEfficiencyProducer.filterPath = 'p'

runInfoAccountingSequence = cms.Sequence(
    preSelFilterEfficiencyProducer + 
    runInfoAccounting
    )
