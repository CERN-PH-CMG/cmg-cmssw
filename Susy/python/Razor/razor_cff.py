import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *

#start with the trigger
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
#preselect data events only if they have passed an HT or Razor trigger
#
# Current loose trigger requires pt > 40; eta < 3; R > 0.14; MR > 150
razorTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = '( hasSelection("isRealData") && getSelection("isRealData")' \
                                            '     && (getSelectionRegExp("HLT_HT*") || getSelectionRegExp("HLT_R0*") ) ) || ' \
                                            '    ( hasSelection("isRealData") && !getSelection("isRealData") ) || '\
                                            '    !hasSelection("isRealData")'
                                            )
razorTriggerCount = cmgCandCount.clone( src = 'razorTriggerSel', minNumber = 1 )#pass-through this for MC

#take the leading selected electrons + muons
from CMGTools.Common.skims.leadingCMGMuonSelector_cfi import leadingCMGMuonSelector
razorLeadingMuon = leadingCMGMuonSelector.clone(inputCollection = "susyMuon", index = cms.int32(1))
from CMGTools.Common.skims.leadingCMGElectronSelector_cfi import leadingCMGElectronSelector
razorLeadingElectron = leadingCMGElectronSelector.clone(inputCollection = "susyElectron", index = cms.int32(1))

razorElectronSequence = cms.Sequence(
    razorLeadingElectron
    )
razorMuonSequence = cms.Sequence(
    razorLeadingMuon
    )

# id the jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
razorPFJetSel = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>29 && abs(eta)<3.1' ) #go a bit loose
razorPFJetSelID = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = '%s && getSelection("cuts_looseJetId")' % razorPFJetSel.cut.value() )

#combine leading leptons with jets
razorPFJetsWithLeadingLeptons = cmgCandMerge.clone(
    src = cms.VInputTag(
      cms.InputTag("razorLeadingElectron"),
      cms.InputTag("razorLeadingMuon"),
      cms.InputTag("razorPFJetSelID"),                    
    )
    )
#skim on leading objects - i.e. at least one ID'd jet and a lepton, or two jets
razorLeadingObjectCount = cmgCandCount.clone( src = 'razorPFJetsWithLeadingLeptons', minNumber = 2 )

#met modifications for the *star* boxes
from CMGTools.Common.Tools.cmgBaseMETModifier_cfi import cmgBaseMETModifier
razorMuStarMet = cmgBaseMETModifier.clone(
    cfg = cmgBaseMETModifier.cfg.clone(
    inputCollection = cms.InputTag("susyMuon"),
    metCollection = cms.InputTag("cmgPFMET"),
    operator = cms.string('+') #Add the muons to the MET                                               
    )
)
razorEleStarMet = cmgBaseMETModifier.clone(
    cfg = razorMuStarMet.cfg.clone(
    inputCollection = cms.InputTag("susyMuon"),
    )
)

#make the hemispheres
from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi

###first the hadronic box 
# Note that the leptons are already included, so its just a matter of which final state to apply
razorHemiHadBox = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      cms.InputTag("razorPFJetSel")#only require kinematics, not ID
      ),
    )
)

razorDiHemiHadBox = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorHemiHadBox'),
    leg2Collection = cms.InputTag('razorHemiHadBox')                    
    ),
    cuts = cmgDiHemi.cuts.clone(
    razor = cms.PSet(
                     deltaPhi = cms.string('deltaPhi(leg1().phi(),leg2().phi()) <= 2.7'),
                     mr = cms.string('mR() >= 145'),
                     r = cms.string('R() >= 0.13')
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
    razorDiHemiHadBoxInfo                                
    )
### now the Mu* box
razorDiHemiMuStarBox = razorDiHemiHadBox.clone(
    cfg = razorDiHemiHadBox.cfg.clone(
    metCollection = cms.InputTag('razorMuStarMet')                            
    ),       
    )
razorDiHemiMuStarBoxSel = razorDiHemiHadBoxSel.clone( src = 'razorDiHemiMuStarBox' )
razorDiHemiHistogramsMuStarBox = razorDiHemiHistogramsHadBox.clone(inputCollection = 'razorDiHemiMuStarBox')
razorMuStarBoxSequence = cms.Sequence(
    razorMuStarMet + 
    razorDiHemiMuStarBox*
    razorDiHemiMuStarBoxSel                                
    )
### now the Ele* box
razorDiHemiEleStarBox = razorDiHemiHadBox.clone(
    cfg = razorDiHemiHadBox.cfg.clone(
    metCollection = cms.InputTag('razorEleStarMet')                            
    ),       
    )
razorDiHemiEleStarBoxSel = razorDiHemiHadBoxSel.clone( src = 'razorDiHemiEleStarBox' )
razorDiHemiHistogramsEleStarBox = razorDiHemiHistogramsHadBox.clone(inputCollection = 'razorDiHemiEleStarBox')
razorEleStarBoxSequence = cms.Sequence(
    razorEleStarMet+
    razorDiHemiEleStarBox*
    razorDiHemiEleStarBoxSel                               
    )
#### finally merge selected hemispheres and count
razorSelectedDiHemi = cmgCandMerge.clone(
    src = cms.VInputTag(
      cms.InputTag("razorDiHemiHadBoxSel"),
    )
)
razorSelectedCount = cmgCandCount.clone( src = 'razorSelectedDiHemi', minNumber = 1 )

razorJetSequence = cms.Sequence(
    razorPFJetSel+
    razorPFJetSelID+
    razorPFJetsWithLeadingLeptons
)

razorBoxesSequence = cms.Sequence(
    razorHadronicBoxSequence +
    razorMuStarBoxSequence+
    razorEleStarBoxSequence    
    )

razorObjectSequence = cms.Sequence(
    razorTriggerSel+
    razorElectronSequence + 
    razorMuonSequence +
    razorJetSequence +
    razorBoxesSequence    
    )

from CMGTools.Susy.histograms.pfBJetHistograms_cff import pfJetHistograms
#BJet histograms
razorBJetHistogramsAll = pfJetHistograms.clone()
razorBJetHistogramsSel = pfJetHistograms.clone(inputCollection = cms.InputTag("razorPFJetSel"))

razorHistogrammingSequence  = cms.Sequence(
    #Razor histograms                                       
    razorDiHemiHistogramsHadBox+
    razorDiHemiHistogramsMuStarBox+
    razorDiHemiHistogramsEleStarBox+
    #btag histograms
    razorBJetHistogramsAll+
    razorBJetHistogramsSel
)

razorSequence = cms.Sequence(
    razorObjectSequence+
    razorHistogrammingSequence
    )

razorSkimSequence = cms.Sequence(
    razorObjectSequence + 
    razorTriggerCount + 
    razorLeadingObjectCount+
    razorSelectedDiHemi*
    razorSelectedCount
    )
