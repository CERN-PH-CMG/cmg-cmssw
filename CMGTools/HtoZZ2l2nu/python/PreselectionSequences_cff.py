import FWCore.ParameterSet.Config as cms
import PhysicsTools.PythonAnalysis.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

##
## standard pre-selection sequences
##
def addPreselectionSequences(process) :

    # require scraping filter
    # https://twiki.cern.ch/twiki/bin/view/CMS/TKPOG2010Collisions
    process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                        applyfilter = cms.untracked.bool(True),
                                        debugOn = cms.untracked.bool(False),
                                        numtrack = cms.untracked.uint32(10),
                                        thresh = cms.untracked.double(0.25)
                                        )

    # filter primary vertex
    # https://twiki.cern.ch/twiki/bin/view/CMS/TWikiTopRefEventSel#PAT_Configuration
    process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                               vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                               minimumNDOF = cms.uint32(4) ,
#                                               minimumNDOF = cms.uint32(7) ,
                                               maxAbsZ = cms.double(24),
                                               maxd0 = cms.double(2)
                                               )
            
    # HB/HE noise filter
    # https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFilters
    process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
    process.HBHENoiseFilter.minIsolatedNoiseSumE = cms.double(999999.)
    process.HBHENoiseFilter.minNumIsolatedNoiseChannels = cms.int32(999999)
    process.HBHENoiseFilter.minIsolatedNoiseSumEt = cms.double(999999.)

    # beam halo filter
    # https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFilters
    #process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')

    # ECAL dead cells filter
    # https://twiki.cern.ch/twiki/bin/view/CMS/SusyEcalMaskedCellSummary#Options_and_Recipes
    #process.load('JetMETAnalysis.simpleDRfilter.simpleDRfilter_cfi')
    #process.simpleDRfilter.debug = cms.untracked.bool(True)
    #fix me jet energy corrections and met 
    #process.simpleDRfilter.jetInputTag = cms.InputTag("ak5PFJetsL2L3")
    #process.simpleDRfilter.metInputTag = cms.InputTag("metJESCorAK5PFJet")
    #process.simpleDRfilter.doFilter = cms.untracked.bool(True) # to enable filter or not
    #process.simpleDRfilter.makeProfileRoot = True

    # filter counters
    process.preSelectionCounter = cms.EDProducer("EventCountProducer")
    process.noScrapCounter = process.preSelectionCounter.clone()
    process.goodVertexCounter = process.preSelectionCounter.clone()
    process.noHBHEnoiseCounter = process.preSelectionCounter.clone()
    #process.nobeamHaloCounter = process.preSelectionCounter.clone()
    #process.noEcalDeadChannelsCounter = process.preSelectionCounter.clone()
    
    # define a preselection path
    process.preselection = cms.Sequence(
        process.preSelectionCounter*
        process.scrapingVeto*
        process.noScrapCounter*
        process.primaryVertexFilter*
        process.goodVertexCounter*
        process.HBHENoiseFilter*
        process.noHBHEnoiseCounter*
        #process.CSCTightHaloFilter*
        #process.nobeamHaloCounter*
        #process.simpleDRfilter*
        #process.noEcalDeadChannelsCounter
        )
    
    print " *** Event preselection defined"


"""
"""
def addLumifilter(process,fname='') :
    if(len(fname)>0):
        myLumis = LumiList.LumiList(filename = fname).getCMSSWString().split(',')
        process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
        process.source.lumisToProcess.extend(myLumis)
        print 'Lumi sections will be filtered with: ' + fname 
                            
    
