# Auto generated configuration fileimport FWCore.Parame
# using: 
# Revision: 1.341 
# Source: /cvs/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: SingleQuarkGun_cfi.py --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco,RECO --conditions START44_V10::All --pileup mix_E7TeV_Chamonix2012_50ns_PoissonOOT --datamix NODATAMIXER --eventcontent AODSIM --datatier AODSIM -n 25
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_E7TeV_Chamonix2012_50ns_PoissonOOT_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic7TeV2011Collision_cfi')

# Output definition

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
					eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
					outputCommands = process.AODSIMEventContent.outputCommands,
					fileName = cms.untracked.string('SingleQuarkGun_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_PU.root'),
					dataset = cms.untracked.PSet(   filterName = cms.untracked.string(''),
									dataTier = cms.untracked.string('AODSIM')
									),
					SelectEvents = cms.untracked.PSet(  SelectEvents = cms.vstring('generation_step')
									    )
					)

# Additional output definition



###########################
process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.pfTools import *
from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets
from PhysicsTools.PatAlgos.tools.trackTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
#PF2PAT
postfix = "PFlow"

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#jet energy corrections
# cf. https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#JetEnCor2011V2
# cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/JECDataMC
jetAlgo='AK5'
jecSetPF = jetAlgo+'PFchs'
jecLevels=['L1FastJet','L2Relative','L3Absolute']

#rho computed up to 2.5
#process.load('RecoJets.Configuration.RecoPFJets_cff')
#process.kt6PFJets25 = process.kt6PFJets.clone( doRhoFastjet = True )
#process.kt6PFJets25.Rho_EtaMax = cms.double(2.5)

#start PF2PAT
print "calling pf2pat"
usePF2PAT(process,
	  runPF2PAT=True,
	  jetAlgo=jetAlgo,
	  runOnMC=True,
	  postfix=postfix,
	  jetCorrections=(jecSetPF, jecLevels),
	  pvCollection=cms.InputTag('offlinePrimaryVertices'),
	  typeIMetCorrections=True,
	  outputModules=['AODSIMoutput']
	  )
print "***** passed here******"
#compute rho and prepend to the jet clustering sequence in PF2PAT
#from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
#process.kt6PFJets = kt4PFJets.clone( rParam = cms.double(0.6),
#				     doAreaFastjet = cms.bool(True),
#				     doRhoFastjet = cms.bool(True)
#				     )
getattr(process,'patJetCorrFactors'+postfix).rho = cms.InputTag("kt6PFJets", "rho")
getattr(process,"patPF2PATSequence"+postfix).replace( getattr(process,"pfNoElectron"+postfix), getattr(process,"pfNoElectron"+postfix)*process.kt6PFJets )


#configure top projections
getattr(process,"pfNoPileUp"+postfix).enable = True
getattr(process,"pfNoMuon"+postfix).enable = True
getattr(process,"pfNoMuon"+postfix).verbose = False
getattr(process,"pfNoElectron"+postfix).enable = True
getattr(process,"pfNoTau"+postfix).enable = False
getattr(process,"pfNoJet"+postfix).enable = False
applyPostfix(process,"pfIsolatedMuons",postfix).isolationCut = cms.double(9999.)
applyPostfix(process,"pfIsolatedElectrons",postfix).isolationCut = cms.double(9999.)

removeSpecificPATObjects( process, names=['Taus'], postfix=postfix, outputModules=['AODSIMoutput'] )
removeSpecificPATObjects( process, names=['Taus'], outputModules=['AODSIMoutput'])

#################################################
# CUSTOM PAT                                    #
#################################################
applyPostfix( process, 'patMuons', '' ).embedCaloMETMuonCorrs=cms.bool(False)
applyPostfix( process, 'patMuons', '' ).embedTcMETMuonCorrs=cms.bool(False)
switchJetCollection(process,
		    cms.InputTag('ak5PFJets'),
		    jetIdLabel = 'AK5',
		    doJTA        = True,
		    doBTagging   = True,
		    jetCorrLabel = ('AK5PF', jecLevels),
		    doType1MET   = False,
		    genJetCollection=cms.InputTag("ak5GenJets"),
		    doJetID      = True,
		    postfix='',
		    outputModules=['AODSIMoutput']
		    )
process.patJets.embedPFCandidates = False
     
print 1
removeCleaning( process,outputModules=['AODSIMoutput'])
print 2
getattr(process,'patPF2PATSequence'+postfix).remove( getattr(process,'photonMatch'+postfix) )
print 3
removeSpecificPATObjects( process, names=['Taus','METs'],outputModules=['AODSIMoutput'] )
print " *** PAT path has been defined"

process.patPath = cms.Path(#process.recoPFJets*
			  getattr(process,"patPF2PATSequence"+postfix)*
			  process.patDefaultSequence)
############################

#from CMG.JetIDAnalysis.minBiasSource_cfg import FileNames as FullMinBiasFileNames
#process.mix.input.fileNames = FullMinBiasFileNames


process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

<<<<<<< SingleQuarkGun_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_PU.py

=======
from CMG.JetIDAnalysis.minBiasSource_cfg import FileNames as FullMinBiasFileNames
process.mix.input.fileNames = FullMinBiasFileNames
#process.mix.input.fileNames = cms.untracked.vstring('/store/relval/CMSSW_4_4_2_patch7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/START44_V9_special_111126-v1/0072/269DF920-8118-E111-945D-003048FFD76E.root',
#						    '/store/relval/CMSSW_4_4_2_patch7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/START44_V9_special_111126-v1/0072/2E28D79F-8218-E111-9E38-003048679084.root',
#						    '/store/relval/CMSSW_4_4_2_patch7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/START44_V9_special_111126-v1/0072/5C4758B3-C818-E111-92FC-003048678FFA.root',
#						    '/store/relval/CMSSW_4_4_2_patch7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/START44_V9_special_111126-v1/0071/4466B2D8-6518-E111-A761-0026189438FD.root')
#print "Hey: random seed is", process.RandomNumberGeneratorService.generator.initialSeed
import random
random.seed(process.RandomNumberGeneratorService.generator.initialSeed.value())
random.shuffle(process.mix.input.fileNames)
flist=open('list_of_pu.txt','w+')
print >> flist, process.mix.input.fileNames
flist.close()
>>>>>>> 1.3

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(25)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
<<<<<<< SingleQuarkGun_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_PU.py
process.configurationMetadata = cms.untracked.PSet( version = cms.untracked.string('$Revision: 1.2 $'),
						    annotation = cms.untracked.string('SingleQuarkGun_cfi.py nevts:25'),
						    name = cms.untracked.string('PyReleaseValidation')
						    )
=======
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.3 $'),
    annotation = cms.untracked.string('SingleQuarkGun_cfi.py nevts:25'),
    name = cms.untracked.string('PyReleaseValidation')
)
>>>>>>> 1.3


# Other statements
process.GlobalTag.globaltag = 'START44_V10::All'

process.generator = cms.EDProducer("Pythia6PtGun",
    PGunParameters = cms.PSet(
        MinPhi = cms.double(-3.14159265359),
        MinPt = cms.double(15.0),
        ParticleID = cms.vint32(1),
        MaxEta = cms.double(5),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-5),
        AddAntiParticle = cms.bool(False),
        MaxPt = cms.double(300)
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single quark pt 15-300'),
    firstRun = cms.untracked.uint32(1),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTU(21)=1     ! Check on possible errors during program execution', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'PARP(82)=1.832 ! pt cutoff for multiparton interactions', 
            'PARP(89)=1800. ! sqrts for which PARP82 is set', 
            'PARP(90)=0.275 ! Multiple interactions: rescaling power', 
            'MSTP(95)=6     ! CR (color reconnection parameters)', 
            'PARP(77)=1.016 ! CR', 
            'PARP(78)=0.538 ! CR', 
            'PARP(80)=0.1   ! Prob. colored parton from BBR', 
            'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
            'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
            'PARP(62)=1.025 ! ISR cutoff', 
            'MSTP(91)=1     ! Gaussian primordial kT', 
            'PARP(93)=10.0  ! primordial kT-max', 
            'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model'),
        parameterSets = cms.vstring('pythiaUESettings')
    )
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.patPath,process.endjob_step,process.AODSIMoutput_step])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

