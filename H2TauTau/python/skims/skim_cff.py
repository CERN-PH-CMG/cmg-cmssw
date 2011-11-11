from CMGTools.Common.skims.cmgTauMuCount_cfi import *
from CMGTools.Common.skims.cmgTauECount_cfi import *

cmgTauMuCount.minNumber = 1
tauMuCount = cmgTauMuCount.clone( minNumber = 1,
                                  src = 'cmgTauMuCorSel')

tauMuBaselineCount = tauMuCount.clone( src = 'cmgTauMuCorBaselineSel')


tauMuSkimSequence = cms.Sequence(
    tauMuCount 
    )


tauMuBaselineSkimSequence = cms.Sequence(
    tauMuBaselineCount
    )


cmgTauECount.minNumber = 1

tauESkimSequence = cms.Sequence(
    cmgTauECount
    )


