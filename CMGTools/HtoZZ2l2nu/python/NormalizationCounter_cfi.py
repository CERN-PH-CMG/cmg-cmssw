import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQM_cfg import *
from DQMServices.Components.DQMEnvironment_cfi import *
from DQMServices.Components.MEtoEDMConverter_cfi import *
from DQMServices.Components.EDMtoMEConverter_cfi import *

normalizationCounter = cms.EDProducer("NormalizationCounter",
                                      ctrNames = cms.vstring('startCounter',
                                                             'preFilterCounter',
                                                             'eeCounter','mumuCounter','emuCounter',
                                                             'endCounter')
                                      )

loadHistosFromRunInfo = cms.Sequence(EDMtoMEConverter+normalizationCounter)
saveHistosInRunInfo   = cms.Sequence(MEtoEDMConverter)


