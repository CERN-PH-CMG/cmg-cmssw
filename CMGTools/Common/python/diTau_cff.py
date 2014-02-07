import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgDiTau_cfi import *
from CMGTools.Common.skims.cmgDiTauSel_cfi import *

from CMGTools.Common.factories.cmgTauMu_cfi import *
from CMGTools.Common.skims.cmgTauMuSel_cfi import *

from CMGTools.Common.factories.cmgTauEle_cfi import *
from CMGTools.Common.skims.cmgTauEleSel_cfi import *

from CMGTools.Common.factories.cmgTauEle_cfi import *
from CMGTools.Common.skims.cmgTauEleSel_cfi import *

from CMGTools.Common.factories.cmgMuEle_cfi import *
from CMGTools.Common.skims.cmgMuEleSel_cfi import *

diTauSequence = cms.Sequence(
    cmgDiTau + 
    cmgDiTauSel +
    cmgTauMu +
    cmgTauMuSel +
    cmgTauEle +
    cmgTauEleSel +
    cmgMuEle +
    cmgMuEleSel
)
