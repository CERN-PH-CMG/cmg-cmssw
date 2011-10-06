import FWCore.ParameterSet.Config as cms

from CMGTools.Common.miscProducers.deltaPhiJetMET_cfi import *
from CMGTools.Common.miscProducers.metSignificance_cfi import *
from CMGTools.Common.miscProducers.collectionSize.collectionSize_cff import *

miscSequence = cms.Sequence(
    deltaPhiJetMET +
    # MET significance can not run reading the patTuple, hence is not included here
    # metSignificance +
    collectionSizeSequence
    ) 
