import FWCore.ParameterSet.Config as cms

from CMGTools.Common.generator.vertexWeight.vertexWeightEPSJul8_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeightLeptonPhoton_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeightMay10ReReco_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeightPromptRecov4_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight05AugReReco_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeightPromptRecov6_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight2invfb_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight2011B_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight2011AB_cfi import *

#Jose Dec 15 2011: pileupTruth histos for Data not available for EPS and LP datasets
from CMGTools.Common.generator.vertexWeight.vertexWeight3DMay10ReReco_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight3DPromptRecov4_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight3D05AugReReco_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight3DPromptRecov6_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight3D2invfb_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight3D2011B_cfi import *
from CMGTools.Common.generator.vertexWeight.vertexWeight3D2011AB_cfi import *

vertexWeightSequence = cms.Sequence(
    vertexWeightEPSJul8
    +vertexWeightLeptonPhoton
    +vertexWeightMay10ReReco
    +vertexWeightPromptRecov4
    +vertexWeight05AugReReco
    +vertexWeightPromptRecov6
    +vertexWeight2invfb
    +vertexWeight2011B
    +vertexWeight2011AB
    +vertexWeight3DMay10ReReco
    +vertexWeight3DPromptRecov4
    +vertexWeight3D05AugReReco
    +vertexWeight3DPromptRecov6
    +vertexWeight3D2invfb
    +vertexWeight3D2011B
    +vertexWeight3D2011AB    
    )
