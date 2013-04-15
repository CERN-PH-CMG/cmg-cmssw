import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.common.susyLepton_cff import *
from CMGTools.Susy.common.boolToInt_cff import *
from CMGTools.Susy.common.jetId_cff import *
from CMGTools.Susy.common.dumpPDFWeights_cff import *
from CMGTools.Susy.common.simpleGenInfo_cff import *
from CMGTools.Susy.common.emulate2011Trigger_cff import *
from CMGTools.Susy.common.topGenInfo_cff import *
from CMGTools.Susy.common.isrWeight_cfi import *
from CMGTools.Susy.common.genParticlesStatus3_cfi import *

from CMGTools.RootTools.utils.vertexWeight.vertexWeights2012_cfi import *

susyGenSequence  = cms.Sequence(
    dumpPdfWeights+
    simpleGenInfo+
    #emulate2011Trigger+
    topGenInfo+
    vertexWeightSummer12MCICHEPData+
    vertexWeightSummer12MC53XHCPData+
    vertexWeightSummer12MC53X2012ABCDData+
    vertexWeightSummer12MC53X2012BCDData+
    isrWeight +
    genParticlesStatus3
    )

susyDataSequence  = cms.Sequence(
    #boolToIntSequence
    )
