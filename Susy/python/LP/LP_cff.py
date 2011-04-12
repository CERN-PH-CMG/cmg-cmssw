import FWCore.ParameterSet.Config as cms

LPmuonCut = 'pt() > 20 && abs(eta()) < 2.1 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")'
LPlooseMuonCut = 'pt() > 10 && abs(eta()) < 2.4 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")'
#LPjetCut  = 'pt() > 20 & abs(eta()) < 3 && getSelection("cuts_looseJetId")'
LPjetCut  = 'pt() > 20 & abs(eta()) < 3'
LPwMtCut    = 'sqrt( pow(leg1().pt()+leg2.pt(), 2) - pow(leg1().px()+leg2.px(), 2) - pow(leg1().py()+leg2.py(), 2)) > 30'
LPwPtCut    = 'pt() > 50'

####SKIMMING
from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *

#muons
from CMGTools.Common.skims.cmgMuonSel_cfi import *
LPcmgMuonSel   = cmgMuonSel.clone( src = 'cmgMuonSel', cut = LPmuonCut ) 
LPcmgLooseMuonSel   = cmgMuonSel.clone( src = 'cmgMuonSel', cut = LPlooseMuonCut ) 
LPcmgMuonCount1 = cmgCandCount.clone( src = 'LPcmgMuonSel', minNumber = 1)
LPcmgMuonCount2 = cmgCandCount.clone( src = 'LPcmgLooseMuonSel', minNumber = 2)
LPmuon = cms.Sequence( LPcmgMuonSel * LPcmgLooseMuonSel )
LPmuonSkim = cms.Sequence( LPcmgMuonCount1 * ~LPcmgMuonCount2)

#jets
LPcmgPFBaseJetSel = cmgCandSel.clone( src = 'cmgPFBaseJetSel', cut = LPjetCut ) 
LPcmgPFBaseJetCount = cmgCandCount.clone( src = 'LPcmgPFBaseJetSel', minNumber = 3 )
LPjet = cms.Sequence( LPcmgPFBaseJetSel )
LpjetSkim = cms.Sequence( LPcmgPFBaseJetCount )

#W
from CMGTools.Common.factories.cmgWMuNu_cfi import cmgWMuNu
LPcmgWMuNu = cmgWMuNu.clone()
LPcmgWMuNu.cfg.leg1Collection = 'LPcmgMuonSel'
wmunuCuts = cms.PSet(
    mt = cms.string( LPwMtCut ),
    pt = cms.string( LPwPtCut )
)
LPcmgWMuNu.cuts = wmunuCuts 
from CMGTools.Common.skims.cmgWMuNuSel_cfi import cmgWMuNuSel
LPcmgWMuNuPtSel = cmgWMuNuSel.clone(src = 'LPcmgWMuNu', cut = 'getSelection("cuts_pt")')
LPcmgWMuNuMtSel = cmgWMuNuSel.clone(src = 'LPcmgWMuNu', cut = 'getSelection("cuts_mt")')
LPcmgWMuNuPtCount = cmgCandCount.clone( src = 'LPcmgWMuNuPtSel', minNumber = 1)
LPcmgWMuNuMtCount = cmgCandCount.clone( src = 'LPcmgWMuNuMtSel', minNumber = 1)
LPw = cms.Sequence( LPcmgWMuNu )
LPwSkim = cms.Sequence( LPcmgWMuNuMtSel * LPcmgWMuNuPtSel * LPcmgWMuNuPtCount * LPcmgWMuNuMtCount )

#ht 
from CMGTools.Common.skims.cmgBaseMETSel_cfi import cmgBaseMETSel
from CMGTools.Common.met_cff import cmgMHTPFJet30
LPcmgMHTPFJet30      = cmgMHTPFJet30.clone() 
LPcmgMHTPFJet30.cfg.inputCollection = 'LPcmgPFBaseJetSel'
LPcmgMHTPFJet30Sel   = cmgBaseMETSel.clone( src = 'LPcmgMHTPFJet30', cut = 'sumEt()>100' )
LPcmgMHTPFJet30Count = cmgCandCount.clone( src = 'LPcmgMHTPFJet30Sel', minNumber = 1 )
LPht = cms.Sequence( LPcmgMHTPFJet30 * LPcmgMHTPFJet30Sel )
LPhtSkim = cms.Sequence( LPcmgMHTPFJet30Count )

LPobjectSequence = cms.Sequence( LPmuon * LPjet * LPht * LPw)
#LPskimSequence   = cms.Sequence( LPmuonSkim * LPjetSkim * LPwSkim * LPhtSkim )
LPskimSequence   = cms.Sequence( LPmuonSkim * LPwSkim )


####HISTOGRAMMING
from CMGTools.Common.histogram_cff import *
from CMGTools.Common.histograms.cmgMuonHistograms_cfi import cmgMuonHistograms
LPMHTPFJet30Histograms = MHTPFJet30Histograms.clone( inputCollection = 'LPcmgMHTPFJet30Sel' )

LPmuonHistograms = cmgMuonHistograms.clone( inputCollection = 'LPcmgMuonSel')

LPhistogrammingSequence = cms.Sequence(LPMHTPFJet30Histograms*LPmuonHistograms)

#####General sequences
LPSequence = cms.Sequence( LPobjectSequence * LPhistogrammingSequence )

LPSkimSequence = cms.Sequence( LPobjectSequence * LPskimSequence)
