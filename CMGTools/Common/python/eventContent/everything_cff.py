import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.particleFlow_cff import *
from CMGTools.Common.eventContent.traditional_cff import * 
from CMGTools.Common.eventContent.runInfoAccounting_cff import runInfoAccounting

everything = particleFlow + traditional + runInfoAccounting

MHT = particleFlowMHT + traditionalMHT
