import FWCore.ParameterSet.Config as cms

from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMETTauMu_cfi import *
from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMETTauEle_cfi import *
from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMETMuEle_cfi import *
from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMETDiTau_cfi import *
from CMGTools.Common.generator.genWorZ_cfi import *
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

from CMGTools.Common.skims.cmgPFJetSel_cfi import cmgPFJetSel
cmgPFJetForRecoil = cmgPFJetSel.clone( cut = 'pt()>30 && abs(eta)<4.5',
                                       src = 'cmgPFJetSel')


metRecoilCorrectionInputSequence = cms.Sequence(
    cmgPFJetForRecoil +
    genWorZ 
    )

metRecoilCorrectionSequence= cms.Sequence(
    metRecoilCorrectionInputSequence +
    recoilCorrectedMETTauMu + 
    recoilCorrectedMETTauEle +
    recoilCorrectedMETMuEle
    # Recoil corrections for DiTau case not studied at all yet
    #recoilCorrectedMETDiTau
    )
