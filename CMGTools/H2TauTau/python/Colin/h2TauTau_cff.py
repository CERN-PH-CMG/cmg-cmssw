import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.Colin.objects.object_cff import *
from CMGTools.H2TauTau.Colin.skims.skim_cff import *

tauMuPath = cms.Path(
    objectSequence + 
    tauMuSkimSequence
    )

tauEPath = cms.Path(
    objectSequence + 
    tauESkimSequence
    )
