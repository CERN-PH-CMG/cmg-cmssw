import FWCore.ParameterSet.Config as cms

pythiaUESettingsBlock = cms.PSet(
    pythiaUESettings = cms.vstring(
    'MSTU(21)=1     ! Check on possible errors during program execution', 
    'MSTJ(11)=5     ! Decay those unstable particles', 
    'MSTJ(22)=2     ! Decay those unstable particles',
    'PARJ(1)=0.073  !',
    'PARJ(2)=0.202  !',
    'PARJ(3)=0.950  !',
    'PARJ(4)=0.033  !',
    'PARJ(11)=0.309 !',
    'PARJ(12)=0.402 !',
    'PARJ(13)=0.544 !',
    'PARJ(21)=0.30  !',
    'PARJ(25)=0.628 !',
    'PARJ(26)=0.129 !',
    'PARJ(41)=0.368 !',
    'PARJ(42)=1.004 !',
    'PARJ(47)=0.873 !',
    'PARJ(71)=10    ! for which ctau  10 mm',
    'PARJ(81)=0.256 !',
    'PARJ(82)=0.830 !',
    'PARU(112)=0.192!',
    'MSTP(2)=1      ! which order running alphaS',
    'MSTP(3)=1      ! ',
    'MSTP(33)=0     ! no K factors in hard cross sections', 
    'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)',
    'MSTP(52)=2     ! work with LHAPDF',
    'MSTP(64)=2     !',
    'MSTP(70)=0     !',
    'MSTP(72)=2     !',
    'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
    'MSTP(82)=4     ! Defines the multi-parton model',
    'MSTP(91)=1     !', 
    'MSTP(95)=6     !',
    'PARP(1)=0.192  !',
    'PARP(61)=0.192 !',
    'PARP(62)=1.13   ! ', 
    'PARP(64)=0.68   ! ', 
    'PARP(67)=1.0    ! amount of initial-state radiation',
    'PARP(72)=0.527  ! amount of initial-state radiation', 
    'PARP(77)=0.491 !',
    'PARP(78)=0.311 !',
    'PARP(80)=0.1   !',
    'PARP(82)=2.26   ! pt cutoff for multiparton interactions', 
    'PARP(83)=0.356  ! Multiple interactions: matter distrbn parameter', 
    'PARP(84)=0.443  ! Multiple interactions: matter distribution parameter', 
    'PARP(89)=1960. ! sqrts for which PARP82 is set', 
    'PARP(90)=0.249  ! Multiple interactions: rescaling power', 
    'PARP(91)=2.0    ! kt distribution', 
    'PARP(93)=10.0   ! '
    )
)
