import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMETTauMu_cfi import *
from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMETTauEle_cfi import *
from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMETMuEle_cfi import *
from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMETDiTau_cfi import *
from CMGTools.Common.generator.genWorZ_cfi import *
from CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff import cmgPFJetForRecoil, metRecoilCorrectionInputSequence
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

metRecoilCorrectionSequence2012 = cms.Sequence(
    metRecoilCorrectionInputSequence +
    recoilCorrectedMETTauMu2012 + 
    recoilCorrectedMETTauEle2012 
    # recoilCorrectedMETMuEle2012
    # Recoil corrections for DiTau case not studied at all yet
    #recoilCorrectedMETDiTau2012
    )
