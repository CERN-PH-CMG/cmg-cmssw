# Auto generated configuration file
# using: 
# Revision: 1.303.2.3 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/test_cff.py -s GEN --conditions DESIGN311_V2::All --datatier GEN-SIM --eventcontent RAWSIM --no_exec -n 100
import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic7TeV2011Collision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(3)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.303.2.3 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/python/test_cff.py nevts:100'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
#    fileName = cms.untracked.string('/tmp/mgouzevi/test_cff_py_GEN.root'),
    fileName = cms.untracked.string('/tmp/mgouzevi/toto.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'DESIGN311_V2::All'

process.genParticlesForFilter = cms.EDProducer("GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("generator"),
    abortOnUnknownPDGCode = cms.untracked.bool(False)
)


process.XtoFFbarFilter = cms.EDFilter("XtoFFbarFilter",
    src = cms.InputTag("genParticlesForFilter"),
    idMotherY = cms.vint32(6000111, 6000112, 6000113),
    idMotherX = cms.vint32(6000111, 6000112, 6000113),
    idDaughterF = cms.vint32(1, 2, 3, 4, 5, 
        6, 11, 13, 15),
    idDaughterG = cms.vint32(1, 2, 3, 4, 5, 
        6, 11, 13, 15)
)


process.genParticles = cms.EDProducer("GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("generator"),
    abortOnUnknownPDGCode = cms.untracked.bool(False)
)


process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    UseExternalGenerators = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(3),
    filterEfficiency = cms.untracked.double(0.99),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(7000),
    maxEventsToPrint = cms.untracked.int32(3),
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
        parameterSets = cms.vstring('pythiaUESettings', 
            'pythiaParameters', 
            'PYUPDAParameters'),
#	PYUPDAParameters = cms.vstring("PYUPDAFILE = \'pyupda.out\'"),
#	PYUPDAParameters = cms.vstring("PYUPDAFILE = \'Configuration/Generator/data/ZprimeTo2LongLived.pyupda\'"),#, "PYUPDApostPYNIT"),
        PYUPDAParameters = cms.vstring("PYUPDAFILE = \'Configuration/Generator/data/ZprimeTo2LongLived_400GeV_50GeV_80mm_ee.pyupda\'"),#, "PYUPDApostPYNIT"),
#	PYUPDAParameters = cms.vstring("PYUPDAFILE = \'Configuration/Generator/data/test.pyupda\'"),
#	PYUPDAParameters = cms.vstring("PYUPDAFILE = \'Configuration/Generator/data/HTo2LongLivedTo4F_1000GeV_150GeV_100mm.pyupda\'"),
#        PYUPDAParameters = cms.vstring("PYUPDAFILE = \'pyupda.out\'", 
#            'PYUPDAWRITE'),
        pythiaParameters = cms.vstring('MSTJ(22)=4    ! Require both long-lived exotics to decay inside the CMS volume', 
            'PARJ(73)=7000. ! radius (mm) of CMS cylinder', 
            'PARJ(74)=9000. ! half (mm) length of CMS cylinder', 
            'MSEL=0', 
            'MSUB(141)=1', 
	    'MSTP(44)=3', # No interference
#	    'MSTP(44)=0', # Phase Space Z' 			      
#            'MWID(32)=2 ! Let me set H0 propertiesPMAS(32,1)=1000.000000 ! This probably has no effect ...'#0, 
            'PMAS(32,1)=400.'
				       )
    )
)

  ##      processParameters = cms.vstring('PMAS(32,1)= 2000.           !mass of Zprime', 
##             'MSEL=0                    !(D=1) to select between full user control (0, then use MSUB) and some preprogrammed alternative', 
##             'MSTP(44) = 3              !only select the Z process', 
##             'MSUB(141) = 1             !ff  gamma z0 Z0', 
##             'MDME(289,1)= 1            !d dbar', 
##             'MDME(290,1)= 1            !u ubar', 
##             'MDME(291,1)= 1            !s sbar', 
##             'MDME(292,1)= 1            !c cbar', 
##             'MDME(293,1)= 0            !b bar', 
##             'MDME(294,1)= 0            !t tbar', 
##             'MDME(295,1)= 0            !4th gen Q Qbar', 
##             'MDME(296,1)= 0            !4th gen Q Qbar', 
##             'MDME(297,1)= 0            !e e', 
##             'MDME(298,1)= 0            !neutrino e e', 
##             'MDME(299,1)= 0            ! mu mu', 
##             'MDME(300,1)= 0            !neutrino mu mu', 
##             'MDME(301,1)= 0            !tau tau', 
##             'MDME(302,1)= 0            !neutrino tau tau', 
##             'MDME(303,1)= 0            !4th generation lepton', 
##             'MDME(304,1)= 0            !4th generation neutrino', 
##             'MDME(305,1)= 0            !W W', 
##             'MDME(306,1)= 0            !H  charged higgs', 
##             'MDME(307,1)= 0            !Z', 
##             'MDME(308,1)= 0            !Z', 
##             'MDME(309,1)= 0            !sm higgs', 
##             'MDME(310,1)= 0            !weird neutral higgs HA'),

process.ProductionFilterSequence = cms.Sequence(process.generator+process.genParticlesForFilter+process.XtoFFbarFilter)


process.pdfWeights = cms.EDProducer("PdfWeightProducer",
				    # Fix POWHEG if buggy (this PDF set will also appear on output,
				    # so only two more PDF sets can be added in PdfSetNames if not "")
				    #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
				    #GenTag = cms.untracked.InputTag("genParticles"),
				    PdfInfoTag = cms.untracked.InputTag("generator"),
				    PdfSetNames = cms.untracked.vstring(
				        "MRST2004nlo.LHgrid",
					"cteq66.LHgrid"#,
#                                  	"MRST2006nnlo.LHgrid"#,
#                                       	"NNPDF10_100.LHgrid"
                                	)
 				    )

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen*process.pdfWeights)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)




process.RAWSIMoutput.outputCommands.extend(
    [
      'keep *_pdfWeights_*_*'
      ])


# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 
