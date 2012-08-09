from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *
from CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff import *

genSequence = cms.Sequence(
    metRecoilCorrectionInputSequence + 
    vertexWeightSequence 
    ) 
