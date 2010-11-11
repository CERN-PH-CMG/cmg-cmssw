import FWCore.ParameterSet.Config as cms

from CMGTools.Common.EventContent.particleFlow_cff import particleFlow
from CMGTools.Common.EventContent.traditional_cff import traditional 
from CMGTools.Common.EventContent.runInfoAccounting_cff import runInfoAccounting

everything = particleFlow + traditional + runInfoAccounting
