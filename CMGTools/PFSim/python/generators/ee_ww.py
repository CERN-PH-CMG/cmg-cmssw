import FWCore.ParameterSet.Config as cms

generator = cms.EDFilter("Pythia6GeneratorFilter",
                                 pythiaPylistVerbosity = cms.untracked.int32(1),
                                 ElectronPositronInitialState = cms.PSet(),
                                 filterEfficiency = cms.untracked.double(1.0),
                                 pythiaHepMCVerbosity = cms.untracked.bool(False),
                                 comEnergy = cms.double(240.0),
                                 PythiaParameters = cms.PSet(
                                     processParameters = cms.vstring(
                                         'PMAS(25,1)=125.0        !Higgs mass',
                                         'MSTJ(22)=2              ! Decay those unstable particles', 
                                         'PARJ(71)=10 .           ! for which ctau  10 mm', 
                                         'MSEL=0                  !(D=1) to select between full user control (0, then use MSUB) and some preprogrammed alternative',
                                         'MSUB(25)=1'),
                                     parameterSets = cms.vstring('processParameters')
                                     )
                                 )
