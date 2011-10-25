import FWCore.ParameterSet.Config as cms

from CMGTools.Common.miscProducers.vertexWeight.vertexWeightEPSJul8_cfi import *

vertexWeightSequence = cms.Sequence(
    vertexWeightEPSJul8
    )
