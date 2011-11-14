from CMGTools.Common.skims.cmgTauMuCount_cfi import *
from CMGTools.Common.skims.cmgTauECount_cfi import *

cmgTauMuCount.minNumber = 1
tauMuPreSelCount = cmgTauMuCount.clone( minNumber = 1,
                                        src = 'cmgTauMuCorPreSelSVFit')

tauMuFullSelCount = tauMuPreSelCount.clone( src = 'cmgTauMuCorFullSelSVFit')


tauMuPreSelSkimSequence = cms.Sequence(
    tauMuPreSelCount 
    )

tauMuFullSelSkimSequence = cms.Sequence(
    tauMuFullSelCount
    )


cmgTauECount.minNumber = 1

tauESkimSequence = cms.Sequence(
    cmgTauECount
    )


