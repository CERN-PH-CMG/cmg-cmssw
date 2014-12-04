import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.jetSkim_cff import *
from CMGTools.Common.skims.metSkim_cff import *
from CMGTools.Common.skims.electronSkim_cff import *
from CMGTools.Common.skims.muonSkim_cff import *
from CMGTools.Common.skims.tauSkim_cff import *

skimSequence = cms.Sequence(
    jetSkimSequence +
    metSkimSequence +
    electronSkimSequence +
    muonSkimSequence +
    tauSkimSequence
    )

