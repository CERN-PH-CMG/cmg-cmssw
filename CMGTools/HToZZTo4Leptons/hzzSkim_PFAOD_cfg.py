import FWCore.ParameterSet.Config as cms
process = cms.Process("hzzSkim")

# cuts
HZZSKIM_MUON_CUT=("pt > 7 && abs(eta)<2.5 && (isGlobalMuon || isTrackerMuon)")
HZZSKIM_ELECTRON_CUT=("pt > 10 && abs(eta)<2.5")
HZZSKIM_DIMUON_CUT=("mass > 40 && max(daughter(0).pt, daughter(1).pt) > 20 && min(daughter(0).pt, daughter(1).pt) > 7")
HZZSKIM_DIELECTRON_CUT=("mass > 40 && max(daughter(0).pt, daughter(1).pt) > 20 && min(daughter(0).pt, daughter(1).pt) > 10")
HZZSKIM_EMU_CUT=("mass > 40 && ((daughter(0).pt>7 && daughter(1).pt()>20) || (daughter(0).pt>20 && daughter(1).pt()>10))")

# single lepton selectors
process.goodHzzMuons = cms.EDFilter("MuonRefSelector",
                            src = cms.InputTag("muons"),
                            cut = cms.string(HZZSKIM_MUON_CUT)
                            )
process.goodHzzElectrons = cms.EDFilter("GsfElectronRefSelector",
                                src = cms.InputTag("gsfElectrons"),
                                cut = cms.string(HZZSKIM_ELECTRON_CUT)
                                )

# dilepton selectors
process.diHzzMuons = cms.EDProducer("CandViewShallowCloneCombiner",
                            decay       = cms.string("goodHzzMuons goodHzzMuons"),
                            checkCharge = cms.bool(False),
                            cut         = cms.string(HZZSKIM_DIMUON_CUT)
                            )
process.diHzzElectrons = cms.EDProducer("CandViewShallowCloneCombiner",
                                decay       = cms.string("goodHzzElectrons goodHzzElectrons"),
                                checkCharge = cms.bool(False),
                                cut         = cms.string(HZZSKIM_DIELECTRON_CUT)
                                )
process.crossHzzLeptons  = cms.EDProducer("CandViewShallowCloneCombiner",
                                  decay       = cms.string("goodHzzMuons goodHzzElectrons"),
                                  checkCharge = cms.bool(False),
                                  cut         = cms.string(HZZSKIM_EMU_CUT)
                                  )

process.dilepHzz = cms.EDProducer("CandViewMerger",
    src = cms.VInputTag(cms.InputTag("diHzzMuons"), cms.InputTag("diHzzElectrons"),cms.InputTag("crossHzzLeptons"),)
)


# counter
process.HzzSkim= cms.EDFilter("CandViewCountFilter",
                                src = cms.InputTag("dilepHzz"),
                                minNumber = cms.uint32(1)
                                )

process.preSkimCounter = cms.EDProducer("EventCountProducer")

process.zzSkimPath = cms.Path(

    process.preSkimCounter +
    process.goodHzzMuons +
    process.goodHzzElectrons +
    process.diHzzMuons +
    process.diHzzElectrons +
    process.crossHzzLeptons +
    process.dilepHzz +
    process.HzzSkim

)

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    # to test MC:
    # 'cmgtools',
    # '/TauPlusX/Run2011A-May10ReReco-v1/AOD/V3',
    # 'cmgtools_group',
    # '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3',
    # to test Data:
    'cmgtools_group',
    #'/DoubleMu/Run2011A-05Aug2011-v1/AOD/V3',
    '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4'
    #'/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4'
    # '.*root'
    )


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('/tmp/botta/PFAOD_hzzSkim.root'),

    outputCommands =  cms.untracked.vstring(

        'keep *',
        'keep edmMergeableCounter_*_*_*',

    ),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring( 
        'zzSkimPath',
    )),
    
)

process.outpath = cms.EndPath(process.out)
