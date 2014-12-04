from CMGTools.Utilities.generator.metRecoilCorrection.metRecoilCorrection_cff import *
#COLIN vertex weights should probably come to this package
from CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff import *

genSequence = cms.Sequence(
    metRecoilCorrectionInputSequence + 
    vertexWeightSequence 
    ) 
