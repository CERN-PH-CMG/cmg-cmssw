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

#this is an isolated track veto - see AN2012-379 for details
from CMGTools.Common.miscProducers.trackIsolationMaker_cfi import trackIsolationMaker

trackIsolationFilter = cms.EDFilter(
    "TrackIsolationFilter",
    pfcands_trkiso = cms.InputTag('trackIsolationMaker','pfcandstrkiso','PAT'),
    pfcands_pt = cms.InputTag('trackIsolationMaker','pfcandspt','PAT'),
    pfcands_chg = cms.InputTag('trackIsolationMaker','pfcandschg','PAT'),
    pt_cut = cms.double(10.0),
    relIso_cut = cms.double(0.1)
    )

trackIsolationMakerSequence = cms.Sequence(trackIsolationMaker*trackIsolationFilter)
trackIsolationMakerFilterPath = cms.Path(trackIsolationMakerSequence)

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
                                trackingFailureSequence
                                # eeBadScSequence+
                                # ecalLaserCorrSequence
                                )

metNoiseCleaningPath = cms.Path(metNoiseCleaning)

from CMGTools.Common.Tools.cmsswRelease import isNewerThan
if isNewerThan('CMSSW_5_2_0'):
    #the HCal noise filter works on AOD in 5.2. in 44, a list of events was used.
    hcalLaserEventFilter.vetoByRunEventNumber=cms.untracked.bool(False)
    hcalLaserEventFilter.vetoByHBHEOccupancy=cms.untracked.bool(True)
    #the ee bad sc filter is only available in 5X 
    ## Bad EE Supercrystal filter
    from RecoMET.METFilters.eeBadScFilter_cfi import eeBadScFilter
    eeBadScFilterPath = cms.Path(eeBadScFilter)
    metNoiseCleaning +=  eeBadScFilter 
    ## EB or EE Xtals with large laser calibration correction
    from RecoMET.METFilters.ecalLaserCorrFilter_cfi import ecalLaserCorrFilter
    ecalLaserFilterPath = cms.Path(ecalLaserCorrFilter)
    metNoiseCleaning += ecalLaserCorrFilter
else:
    print >> sys.stderr, 'hcalLaserFilterFromAOD, eeBadScFilter and ecalLaserFilter only available in releases >= 5.2'

if isNewerThan('CMSSW_5_3_0'):
    #the tracking PoG filters only work in 53
    from RecoMET.METFilters.trackingPOGFilters_cfi import *
    trkPOGFiltersSequence = cms.Sequence(~manystripclus53X * ~toomanystripclus53X * ~logErrorTooManyClusters)
    trkPOGFiltersPath = cms.Path(trkPOGFiltersSequence)
else:
    print >> sys.stderr, 'trkPOGFilters only available in releases >= 5.3'
