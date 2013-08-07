import FWCore.ParameterSet.Config as cms


sep_line = '-'*70
########## CONTROL CARDS

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

##########



# Input  & JSON             -------------------------------------------------


# process.setName_('H2TAUTAU')

dataset_user = 'cmgtools' 
#dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0'
#dataset_name = '/TauPlusX/Run2011A-PromptReco-v4/AOD/V5/PAT_CMG_V5_2_0'
# dataset_name = '/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_2_0'
# dataset_name = '/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/V5/PAT_CMG_V5_2_0'
# dataset_name = '/VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0'

dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_3_0_TEST'

dataset_files = 'cmgTuple.*root'

## creating the source
#from CMGTools.Production.datasetToSource import *
#process.source = datasetToSource(
#    dataset_user,
#    dataset_name,
#    dataset_files,
#    )


process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1/cmgTuple_0.root'
    #'/store/cmst3/user/cmgtools/CMG/TauPlusX/Run2011B-PromptReco-v1/AOD/V5/PAT_CMG_V5_4_1/cmgTuple_999.root'
    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0_NewType1MET/cmgTuple_0.root'
    #'file:../../../Common/prod/TEST/cmgTuple_HToTauTau.root'
    )
    )

# process.source.fileNames = ['file:DYJets.root']


print process.source.fileNames

###ProductionTaskHook$$$
    
runOnMC = process.source.fileNames[0].find('Run201')==-1 and process.source.fileNames[0].find('embedded')==-1


# Sequence & path definition -------------------------------------------------

##path definition starts here-----------------------------
process.SkimPath = cms.Path()


####  ----------------------
process.load('CMGTools.Common.factories.cmgTauMu_cfi')
process.cmgTauMu.cfg.leg1Collection = 'cmgTauSel'
process.cmgTauMu.cfg.leg2Collection = 'cmgMuonSel'
process.SkimPath +=  process.cmgTauMu

process.load('CMGTools.Common.skims.cmgTauMuSel_cfi')
process.cmgTauMuSkim = process.cmgTauMuSel.clone()
process.cmgTauMuSkim.cut = cms.string('leg1().eta()!=leg2().eta() && leg1().pt()>18.0 && abs(leg1().eta())<2.3 && leg1().tauID("decayModeFinding")>0.5 && leg2().pt()>16.0 && abs(leg2().eta())<2.1' )
process.SkimPath +=  process.cmgTauMuSkim 


# event filter --------------------------------
process.load('CMGTools.Common.skims.cmgTauMuCount_cfi')
process.cmgTauMuCount.src = 'cmgTauMuSkim'
process.cmgTauMuCount.minNumber = 1
process.SkimPath +=  process.cmgTauMuCount



process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string( 'tauMu_fullsel_tree_CMG.root' ),
    SelectEvents   = cms.untracked.PSet(SelectEvents = cms.vstring('SkimPath')),
    outputCommands = cms.untracked.vstring('drop *')
    )
process.out.outputCommands.extend( [
    'keep cmgMuons_cmgMuonSel__PAT',
    'keep cmgTaus_cmgTauSel__PAT',
    'keep recoPFMETs_*_*_*',
    'keep cmgBaseMETs_*_*_PAT',
    'keep *_cmgPFBaseJetLead_*_PAT',
    'keep double_*_*_*',
    'keep int_*_*_*',
    'keep patMuons_patMuonsWithTrigger__PAT',
    'keep cmgTriggerObjects_cmgTriggerObjectSel__PAT',
    'keep cmgTriggerObjects_cmgTriggerObjectListSel__PAT',
    'keep recoVertexs_offlinePrimaryVertices__RECO',
    'keep cmgPFJets_cmgPFJetSel__PAT',
    'keep recoGenParticles_genParticlesPruned__PAT',
    'keep PileupSummaryInfos_addPileupInfo__HLT',
    'keep cmgMETSignificance_pfMetSignificance__PAT',
    'keep GenEventInfoProduct_generator_*_*',
    'keep GenFilterInfo_generator_*_*',
    ])


# Path definition --------------------------
process.outpath = cms.EndPath(process.out)
process.schedule = cms.Schedule(
    process.SkimPath,
    process.outpath
    )




