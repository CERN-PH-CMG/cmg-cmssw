import FWCore.ParameterSet.Config as cms

from CMGTools.Common.generator.vertexWeight.vertexWeightEPSJul8_cfi import *

vertexWeightSequence = cms.Sequence(
    vertexWeightEPSJul8
    )
