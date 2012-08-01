import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.cmsswRelease import *

#this one
def getSchedule(process, runOnMC):

    result = cms.Schedule(
        process.p,
        process.EcalDeadCellTriggerPrimitiveFilterPath,
        process.hcalLaserEventFilterPath,
        process.trackingFailureFilterPath,
        process.CSCTightHaloFilterPath,
        process.HBHENoiseFilterPath,
        process.primaryVertexFilterPath,
        process.noscrapingFilterPath,
        process.eeBadScFilterPath,
        process.metNoiseCleaningPath
        )
    if runOnMC:
        result.append(process.totalKinematicsFilterPath)
    return result
