import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgDiTau_cfi import *
from CMGTools.Common.skims.cmgDiTauSel_cfi import *

from CMGTools.Common.factories.cmgTauMu_cfi import *
from CMGTools.Common.skims.cmgTauMuSel_cfi import *

from CMGTools.Common.factories.cmgTauE_cfi import *
from CMGTools.Common.skims.cmgTauESel_cfi import *

from CMGTools.Common.factories.cmgTauE_cfi import *
from CMGTools.Common.skims.cmgTauESel_cfi import *

from CMGTools.Common.factories.cmgMuE_cfi import *
from CMGTools.Common.skims.cmgMuESel_cfi import *

diTauSequence = cms.Sequence(
    cmgDiTau + 
    cmgDiTauSel +
    cmgTauMu +
    cmgTauMuSel +
    cmgTauE +
    cmgTauESel +
    cmgMuE +
    cmgMuESel
)
