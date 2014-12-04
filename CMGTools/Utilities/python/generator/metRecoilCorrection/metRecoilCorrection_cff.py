import FWCore.ParameterSet.Config as cms

# from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMETTauMu_cfi import *
# from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMETTauEle_cfi import *
# from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMETMuEle_cfi import *
# from CMGTools.H2TauTau.generator.metRecoilCorrection.recoilCorrectedMETDiTau_cfi import *
from CMGTools.Utilities.generator.metRecoilCorrection.recoilCorrectedMETTauMu_cfi import *
from CMGTools.Utilities.generator.metRecoilCorrection.recoilCorrectedMETTauEle_cfi import *
from CMGTools.Utilities.generator.metRecoilCorrection.recoilCorrectedMETMuEle_cfi import *
from CMGTools.Utilities.generator.metRecoilCorrection.recoilCorrectedMETDiTau_cfi import *
from CMGTools.Common.generator.genWorZ_cfi import *
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *


from CMGTools.Common.skims.cmgPFJetSel_cfi import cmgPFJetSel
cmgPFJetForRecoil = cmgPFJetSel.clone(
    cut = 'pt()>30 && abs(eta)<4.5 && getSelection("cuts_looseJetId") && passLooseFullPuJetId()',
    src = 'cmgPFJetSel')


metRecoilCorrectionInputSequence = cms.Sequence(
    cmgPFJetForRecoil +
    genWorZ 
    )

metRecoilCorrectionSequence2012 = cms.Sequence(
    metRecoilCorrectionInputSequence +
    recoilCorrectedMETTauMu2012 + 
    recoilCorrectedMETTauEle2012 +
    recoilCorrectedMETMuEle2012
    # Recoil corrections for DiTau case not studied at all yet
    #recoilCorrectedMETDiTau2012
    )
