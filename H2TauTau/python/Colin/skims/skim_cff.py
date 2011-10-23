from CMGTools.Common.skims.cmgTauMuCount_cfi import *
from CMGTools.Common.skims.cmgTauECount_cfi import *

cmgTauMuCount.minNumber = 1

cmgTauMuBaselineCount = cmgTauMuCount.clone(
    minNumber = 1,
    src = 'cmgTauMuBaselineSel'
    )


tauMuSkimSequence = cms.Sequence(
    cmgTauMuCount
    )

tauMuBaselineSkimSequence = cms.Sequence(
    cmgTauMuBaselineCount
    )


cmgTauECount.minNumber = 1

tauESkimSequence = cms.Sequence(
    cmgTauECount
    )


