import FWCore.ParameterSet.Config as cms

#from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *
import copy, os, fnmatch, sys, copy
#import CMGTools.Production.eostools  as eostools 
from CMGTools.External.jec_2012_cff import use2012JecPreview

def getListOfFiles(expr, baseDir, filePattern):
    if baseDir.startswith("/store"):
        return [ "root://eoscms/%s" % f for f in eostools.listFiles( expr.format( baseDir=baseDir, filePattern="" ) ) if fnmatch.fnmatch(f,filePattern) ]
    else:
        return expr.format( baseDir=baseDir, filePattern=filePattern ) 

process = cms.Process("analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

baseDir = '/store/cmst3/user/psilva/Data4/G'
filePattern = '*.root'

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#runOnMC=True
#from Configuration.AlCa.autoCond import autoCond
#if(runOnMC) : process.GlobalTag.globaltag=cms.string(autoCond.get('startup',autoCond['mc']))
#else        : process.GlobalTag.globaltag=cms.string(autoCond['com10'])
#use2012JecPreview(process)
process.GlobalTag.globaltag="START53_V22::All"#START53_V15::All"

#process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring(LISTOFFILES),
#                            skipEvents = cms.untracked.uint32(0)
#                            )

                            
process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring('/store/cmst3/group/cmgtools/CMG/QCD_Pt-15to3000_TuneEE3C_Flat_8TeV_herwigpp/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PFAOD_943.root'),
                            skipEvents = cms.untracked.uint32(0)                        
                            )


# muon fileter
process.load('PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi')
process.MuonsFilter = process.countPatMuons.clone(
    src       = cms.InputTag("muons"),#selectedPatMuonsPFlow"),
    minNumber = cms.uint32(0)
)

from CMG.JetIDAnalysis.jetsubstructureanalyzer_cfi import *

#Uses reco objects only right now
process.pfjetanalyzer = jetsubstructureanalyzer.clone(
    JetTag      = cms.InputTag("ak5PFJets",""),
    GenJetTag   = cms.InputTag("ak5GenJets",""),
    dataFlag = cms.untracked.bool(False),
    requireZ = cms.untracked.bool(False),
    applyJec = cms.bool(True),
    residualsFromTxt = cms.bool(False),
    residualsTxt     = cms.FileInPath("CMGTools/External/data/START52_V9::All_L2L3Residual_AK5PF.txt")
)

#chs jets are not working need to add sequence
process.chspfjetanalyzer = jetsubstructureanalyzer.clone(
    JetTag      = cms.InputTag("pfJets",""),            
    GenJetTag   = cms.InputTag("ak5GenJets",""),
    dataFlag = cms.untracked.bool(False),
    requireZ = cms.untracked.bool(False),
    MvaTags = cms.untracked.VInputTag(#cms.InputTag("puJetMvaChs","simpleDiscriminant"),
                                      cms.InputTag("puJetMvaChs","full53xCHSDiscriminant"),
                                      cms.InputTag("puJetMvaChs","cutbasedDiscriminant"),
                                      ),
    IdTags = cms.untracked.VInputTag( #cms.InputTag("puJetMvaChs","simpleId"),
                                      cms.InputTag("puJetMvaChs","full53xCHSId"),
                                      cms.InputTag("puJetMvaChs","cutbasedId"),
                                      ),
    jecTag = cms.string("AK5PFchs"),
    puJetIDAlgo = full_53x_chs,
    JetFlavTag  = cms.InputTag("AK5byValPhysChs",""),
    applyJec = cms.bool(True)
)

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string("OUTPUTFILENAME"),
                                       closeFileFast = cms.untracked.bool(True)
                                   )

## jet id sequence
process.load("CMGTools.External.pujetidsequence_cff")
process.puJetId.jets     = "ak5PFJets"
process.puJetMva.jets    = "ak5PFJets"
process.puJetId.applyJec = True
process.puJetMva.applyJec = True

process.puJetIdChs.jets  = "pfJets"
process.puJetMvaChs.jets = "pfJets"

process.puJetId.residualsFromTxt = False
process.puJetId.residualsTxt     = "CMGTools/External/data/START52_V9::All_L2L3Residual_AK5PF.txt"

process.puJetMva.residualsFromTxt = False
process.puJetMva.residualsTxt     = "CMGTools/External/data/START52_V9::All_L2L3Residual_AK5PF.txt"


#Gen Jet Sequence
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.Configuration.RecoGenJets_cff") 

# Flavour byReference
process.partons  = cms.EDProducer("PartonSelector",
                                  withLeptons = cms.bool(False),
                                  src = cms.InputTag("genParticles")
                                  )

process.AK5byRef = cms.EDProducer("JetPartonMatcher",
                                  jets = cms.InputTag("ak5PFJets"),
                                  coneSizeToAssociate = cms.double(0.3),
                                  partons = cms.InputTag("partons")
                                  )

process.AK5byRefChs = cms.EDProducer("JetPartonMatcher",
                                     jets = cms.InputTag("pfJets"),
                                     coneSizeToAssociate = cms.double(0.3),
                                     partons = cms.InputTag("partons")
                                     )
# Flavour byValue PhysDef
process.AK5byValPhys = cms.EDProducer("JetFlavourIdentifier",
                                      srcByReference = cms.InputTag("AK5byRef"),
                                      physicsDefinition = cms.bool(True),
                                      leptonInfo = cms.bool(True)
                                      )

process.AK5byValPhysChs = cms.EDProducer("JetFlavourIdentifier",
                                      srcByReference = cms.InputTag("AK5byRefChs"),
                                      physicsDefinition = cms.bool(True),
                                      leptonInfo = cms.bool(True)
                                      )
# Flavour byValue AlgoDef
process.AK5byValAlgo = cms.EDProducer("JetFlavourIdentifier",
                                      srcByReference = cms.InputTag("AK5byRef"),
                                      physicsDefinition = cms.bool(False),
                                      leptonInfo = cms.bool(True))

process.AK5byValAlgoChs = cms.EDProducer("JetFlavourIdentifier",
                                      srcByReference = cms.InputTag("AK5byRefChs"),
                                      physicsDefinition = cms.bool(False),
                                      leptonInfo = cms.bool(True))

process.jetFlavour    = cms.Sequence(process.partons*process.AK5byRef*process.AK5byValPhys*process.AK5byValAlgo)
process.jetFlavourChs = cms.Sequence(process.partons*process.AK5byRefChs*process.AK5byValPhysChs*process.AK5byValAlgoChs)

#CHS
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0),maxZ = cms.double(24.0) ),
    src = cms.InputTag('offlinePrimaryVertices')
    )

process.load("CommonTools.ParticleFlow.pfNoPileUp_cff")
process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")
process.load("CommonTools.ParticleFlow.pfPhotons_cff")
process.load("CommonTools.ParticleFlow.pfElectrons_cff")
process.load("CommonTools.ParticleFlow.pfMuons_cff")
process.load("CommonTools.ParticleFlow.pfJets_cff")
process.load("CommonTools.ParticleFlow.TopProjectors.pfNoMuon_cfi")
process.load("CommonTools.ParticleFlow.TopProjectors.pfNoElectron_cfi")
process.load("CommonTools.ParticleFlow.TopProjectors.pfNoJet_cfi")
process.pfPileUp.PFCandidates          = 'particleFlow'
process.pfNoPileUp.bottomCollection    = 'particleFlow'
process.pfPileUpIso.PFCandidates       = 'particleFlow'
process.pfNoPileUpIso.bottomCollection = 'particleFlow'
process.pfPileUp.Enable                = True
process.pfPileUp.Vertices              = 'goodOfflinePrimaryVertices'
process.pfPileUp.checkClosestZVertex   = cms.bool(True)
process.pfJets.doAreaFastjet           = True
process.pfJets.doRhoFastjet            = False

process.chs                    = cms.Sequence(process.goodOfflinePrimaryVertices*
                                              process.pfNoPileUpSequence*
                                              process.pfParticleSelectionSequence*
                                              process.pfPhotonSequence*
                                              process.pfMuonSequence*
                                              process.pfNoMuon*
                                              process.pfElectronSequence*
                                              process.pfNoElectron*
                                              process.pfJetSequence*
                                              process.pfNoJet
                                              )
#                                  l1FastJetSequenceCHS
#                                     )

                                 
process.genana  = cms.Sequence(process.genJetParticles*process.ak5GenJets*process.jetFlavour*process.jetFlavourChs)
process.ana     = cms.Sequence(process.pfjetanalyzer*process.chspfjetanalyzer)
process.anareco = cms.Sequence(process.pfjetanalyzer)#+process.chspfjetanalyzer)
process.p       = cms.Path(process.MuonsFilter*process.chs*process.puJetIdSqeuence*process.puJetIdSqeuenceChs*process.genana*process.ana)#process.puJetIdSqeuenceChs*process.ana)
#process.p       = cms.Path(process.MuonsFilter*process.chs*process.ana)#process.puJetIdSqeuenceChs*process.ana)
#process.p = cms.Path(process.ana)#process.puJetIdSqeuence*process.anareco)#process.puJetIdSqeuenceChs*process.ana)

