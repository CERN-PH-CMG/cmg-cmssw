import FWCore.ParameterSet.Config as cms

from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMET_cfi import *
from CMGTools.Common.generator.genWorZ_cfi import *
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

from CMGTools.Common.skims.cmgPFJetSel_cfi import cmgPFJetSel
cmgPFJetForRecoil = cmgPFJetSel.clone( cut = 'pt()>30 && abs(eta)<4.5',
                                       src = 'cmgPFJetSel')


metRecoilCorrectionSequence= cms.Sequence(
    cmgPFJetForRecoil +
    genWorZ +
    recoilCorrectedMET
    )
