import FWCore.ParameterSet.Config as cms
import sys

#in this file, we run the various filters and store the results
# wreece - 01/08/12 - Checked against http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoMET/METFilters/test/exampleICHEPrecommendation_cfg.py?revision=1.1&view=markup&pathrev=V00-00-07

##
# Non-MET related
## 

#some standard filters not related to MET
primaryVertexFilter = cms.EDFilter(
    "VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
    filter = cms.bool(True)
    )
primaryVertexFilterPath = cms.Path(primaryVertexFilter)

noscraping = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )
noscrapingFilterPath = cms.Path(noscraping)

#catches events with a bug in the Madgraph config
from GeneratorInterface.GenFilters.TotalKinematicsFilter_cfi import totalKinematicsFilter
totalKinematicsFilterPath = cms.Path(totalKinematicsFilter)

##
# MET related
## 

#these filters are from this page: https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFilters

#from the RecoMET package

## The iso-based HBHE noise filter
from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import HBHENoiseFilter
HBHENoiseFilterPath = cms.Path(HBHENoiseFilter)

## The CSC beam halo tight filter
from RecoMET.METAnalyzers.CSCHaloFilter_cfi import CSCTightHaloFilter
CSCTightHaloFilterPath = cms.Path(CSCTightHaloFilter)

## The HCAL laser filter
from RecoMET.METFilters.hcalLaserEventFilter_cfi import hcalLaserEventFilter
hcalLaserEventFilterPath = cms.Path(hcalLaserEventFilter)

## The ECAL dead cell trigger primitive filter
from RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi import EcalDeadCellTriggerPrimitiveFilter
## For AOD and RECO recommendation to use recovered rechits
EcalDeadCellTriggerPrimitiveFilter.tpDigiCollection = cms.InputTag("ecalTPSkimNA")
EcalDeadCellTriggerPrimitiveFilterPath = cms.Path(EcalDeadCellTriggerPrimitiveFilter)

## The EE bad SuperCrystal filter
from RecoMET.METFilters.eeBadScFilter_cfi import eeBadScFilter
eeBadScFilterPath = cms.Path(eeBadScFilter)

## The Good vertices collection needed by the tracking failure filter
goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

## The tracking failure filter
from RecoMET.METFilters.trackingFailureFilter_cfi import trackingFailureFilter
trackingFailureFilter.VertexSource = "goodVertices"
trackingFailureFilter.JetSource = 'ak5PFJets'
trackingFailureSequence = cms.Sequence(goodVertices*trackingFailureFilter)
trackingFailureFilterPath = cms.Path(trackingFailureSequence)


metNoiseCleaning = cms.Sequence(primaryVertexFilter+
                                noscraping+
                                CSCTightHaloFilter+
                                HBHENoiseFilter+
                                hcalLaserEventFilter+
                                EcalDeadCellTriggerPrimitiveFilter+
                                trackingFailureSequence+
                                eeBadScFilter)
metNoiseCleaningPath = cms.Path(metNoiseCleaning)

#the HCal noise filter only works on AOD in 5.2
from CMGTools.Common.Tools.cmsswRelease import isNewerThan
if isNewerThan('CMSSW_5_2_0'):
    hcalLaserEventFilter.vetoByRunEventNumber=cms.untracked.bool(False)
    hcalLaserEventFilter.vetoByHBHEOccupancy=cms.untracked.bool(True)
else:
    print >> sys.stderr, 'hcalLaserFilterFromAOD only available in releases >= 5.2'
