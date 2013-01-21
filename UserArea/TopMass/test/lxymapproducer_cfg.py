import FWCore.ParameterSet.Config as cms

import FWCore.ParameterSet.VarParsing as VarParsing


runOnData = False

process = cms.Process("LxyMap")
options = VarParsing.VarParsing ('analysis')
options.maxEvents = -1 # -1
options.inputFiles= '/store/cmst3/user/cmgtools/CMG/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0/cmgTuple_112.root'
options.outputFile = './'

options.register ('sampleLocation',
                  '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Location of the sample")

options.register ('sampleName',
                  'ttbar', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Name of the sample")



options.parseArguments()

sample = options.sampleName


process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

from CMGTools.Production.datasetToSource import *
## use the following line to run over an entire sample set
process.source = datasetToSource('cmgtools', options.sampleLocation, 'cmgTuple_.*root')

## use the following lines to run over single files
# process.source = cms.Source("PoolSource",
#                                 fileNames = cms.untracked.vstring(options.inputFiles),
# )

from CMGTools.Common.skims.cmgMuonSel_cfi import *
from CMGTools.Common.skims.cmgElectronSel_cfi import *
from CMGTools.Common.skims.cmgPFJetSel_cfi import *

process.primaryVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),    
   filter = cms.bool(True),							    # otherwise it won't filter the events, just produce an empty vertex collection.
   debugOn = cms.untracked.bool(True),
   )

# Filters defined in CMGTools/Common/python/PAT/addFilterPaths_cff.py
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.scrapingFilter = hltHighLevel.clone()
process.scrapingFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","PAT")
process.scrapingFilter.andOr = cms.bool(False)
process.scrapingFilter.HLTPaths = cms.vstring('noscrapingFilterPath')

process.metFilter = hltHighLevel.clone()
process.metFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","PAT")
process.metFilter.andOr = cms.bool(False)
process.metFilter.HLTPaths = cms.vstring('HBHENoiseFilterPath', 'CSCTightHaloFilterPath', 'hcalLaserEventFilterPath', 'EcalDeadCellTriggerPrimitiveFilterPath', 'trackingFailureFilterPath', 'eeBadScFilterPath')

## tight muon selection for jet cleaning
tightMuon = "(isPF() && isGlobal() && pt()>26 && abs(eta())<2.1 && normalizedChi2()<10  && trackerLayersWithMeasurement()>5  && numberOfValidMuonHits()>0 && abs(dxy())<0.02  && abs(dz())<0.5 && relIso(0.5, 0, 0.4)<0.12 && numberOfValidPixelHits()>0 && numberOfMatches()>1)"
process.cmgTopTightMuonMuJetSel = cmgMuonSel.clone(
    src = 'cmgMuonSel',
    cut = cms.string( tightMuon ),
    )
process.oneTightMuonMuJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (1),
     maxNumber = cms.uint32  (1),
     src       = cms.InputTag("cmgTopTightMuonMuJetSel"),
)


## tight electron selection for jet cleaning
tightElectron = "(pt()>30 && abs(eta())<2.5&& (abs(sourcePtr().superCluster().eta())>1.5660 || abs(sourcePtr().superCluster().eta())<1.4442) && abs(dxy())<0.02 && passConversionVeto()==1 && mvaTrigV0()>0.0 && numberOfHits()<=0 && relIso(0.5, 0, 0.3)<0.1)"
process.cmgTopTightElecEleJetSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string(tightElectron)
   ) 
process.oneTightElecEleJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (1),
     maxNumber = cms.uint32  (1),
     src       = cms.InputTag("cmgTopTightElecEleJetSel"),
)


# Apply deltaR jet-muon cleaning 
# http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/Common/plugins/DeltaRVetoProducer.cc?revision=1.3&view=markup
# Producer that makes a copy of the inputCollection, vetoing objects that lie within minDeltaR of *any* object in the vetoCollection.
# Algorithm is order N^2, so please use reduced collections as input
process.cleanMuonJets = cms.EDProducer("DeltaRVetoProducerPFJet",
        inputCollection = cms.InputTag('cmgPFJetSelCHS'),
        MatchingParams = cms.VPSet(cms.PSet( 				    
        vetoCollection=cms.InputTag("cmgTopTightMuonMuJetSel"),
        minDeltaR=cms.double(0.3),
        removeMatchedObject=cms.bool(True)
            )
            ),
    verbose = cms.untracked.bool(False)
    )
process.cleanElectronJets = cms.EDProducer("DeltaRVetoProducerPFJet",
                                           #inputCollection = cms.InputTag('cmgPFJetSelCHS'),
        inputCollection = cms.InputTag('cleanMuonJets'),
        MatchingParams = cms.VPSet(cms.PSet( 				    
        vetoCollection=cms.InputTag("cmgTopTightElecEleJetSel"),
        minDeltaR=cms.double(0.3),
            removeMatchedObject=cms.bool(True)
            )
            ),
    verbose = cms.untracked.bool(False)
    )


##FIXME 
process.cmgTopJetPreSel = cmgPFJetSel.clone(
    #src = 'cleanElectronJets',
    src = 'cmgPFJetSelCHS',
   cut = cms.string("pt()>=15 && abs(eta())<5 && abs(phi()) < 3.2  &&  getSelection(\"cuts_looseJetId\") " )
   )

process.fourJetsMuJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (4),
     maxNumber = cms.uint32  (9999),
     src       = cms.InputTag("cmgTopJetPreSel"),
)


# Run the weights for PU
process.load('CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff')
process.genSequence = cms.Sequence(
   process.vertexWeightSequence 
)


process.map = cms.EDAnalyzer('LxyMapProducer'
)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string(sample+'_lxyMap.root')
    )


process.mapper = cms.Path(
    process.primaryVertexFilter      +
    process.scrapingFilter           + 	 
    process.metFilter                +
    #process.cmgTopTightMuonMuJetSel  +
    #process.oneTightMuonMuJetSel     +
    #process.cmgTopTightElecEleJetSel +
    #process.oneTightElecEleJetSel    +
    #process.cleanMuonJets            +
    #process.cleanElectronJets        +
    process.cmgTopJetPreSel          +
    #process.fourJetsMuJetSel        +
    process.vertexWeightSequence     +
    process.map                      
    )

