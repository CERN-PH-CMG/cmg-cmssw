import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.cmgEmuDilepton_cfi import *
from CMGTools.HtoZZ2l2nu.cmgEmuDileptonCount_cfi import *

emuSequence = cms.Sequence(cmgEmuDilepton +
                           cmgEmuDileptonCount
                           )

