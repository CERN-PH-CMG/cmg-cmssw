import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQM_cfg import *
from DQMServices.Components.DQMEnvironment_cfi import *
from DQMServices.Components.MEtoEDMConverter_cfi import *
from DQMServices.Components.EDMtoMEConverter_cfi import *

normalizationCounter = cms.EDAnalyzer("NormalizationCounter",
                                      ctrNames = cms.vstring('startCounter',
                                                             'preFilterCounter',
                                                             'eeCounter','mumuCounter','emuCounter',
                                                             'endCounter')
                                      )

loadNormalizationCounters = cms.Sequence(EDMtoMEConverter+normalizationCounter)
saveNormalizationCounters = cms.Sequence(MEtoEDMConverter)


