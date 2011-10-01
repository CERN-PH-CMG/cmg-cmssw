from CMGTools.Common.skims.cmgTauMuCount_cfi import *
from CMGTools.Common.skims.cmgTauECount_cfi import *

cmgTauMuCount.minNumber = 1
cmgTauECount.minNumber = 1

tauMuSkimSequence = cms.Sequence(
    cmgTauMuCount
    )

tauESkimSequence = cms.Sequence(
    cmgTauECount
    )


