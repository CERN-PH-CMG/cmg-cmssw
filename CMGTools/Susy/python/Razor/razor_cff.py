import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *

# muon cuts
from CMGTools.Common.skims.cmgMuonSel_cfi import *

#razorMuon = cmgMuonSel.clone(cut = 'pt() > 20 && abs(eta()) < 2.1 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")', src = 'cmgMuonSel')

#COLIN why don't we take the source from cmgMuonSel? 
from CMGTools.Common.skims.leadingCMGMuonSelector_cfi import leadingCMGMuonSelector
razorMuonSel = cmgMuonSel.clone(
    cut = 'pt() > 20 && abs(eta()) < 2.1 && getSelection("cuts_vbtfmuon") && getSelection("cuts_isomuon")',
    src = 'cmgMuonSel'
    )
razorLeadingMuon = leadingCMGMuonSelector.clone(inputCollection = "razorMuonSel", index = cms.untracked.int32(1))

razorMuonFail = invertSelector(razorMuonSel)

# electron cuts - TODO: add VBTF selection 
from CMGTools.Common.skims.cmgElectronSel_cfi import *

#razorElectron = cmgElectronSel.clone(cut = 'pt() > 20 && abs(eta()) < 2.5 && (abs(eta()) < 1.4442 || abs(eta()) > 1.566)', src = 'cmgElectronSel')

#COLIN why don't we take the source from cmgElectronSel? 
from CMGTools.Common.skims.leadingCMGElectronSelector_cfi import leadingCMGElectronSelector
razorElectronSel = cmgElectronSel.clone(
    cut = 'pt() > 20 && abs(eta()) < 2.5 && (abs(eta()) < 1.4442 || abs(eta()) > 1.566)',
    src = 'cmgElectronSel'
    )
razorLeadingElectron = leadingCMGElectronSelector.clone(inputCollection = "razorElectronSel", index = cms.untracked.int32(1))

razorElectronFail = invertSelector(razorElectronSel)

razorElectronSequence = cms.Sequence(
    razorElectronSel +
    razorLeadingElectron +
    razorElectronFail 
    )

razorMuonSequence = cms.Sequence(
    razorMuonSel *
    razorLeadingMuon +
    razorMuonFail
    )

# id the jets
razorPFJetSel = cmgCandSel.clone( src = 'cmgPFJetSel', cut = 'pt()>30 && abs(eta)<3.0 && getSelection("cuts_looseJetId")' )
#combine leading leptons with jets
razorPFJetsWithLeadingLeptons = cmgCandMerge.clone(
    src = cms.VInputTag(
    "razorLeadingElectron",
    "razorLeadingMuon",
    "razorPFJetSel",                    
    )
    )
#skim on leading objects - i.e. at least one jet and a lepton, or two jets
razorLeadingObjectCount = cmgCandCount.clone( src = 'razorPFJetsWithLeadingLeptons', minNumber = 2 )

#met
from CMGTools.Common.Tools.cmgBaseMETModifier_cfi import cmgBaseMETModifier
razorMuStarMet = cmgBaseMETModifier.clone(
    cfg = cmgBaseMETModifier.cfg.clone(
    inputCollection = cms.InputTag("razorMuonSel"),
    metCollection = cms.InputTag("cmgPFMET"),
    operator = cms.string('+') #Add the muons to the MET                                               
    )
    )

razorEleStarMet = cmgBaseMETModifier.clone(
    cfg = razorMuStarMet.cfg.clone(
    inputCollection = cms.InputTag("razorElectronSel"),
    )
    )

# razorMETSequence = cms.Sequence(
#    razorMuStarMet+
#    razorEleStarMet
#    )

#make the hemispheres
from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi


###first the hadronic box 
razorHemiHadBox = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      "razorMuonFail",
      "razorElectronFail",
      "razorPFJetSel"),
    maxCand = cms.uint32(20)
    )
    )

razorDiHemiHadBox = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorHemiHadBox'),
    leg2Collection = cms.InputTag('razorHemiHadBox')                    
    ),
    cuts = cmgDiHemi.cuts.clone(
    razor = cms.PSet(
    deltaPhi = cms.string('deltaPhi(leg1().phi(),leg2().phi()) <= 2.8'),
    mr = cms.string('mR() >= 500'),
    r = cms.string('R() >= 0.45')
    )
    )      
    )

razorDiHemiHadBoxSel = cmgCandSel.clone( src = 'razorDiHemiHadBox', cut = 'getSelection("cuts_razorbeta_betaR") && getSelection("cuts_razor") && getSelection("cuts_razorbeta_useMR")' )

from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter
razorDiHemiHadBoxInfo = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("razorDiHemiHadBox"),
    printSelections = cms.untracked.bool(True)
    )

from CMGTools.Susy.Razor.razorHistograms_cff import razorDiHemiHistograms
razorDiHemiHistogramsHadBox = razorDiHemiHistograms.clone(inputCollection = 'razorDiHemiHadBox')

razorHadronicBoxSequence = cms.Sequence(
    razorHemiHadBox*
    razorDiHemiHadBox*
    razorDiHemiHadBoxSel*
    razorDiHemiHadBoxInfo*
    razorDiHemiHistogramsHadBox                                
    )
### now the mu box
razorHemiMuBox = razorHemiHadBox.clone()
razorHemiMuBox.cfg.inputCollection.append('razorMuonSel')
razorDiHemiMuBox = razorDiHemiHadBox.clone(
    cfg = razorDiHemiHadBox.cfg.clone(
    leg1Collection = cms.InputTag('razorHemiMuBox'),
    leg2Collection = cms.InputTag('razorHemiMuBox')                    
    ),    
    )
razorDiHemiMuBoxSel = razorDiHemiHadBoxSel.clone( src = 'razorDiHemiMuBox' )
razorDiHemiHistogramsMuBox = razorDiHemiHistograms.clone(inputCollection = 'razorDiHemiMuBox')
razorMuBoxSequence = cms.Sequence(
    razorHemiMuBox*
    razorDiHemiMuBox*
    razorDiHemiMuBoxSel*
    razorDiHemiHistogramsMuBox                                
    )
### now the Mu* box
razorDiHemiMuStarBox = razorDiHemiMuBox.clone(
    cfg = razorDiHemiMuBox.cfg.clone(
    metCollection = cms.InputTag('razorMuStarMet')                            
    ),       
    )
razorDiHemiMuStarBoxSel = razorDiHemiMuBoxSel.clone( src = 'razorDiHemiMuStarBox' )
razorDiHemiHistogramsMuStarBox = razorDiHemiHistogramsMuBox.clone(inputCollection = 'razorDiHemiMuStarBox')
razorMuStarBoxSequence = cms.Sequence(
    razorMuStarMet + 
    razorDiHemiMuStarBox*
    razorDiHemiMuStarBoxSel*
    razorDiHemiHistogramsMuStarBox                                
    )
### now the ele box
razorHemiEleBox = razorHemiHadBox.clone()
razorHemiEleBox.cfg.inputCollection.append('razorElectronSel')
razorDiHemiEleBox = razorDiHemiHadBox.clone(
    cfg = razorDiHemiHadBox.cfg.clone(
    leg1Collection = cms.InputTag('razorHemiEleBox'),
    leg2Collection = cms.InputTag('razorHemiEleBox')                    
    ),    
    )
razorDiHemiEleBoxSel = razorDiHemiHadBoxSel.clone( src = 'razorDiHemiEleBox' )
razorDiHemiHistogramsEleBox = razorDiHemiHistograms.clone(inputCollection = 'razorDiHemiEleBox')
razorEleBoxSequence = cms.Sequence(
    razorHemiEleBox*
    razorDiHemiEleBox*
    razorDiHemiEleBoxSel*
    razorDiHemiHistogramsEleBox                                
    )
### now the Ele* box
razorDiHemiEleStarBox = razorDiHemiEleBox.clone(
    cfg = razorDiHemiEleBox.cfg.clone(
    metCollection = cms.InputTag('razorEleStarMet')                            
    ),       
    )
razorDiHemiEleStarBoxSel = razorDiHemiEleBoxSel.clone( src = 'razorDiHemiEleStarBox' )
razorDiHemiHistogramsEleStarBox = razorDiHemiHistogramsEleBox.clone(inputCollection = 'razorDiHemiEleStarBox')
razorEleStarBoxSequence = cms.Sequence(
    razorEleStarMet+
    razorDiHemiEleStarBox*
    razorDiHemiEleStarBoxSel*
    razorDiHemiHistogramsEleStarBox                                
    )
#### finally merge selected hemispheres and count
razorSelectedDiHemi = cmgCandMerge.clone(
    src = cms.VInputTag(
    "razorDiHemiHadBoxSel",
    "razorDiHemiMuBoxSel",
    "razorDiHemiMuStarBoxSel",
    "razorDiHemiEleBoxSel",
    "razorDiHemiEleStarBoxSel",                                                            
    )
    )
razorSelectedCount = cmgCandCount.clone( src = 'razorSelectedDiHemi', minNumber = 1 )

razorJetSequence = cms.Sequence(
    # razorLepton*
    # razorMETSequence*
    razorPFJetSel*
    razorPFJetsWithLeadingLeptons
)

razorBoxesSequence = cms.Sequence(
    razorHadronicBoxSequence +
    razorMuBoxSequence * 
    razorMuStarBoxSequence+
    razorEleBoxSequence * 
    razorEleStarBoxSequence    
    )

razorObjectSequence = cms.Sequence(
    razorElectronSequence + 
    razorMuonSequence +
    # razorMETSequence + 
    razorJetSequence +
    razorBoxesSequence    
    )

#razorHistogrammingSequence  = cms.Sequence(
#    )

razorSequence = cms.Sequence(
    razorObjectSequence
#    +
#    razorHistogrammingSequence
    )

razorSkimSequence = cms.Sequence(
    razorObjectSequence + 
    razorLeadingObjectCount+
    razorSelectedDiHemi*
    razorSelectedCount
    )
