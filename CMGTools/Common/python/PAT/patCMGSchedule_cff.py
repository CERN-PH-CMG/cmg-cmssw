import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.cmsswRelease import *

#this one
def getSchedule(process, runOnMC, runOnFastSim):

    result = cms.Schedule(
        process.p,
        process.EcalDeadCellTriggerPrimitiveFilterPath,
        process.hcalLaserEventFilterPath,
        process.trackingFailureFilterPath,
        process.primaryVertexFilterPath,
        process.noscrapingFilterPath,
        process.trackIsolationMakerFilterPath,
        process.metNoiseCleaningPath
        )

    if isNewerThan('CMSSW_5_2_0'):
        result.append( process.eeBadScFilterPath )
    if isNewerThan('CMSSW_5_3_0'):
        result.append( process.ecalLaserFilterPath )
        if not runOnMC:
            result.append( process.trkPOGFiltersPath )

    if runOnMC:
        result.append(process.totalKinematicsFilterPath)
    if not( runOnFastSim ):
        result.append(process.CSCTightHaloFilterPath)
        result.append(process.HBHENoiseFilterPath)
    else:
        process.metNoiseCleaningPath.remove(process.CSCTightHaloFilter)
        process.metNoiseCleaningPath.remove(process.HBHENoiseFilter)
    return result

