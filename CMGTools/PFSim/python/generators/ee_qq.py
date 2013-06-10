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
                                         'MSTJ(22)=2     ! Decay those unstable particles', 
                                         'PARJ(71)=10 .  ! for which ctau  10 mm', 
                                         'MSEL=0         ! (D=1) to select between full user control (0, then use MSUB) and some preprogrammed alternative',
                                         'MSUB(1)=1      ! e+e- -> gamma*/Z0',
                                         'CKIN(1)=20.    ! mass cut off on the Z/gamma*',
                                         'MDME(174,1)=1           !Z decay into d dbar', 
                                         'MDME(175,1)=1           !Z decay into u ubar', 
                                         'MDME(176,1)=1           !Z decay into s sbar', 
                                         'MDME(177,1)=1           !Z decay into c cbar', 
                                         'MDME(178,1)=1           !Z decay into b bbar', 
                                         'MDME(179,1)=0           !Z decay into t tbar', 
                                         'MDME(182,1)=0           !Z decay into e- e+', 
                                         'MDME(183,1)=0           !Z decay into nu_e nu_ebar', 
                                         'MDME(184,1)=0           !Z decay into mu- mu+', 
                                         'MDME(185,1)=0           !Z decay into nu_mu nu_mubar', 
                                         'MDME(186,1)=0           !Z decay into tau- tau+', 
                                         'MDME(187,1)=0           !Z decay into nu_tau nu_taubar'),
                                     parameterSets = cms.vstring('processParameters')
                                     )
                                 )

