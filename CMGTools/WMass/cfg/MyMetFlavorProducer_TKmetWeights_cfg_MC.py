import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("MyMET")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
# numberOfFilesToProcess = 100
numberOfFilesToProcess = 1

# Input  & JSON             -------------------------------------------------
# process.lumiJSON = cms.EDAnalyzer("JsonLogger",
                                  # fileName = cms.untracked.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt")
                                  # )

# process.setName_('H2TAUTAU')

dataset_user = 'cmgtools_group' 
# dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
# dataset_name = '/DoubleMu/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_V5_6_0_B'
# dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START44_V9B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
# dataset_name = '/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Fall11-PU_S6_START44_V9B-v1/AODSIM/V5_B/PAT_CMG_5_6_0_B_pfcandidates'
dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B'
dataset_files = 'PFAOD.*root'

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

# restricting the number of files to process to a given number
if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

import pprint
pprint.pprint(process.source.fileNames[:10])
print '.. and so on.'


###ProductionTaskHook$$$
    
process.load('CMGTools.Common.skims.cmgDiMuonSel_cfi')
process.load('CMGTools.Common.skims.cmgDiMuonCount_cfi')

process.cmgDiMuonSel.src = 'cmgDiMuonSel'
process.cmgDiMuonSel.cut = ('mass()>40 && (leg1().pt()>25 && leg2().pt()>6)')
process.cmgDiMuonCount.minNumber = 1


from CMGTools.WMass.myMetFlavors_cff import *
# COMMA SEPARATED AMONG "X,h,e,mu,gamma,h0,hHF,egammaHF"
process.myPFmet         =  myMetFlavor.clone(  
                                            MetFlavor = cms.int32(0),
                                            candFlavor      = cms.string(""), 
                                            candPtMin       = cms.double(0.0),
                                            candEtaMin      = cms.double(-100.0),
                                            candEtaMax      = cms.double(100.0),
                                            )
myMetEtaCuts=[-5.0,-3.0,-2.4,-2.1,-1.4,1.4,2.1,2.4,3.0,5.0]
myMetPtCuts=[0.0,0.5,1.0,1.5,2.0]
#INCLUSIVE
process.mygammaPFmet         =  process.myPFmet.clone(  
                                                  candFlavor      = cms.string("gamma"), 
                                            )
process.myh0PFmet         =  process.myPFmet.clone(  
                                                  candFlavor      = cms.string("h0"), 
                                            )
process.myhegammaHFPFmet         =  process.myPFmet.clone(  
                                                  candFlavor      = cms.string("h_HF,egamma_HF"), 
                                            )
process.mytkmethemuPFmet         =  process.myPFmet.clone(  
                                                  MetFlavor = cms.int32(1),
                                                  candFlavor      = cms.string("h,e,mu"), 
                                            )
                                           
process.myStrangeTKMet   = cms.EDProducer(
    "MyMetFlavorProducerWeights",
    MetFlavor       = cms.int32(2),  # 0 genPFMET  1 genTkMET  2 pfTKMET_strangetk
    vtxTag          = cms.InputTag("offlinePrimaryVertices"),
    myweight        = cms.untracked.double(0.05),
    dzcut           = cms.untracked.double(0.1),
    isMC            = cms.bool(True),
    chargedCandPtMin = cms.untracked.double(0.0),
    # PFCandidateName = cms.InputTag("cmgCandidates"),
    # dZMin           = cms.double(0.1),
    # candEtaMin = cms.double(0.0),
    # candEtaMax = cms.double(100.0),
    # neutralCandPMin = cms.double(0.0),
    # candFlavor = cms.string(''),    
    )

process.p = cms.Path( 
                      # (process.cmgDiMuonSel +
                      # process.cmgDiMuonCount )
                      # +process.mygammaPFmet
                      # +process.myh0PFmet
                      # +process.myhegammaHFPFmet
                      # +process.mytkmethemuPFmet
                        process.myStrangeTKMet
                      )

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('PFAOD.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = cms.untracked.vstring('keep *')
    )

process.endpath = cms.EndPath(process.out)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
