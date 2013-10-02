import FWCore.ParameterSet.Config as cms

from CMGTools.Common.miscProducers.deltaPhiJetMET_cfi import *
from CMGTools.Common.miscProducers.metSignificance_cfi import *
from CMGTools.Common.miscProducers.simpleParticleFlow_cfi import *
from CMGTools.Common.miscProducers.collectionSize.collectionSize_cff import *

miscSequence = cms.Sequence(
    # this guy looks for a MET in its sequence, due to our cloning... 
    # deltaPhiJetMET +
    # MET significance can not run reading the patTuple in input, hence is not included here
    # metSignificance +
    collectionSizeSequence
    # + simpleParticleFlow
    ) 
