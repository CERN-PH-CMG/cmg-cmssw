import FWCore.ParameterSet.Config as cms

#scrapingFilter = cms.EDFilter("FilterOutScraping",
#                              applyfilter = cms.untracked.bool(True),
scrapingFilter = cms.EDFilter("ScrapingFilter",
                              debugOn     = cms.untracked.bool(False),
                              numtrack    = cms.untracked.uint32(10),
                              thresh      = cms.untracked.double(0.25),
                              TaggingMode = cms.bool(True)
                              )
