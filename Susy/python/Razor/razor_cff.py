import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *

#start with the trigger
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
#preselect data events only if they have passed an HT or Razor trigger
#
# Current loose trigger requires pt > 40; eta < 3; R > 0.14; MR > 150
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
razorPFJetSel = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>60 && abs(eta)<3.0' )
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
    #these are a little looser than the analysis cuts to give some sidebands                            
    razor = cms.PSet(
                     deltaPhi = cms.string('deltaPhi(leg1().phi(),leg2().phi()) < 2.8'),
                     mr = cms.string('mR() >= 145'),
                     r = cms.string('R() >= 0.13')
    )
    )      
)
razorDiHemiHadBoxSel = cmgCandSel.clone( src = 'razorDiHemiHadBox', cut = 'getSelection("cuts_razor")' )

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
#clean jets of muons using deltaR
from CMGTools.Common.miscProducers.deltaRJetMuons_cfi import deltaRJetMuons
razorPFJetsMuonVeto = deltaRJetMuons.clone(
    inputCollection = cms.InputTag('razorPFJetSel'),#only require kinematics, not ID
    vetoCollection = cms.InputTag('susyMuon')
)
#recalculate the MET
from CMGTools.Common.Tools.cmgBaseMETModifier_cfi import cmgBaseMETModifier
razorMuStarMet = cmgBaseMETModifier.clone(
    cfg = cmgBaseMETModifier.cfg.clone(
    inputCollection = cms.InputTag("susyMuon"),
    metCollection = cms.InputTag("cmgPFMET"),
    operator = cms.string('+') #Add the muons to the MET                                               
    )
)
#make the hemispheres using only jets that are not muons
razorHemiMuStarBox = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      cms.InputTag("razorPFJetsMuonVeto")
      ),
    )
)
razorDiHemiMuStarBox = razorDiHemiHadBox.clone(
    cfg = razorDiHemiHadBox.cfg.clone(
       leg1Collection = cms.InputTag('razorHemiMuStarBox'),
       leg2Collection = cms.InputTag('razorHemiMuStarBox'),                                      
       metCollection = cms.InputTag('razorMuStarMet')                 
    ),       
    )
razorDiHemiMuStarBoxSel = razorDiHemiHadBoxSel.clone( src = 'razorDiHemiMuStarBox' )
razorDiHemiHistogramsMuStarBox = razorDiHemiHistogramsHadBox.clone(inputCollection = 'razorDiHemiMuStarBox')
razorMuStarBoxSequence = cms.Sequence(
    razorPFJetsMuonVeto* 
    razorMuStarMet* 
    razorHemiMuStarBox*
    razorDiHemiMuStarBox*
    razorDiHemiMuStarBoxSel                                
    )
#we don't need an Ele* box as the electrons are seen by the trigger
#### finally merge selected hemispheres and count
razorSelectedDiHemi = cmgCandMerge.clone(
    src = cms.VInputTag(
      cms.InputTag("razorDiHemiHadBoxSel"),
      cms.InputTag("razorDiHemiMuStarBoxSel")
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
    razorMuStarBoxSequence
    )

razorObjectSequence = cms.Sequence(
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
    razorLeadingObjectCount+
    razorSelectedDiHemi*
    razorSelectedCount
    )
