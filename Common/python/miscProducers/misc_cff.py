import FWCore.ParameterSet.Config as cms

from CMGTools.Common.miscProducers.deltaPhiJetMET_cfi import *
from CMGTools.Common.miscProducers.metSignificance_cfi import *

miscSequence = cms.Sequence(
    deltaPhiJetMET +
    metSignificance
    ) 
