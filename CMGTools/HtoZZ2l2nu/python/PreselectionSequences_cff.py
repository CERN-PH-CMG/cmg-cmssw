import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
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

    #MET filters
    # https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFilters
    
    # HB/HE noise filter
    process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
    process.HBHENoiseFilter.minIsolatedNoiseSumE = cms.double(999999.)
    process.HBHENoiseFilter.minNumIsolatedNoiseChannels = cms.int32(999999)
    process.HBHENoiseFilter.minIsolatedNoiseSumEt = cms.double(999999.)

    # EB/EE Xtals with large laser calibration
    process.load('RecoMET.METFilters.ecalLaserCorrFilter_cfi')

    #Bad EE Supercrystal filter
    process.load('RecoMET.METFilters.eeBadScFilter_cfi')

    #CSC Beam Halo Filter 
    #process.load('RecoMET.METAnalyzers.CSCHaloFilter_cfi')

    #HCAL laser events
    process.load("RecoMET.METFilters.hcalLaserEventFilter_cfi")
    process.hcalLaserEventFilter.vetoByRunEventNumber=cms.untracked.bool(False)
    process.hcalLaserEventFilter.vetoByHBHEOccupancy=cms.untracked.bool(True)

    #ECAL dead cell filter  
    process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
    process.EcalDeadCellTriggerPrimitiveFilter.tpDigiCollection = cms.InputTag("ecalTPSkimNA")
    process.load('RecoMET.METFilters.EcalDeadCellBoundaryEnergyFilter_cfi')
    process.EcalDeadCellBoundaryEnergyFilter.taggingMode = cms.bool(False)
    process.EcalDeadCellBoundaryEnergyFilter.cutBoundEnergyDeadCellsEB=cms.untracked.double(10)
    process.EcalDeadCellBoundaryEnergyFilter.cutBoundEnergyDeadCellsEE=cms.untracked.double(10)
    process.EcalDeadCellBoundaryEnergyFilter.cutBoundEnergyGapEB=cms.untracked.double(100)
    process.EcalDeadCellBoundaryEnergyFilter.cutBoundEnergyGapEE=cms.untracked.double(100)
    process.EcalDeadCellBoundaryEnergyFilter.enableGap=cms.untracked.bool(False)
    process.EcalDeadCellBoundaryEnergyFilter.limitDeadCellToChannelStatusEB = cms.vint32(12,14)
    process.EcalDeadCellBoundaryEnergyFilter.limitDeadCellToChannelStatusEE = cms.vint32(12,14)

    # filter counters
    process.preSelectionCounter = cms.EDProducer("EventCountProducer")
    process.noScrapCounter = process.preSelectionCounter.clone()
    process.goodVertexCounter = process.preSelectionCounter.clone()
    process.noBadMetCounter = process.preSelectionCounter.clone()
    
    # define a preselection sequence
    process.preselection = cms.Sequence(
        process.preSelectionCounter*
        process.scrapingVeto*
        process.noScrapCounter*
        process.primaryVertexFilter*
        process.goodVertexCounter*
        process.HBHENoiseFilter*
        process.ecalLaserCorrFilter*
        process.eeBadScFilter*
        #process.CSCTightHaloFilter*
        process.hcalLaserEventFilter*
        process.EcalDeadCellTriggerPrimitiveFilter*
        process.noBadMetCounter
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
    
