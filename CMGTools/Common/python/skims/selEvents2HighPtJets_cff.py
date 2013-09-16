import FWCore.ParameterSet.Config as cms

# do not rely on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.


from CMGTools.Common.skims.cmgPFJetSel_cfi import *
from CMGTools.Common.skims.cmgPFJetCount_cfi import *


cmgPFJetSel.cut = 'pt()>50'
cmgPFJetCount.minNumber = 2

selEvents2HighPtJetsSequence = cms.Sequence(
    cmgPFJetSel +
    cmgPFJetCount
    )
