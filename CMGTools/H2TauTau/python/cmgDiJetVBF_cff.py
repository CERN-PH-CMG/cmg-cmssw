

from CMGTools.H2TauTau.cmgDiJetVBF_cfi import *

cmgDiJetVBFSequence = cms.Sequence(
    cmgPFJetVBF
    +cmgDiJetVBF
    +cmgDiJetVBFSel
    )

