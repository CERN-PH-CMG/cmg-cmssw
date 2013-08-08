import FWCore.ParameterSet.Config as cms

process = cms.Process("HLT")

# Number of events to be generated
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Include DQMStore, needed by the famosSimHits
process.DQMStore = cms.Service( "DQMStore")

# Include the RandomNumberGeneratorService definition
process.load("IOMC.RandomEngine.IOMC_cff")

# gen = 'pgun'
gen = 'jetgun'

if gen == 'pgun':
    process.generator = cms.EDProducer("FlatRandomPtGunProducer",
        PGunParameters = cms.PSet(
            MinPt = cms.double(0),
            MaxPt = cms.double(100.0),
            #NIKITA: see http://www.physics.ox.ac.uk/CDF/Mphys/old/notes/pythia_codeListing.html for pdgId listing. in particular:
            # 211: pi+
            # 130: Kzero long (stable neutral hadron)
            # 22: photon
            # 11: electron
            # 13: muon
            PartID = cms.vint32(211),
            MaxEta = cms.double(1.0),
            MinEta = cms.double(-1.0),
            MaxPhi = cms.double(3.14159265359),
            MinPhi = cms.double(-3.14159265359)
        ),
        Verbosity = cms.untracked.int32(0),
        AddAntiParticle = cms.bool(False),
        firstRun = cms.untracked.uint32(1)
    )
elif gen == 'jetgun':
    jetA = cms.vint32(211, 211, -211, 22, 22, 130)
    process.generator = cms.EDProducer("Pythia6JetGun",
        PGunParameters = cms.PSet(
            ParticleID = jetA,
            # this defines "absolute" energy spread of particles in the jet
            MinE = cms.double(0.5),
            MaxE = cms.double(2.0),
            # the following params define the boost
            MinP = cms.double(30.0),
            MaxP = cms.double(30.0),
            MinPhi = cms.double(-3.1415926535),
            MaxPhi = cms.double(+3.1415926535),
            MinEta = cms.double(-1.0),
            MaxEta = cms.double(1.0)
        ),

        # no detailed pythia6 settings necessary    
        PythiaParameters = cms.PSet(
            parameterSets = cms.vstring()
        )
    )
else:
    print 'define a process.generator object.'
    sys.exit(1)

# --- This was for 2e30 :
# process.load("Configuration.StandardSequences.L1TriggerDefaultMenu_cff")

# --- This is for 8e29 :NEW DEFAULT 
#process.load('L1Trigger/Configuration/L1StartupConfig_cff')
#process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/startup/L1Menu_Commissioning2009_v0_L1T_Scales_20080926_startup_Imp0_Unprescaled_cff')

# --- This is for 1e31 :
#process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1031/L1Menu_MC2009_v0_L1T_Scales_20080922_Imp0_Unprescaled_cff')


# Common inputs, with fake conditions
#process.load("FastSimulation.Configuration.CommonInputs_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('FastSimulation.Configuration.Geometries_cff')

# L1 Emulator and HLT Setup
process.load("FastSimulation.HighLevelTrigger.HLTSetup_cff")

# Famos sequences
process.load("FastSimulation.Configuration.FamosSequences_cff")

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

# HLT paths - defined by configDB
# This one is created on the fly by FastSimulation/Configuration/test/IntegrationTestWithHLT_py.csh
process.load("FastSimulation.Configuration.HLT_GRun_cff")

# Only event accepted by L1 + HLT are reconstructed
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)

# Schedule the HLT paths (and allows HLTAnalyzers for this test):
from FastSimulation.HighLevelTrigger.HLTSetup_cff import hltL1GtTrigReport
process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT' )
)
process.HLTAnalyzerEndpath = cms.EndPath( hltL1GtTrigReport + process.hltTrigReport )
process.HLTSchedule.append(process.HLTAnalyzerEndpath)
process.schedule = cms.Schedule()
process.schedule.extend(process.HLTSchedule)

# If uncommented : All events are reconstructed, including those rejected at L1/HLT
#process.reconstruction = cms.Path(process.reconstructionWithFamos)
#process.schedule.append(process.reconstruction)

# Simulation sequence
#process.simulation = cms.Sequence(process.ProductionFilterSequence*process.simulationWithFamos)
process.source = cms.Source("EmptySource")
process.simulation = cms.Sequence(process.generator*process.simulationWithFamos)

# You many not want to simulate everything
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
# Parameterized magnetic field
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True
# Number of pileup events per crossing
process.load('FastSimulation.PileUpProducer.PileUpSimulator_2012_Startup_inTimeOnly_cff')
#process.load('FastSimulation.PileUpProducer.mix_2012_Startup_inTimeOnly_cff')

noPileUp = True
if noPileUp:
    process.famosPileUp.PileUpSimulator.averageNumber = 0.0
    process.famosPileUp.PileUpSimulator.usePoisson = True

# Get frontier conditions   - not applied in the HCAL, see below
from HLTrigger.Configuration.AutoCondGlobalTag import AutoCondGlobalTag
process.GlobalTag = AutoCondGlobalTag(process.GlobalTag,'auto:startup_GRun')

# Apply ECAL miscalibration 
process.ecalRecHit.doMiscalib = True

# Apply Tracker misalignment
process.famosSimHits.ApplyAlignment = True
process.misalignedTrackerGeometry.applyAlignment = True
process.misalignedDTGeometry.applyAlignment = True
process.misalignedCSCGeometry.applyAlignment = True

# Attention ! for the HCAL IDEAL==STARTUP
# process.caloRecHits.RecHitsFactory.HCAL.Refactor = 1.0
# process.caloRecHits.RecHitsFactory.HCAL.Refactor_mean = 1.0
# process.caloRecHits.RecHitsFactory.HCAL.fileNameHcal = "hcalmiscalib_0.0.xml"

# Note : if your process is not called HLT, you have to change that! 
# process.hltTrigReport.HLTriggerResults = TriggerResults::PROD
# process.hltHighLevel.TriggerResultsTag = TriggerResults::PROD 


# To write out events 
process.load("FastSimulation.Configuration.EventContent_cff")
process.o1 = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    fileName = cms.untracked.string('AODIntegrationTestWithHLT_NoMatEff.root')
)
process.outpath = cms.EndPath(process.o1)

# Add endpaths to the schedule
process.schedule.append(process.outpath)

# Keep the logging output to a nice level #
# process.Timing =  cms.Service("Timing")
process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.categories.append('L1GtTrigReport')
process.MessageLogger.categories.append('HLTrigReport')
#process.MessageLogger.destinations = cms.untracked.vstring("pyDetailedInfo.txt","cout")
#process.MessageLogger.categories.append("FamosManager")
#process.MessageLogger.cout = cms.untracked.PSet(threshold=cms.untracked.string("INFO"),
#                                                default=cms.untracked.PSet(limit=cms.untracked.int32(0)),
#                                                FamosManager=cms.untracked.PSet(limit=cms.untracked.int32(100000)))

# Make the job crash in case of missing product
process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )


#NIKITA
# remove material effects in the tracker?
materialEffects = True
if not materialEffects:
    process.famosSimHits.MaterialEffects.NuclearInteraction = False
    process.famosSimHits.MaterialEffects.PairProduction = False
    process.famosSimHits.MaterialEffects.Bremsstrahlung = False
    # I think we should keep dE/dx and multiple scattering


## caloNoise = True
## if not caloNoise:
##     process.ecalRecHit.doMiscalib = True
##     process.ecalRecHit.RecHitsFactory.ECALBarrel.Noise = cms.double(0)
##     process.ecalRecHit.RecHitsFactory.ECALBarrel.Threshold = cms.double(0.001)
##     process.ecalRecHit.RecHitsFactory.ECALBarrel.HighNoiseParameters = cms.vdouble()
##     process.ecalRecHit.RecHitsFactory.ECALBarrel.SRThreshold = cms.double(0.0)

##     process.ecalRecHit.RecHitsFactory.ECALEndcap.Noise = cms.double(0)
##     process.ecalRecHit.RecHitsFactory.ECALEndcap.Threshold = cms.double(0.001)
##     process.ecalRecHit.RecHitsFactory.ECALEndcap.SRThreshold = cms.double(0.0)
##     process.ecalRecHit.RecHitsFactory.ECALEndcap.HighNoiseParameters = cms.vdouble()

##     process.ecalPreshowerRecHit.RecHitsFactory.Noise = cms.double(0.000001)
##     process.ecalPreshowerRecHit.RecHitsFactory.Threshold = cms.double(4.5e-10)

##     process.hbhereco.doMiscalib = True
##     process.horeco.doMiscalib = True
##     process.hfreco.doMiscalib = True

##     process.hbhereco.RecHitsFactory.HCAL.Noise = cms.vdouble(0, 0)
##     process.horeco.RecHitsFactory.HCAL.Noise = cms.vdouble(0)
##     process.hfreco.RecHitsFactory.HCAL.Noise = cms.vdouble(0)
