from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.Common.generator.vertexWeight.vertexWeight_cff import *

genSequence = cms.Sequence(
    metRecoilCorrectionInputSequence + 
    vertexWeightSequence 
    ) 
