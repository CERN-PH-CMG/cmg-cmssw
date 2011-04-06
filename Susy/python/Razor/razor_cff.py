import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *

# muon cuts
from CMGTools.Common.skims.cmgMuonSel_cfi import *
RazorCmgMuon = cmgMuonSel.clone(cut = 'pt() > 20 && abs(eta()) < 2.1 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")')
RazorCmgMuonFailed = invertSelector(RazorCmgMuon)

# electron cuts - TODO: add VBTF selection 
from CMGTools.Common.skims.cmgElectronSel_cfi import *
RazorCmgElectron = cmgElectronSel.clone(cut = 'pt() > 20 && abs(eta()) < 2.5 && (abs(eta()) < 1.4442 || abs(eta()) > 1.566)')
RazorCmgElectronFailed = invertSelector(RazorCmgElectron)

# jet skim
RazorCmgPFJetSel = cmgCandSel.clone( src = 'cmgPFJetSel', cut = 'pt()>30 && abs(eta)<3.0 && getSelection("cuts_looseJetId")' )
RazorCmgPFJetCount = cmgCandCount.clone( src = 'RazorCmgPFJetSel', minNumber = 2 )
RazorCmgPFJetsWithFailedLeptons = cmgCandMerge.clone(
                                        src = cms.VInputTag(
                                          "RazorCmgMuonFailed",
                                          "RazorCmgElectronFailed",
                                          "RazorCmgPFJetSel",                    
                                          )
                                        )
#met
from CMGTools.Common.factories.cmgBaseCandMET_cfi import cmgBaseCandMET
from CMGTools.Susy.Razor.razorTopProjectors_cff import *
# MET from PFCandidates
RazorMETPFCandidates = cmgBaseCandMET.clone()
RazorMETPFCandidates.cfg.inputCollection = "particleFlow"

RazorMET = cms.Sequence(
    RazorMETPFCandidates
    )

#make the hemispheres
from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi

RazorHemiAllPFJets = cmgHemi.clone(cfg = cmgHemi.cfg.clone(
                                                           inputCollection = cms.VInputTag(
                                                                                        "RazorCmgMuonFailed",
                                                                                        "RazorCmgElectronFailed",
                                                                                        "RazorCmgPFJetSel"),
                                                           maxCand = cms.uint32(20))
                                    )

RazorDiHemiAllPFJets = cmgDiHemi.clone(
                                       cfg = cmgDiHemi.cfg.clone(
                                             leg1Collection = cms.InputTag('RazorHemiAllPFJets'),
                                             leg2Collection = cms.InputTag('RazorHemiAllPFJets')                    
                                             ),
                                       cuts = cmgDiHemi.cuts.clone(
                                             razor = cms.PSet(
                                                              deltaPhi = cms.string('deltaPhi(leg1().phi(),leg2().phi()) <= 2.8'),
                                                              mr = cms.string('mR() >= 500'),
                                                              r = cms.string('R() >= 0.45')
                                                              )
                                            )      
                                       )
RazorDiHemiAllPFJetsSel = cmgCandSel.clone( src = 'RazorDiHemiAllPFJets', cut = 'getSelection("cuts_razorbeta_betaR") && getSelection("cuts_razor") && getSelection("cuts_razorbeta_useMR")' )
RazorDiHemiAllPFJetsCount = cmgCandCount.clone( src = 'RazorDiHemiAllPFJetsSel', minNumber = 1 )

RazorLepton = cms.Sequence(
    (RazorCmgElectron +
    RazorCmgElectronFailed + 
    RazorCmgMuon +
    RazorCmgMuonFailed)
#    RazorParticleFlow
)

from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter
RazorDiHemiAllPFJetsInfo = physicsObjectPrinter.clone(
                                                      inputCollection = cms.untracked.InputTag("RazorDiHemiAllPFJets"),
                                                      printSelections = cms.untracked.bool(True)
                                                      )
from CMGTools.Susy.Razor.razorHistograms_cff import *


RazorJet = cms.Sequence(
    RazorLepton*
    RazorMET*
    RazorCmgPFJetSel*
    RazorCmgPFJetsWithFailedLeptons*  
    RazorHemiAllPFJets*
    RazorDiHemiAllPFJets*
    RazorDiHemiAllPFJetsSel*
    RazorDiHemiAllPFJetsInfo*
    razorDiHemiHistograms
)

razorSequence  = cms.Sequence(
    RazorLepton + 
    RazorJet
    ) 

razorSkimSequence = cms.Sequence(
    RazorCmgPFJetCount +
    RazorDiHemiAllPFJetsCount
    )
