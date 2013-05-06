import FWCore.ParameterSet.Config as cms

isMC=True
gtag="START53_V20::All"

process = cms.Process("DataAna")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = gtag

process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True)) #False))

#the source and output
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/cmst3/user/psilva/VBFNLO/eejj/8TeV/AODSIM/Events_1194.root',
                                                              '/store/cmst3/user/psilva/VBFNLO/eejj/8TeV/AODSIM/Events_1195.root',
                                                              '/store/cmst3/user/psilva/VBFNLO/eejj/8TeV/AODSIM/Events_1196.root',
                                                              '/store/cmst3/user/psilva/VBFNLO/eejj/8TeV/AODSIM/Events_1197.root'
                                                              )
                            )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring('keep *'),
                               fileName = cms.untracked.string('Events.root')
                               )

if(isMC) : process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.TFileService = cms.Service("TFileService", fileName = cms.string("DataAnalysis.root") )



##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

##-------------------- Import the Jet RECO modules -----------------------
process.load('RecoJets.Configuration.RecoPFJets_cff')

process.kt6PFJets.doRhoFastjet = True
process.ak5PFJets.doAreaFastjet = True

#apply a good vertex selector and filter out scraping
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter( "PrimaryVertexObjectFilter",
                                                   filterParams = pvSelector.clone( minNdof = cms.double(4.0),
                                                                                    maxZ = cms.double(24.0),
                                                                                    maxd0 = cms.double(2.0)
                                                                                    ),
                                                   src=cms.InputTag('offlinePrimaryVertices')
                                                   )
process.goodVertexFilter = cms.EDFilter("GoodVertexFilter",
                                        vertexCollection = cms.InputTag('goodOfflinePrimaryVertices'),
                                        minimumNDOF = cms.uint32(4),
                                        maxAbsZ = cms.double(24),
                                        maxd0 = cms.double(2)
                                        )

process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
                               )


#PF2PAT
process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
jetAlgo="AK5"
jecLevels=['L1FastJet', 'L2Relative', 'L3Absolute']
usePF2PAT(process,
          runPF2PAT=True,
          jetAlgo=jetAlgo,
          runOnMC=isMC,
          postfix=postfix,
          jetCorrections=('AK5PFchs', jecLevels),
          pvCollection=cms.InputTag('goodOfflinePrimaryVertices'),
          typeIMetCorrections=False)
useGsfElectrons(process,postfix=postfix,dR="03")

#add electron MVA id
process.load('EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi')
process.eidMVASequence = cms.Sequence(  process.mvaTrigV0 + process.mvaNonTrigV0 )
process.patElectronsPFlow.electronIDSources.mvaTrigV0    = cms.InputTag("mvaTrigV0")
process.patElectronsPFlow.electronIDSources.mvaNonTrigV0 = cms.InputTag("mvaNonTrigV0")

#add q/g discriminator
process.load('QuarkGluonTagger.EightTeV.QGTagger_RecoJets_cff')
process.QGTagger.srcJets    = cms.InputTag("selectedPatJets"+postfix)
process.QGTagger.isPatJet  = cms.untracked.bool(True)
process.QGTagger.useCHS    = cms.untracked.bool(True) 
process.QGTagger.srcRho    = cms.InputTag('kt6PFJets','rho')
process.QGTagger.srcRhoIso = cms.InputTag('kt6PFJetsCentral','rho')
process.qgSequence=cms.Sequence(process.goodOfflinePrimaryVerticesQG+process.QGTagger)

#compute rho from central pf candidates only
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJetsCentral = kt4PFJets.clone( rParam = cms.double(0.6),
                                            doAreaFastjet = cms.bool(True),
                                            doRhoFastjet = cms.bool(True),
                                            Rho_EtaMax = cms.double(2.5),
                                            Ghost_EtaMax = cms.double(2.5) )


# CHECK THIS POINT FWD
#applyPostfix(process,"pfPileUp",postfix).checkClosestZVertex = cms.bool(False) 
#from PhysicsTools.PatAlgos.tools.metTools import *
#process.pfPileUpPF2PAT.Enable = True
#process.pfPileUpPF2PAT.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
#process.load('RecoJets.Configuration.RecoJets_cff')
#from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets

#process.kt6PFJets               = kt4PFJets.clone()
#process.kt6PFJets.rParam        = 0.6     
##process.kt6PFJets.src           = cms.InputTag('pfNoElectron'+postfix)
#process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
#process.kt6PFJets.doRhoFastjet  = True
#process.kt6PFJets.doAreaFastjet = True
##process.kt6PFJets.voronoiRfact  = 0.9

##process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJets", "rho")
#getattr(process,"patJetCorrFactors"+postfix).rho = cms.InputTag("kt6PFJets", "rho")

from UserCode.EWKV.btvDefaultSequence_cff import *
btvDefaultSequence(process,isMC,"selectedPatJets"+postfix,"goodOfflinePrimaryVertices")

#the analyzer
process.load("UserCode.EWKV.dataAnalyzer_cfi")

#counters for specific filters
process.startCounter = cms.EDProducer("EventCountProducer")
process.scrapCounter = process.startCounter.clone()
process.vtxCounter   = process.startCounter.clone()

process.p = cms.Path( process.startCounter
                      *process.noscraping
                      *process.scrapCounter
                      *process.goodOfflinePrimaryVertices
                      *process.goodVertexFilter
                      *process.vtxCounter
                      *process.eidMVASequence
                      *getattr(process,"patPF2PATSequence"+postfix)
                      *process.btvSequence
                      *process.kt6PFJetsCentral
                      *process.qgSequence
                      *process.dataAnalyzer
                      )
	




