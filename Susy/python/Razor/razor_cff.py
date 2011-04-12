import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *

# muon cuts
from CMGTools.Common.skims.cmgMuonSel_cfi import *

#RazorMuon = cmgMuonSel.clone(cut = 'pt() > 20 && abs(eta()) < 2.1 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")', src = 'cmgMuonSel')

#COLIN why don't we take the source from cmgMuonSel? 
from CMGTools.Common.skims.leadingCMGMuonSelector_cfi import leadingCMGMuonSelector
RazorMuonSel = cmgMuonSel.clone(
    cut = 'pt() > 20 && abs(eta()) < 2.1 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")',
    src = 'cmgMuonSel'
    )
RazorLeadingMuon = leadingCMGMuonSelector.clone(inputCollection = "RazorMuonSel", index = cms.untracked.int32(1))

RazorMuonFail = invertSelector(RazorMuonSel)

# electron cuts - TODO: add VBTF selection 
from CMGTools.Common.skims.cmgElectronSel_cfi import *

#RazorElectron = cmgElectronSel.clone(cut = 'pt() > 20 && abs(eta()) < 2.5 && (abs(eta()) < 1.4442 || abs(eta()) > 1.566)', src = 'cmgElectronSel')

#COLIN why don't we take the source from cmgElectronSel? 
from CMGTools.Common.skims.leadingCMGElectronSelector_cfi import leadingCMGElectronSelector
RazorElectronSel = cmgElectronSel.clone(
    cut = 'pt() > 20 && abs(eta()) < 2.5 && (abs(eta()) < 1.4442 || abs(eta()) > 1.566)',
    src = 'cmgElectronSel'
    )
RazorLeadingElectron = leadingCMGElectronSelector.clone(inputCollection = "RazorElectronSel", index = cms.untracked.int32(1))

RazorElectronFail = invertSelector(RazorElectronSel)

RazorElectronSequence = cms.Sequence(
    RazorElectronSel +
    RazorLeadingElectron +
    RazorElectronFail 
    )

RazorMuonSequence = cms.Sequence(
    RazorMuonSel *
    RazorLeadingMuon +
    RazorMuonFail
    )

# id the jets
RazorPFJetSel = cmgCandSel.clone( src = 'cmgPFJetSel', cut = 'pt()>30 && abs(eta)<3.0 && getSelection("cuts_looseJetId")' )
#combine leading leptons with jets
RazorPFJetsWithLeadingLeptons = cmgCandMerge.clone(
    src = cms.VInputTag(
    "RazorLeadingElectron",
    "RazorLeadingMuon",
    "RazorPFJetSel",                    
    )
    )
#skim on leading objects - i.e. at least one jet and a lepton, or two jets
RazorLeadingObjectCount = cmgCandCount.clone( src = 'RazorPFJetsWithLeadingLeptons', minNumber = 2 )

#met
from CMGTools.Common.Tools.cmgBaseMETModifier_cfi import cmgBaseMETModifier
RazorMuStarMet = cmgBaseMETModifier.clone(
    cfg = cmgBaseMETModifier.cfg.clone(
    inputCollection = cms.InputTag("RazorMuonSel"),
    metCollection = cms.InputTag("cmgPFMET"),
    operator = cms.string('+') #Add the muons to the MET                                               
    )
    )

RazorEleStarMet = cmgBaseMETModifier.clone(
    cfg = RazorMuStarMet.cfg.clone(
    inputCollection = cms.InputTag("RazorElectronSel"),
    )
    )

RazorMETSequence = cms.Sequence(
    RazorMuStarMet+
    RazorEleStarMet
    )

#make the hemispheres
from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi


###first the hadronic box 
RazorHemiHadBox = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      "RazorMuonFail",
      "RazorElectronFail",
      "RazorPFJetSel"),
    maxCand = cms.uint32(20)
    )
    )

RazorDiHemiHadBox = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('RazorHemiHadBox'),
    leg2Collection = cms.InputTag('RazorHemiHadBox')                    
    ),
    cuts = cmgDiHemi.cuts.clone(
    razor = cms.PSet(
    deltaPhi = cms.string('deltaPhi(leg1().phi(),leg2().phi()) <= 2.8'),
    mr = cms.string('mR() >= 500'),
    r = cms.string('R() >= 0.45')
    )
    )      
    )

RazorDiHemiHadBoxSel = cmgCandSel.clone( src = 'RazorDiHemiHadBox', cut = 'getSelection("cuts_razorbeta_betaR") && getSelection("cuts_razor") && getSelection("cuts_razorbeta_useMR")' )

from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter
RazorDiHemiHadBoxInfo = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("RazorDiHemiHadBox"),
    printSelections = cms.untracked.bool(True)
    )

from CMGTools.Susy.Razor.razorHistograms_cff import razorDiHemiHistograms
RazorDiHemiHistogramsHadBox = razorDiHemiHistograms.clone(inputCollection = 'RazorDiHemiHadBox')

RazorHadronicBoxSequence = cms.Sequence(
    RazorHemiHadBox*
    RazorDiHemiHadBox*
    RazorDiHemiHadBoxSel*
    RazorDiHemiHadBoxInfo*
    RazorDiHemiHistogramsHadBox                                
    )
### now the mu box
RazorHemiMuBox = RazorHemiHadBox.clone()
RazorHemiMuBox.cfg.inputCollection.append('RazorMuonSel')
RazorDiHemiMuBox = RazorDiHemiHadBox.clone(
    cfg = RazorDiHemiHadBox.cfg.clone(
    leg1Collection = cms.InputTag('RazorHemiMuBox'),
    leg2Collection = cms.InputTag('RazorHemiMuBox')                    
    ),    
    )
RazorDiHemiMuBoxSel = RazorDiHemiHadBoxSel.clone( src = 'RazorDiHemiMuBox' )
RazorDiHemiHistogramsMuBox = razorDiHemiHistograms.clone(inputCollection = 'RazorDiHemiMuBox')
RazorMuBoxSequence = cms.Sequence(
    RazorHemiMuBox*
    RazorDiHemiMuBox*
    RazorDiHemiMuBoxSel*
    RazorDiHemiHistogramsMuBox                                
    )
### now the Mu* box
RazorDiHemiMuStarBox = RazorDiHemiMuBox.clone(
    cfg = RazorDiHemiMuBox.cfg.clone(
    metCollection = cms.InputTag('RazorMuStarMet')                            
    ),       
    )
RazorDiHemiMuStarBoxSel = RazorDiHemiMuBoxSel.clone( src = 'RazorDiHemiMuStarBox' )
RazorDiHemiHistogramsMuStarBox = RazorDiHemiHistogramsMuBox.clone(inputCollection = 'RazorDiHemiMuStarBox')
RazorMuStarBoxSequence = cms.Sequence(
    RazorDiHemiMuStarBox*
    RazorDiHemiMuStarBoxSel*
    RazorDiHemiHistogramsMuStarBox                                
    )
### now the ele box
RazorHemiEleBox = RazorHemiHadBox.clone()
RazorHemiEleBox.cfg.inputCollection.append('RazorElectronSel')
RazorDiHemiEleBox = RazorDiHemiHadBox.clone(
    cfg = RazorDiHemiHadBox.cfg.clone(
    leg1Collection = cms.InputTag('RazorHemiEleBox'),
    leg2Collection = cms.InputTag('RazorHemiEleBox')                    
    ),    
    )
RazorDiHemiEleBoxSel = RazorDiHemiHadBoxSel.clone( src = 'RazorDiHemiEleBox' )
RazorDiHemiHistogramsEleBox = razorDiHemiHistograms.clone(inputCollection = 'RazorDiHemiEleBox')
RazorEleBoxSequence = cms.Sequence(
    RazorHemiEleBox*
    RazorDiHemiEleBox*
    RazorDiHemiEleBoxSel*
    RazorDiHemiHistogramsEleBox                                
    )
### now the Ele* box
RazorDiHemiEleStarBox = RazorDiHemiEleBox.clone(
    cfg = RazorDiHemiEleBox.cfg.clone(
    metCollection = cms.InputTag('RazorEleStarMet')                            
    ),       
    )
RazorDiHemiEleStarBoxSel = RazorDiHemiEleBoxSel.clone( src = 'RazorDiHemiEleStarBox' )
RazorDiHemiHistogramsEleStarBox = RazorDiHemiHistogramsEleBox.clone(inputCollection = 'RazorDiHemiEleStarBox')
RazorEleStarBoxSequence = cms.Sequence(
    RazorDiHemiEleStarBox*
    RazorDiHemiEleStarBoxSel*
    RazorDiHemiHistogramsEleStarBox                                
    )
#### finally merge selected hemispheres and count
RazorSelectedDiHemi = cmgCandMerge.clone(
    src = cms.VInputTag(
    "RazorDiHemiHadBoxSel",
    "RazorDiHemiMuBoxSel",
    "RazorDiHemiMuStarBoxSel",
    "RazorDiHemiEleBoxSel",
    "RazorDiHemiEleStarBoxSel",                                                            
    )
    )
RazorSelectedCount = cmgCandCount.clone( src = 'RazorSelectedDiHemi', minNumber = 1 )

RazorJetSequence = cms.Sequence(
    # RazorLepton*
    # RazorMETSequence*
    RazorPFJetSel*
    RazorPFJetsWithLeadingLeptons
)

RazorBoxesSequence = cms.Sequence(
    RazorHadronicBoxSequence +
    RazorMuBoxSequence * 
    RazorMuStarBoxSequence+
    RazorEleBoxSequence * 
    RazorEleStarBoxSequence    
    )

razorSequence  = cms.Sequence(
    RazorElectronSequence + 
    RazorMuonSequence +
    RazorMETSequence + 
    RazorJetSequence +
    RazorBoxesSequence
    ) 

razorSkimSequence = cms.Sequence(
    RazorLeadingObjectCount+
    RazorSelectedDiHemi*
    RazorSelectedCount
    )
