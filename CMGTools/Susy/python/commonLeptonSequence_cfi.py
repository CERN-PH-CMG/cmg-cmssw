import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgElectronSel_cfi import *
from CMGTools.Common.skims.cmgMuonSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *

susyElectron = cmgElectronSel.clone(
    cut = "(pt()> 10.) && (abs(eta()) < 2.5) && (abs(eta()) < 1.4442 || abs(eta()) > 1.566) && relIso()<0.15 && getSelection('cuts_mediumID') && getSelection('cuts_mediumCR')",
    src = 'cmgElectronSel'
    )

susyMuon = cmgMuonSel.clone(
    cut = "(pt() > 10.) && (abs(eta()) < 2.4) && relIso()<0.15 && getSelection('cuts_vbtfmuon')", 
    src = 'cmgMuonSel'
    )

commonLeptonSequence = cms.Sequence(
    susyElectron  +
    susyMuon
    )

