import FWCore.ParameterSet.Config as cms


from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import *

from CMGTools.H2TauTau.objects.tauMuObjects_cff import *
from CMGTools.H2TauTau.objects.tauEleObjects_cff import *
from CMGTools.H2TauTau.objects.muEleObjects_cff import *
from CMGTools.H2TauTau.objects.diTauObjects_cff import *

# the following sequence is generic
metRecoilCorInputSequence = cms.Sequence( cmgPFJetForRecoil +
                                          genWorZ )

objectSequence = cms.Sequence( metRecoilCorInputSequence + 
                               tauMuSequence +
                               tauEleSequence +
                               muEleSequence +
                               diTauSequence 
                               )
