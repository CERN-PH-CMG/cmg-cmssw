import FWCore.ParameterSet.Config as cms

#in this file, we run the various filters and store the results

#catches events with a bug in the Madgraph config
from GeneratorInterface.GenFilters.TotalKinematicsFilter_cfi import totalKinematicsFilter
totalKinematicsFilterPath = cms.Path(totalKinematicsFilter)

#these filters are from this page: https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFilters

# 44X does not work
# from the RecoMET package
# from RecoMET.METFilters.EcalDeadCellBoundaryEnergyFilter_cfi import EcalDeadCellBoundaryEnergyFilter
# EcalDeadCellBoundaryEnergyFilterPath = cms.Path(EcalDeadCellBoundaryEnergyFilter)

# from RecoMET.METFilters.EcalDeadCellDeltaRFilter_cfi import simpleDRfilter
# simpleDRfilterPath = cms.Path(simpleDRfilter)

#from RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi import EcalDeadCellTriggerPrimitiveFilter
#EcalDeadCellTriggerPrimitiveFilter = cms.Path(EcalDeadCellTriggerPrimitiveFilter)

#from RecoMET.METFilters.greedyMuonPFCandidateFilter_cfi import greedyMuonPFCandidateFilter
#greedyMuonPFCandidateFilterPath = cms.Path(greedyMuonPFCandidateFilter)

#from RecoMET.METFilters.hcalLaserEventFilter_cfi import hcalLaserEventFilter
#hcalLaserEventFilterPath = cms.Path(hcalLaserEventFilter)

# from RecoMET.METFilters.inconsistentMuonPFCandidateFilter_cfi import inconsistentMuonPFCandidateFilter
# inconsistentMuonPFCandidateFilterPath = cms.Path(inconsistentMuonPFCandidateFilter)

# from RecoMET.METFilters.trackingFailureFilter_cfi import trackingFailureFilter
# trackingFailureFilter.VertexSource = "offlinePrimaryVertices"
# trackingFailureFilter.JetSource = 'ak5PFJets'
# trackingFailureFilterPath = cms.Path(trackingFailureFilter)

#from RecoMET.METAnalyzers.CSCHaloFilter_cfi import CSCTightHaloFilter
#CSCTightHaloFilterPath = cms.Path(CSCTightHaloFilter)

from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import HBHENoiseFilter
HBHENoiseFilterPath = cms.Path(HBHENoiseFilter)

#now some standard filters not related to MET
from DPGAnalysis.Skims.goodvertexSkim_cff import primaryVertexFilter
primaryVertexFilterPath = cms.Path(primaryVertexFilter)

from DPGAnalysis.Skims.goodvertexSkim_cff import noscraping
noscrapingFilterPath = cms.Path(noscraping)
