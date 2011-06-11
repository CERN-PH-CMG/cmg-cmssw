import FWCore.ParameterSet.Config as cms

###
### standard pre-selection sequences
###
def addPreselectionSequences(p) :

    # require scraping filter
    process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                        applyfilter = cms.untracked.bool(True),
                                        debugOn = cms.untracked.bool(False),
                                        numtrack = cms.untracked.uint32(10),
                                        thresh = cms.untracked.double(0.2)
                                        )

    # filter primary vertex 
    process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                               vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                               minimumNDOF = cms.uint32(7) ,
                                               maxAbsZ = cms.double(24),
                                               maxd0 = cms.double(2)
                                               )
            
    # HB/HE noise filter
    process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')

    # beam halo filter
    process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')

    # ECAL dead cells filter
    process.load('JetMETAnalysis.simpleDRfilter.simpleDRfilter_cfi')
    process.simpleDRfilter.debug = cms.untracked.bool(True)
    process.simpleDRfilter.jetInputTag = cms.InputTag("ak5PFJetsL2L3")
    process.simpleDRfilter.metInputTag = cms.InputTag("metJESCorAK5PFJet")
    process.simpleDRfilter.doFilter = cms.untracked.bool(True) # to enable filter or not
    process.simpleDRfilter.makeProfileRoot = True

    # filter counters
    process.preSelectionCounter = cms.EDProducer("EventCountProducer")
    process.noScrapCounter = process.preSelectionCounter.clone()
    process.goodVertexCounter = process.preSelectionCounter.clone()
    process.noHBHEnoiseCounter = process.preSelectionCounter.clone()
    process.nobeamHaloCounter = process.preSelectionCounter.clone()
    process.noEcalDeadChannelsCounter = process.preSelectionCounter.clone()
    
    # define a preselection path
    process.preselection = cms.Path(
        process.preSelectionCounter*
        process.scrapingVeto*
        process.noScrapCounter*
        process.primaryVertexFilter*
        process.goodVertexCounter*
        process.HBHENoiseFilter*
        process.noHBHEnoiseCounter*
        process.CSCTightHaloFilter*
        process.nobeamHaloCounter*
        process.simpleDRfilter*
        process.noEcalDeadChannelsCounter
        )
    
    print " *** Event preselection defined"

