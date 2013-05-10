import FWCore.ParameterSet.Config as cms

from SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi import *
from SimTracker.TrackAssociation.TrackAssociatorByHits_cfi import *
from RecoBTag.ImpactParameter.impactParameter_cfi import *

def btvDefaultSequence(process, isMC=True, jetCollection="selectedPatJetsPFlow",vtxCollection="goodOfflinePrimaryVertices") :

    process.load("PhysicsTools.JetMCAlgos.CaloJetsMCFlavour_cfi")
    process.load("SimTracker.TrackAssociation.TrackAssociatorByChi2_cfi")
    process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
    process.load("SimTracker.TrackHistory.TrackHistory_cff")
    process.load("SimTracker.TrackHistory.TrackClassifier_cff")
    process.load("RecoBTau.JetTagComputer.jetTagRecord_cfi")
    
    ############################################################################
    # for Impact Parameter based taggers
    ############################################################################
    process.load("RecoBTag.ImpactParameter.negativeOnlyJetBProbabilityComputer_cfi")
    process.load("RecoBTag.ImpactParameter.negativeOnlyJetProbabilityComputer_cfi")
    process.load("RecoBTag.ImpactParameter.positiveOnlyJetProbabilityComputer_cfi")
    process.load("RecoBTag.ImpactParameter.positiveOnlyJetBProbabilityComputer_cfi")
    process.load("RecoBTag.ImpactParameter.negativeTrackCounting3D2ndComputer_cfi")
    process.load("RecoBTag.ImpactParameter.negativeTrackCounting3D3rdComputer_cfi")
    process.load("RecoBTag.Configuration.RecoBTag_cff")
    process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")
    process.load("RecoBTag.ImpactParameter.negativeOnlyJetBProbabilityJetTags_cfi")
    process.load("RecoBTag.ImpactParameter.negativeOnlyJetProbabilityJetTags_cfi")
    process.load("RecoBTag.ImpactParameter.positiveOnlyJetProbabilityJetTags_cfi")
    process.load("RecoBTag.ImpactParameter.positiveOnlyJetBProbabilityJetTags_cfi")
    process.load("RecoBTag.ImpactParameter.negativeTrackCountingHighPur_cfi")
    process.load("RecoBTag.ImpactParameter.negativeTrackCountingHighEffJetTags_cfi")
    process.load("RecoBTag.ImpactParameter.jetProbabilityBJetTags_cfi")
    process.load("RecoBTag.ImpactParameter.jetBProbabilityBJetTags_cfi")

    ############################################################################
    # for Secondary Vertex taggers
    ############################################################################
    process.load("RecoBTag.SecondaryVertex.secondaryVertexTagInfos_cfi")
    process.load("RecoBTag.SecondaryVertex.secondaryVertexNegativeTagInfos_cfi")
    process.load("RecoBTag.SecondaryVertex.simpleSecondaryVertexHighEffBJetTags_cfi")
    process.load("RecoBTag.SecondaryVertex.simpleSecondaryVertexHighPurBJetTags_cfi")
    process.load("RecoBTag.SecondaryVertex.simpleSecondaryVertexNegativeHighEffBJetTags_cfi")
    process.load("RecoBTag.SecondaryVertex.simpleSecondaryVertexNegativeHighPurBJetTags_cfi")
    process.load("RecoBTag.SecondaryVertex.combinedSecondaryVertexNegativeBJetTags_cfi")
    process.load("RecoBTag.SecondaryVertex.combinedSecondaryVertexNegativeES_cfi")
    process.load("RecoBTag.SecondaryVertex.combinedSecondaryVertexPositiveBJetTags_cfi")
    process.load("RecoBTag.SecondaryVertex.combinedSecondaryVertexPositiveES_cfi")

    ############################################################################
    # for Inclusive Secondary Vertexing
    ############################################################################
    process.load("RecoVertex.AdaptiveVertexFinder.inclusiveVertexing_cff")
    process.load("RecoBTag.SecondaryVertex.bToCharmDecayVertexMerger_cfi")

    process.combinedInclusiveSecondaryVertexPositiveBJetTags = process.combinedInclusiveSecondaryVertexBJetTags.clone()
    process.combinedInclusiveSecondaryVertexPositiveBJetTags.jetTagComputer = cms.string('combinedSecondaryVertexPositive')

    ############################################################################
    # For the Retrained CSV
    ############################################################################
    process.combinedSecondaryVertexRetrained = process.combinedSecondaryVertex.clone(
      calibrationRecords = cms.vstring(
      'CombinedSVRetrainRecoVertex',
      'CombinedSVRetrainPseudoVertex',
      'CombinedSVRetrainNoVertex'
      )
      )
    process.combinedSecondaryVertexRetrainedBJetTags = process.combinedSecondaryVertexBJetTags.clone(
      jetTagComputer = cms.string('combinedSecondaryVertexRetrained')
      )

    process.combinedSecondaryVertexRetrainedNegative = process.combinedSecondaryVertexNegative.clone(
      calibrationRecords = cms.vstring(
      'CombinedSVRetrainRecoVertex', 
      'CombinedSVRetrainPseudoVertex', 
      'CombinedSVRetrainNoVertex'
      )
      )
    process.combinedSecondaryVertexRetrainedNegativeBJetTags = process.combinedSecondaryVertexNegativeBJetTags.clone(
      jetTagComputer = cms.string('combinedSecondaryVertexRetrainedNegative')
      )

    process.combinedSecondaryVertexRetrainedPositive = process.combinedSecondaryVertexPositive.clone(
      calibrationRecords = cms.vstring(
      'CombinedSVRetrainRecoVertex', 
      'CombinedSVRetrainPseudoVertex', 
      'CombinedSVRetrainNoVertex'
      )
      )
    process.combinedSecondaryVertexRetrainedPositiveBJetTags = process.combinedSecondaryVertexPositiveBJetTags.clone(
      jetTagComputer = cms.string('combinedSecondaryVertexRetrainedPositive')
      )


    ############################################################################
    # For the Soft Lepton taggers
    ############################################################################
    process.load("RecoBTag.SoftLepton.negativeSoftMuonES_cfi")
    process.load("RecoBTag.SoftLepton.positiveSoftMuonES_cfi")
    process.load("RecoBTag.SoftLepton.negativeSoftMuonBJetTags_cfi")
    process.load("RecoBTag.SoftLepton.positiveSoftMuonBJetTags_cfi")

    process.load("RecoBTag.SoftLepton.negativeSoftLeptonByPtES_cfi")
    process.load("RecoBTag.SoftLepton.positiveSoftLeptonByPtES_cfi")
    process.load("RecoBTag.SoftLepton.negativeSoftMuonByPtBJetTags_cfi")
    process.load("RecoBTag.SoftLepton.positiveSoftMuonByPtBJetTags_cfi")

    # for new softElectron tagger 
    process.softPFElectronRetrained = process.softElectron.clone()
    process.softPFElectronRetrainedBJetsTags = process.softPFElectronBJetTags.clone( jetTagComputer = 'softPFElectronRetrained' )

    process.load("RecoBTag.SoftLepton.negativeSoftElectronES_cfi")
    process.load("RecoBTag.SoftLepton.negativeSoftPFElectronBJetTags_cfi")
    process.negativeSoftPFElectronRetrained = process.negativeSoftElectron.clone()
    process.negativeSoftPFElectronRetrainedBJetsTags = process.negativeSoftPFElectronBJetTags.clone( jetTagComputer = 'negativeSoftPFElectronRetrained' )

    process.load("RecoBTag.SoftLepton.positiveSoftElectronES_cfi")
    process.load("RecoBTag.SoftLepton.positiveSoftPFElectronBJetTags_cfi")
    process.positiveSoftPFElectronRetrained = process.positiveSoftElectron.clone()
    process.positiveSoftPFElectronRetrainedBJetsTags = process.positiveSoftPFElectronBJetTags.clone( jetTagComputer = 'positiveSoftPFElectronRetrained' )

    # for new softMuon tagger
    process.softPFMuonRetrained = process.softMuon.clone()
    process.softPFMuonRetrainedBJetsTags = process.softPFMuonBJetTags.clone( jetTagComputer = 'softPFMuonRetrained' )

    process.load("RecoBTag.SoftLepton.negativeSoftMuonES_cfi")
    process.load("RecoBTag.SoftLepton.negativeSoftPFMuonBJetTags_cfi")
    process.negativeSoftPFMuonRetrained = process.negativeSoftMuon.clone()
    process.negativeSoftPFMuonRetrainedBJetsTags = process.negativeSoftPFMuonBJetTags.clone( jetTagComputer = 'negativeSoftPFMuonRetrained' )

    process.load("RecoBTag.SoftLepton.positiveSoftMuonES_cfi")
    process.load("RecoBTag.SoftLepton.positiveSoftPFMuonBJetTags_cfi")
    process.positiveSoftPFMuonRetrained = process.positiveSoftMuon.clone()
    process.positiveSoftPFMuonRetrainedBJetsTags = process.positiveSoftPFMuonBJetTags.clone( jetTagComputer = 'positiveSoftPFMuonRetrained' )

    ############################################################################
    # For the combined CSV + JP tagger
    ############################################################################
    process.combinedCSVJP = process.combinedMVA.clone(
      calibrationRecord = 'CombinedCSVJP',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = cms.string('jetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = cms.string('softPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('softPFElectronRetrained')
      )
      )
      )
    process.combinedCSVJPBJetTags = process.combinedMVABJetTags.clone(
      jetTagComputer = 'combinedCSVJP',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    process.load("RecoBTau.JetTagComputer.negativeCombinedMVAES_cfi")
    process.negativeCombinedCSVJP = process.negativeCombinedMVA.clone(
      calibrationRecord = 'CombinedCSVJP',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeOnlyJetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrainedNegative')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeSoftPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeSoftPFElectronRetrained')
      )
      )
      )
    process.load("RecoBTau.JetTagComputer.negativeCombinedMVABJetTags_cfi")
    process.negativeCombinedCSVJPBJetTags = process.negativeCombinedMVABJetTags.clone(
      jetTagComputer = 'negativeCombinedCSVJP',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    process.load("RecoBTau.JetTagComputer.positiveCombinedMVAES_cfi")
    process.positiveCombinedCSVJP = process.positiveCombinedMVA.clone(
      calibrationRecord = 'CombinedCSVJP',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveOnlyJetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrainedPositive')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveSoftPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveSoftPFElectronRetrained')
      )
      )
      )
    process.load("RecoBTau.JetTagComputer.positiveCombinedMVABJetTags_cfi")
    process.positiveCombinedCSVJPBJetTags = process.positiveCombinedMVABJetTags.clone(
      jetTagComputer = 'positiveCombinedCSVJP',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    ############################################################################
    # For the combined CSV + JP + SL tagger
    ############################################################################
    process.combinedCSVJPSL = process.combinedMVA.clone(
      calibrationRecord = 'CombinedCSVJPSL',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = cms.string('jetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = cms.string('softPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('softPFElectronRetrained')
      )
      )
      )
    process.combinedCSVJPSLBJetTags = process.combinedMVABJetTags.clone(
      jetTagComputer = 'combinedCSVJPSL',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    process.negativeCombinedCSVJPSL = process.negativeCombinedMVA.clone(
      calibrationRecord = 'CombinedCSVJPSL',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeOnlyJetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrainedNegative')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeSoftPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeSoftPFElectronRetrained')
      )
      )
      )
    process.negativeCombinedCSVJPSLBJetTags = process.negativeCombinedMVABJetTags.clone(
      jetTagComputer = 'negativeCombinedCSVJPSL',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    process.positiveCombinedCSVJPSL = process.positiveCombinedMVA.clone(
      calibrationRecord = 'CombinedCSVJPSL',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveOnlyJetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrainedPositive')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveSoftPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveSoftPFElectronRetrained')
      )
      )
      )
    process.positiveCombinedCSVJPSLBJetTags = process.positiveCombinedMVABJetTags.clone(
      jetTagComputer = 'positiveCombinedCSVJPSL',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    ############################################################################
    # For the combined CSV + SL tagger
    ############################################################################
    process.combinedCSVSL = process.combinedMVA.clone(
      calibrationRecord = 'CombinedCSVSL',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = cms.string('jetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = cms.string('softPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('softPFElectronRetrained')
      )
      )
      )
    process.combinedCSVSLBJetTags = process.combinedMVABJetTags.clone(
      jetTagComputer = 'combinedCSVSL',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    process.negativeCombinedCSVSL = process.negativeCombinedMVA.clone(
      calibrationRecord = 'CombinedCSVSL',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeOnlyJetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrainedNegative')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeSoftPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('negativeSoftPFElectronRetrained')
      )
      )
      )
    process.negativeCombinedCSVSLBJetTags = process.negativeCombinedMVABJetTags.clone(
      jetTagComputer = 'negativeCombinedCSVSL',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    process.positiveCombinedCSVSL = process.positiveCombinedMVA.clone(
      calibrationRecord = 'CombinedCSVSL',
      jetTagComputers = cms.VPSet(
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveOnlyJetProbability')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('combinedSecondaryVertexRetrainedPositive')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveSoftPFMuonRetrained')
      ),
      cms.PSet(
      discriminator = cms.bool(True),
      variables = cms.bool(False),
      jetTagComputer = 
      cms.string('positiveSoftPFElectronRetrained')
      )
      )
      )
    process.positiveCombinedCSVSLBJetTags = process.positiveCombinedMVABJetTags.clone(
      jetTagComputer = 'positiveCombinedCSVSL',
      tagInfos = cms.VInputTag(
      cms.InputTag("impactParameterTagInfos"),
      cms.InputTag("secondaryVertexTagInfos"),
      cms.InputTag("softPFMuonsTagInfos"),
      cms.InputTag("softPFElectronsTagInfos")
      )
      )

    ##############################################
    # Get calibrations for the CSV taggers
    ##############################################
    process.load("CondCore.DBCommon.CondDBSetup_cfi")
    process.BTauMVAJetTagComputerRecord = cms.ESSource("PoolDBESSource",
                                                       process.CondDBSetup,
                                                       timetype = cms.string('runnumber'),
                                                       toGet = cms.VPSet(cms.PSet(
        record = cms.string('BTauGenericMVAJetTagComputerRcd'),
        tag = cms.string('MVAComputerContainer_Retrained53X_JetTags_v2')
        )),
                                                       connect = cms.string('frontier://FrontierProd/CMS_COND_PAT_000'),
                                                       BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService')
                                                       )
    
    process.es_prefer_BTauMVAJetTagComputerRecord = cms.ESPrefer("PoolDBESSource","BTauMVAJetTagComputerRecord") 

    ### JP calibration for MC only 
    if(isMC) :
        process.GlobalTag.toGet = cms.VPSet(
            cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
                     tag = cms.string("TrackProbabilityCalibration_2D_MC53X_v2"),
                     connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU")),
            cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
                     tag = cms.string("TrackProbabilityCalibration_3D_MC53X_v2"),
                     connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU"))
            )
#    else:
#        process.GlobalTag.toGet = cms.VPSet(
#            cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
#                     tag = cms.string("TrackProbabilityCalibration_2D_Data53X_v2"),
#                     connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU")),
#            cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
#                     tag = cms.string("TrackProbabilityCalibration_3D_Data53X_v2"),
#                     connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU"))
#            )
        
    #user specific configurations
    process.AK5byRef.jets                       = jetCollection
    process.ak5JetTracksAssociatorAtVertex.jets = jetCollection
    process.softMuonTagInfos.jets               = jetCollection 
    process.softPFMuonsTagInfos.primaryVertex = cms.InputTag(vtxCollection)
    process.softPFMuonsTagInfos.jets            = jetCollection 
    process.softPFElectronsTagInfos.primaryVertex = cms.InputTag(vtxCollection)
    process.softPFElectronsTagInfos.jets        = jetCollection

    


    #the sequence
    process.mainBtvSequence=cms.Sequence(process.inclusiveVertexing*process.inclusiveMergedVerticesFiltered*process.bToCharmDecayVertexMerged
                                     *process.ak5JetTracksAssociatorAtVertex
                                     *process.btagging
                                     *process.positiveOnlyJetProbabilityJetTags*process.negativeOnlyJetProbabilityJetTags
                                     *process.negativeTrackCountingHighEffJetTags*process.negativeTrackCountingHighPur
                                     *process.secondaryVertexTagInfos*process.simpleSecondaryVertexHighEffBJetTags*process.simpleSecondaryVertexHighPurBJetTags
                                     *process.secondaryVertexNegativeTagInfos*process.simpleSecondaryVertexNegativeHighEffBJetTags*process.simpleSecondaryVertexNegativeHighPurBJetTags
                                     *process.combinedSecondaryVertexNegativeBJetTags*process.combinedSecondaryVertexPositiveBJetTags
                                     #*process.negativeSoftMuonBJetTags*process.positiveSoftMuonBJetTags
                                     *process.positiveSoftLeptonByPtBJetTags	
                                     #*process.softPFMuonsTagInfos  		
                                     *process.softPFMuonRetrainedBJetsTags*process.negativeSoftPFMuonRetrainedBJetsTags*process.positiveSoftPFMuonRetrainedBJetsTags
                                     #*process.softPFElectronsTagInfos  	
                                     *process.softPFElectronRetrainedBJetsTags*process.negativeSoftPFElectronRetrainedBJetsTags*process.positiveSoftPFElectronRetrainedBJetsTags
                                     #*process.negativeSoftLeptonByPtBJetTags*process.positiveSoftLeptonByPtBJetTags	
                                     *process.negativeOnlyJetBProbabilityJetTags*process.positiveOnlyJetBProbabilityJetTags
                                     *process.combinedSecondaryVertexRetrainedBJetTags*process.combinedSecondaryVertexRetrainedNegativeBJetTags*process.combinedSecondaryVertexRetrainedPositiveBJetTags
                                     *process.combinedCSVJPBJetTags*process.negativeCombinedCSVJPBJetTags*process.positiveCombinedCSVJPBJetTags
                                     *process.combinedCSVSLBJetTags*process.negativeCombinedCSVSLBJetTags*process.positiveCombinedCSVSLBJetTags
                                     *process.combinedCSVJPSLBJetTags*process.negativeCombinedCSVJPSLBJetTags*process.positiveCombinedCSVJPSLBJetTags
                                     *process.inclusiveSecondaryVertexFinderTagInfosFiltered*process.simpleInclusiveSecondaryVertexHighEffBJetTags*process.simpleInclusiveSecondaryVertexHighPurBJetTags
                                     *process.doubleSecondaryVertexHighEffBJetTags
                                     *process.inclusiveSecondaryVertexFinderTagInfos*process.combinedInclusiveSecondaryVertexBJetTags*process.combinedInclusiveSecondaryVertexPositiveBJetTags)
    if(isMC) : process.btvSequence=cms.Sequence(process.myPartons*process.AK5Flavour*process.mainBtvSequence)
    else     : process.btvSequence=cms.Sequence(process.mainBtvSequence)
