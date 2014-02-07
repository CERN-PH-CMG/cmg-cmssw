import FWCore.ParameterSet.Config as cms

from CMGTools.Utilities.metRecoilCorrection.recoilCorrectedMETTauMu_cfi import *
from CMGTools.Utilities.metRecoilCorrection.recoilCorrectedMETTauEle_cfi import *
from CMGTools.Utilities.metRecoilCorrection.recoilCorrectedMETMuEle_cfi import *
from CMGTools.Utilities.metRecoilCorrection.recoilCorrectedMETDiTau_cfi import *
from CMGTools.Common.generator.genWorZ_cfi import *
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *


from CMGTools.Common.skims.cmgPFJetSel_cfi import cmgPFJetSel
cmgPFJetForRecoilPresel = cmgPFJetSel.clone(
    cut = 'pt()>30 && abs(eta)<4.7 && getSelection("cuts_looseJetId")',
    src = 'cmgPFJetSel')

from CMGTools.Common.skims.cmgPFJetPUIDSel_cfi import cmgPFJetPUIDSel
cmgPFJetForRecoil = cmgPFJetPUIDSel.clone(
    src = 'cmgPFJetForRecoilPresel')

metRecoilCorrectionInputSequence = cms.Sequence(
    cmgPFJetForRecoilPresel +
    cmgPFJetForRecoil +
    genWorZ 
    )

metRecoilCorrectionSequence  = cms.Sequence(
    metRecoilCorrectionInputSequence +
    recoilCorrectedMETTauMu  + 
    recoilCorrectedMETTauEle  +
    recoilCorrectedMETMuEle 
    # Recoil corrections for DiTau case not studied at all yet
    #recoilCorrectedMETDiTau 
    )
