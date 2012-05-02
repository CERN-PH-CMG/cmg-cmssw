import FWCore.ParameterSet.Config as cms

#this one
def getSchedule(process, runOnMC):

    
    result = cms.Schedule(
        process.p,
        process.EcalDeadCellBoundaryEnergyFilterPath,
        process.simpleDRfilterPath,
        #process.EcalDeadCellTriggerPrimitiveFilter,
        #process.greedyMuonPFCandidateFilterPath,
        process.hcalLaserEventFilterPath,
        process.inconsistentMuonPFCandidateFilterPath,
        process.trackingFailureFilterPath,
        #process.CSCTightHaloFilterPath,
        process.HBHENoiseFilterPath,
        process.primaryVertexFilterPath,
        process.noscrapingFilterPath
        )
    if runOnMC:
        result.append(process.totalKinematicsFilterPath)
    return result
