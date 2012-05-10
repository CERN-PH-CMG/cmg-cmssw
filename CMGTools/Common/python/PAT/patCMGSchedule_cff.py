import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.cmsswRelease import *

#this one
def getSchedule(process, runOnMC):

    result = cms.Schedule(
        process.p,
        process.EcalDeadCellBoundaryEnergyFilterPath,
        process.simpleDRfilterPath,
        process.EcalDeadCellTriggerPrimitiveFilterPath,
        process.greedyMuonPFCandidateFilterPath,
        process.hcalLaserEventFilterPath,
        process.inconsistentMuonPFCandidateFilterPath,
        process.trackingFailureFilterPath,
        process.CSCTightHaloFilterPath,
        process.HBHENoiseFilterPath,
        process.primaryVertexFilterPath,
        process.noscrapingFilterPath
        )
    if runOnMC:
        result.append(process.totalKinematicsFilterPath)
    if cmsswIs52X():
        result.append(process.hcalLaserFilterFromAODPath)
    return result
