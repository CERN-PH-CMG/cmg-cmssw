#$Revision: 1.20 $
import FWCore.ParameterSet.Config as cms

process = cms.Process("TheNtupleMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
# See TheNtupleMaker twiki for a brief explanation
#process.MessageLogger.destinations = cms.untracked.vstring("cerr")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 5

# This is required in order to configure HLTConfigProducer
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")


# Get Will's SUSY MJ objects/sequences:
process.load("Configuration.StandardSequences.GeometryDB_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

# Run on MC or data

runOnMC = False
runPATCMG = False
recalibrateJets = True

# Input file

dataset_user = 'cmgtools' 
##dataset_name = '/SingleMu/Run2012A-13Jul2012-v1/AOD/PAT_CMG_V5_6_0_B'
#dataset_name = '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
#dataset_name = '/HT/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076'
dataset_name = '/HT/Run2012A-13Jul2012-v1/AOD/PAT_CMG_V5_6_0_B'
#dataset_name = '/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM/V5/PAT_CMG_V5_4_0'
dataset_files = 'patTuple.*root'

if runPATCMG:
    dataset_user = 'CMS' 
    dataset_name = '/HT/Run2012A-13Jul2012-v1/AOD'
    dataset_files = '*.root'

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

print 'input:', process.source.fileNames

# set up JSON ---------------------------------------------------------------

if runOnMC==False:
    from CMGTools.Common.Tools.applyJSON_cff import *
    # Run2012 A+B 13Jul2012ReReco
    #json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt'
    # Run2012 A 06Aug2012ReReco
    #json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
    # Run2012C v1+v2
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
    # Run2012D
    #json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
    print 'json:', json
    applyJSON(process, json )

print 'runOnMC:', runOnMC

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

#process.p = cms.Path(process.selectedPatJetspuJetId * process.selectedPatJetsCHSpuJetId * process.ak5PFJetsCHSprunedSubJetspuJetId * process.demo)

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

GT = None
if runOnMC:
    GT = 'START53_V15::All' # for Summer12 MC
else:
    GT = 'GR_P_V39_AN3::All' # for Moriond data
process.GlobalTag.globaltag = GT

#### AK5 CHS jets
process.load('CMGTools.Common.PAT.PATCMG_cff')

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

    process.PATCMGTauSequence.remove( process.tauGenJets )
    process.PATCMGTauSequence.remove( process.tauGenJetsSelectorAllHadrons )
    process.PATCMGTauSequence.remove( process.tauGenJetMatch )
    process.PATCMGTauSequence.remove( process.tauMatch )
    process.patTaus.addGenJetMatch = False
    process.patTaus.addGenMatch = False

    process.patMETs.addGenMET = False 
    process.patMETsRaw.addGenMET = False 

    # adding L2L3Residual corrections
    process.patJetCorrFactors.levels.append('L2L3Residual')

#### AK5 CHS

print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.PATCMGJetCHSSequence = cloneProcessingSnippet(process, process.PATCMGJetSequence, 'CHS')
process.PATCMGJetCHSSequence.insert( 0, process.ak5PFJetsCHS )
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.PATCMGJetCHSSequence, 'ak5PFJets', 'ak5PFJetsCHS')
replaceSrc( process.PATCMGJetCHSSequence, 'particleFlow', 'pfNoPileUp')
process.patJetCorrFactorsCHS.payload = 'AK5PFchs'
process.selectedPatJetsCHS.cut = 'pt()>10'

##### AK7 jets

process.load("CMGTools.Common.PAT.jetSubstructure_cff")
if not runOnMC:
    process.PATCMGJetSequenceAK7CHSpruned.remove( process.jetMCSequenceAK7CHSpruned )
    process.patJetsAK7CHSpruned.addGenJetMatch = False
    process.patJetsAK7CHSpruned.addGenPartonMatch = False
    process.patJetCorrFactorsAK7CHSpruned.levels.append('L2L3Residual')

process.load("Ntuples.TNMc1.PAT_ak7jets_cff")
if not runOnMC:
    process.PATCMGJetSequenceAK7CHS.remove( process.jetMCSequenceAK7CHS )
    process.patJetsAK7CHS.addGenJetMatch = False
    process.patJetsAK7CHS.addGenPartonMatch = False
    process.patJetCorrFactorsAK7CHS.levels.append('L2L3Residual')

#### Adding CA8 jets and CA8 pruned jets

process.load("ExoDiBosonResonances.PATtupleProduction.PAT_ca8jets_cff")
if not runOnMC:
    process.PATCMGJetSequenceCA8CHS.remove( process.jetMCSequenceCA8CHS )
    process.patJetsCA8CHS.addGenJetMatch = False
    process.patJetsCA8CHS.addGenPartonMatch = False
    process.patJetCorrFactorsCA8CHS.levels.append('L2L3Residual')
    process.PATCMGJetSequenceCA8CHSpruned.remove( process.jetMCSequenceCA8CHSpruned )
    process.patJetsCA8CHSpruned.addGenJetMatch = False
    process.patJetsCA8CHSpruned.addGenPartonMatch = False
    process.patJetCorrFactorsCA8CHSpruned.levels.append('L2L3Residual')

##### Razor stuff

process.load("CMGTools.Susy.RazorMultiJet.razorMultijet_cff")
process.load("CMGTools.Susy.common.susy_cff")

process.razorMJObjectSequence.remove(process.razorMJHemiSequence)
# This is the UCSB tau veto.  Need to remove for SMSs:
process.razorMJTauSequence.remove(process.razorMJTauVeto)

if not runOnMC:
    process.demo.buffers.remove('sint')
    process.demo.buffers.remove('recoLeafCandidate')

if runOnMC:
    process.demo.buffers.remove('hcalFilter')
    process.demo.buffers.remove('edmTriggerResultsHelper')
    process.demo.buffers.remove('edmTriggerResultsHelper1')

##### Sequence

print 'Global tag       : ', process.GlobalTag.globaltag

process.p = cms.Path()
process.schedule = cms.Schedule(process.p)
if runPATCMG:
  process.load('CMGTools.Common.PAT.addFilterPaths_cff')
  process.p = cms.Path(
    process.PATCMGSequence + 
    process.PATCMGJetCHSSequence 
  )
  from CMGTools.Common.PAT.patCMGSchedule_cff import getSchedule
  process.schedule = getSchedule(process, runOnMC, False)


if runOnMC==True:
    #process.tnmc1 = cms.Sequence(process.razorMJObjectSequence+process.susyGenSequence+process.PATCMGJetSequenceAK7CHS+process.PATCMGJetSequenceCA8CHS+process.PATCMGJetSequenceAK7CHSpruned+process.PATCMGJetSequenceCA8CHSpruned+process.demo)
    process.tnmc1 = cms.Sequence(process.razorMJObjectSequence+process.susyGenSequence+process.PATCMGJetSequenceCA8CHS+process.PATCMGJetSequenceCA8CHSpruned+process.demo)
else:
    #process.tnmc1 = cms.Sequence(process.razorMJObjectSequence+process.PATCMGJetSequenceAK7CHS+process.PATCMGJetSequenceCA8CHS+process.PATCMGJetSequenceAK7CHSpruned+process.PATCMGJetSequenceCA8CHSpruned+process.demo)
    process.tnmc1 = cms.Sequence(process.razorMJObjectSequence+process.PATCMGJetSequenceCA8CHS+process.PATCMGJetSequenceCA8CHSpruned+process.demo)
process.p += process.tnmc1

##### HCAL laser filter for 2012

if not runOnMC:
    process.load("Ntuples.TNMc1.hcallasereventfilter2012_cfi")
    #inputfilelist=["data/AllBadHCALLaser.txt"]
    #for f in inputfilelist:
    #    mylist=open(f,'r').readlines()
    #     for j in mylist:
    #         process.hcallasereventfilter2012.EventList.append(j.strip())
    print "load laser event list"
    from Ntuples.TNMc1.AllBadHCALLaser import eventlist
    process.hcallasereventfilter2012.EventList=eventlist
    process.hcallasereventfilter2012Path=cms.Path(process.hcallasereventfilter2012)
    process.schedule = cms.Schedule(process.hcallasereventfilter2012Path,*[p for p in process.schedule])

#### no AK5 pruned
if runPATCMG:
    process.p.remove(process.PATCMGJetSequenceCHSpruned)
process.p.remove(process.razorMJStructureJetSel30)

#### recalibrated jets
if recalibrateJets:
  from CMGTools.Common.miscProducers.cmgPFJetCorrector_cfi import cmgPFJetCorrector
  process.cmgPFJetCor = cmgPFJetCorrector.clone(src='cmgPFJetSel',
					      payload='AK5PF')
  process.cmgPFJetCorCHS = cmgPFJetCorrector.clone(src='cmgPFJetSelCHS',
						 payload='AK5PFchs')
  from CMGTools.Common.skims.cmgPFJetSel_cfi import cmgPFJetSel
  process.cmgPFJetSel = cmgPFJetSel.clone(src='cmgPFJetCor',
					      cut='pt()>30')
  process.cmgPFJetSelCHS = cmgPFJetSel.clone(src='cmgPFJetCorCHS',
					      cut='pt()>30')
  if runOnMC:
    process.cmgPFJetCor.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute')
    process.cmgPFJetCorCHS.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute')
  else:
    process.cmgPFJetCor.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute','L2L3Residual')
    process.cmgPFJetCorCHS.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute','L2L3Residual')
  process.p.insert(0,process.cmgPFJetSelCHS)
  process.p.insert(0,process.cmgPFJetSel)
  process.p.insert(0,process.cmgPFJetCorCHS)
  process.p.insert(0,process.cmgPFJetCor)

#### evaluate speed

print 'Fastjet instances (dominating our processing time...):'
from CMGTools.Common.Tools.visitorUtils import SeqVisitor
v = SeqVisitor('FastjetJetProducer')
process.p.visit(v)

