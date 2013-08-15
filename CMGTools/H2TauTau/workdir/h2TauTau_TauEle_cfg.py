import FWCore.ParameterSet.Config as cms


sep_line = '-'*70
########## CONTROL CARDS

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )


process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )



##########



# Input  & JSON             -------------------------------------------------


# process.setName_('H2TAUTAU')

dataset_user = 'cmgtools' 
#dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1'
dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/V5/PAT_CMG_V5_4_0_NewType1MET'
dataset_files = 'cmgTuple.*root'

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(dataset_user,dataset_name,dataset_files)



#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1/cmgTuple_0.root'
#    #'file:../../../Common/prod/TEST/cmgTuple_HToTauTau.root'
#    )
#    )

# process.source.fileNames = ['file:DYJets.root']

print process.source.fileNames

###ProductionTaskHook$$$
    
runOnMC = process.source.fileNames[0].find('Run201')==-1 and process.source.fileNames[0].find('embedded')==-1


# Sequence & path definition -------------------------------------------------


## set up JSON ---------------------------------------------------------------
#if runOnMC==False:
#    from CMGTools.H2TauTau.tools.setupJSON import setupJSON
#    json = setupJSON(process)
#    print 'json:', json
#    print process.PoolSource.lumisToProcess

##path definition starts here-----------------------------
process.TauElePath = cms.Path()


## gen  ---------------------------
#if runOnMC:
#    process.load('CMGTools.Common.generator.vertexWeight.vertexWeight_cff')
#    process.genSequence = cms.Sequence(
#        process.vertexWeightSequence 
#        )
#    process.TauElePath += process.genSequence 


## tau-mu selections ----------------------
process.load('CMGTools.Common.factories.cmgTauScaler_cfi')
process.TauElePath +=  process.cmgTauScaler
# process.cmgTauScaler.cfg.nSigma = -1

process.load('CMGTools.Common.factories.cmgTauEle_cfi')
#process.cmgTauEle.cfg.leg1Collection = 'cmgTauSel'
process.cmgTauEle.cfg.leg1Collection = 'cmgTauScaler'
process.cmgTauEle.cfg.leg2Collection = 'cmgElectronSel'
process.cmgTauEle.cfg.metCollection = 'cmgPFMETRaw'
process.TauElePath +=  process.cmgTauEle

##apply the preselections
process.load('CMGTools.Common.skims.cmgTauEleSel_cfi')
process.cmgTauElePreSel = process.cmgTauEleSel.clone()
process.cmgTauElePreSel.cut = cms.string('mass()>10.0 && leg1().pt()>19.0 && abs(leg1().eta())<2.3 && leg1().tauID("decayModeFinding")>0.5 && leg2().pt()>19.0 && abs(leg2().eta())<2.1' )
process.TauElePath +=  process.cmgTauElePreSel 

# event filter --------------------------------
process.load('CMGTools.Common.skims.cmgTauEleCount_cfi')
process.cmgTauEleCount.src = 'cmgTauElePreSel'
process.cmgTauEleCount.minNumber = 1
process.TauElePath +=  process.cmgTauEleCount



## output definition
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string( 'tauEle_fullsel_tree_CMG.root' ),
    SelectEvents   = cms.untracked.PSet(SelectEvents = cms.vstring('TauElePath')),
    outputCommands = cms.untracked.vstring('drop *')
    )
process.out.outputCommands.extend( [
    'keep cmgTaucmgElectroncmgDiObjects_*_*_*',
    'drop *_cmgTauEle_*_*',
    'keep recoPFMETs_*_*_*',
    'keep *_cmgPFBaseJetLead_*_*',
    'keep *_mvaMETTauEle_*_*',
    'keep double_*_*_*',
    'keep int_*_*_*',
    'keep *_patElectronsWithTrigger_*_*',
    'keep cmgTriggerObjects_cmgTriggerObjectSel__PAT',
    'keep cmgTriggerObjects_cmgTriggerObjectListSel__PAT',
    'keep recoVertexs_offlinePrimaryVertices__RECO',    
    'keep cmgPFJets_cmgPFJetSel__PAT',
    'keep recoGenParticles_genParticlesPruned__PAT',
    'keep cmgElectrons_cmgElectronSel__PAT',
    'keep PileupSummaryInfos_addPileupInfo__HLT',
    'keep cmgMETSignificance_pfMetSignificance__PAT',
    'keep cmgBaseMETs_*_*_*',    
    'keep GenEventInfoProduct_generator_*_*',
    'keep GenFilterInfo_generator_*_*',
    ])



# Path definition --------------------------
process.outpath = cms.EndPath(process.out)
process.schedule = cms.Schedule(
    process.TauElePath,
    process.outpath
    )



