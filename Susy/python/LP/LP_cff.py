import FWCore.ParameterSet.Config as cms

LPMuonCut = 'pt() > 20 && abs(eta()) < 2.1 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")'
LPLooseMuonCut = 'pt() > 10 && abs(eta()) < 2.4 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")'
#LPJetCut  = 'pt() > 20 & abs(eta()) < 3 && getSelection("cuts_looseJetId")'
LPJetCut  = 'pt() > 20 & abs(eta()) < 3'
LPWMtCut    = 'sqrt( pow(leg1().pt()+leg2.pt(), 2) - pow(leg1().px()+leg2.px(), 2) - pow(leg1().py()+leg2.py(), 2)) > 30'
LPWPtCut    = 'pt() > 50'

####SKIMMING
from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *

#muons

from CMGTools.Common.skims.cmgMuonSel_cfi import *
LPMuon   = cmgMuonSel.clone( src = 'cmgMuonSel', cut = LPMuonCut ) 
LPLooseMuon   = cmgMuonSel.clone( src = 'cmgMuonSel', cut = LPLooseMuonCut ) 
LPMuonCount1 = cmgCandCount.clone( src = 'LPMuon', minNumber = 1)
LPMuonCount2 = cmgCandCount.clone( src = 'LPLooseMuon', minNumber = 2)
LPMuonSequence = cms.Sequence( LPMuon + LPLooseMuon )
LPMuonSkimSequence = cms.Sequence( LPMuonCount1 + ~LPMuonCount2)

#jets for HT computation

from CMGTools.Common.skims.cmgPFJetSel_cfi import *

LPPFJetSel = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = LPJetCut ) 
LPPFJetCount = cmgCandCount.clone( src = 'LPPFJetSel', minNumber = 3 )

# LPJetSequence = cms.Sequence( LPPFJetSel )
# LPJetSkimSequence = cms.Sequence( LPPFJetCount )

#HT

from CMGTools.Common.met_cff import cmgMHTPFJet30
LPMHTPFJet20 = cmgMHTPFJet30.clone()
LPMHTPFJet20.cfg.ptThreshold = 20.0
LPMHTPFJet20.cfg.inputCollection = 'LPPFJetSel'

from CMGTools.Common.skims.cmgBaseMETSel_cfi import cmgBaseMETSel

LPMHTPFJet20Sel = cmgBaseMETSel.clone( src = 'LPMHTPFJet20',
                                       cut = 'sumEt()>100') 

LPMHTPFJet20Count = cmgCandCount.clone( src = 'LPMHTPFJet20Sel',
                                        minNumber = 1 )

#W

from CMGTools.Common.factories.cmgWMuNu_cfi import cmgWMuNu
LPWMuNu = cmgWMuNu.clone()
LPWMuNu.cfg.leg1Collection = 'LPMuon'
wmunuCuts = cms.PSet(
    mt = cms.string( LPWMtCut ),
    pt = cms.string( LPWPtCut )
)
LPWMuNu.cuts = wmunuCuts 
from CMGTools.Common.skims.cmgWMuNuSel_cfi import cmgWMuNuSel
LPWMuNuPtSel = cmgWMuNuSel.clone(src = 'LPWMuNu', cut = 'getSelection("cuts_pt")')
LPWMuNuMtSel = cmgWMuNuSel.clone(src = 'LPWMuNu', cut = 'getSelection("cuts_mt")')
LPWMuNuPtCount = cmgCandCount.clone( src = 'LPWMuNuPtSel', minNumber = 1)
LPWMuNuMtCount = cmgCandCount.clone( src = 'LPWMuNuMtSel', minNumber = 1)
#LPWSequence = cms.Sequence( LPWMuNu )





LPObjectSequence = cms.Sequence(
    LPMuonSequence +
    LPPFJetSel +
    LPMHTPFJet20 +
    LPMHTPFJet20Sel + 
    # LPHTSequence + 
    LPWMuNu + 
    LPWMuNuMtSel +
    LPWMuNuPtSel 
    )

LPWSkimSequence = cms.Sequence(
    LPWMuNuPtCount +
    LPWMuNuMtCount
    )

#LPSkimSequence   = cms.Sequence( LPMuonSkim + LPJetSkim + LPWSkim + LPHTSkim )


####HISTOGRAMMING
from CMGTools.Common.histogram_cff import *
from CMGTools.Common.histograms.cmgMuonHistograms_cfi import cmgMuonHistograms
LPMHTPFJet30Histograms = MHTPFJet30Histograms.clone( inputCollection = 'LPMHTPFJet30Sel' )

LPMuonHistograms = cmgMuonHistograms.clone( inputCollection = 'LPMuon')

LPHistogrammingSequence = cms.Sequence(
    # LPMHTPFJet30Histograms +
    LPMuonHistograms )

#####General sequences
LPSequence = cms.Sequence(
    LPObjectSequence +
    LPHistogrammingSequence
    )

# LPSkimSequence = cms.Sequence(
    # LPObjectSequence +
#    LPSkimSequence
#    )

LPSkimSequence   = cms.Sequence(
    LPObjectSequence + 
    LPMuonSkimSequence +
    LPWSkimSequence +
    LPPFJetCount +
    LPMHTPFJet20Count
    )
