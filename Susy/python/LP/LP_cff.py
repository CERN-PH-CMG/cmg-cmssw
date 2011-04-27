import FWCore.ParameterSet.Config as cms

LPMuonCut = 'pt() > 20 && abs(eta()) < 2.1'
LPLooseMuonCut = 'pt() > 10 && abs(eta()) < 2.4'
LPJetCut  = 'pt() > 30 & abs(eta()) < 3'
#LPWMtCut    = 'sqrt( pow(leg1().pt()+leg2.pt(), 2) - pow(leg1().px()+leg2.px(), 2) - pow(leg1().py()+leg2.py(), 2)) > 30'
LPHTLooseCut    = 'sumEt() > 150'
LPHTTightCut    = 'sumEt() > 250'
LPWMtCut    = 'mt() > 30'
LPWPtLooseCut    = 'pt() > 50'
LPWPtTightCut    = 'pt() > 150'

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
LPMuonSkimSequence = cms.Sequence( LPMuonCount1 ) #+ ~LPMuonCount2)

#jets for HT computation
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
LPPFJetSel = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = LPJetCut ) 
LPPFJetCount = cmgCandCount.clone( src = 'LPPFJetSel', minNumber = 2 )

#HT
from CMGTools.Common.met_cff import cmgMHTPFJet30
LPMHTPFJet30 = cmgMHTPFJet30.clone()
LPMHTPFJet30.cfg.ptThreshold = 30.0
LPMHTPFJet30.cfg.inputCollection = 'LPPFJetSel'
#HT including selected muons
from CMGTools.Common.Tools.cmgBaseMETModifier_cfi import cmgBaseMETModifier
LPLeptonicMHTPFJet30 = cmgBaseMETModifier.clone() 
LPLeptonicMHTPFJet30.cfg.inputCollection = 'LPMuon'
LPLeptonicMHTPFJet30.cfg.metCollection   = 'LPMHTPFJet30' 
LPLeptonicMHTPFJet30.cfg.operator = '+'

from CMGTools.Common.skims.cmgBaseMETSel_cfi import cmgBaseMETSel

LPMHTPFJet30LooseSel = cmgBaseMETSel.clone( src = 'LPMHTPFJet30',
                                            cut = LPHTLooseCut ) 

LPMHTPFJet30LooseCount = cmgCandCount.clone( src = 'LMHTPFJet30LooseSel',
                                             minNumber = 1 )

LPMHTPFJet30TightSel = cmgBaseMETSel.clone( src = 'LPMHTPFJet30',
                                            cut = LPHTTightCut )

LPMHTPFJet30TightCount = cmgCandCount.clone( src = 'LMHTPFJet30TightSel',
                                             minNumber = 1 )

#W
from CMGTools.Common.factories.cmgWMuNu_cfi import cmgWMuNu
LPWMuNu = cmgWMuNu.clone()
LPWMuNu.cfg.leg1Collection = 'LPMuon'
wmunuCuts = cms.PSet(
    muon = cms.string( 'leg1().getSelection(\"cuts_vbtfmuon\")'),
    isolation = cms.string('leg1().getSelection(\"cuts_isomuon\")'),
    mt = cms.string( LPWMtCut ),
    ptLoose = cms.string( LPWPtLooseCut ),
    ptTight = cms.string( LPWPtTightCut )
)
LPWMuNu.cuts = wmunuCuts 

LPMuMHT = cmgWMuNu.clone()
LPMuMHT.cfg.leg1Collection = 'LPMuon'
LPMuMHT.cfg.leg2Collection = 'LPLeptonicMHTPFJet30'
MuMHTCuts = cms.PSet(
    muon = cms.string( 'leg1().getSelection(\"cuts_vbtfmuon\")'),
    isolation = cms.string('leg1().getSelection(\"cuts_isomuon\")'),
    mt = cms.string( LPWMtCut ),
    ptLoose = cms.string( LPWPtLooseCut ),
    ptTight = cms.string( LPWPtTightCut )
)
LPMuMHT.cuts = MuMHTCuts


LPObjectSequence = cms.Sequence(
    LPMuonSequence +
    LPPFJetSel +
    LPMHTPFJet30 +
    LPMHTPFJet30LooseSel +
    LPMHTPFJet30TightSel +
    LPLeptonicMHTPFJet30 +
    LPWMuNu +
    LPMuMHT 
    )


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

LPSkimSequence   = cms.Sequence(
    LPObjectSequence + 
    LPMuonSkimSequence #+
    #LPPFJetCount +
    #LPMHTPFJet20Count
    )
