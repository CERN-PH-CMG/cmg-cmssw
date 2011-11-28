import FWCore.ParameterSet.Config as cms

from CMGTools.Common.generator.vertexWeight.vertexWeightEPSJul8_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeightLeptonPhoton_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeightMay10ReReco_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeightPromptRecov4_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight05AugReReco_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeightPromptRecov6_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight2011B_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight2011AB_cfi import *

vertexWeightSequence = cms.Sequence(
    vertexWeightEPSJul8
    +vertexWeightLeptonPhoton
    +vertexWeightMay10ReReco
    +vertexWeightPromptRecov4
    +vertexWeight05AugReReco
    +vertexWeightPromptRecov6
    +vertexWeight2011B
    +vertexWeight2011AB
    )
