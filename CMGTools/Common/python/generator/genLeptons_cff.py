from CMGTools.Common.generator.genLeptonsSatus1_cfi import *
from CMGTools.Common.generator.genLeptonsSatus2_cfi import *

genLeptonsSequence = cms.Sequence(
    genLeptonsStatus1
    + genLeptonsStatus2
    )
    
