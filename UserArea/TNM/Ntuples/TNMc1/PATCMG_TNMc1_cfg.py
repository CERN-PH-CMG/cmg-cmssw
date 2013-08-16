import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X
# if not cmsswIs44X():
#    raise ValueError('Sorry, you are not working in 44X. use the correct cfg')

sep_line = '-'*67
print sep_line
print 'CMG PAT-tuplizer, contact Colin before any modification'
print sep_line

process = cms.Process("PAT")


print 'querying database for source files'


runOnMC = True


from CMGTools.Production.datasetToSource import *
#process.source = datasetToSource(
#   #'cmgtools_group',
#   # '/GluGluToHToZZTo4L_M-126_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5'
#   #'/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5',
#   'CMS',
#   '/JetHT/Run2012C-PromptReco-v1/AOD'
#   #'/JetHT/Run2012C-PromptReco-v2/AOD'
#   # '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V5-v2/AODSIM',
#   #'/TauPlusX/Run2011A-PromptReco-v4/AOD/V5'
#   # 'CMS',
#   # '/TauPlusX/Run2011A-03Oct2011-v1/AOD'
#   )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_17_1_Y2R.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_11_1_vyc.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_2_1_6WA.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_7_1_rqk.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_16_1_HsB.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_18_1_j1j.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_3_1_sH8.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_20_1_bLm.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_19_1_YNp.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_14_1_Jw6.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_6_1_5gi.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_13_1_D5s.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_15_1_XxY.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_10_1_fkb.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_12_1_bea.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_4_1_fxj.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_8_1_n6p.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_5_1_eA5.root',
#'/store/user/yxin/yxin_RSG_WW_1000_pythia6_01/yxin_RSG_WW_1000_pythia6_01/52e9c298e8547223f910bab8db11615e/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_9_1_3Al.root',

#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_11_1_gB9.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_19_1_MaO.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_14_1_5rP.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_3_1_ozZ.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_20_1_q6F.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_9_1_tIK.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_15_1_Ztn.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_10_1_avT.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_2_1_IBY.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_1_1_rRO.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_12_1_hwh.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_13_1_6k9.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_8_1_EE0.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_18_1_z9Q.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_7_1_WXc.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_4_1_SZz.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_5_1_nZV.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_17_1_PCY.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_6_1_dG3.root',
#'/store/user/yxin/yxin_RSG_WW_2000_pythia6_01/yxin_RSG_WW_2000_pythia6_01/54dbdee3e49fbf0c9fb4ed9452c44bd3/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_16_1_ce9.root',

#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_10_1_fkb.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_11_1_vyc.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_12_1_bea.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_13_1_D5s.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_1000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_14_1_Jw6.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_10_1_avT.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_11_1_gB9.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_12_1_hwh.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_13_1_6k9.root',
#'file:/afs/cern.ch/user/h/hinzmann/workspace/ditau/RSGravitonToWW_kMpl01_M_2000_TuneZ2star_8TeV_pythia6_cff_py_GEN_FASTSIM_HLT_14_1_5rP.root',
    )
)

print sep_line
print process.source.fileNames
print sep_line 

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

#### Adding AK7 pruned jets

from CMGTools.Common.PAT.jetSubstructure_cff import *
process.PATCMGSequence += PATCMGJetSequenceAK7CHSpruned

if runOnMC is False:
    # removing MC stuff
    print 'removing MC stuff, as we are running on Data'

    process.patElectrons.addGenMatch = False
    process.makePatElectrons.remove( process.electronMatch )
    
    process.patMuons.addGenMatch = False
    process.makePatMuons.remove( process.muonMatch )
    
    process.PATCMGSequence.remove( process.PATCMGGenSequence )
    process.PATCMGJetSequence.remove( process.jetMCSequence )
    process.PATCMGJetSequence.remove( process.patJetFlavourId )
    process.patJets.addGenJetMatch = False
    process.patJets.addGenPartonMatch = False

    if cmsswIs52X():
        process.PATCMGJetSequenceCHSpruned.remove( process.jetMCSequenceCHSpruned )
        process.patJetsCHSpruned.addGenJetMatch = False
        process.patJetsCHSpruned.addGenPartonMatch = False

    process.PATCMGTauSequence.remove( process.tauGenJets )
    process.PATCMGTauSequence.remove( process.tauGenJetsSelectorAllHadrons )
    process.PATCMGTauSequence.remove( process.tauGenJetMatch )
    process.PATCMGTauSequence.remove( process.tauMatch )
    process.patTaus.addGenJetMatch = False
    process.patTaus.addGenMatch = False

    process.patMETs.addGenMET = False 
    process.patMETsRaw.addGenMET = False 

    # setting up JSON file
    # json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt'
    # print 'using json file: ', json
    # from CMGTools.Common.Tools.applyJSON_cff import *
    # applyJSON(process, json )

    # adding L2L3Residual corrections
    process.patJetCorrFactors.levels.append('L2L3Residual')
    if cmsswIs52X():
        process.patJetCorrFactorsCHSpruned.levels.append('L2L3Residual')
        process.patJetCorrFactorsAK7CHSpruned.levels.append('L2L3Residual')


print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.PATCMGJetCHSSequence = cloneProcessingSnippet(process, process.PATCMGJetSequence, 'CHS')
process.PATCMGJetCHSSequence.insert( 0, process.ak5PFJetsCHS )
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.PATCMGJetCHSSequence, 'ak5PFJets', 'ak5PFJetsCHS')
replaceSrc( process.PATCMGJetCHSSequence, 'particleFlow', 'pfNoPileUp')
process.patJetCorrFactorsCHS.payload = 'AK5PFchs'


########################################################
## Path definition
########################################################

process.dump = cms.EDAnalyzer('EventContentAnalyzer')

process.load('CMGTools.Common.PAT.addFilterPaths_cff')
process.p = cms.Path(
    process.prePathCounter + 
    process.PATCMGSequence + 
    process.PATCMGJetCHSSequence 
    )

process.p += process.postPathCounter

# For testing, you can remove some of the objects:
# NOTE: there are a few dependencies between these sequences
# process.PATCMGSequence.remove(process.PATCMGPileUpSubtractionSequence)
# process.PATCMGSequence.remove(process.PATCMGRhoSequence)
# process.PATCMGSequence.remove(process.PATCMGMuonSequence)
# process.PATCMGSequence.remove(process.PATCMGElectronSequence)
# process.PATCMGSequence.remove(process.PATCMGGenSequence)
# process.PATCMGSequence.remove(process.PATCMGJetSequence)
# process.PATCMGSequence.remove(process.PATCMGTauSequence)
# process.PATCMGSequence.remove(process.PATCMGMetSequence)
# process.p.remove(process.PATCMGJetCHSSequence)


########################################################
## Setup electron energy corrections
########################################################

if cmsswIs44X():
    from CMGTools.Common.Tools.setupGsfElectronCalibration import setupGsfElectronCalibration
    setupGsfElectronCalibration( process, runOnMC )


########################################################
## PAT output definition
########################################################

## Output Module Configuration (expects a path 'p')
from CMGTools.Common.eventContent.patEventContentCMG_cff import patEventContentCMG
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = patEventContentCMG
                               )
# needed to override the CMG format, which drops the pat taus
process.out.outputCommands.append('keep patTaus_selectedPatTaus_*_*')

#FIXME now keeping the whole event content...
# process.out.outputCommands.append('keep *_*_*_*')

process.outpath = cms.EndPath(process.out)

########################################################
## CMG output definition
########################################################

from CMGTools.Common.eventContent.patEventContentCMG_cff import everything
process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = everything,
    dropMetaData = cms.untracked.string('PRIOR')
    )

process.outpath += process.outcmg




########################################################
## Conditions 
########################################################


process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

GT = None
if cmsswIs44X():
    if runOnMC:
        GT = 'START44_V13::All'
    else:
        GT = 'GR_R_44_V15::All'
else:
    if runOnMC:
        #GT = 'START52_V9::All'
        GT = 'START52_V9B::All' # for Summer12 MC
    else:
        #GT = 'GR_R_52_V7::All'
        #GT = 'GR_R_52_V7C::All' # for run2012A
        GT = 'GR_P_V41_AN1::All' # for run2012C
process.GlobalTag.globaltag = GT

print 'Global tag       : ', process.GlobalTag.globaltag


########################################################
## Below, stuff that you probably don't want to modify
########################################################



## Geometry and Detector Conditions (needed for a few patTuple production steps)

from CMGTools.Common.PAT.patCMGSchedule_cff import getSchedule
process.schedule = getSchedule(process, runOnMC)
process.schedule.append( process.outpath )

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

print sep_line

print 'Fastjet instances (dominating our processing time...):'
from CMGTools.Common.Tools.visitorUtils import SeqVisitor
v = SeqVisitor('FastjetJetProducer')
process.p.visit(v)

print sep_line

print 'starting CMSSW'


#### TNMc1 part

process.MessageLogger.cerr.default.limit = 5

# This is required in order to configure HLTConfigProducer
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")

# set up JSON ---------------------------------------------------------------

if runOnMC==False:
    from CMGTools.Common.Tools.applyJSON_cff import *
    #json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON.txt'
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-200245_8TeV_PromptReco_Collisions12_JSON.txt'
    print 'json:', json
    applyJSON(process, json )

process.load("Ntuples.TNMc1.ntuple_cfi")

from CMGTools.External.pujetidsequence_cff import *

process.selectedPatJetspuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("selectedPatJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

process.selectedPatJetsCHSpuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("selectedPatJetsCHS"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

process.ak5PFJetsCHSprunedSubJetspuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("ak5PFJetsCHSpruned:SubJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

process.p += cms.Sequence(process.selectedPatJetspuJetId * process.selectedPatJetsCHSpuJetId * process.ak5PFJetsCHSprunedSubJetspuJetId * process.demo)

### Tweaks to run on fastsim samples

process.schedule.remove(process.hcalLaserFilterFromAODPath)
process.schedule.remove(process.CSCTightHaloFilterPath)
process.schedule.remove(process.HBHENoiseFilterPath)
process.demo.buffers.remove('edmTriggerResultsHelper1')
process.p.remove(process.PATCMGTauSequence)
process.demo.buffers.remove('patTau')
process.p.remove(process.PATCMGMetSequence)
process.p.remove(process.MetSignificanceSequence)
process.demo.buffers.remove('patMET')
process.demo.buffers.remove('patMET1')
process.p.remove(process.vertexWeightSequence)
process.outpath = cms.EndPath()