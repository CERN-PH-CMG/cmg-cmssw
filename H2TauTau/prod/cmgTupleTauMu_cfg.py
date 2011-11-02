from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os.path

vertexCollection = "offlinePrimaryVertices"

process.setName_('H2TAUTAU')

#process.source.fileNames = cms.untracked.vstring(['/store/cmst3/user/benitezj/CMG/GluGluToHToTauTau_M-105_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/tree_CMG_1.root'])

dataset_user = 'cmgtools'
#dataset_name = '/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_3_0'
#dataset_name = '/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_3_0'
#dataset_name = '/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2/PAT_CMG_V2_3_0'
#dataset_name = '/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_3_0'
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(dataset_user,dataset_name,'tree.*root') 

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.out.fileName = cms.untracked.string(os.path.expandvars('tree_CMG.root'))
process.TFileService = cms.Service("TFileService",fileName = cms.string("histograms_CMG.root"))
runOnMC = False
selectSS = False

####apply json
if runOnMC == False:
    from CMGTools.Common.Tools.applyJSON_cff  import *
    applyJSON(process,'/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/json/finalTauPlusX.txt')
    
######
process.load('CMGTools.Common.tauMu_cff')
process.tauMuFactory.leg1Collection = cms.InputTag("cmgTauSel")
process.tauMuFactory.leg2Collection = cms.InputTag("cmgMuonSel")
process.cmgTauMu.cfg = process.tauMuFactory.clone()

######
process.load('CMGTools.H2TauTau.skims.cmgTauMuSelClean_cff')
process.load('CMGTools.H2TauTau.cmgDiJetVBF_cff')

#####select the SS sample
if selectSS:
    print "Warning: Selecting SS sample\n";
    process.cmgTauMuSelCharge.cut = cms.string( " abs(charge()) == 2.0 " )

##Filter events without candidates
process.EventFilter = cms.EDFilter("CandViewCountFilter",
                                   src = cms.InputTag("cmgTauMuSelClean"),
                                   minNumber = cms.uint32(1),
                                   )

###########
process.analysisSequence = cms.Sequence(process.tauMuSequence
                                        +process.cmgTauMuSelCleanSequence
                                        +process.cmgTauMuSelCleanHistogramsSequence
                                        +process.cmgDiJetVBFSequence
                                        +process.EventFilter
                                        )

process.p = cms.Path(process.analysisSequence)

###########
from CMGTools.Common.eventContent.runInfoAccounting_cff import *
process.out.outputCommands = cms.untracked.vstring( 'drop *') + runInfoAccounting
process.out.outputCommands.append( 'keep *_addPileupInfo_*_HLT')
process.out.outputCommands.append( "keep *_"+vertexCollection+"_*_RECO" )
process.out.outputCommands.append( 'keep *_TriggerResults_*_PAT')
process.out.outputCommands.append( 'keep *_cmgTriggerObjectSel_*_PAT')
process.out.outputCommands.append( 'keep *_cmgTauSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgMuonSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgElectronSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgPFJetSel_*_*' )
process.out.outputCommands.append( 'keep *_cmgPFMET_*_*')
process.out.outputCommands.append( 'keep *_cmgTauMuSelClean_*_*' )
process.out.outputCommands.append( 'keep *_cmgDiJetVBFSel_*_*')
process.out.outputCommands.append( 'keep *_PFMETSignificance*_*_*')


if runOnMC: 
    process.out.outputCommands.append( 'keep *_genParticlesStatus3_*_PAT')






