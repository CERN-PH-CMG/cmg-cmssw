import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter

# Current loose trigger requires pt > 40; eta < 3; R > 0.14; MR > 150
#take the leading selected electrons + muons
from CMGTools.Common.skims.leadingCMGMuonSelector_cfi import leadingCMGMuonSelector
razorLeadingMuon = leadingCMGMuonSelector.clone(inputCollection = "susyMuon", index = cms.int32(1))
# use an additional pt cut for electrons, but keep the common ID
from CMGTools.Common.skims.cmgElectronSel_cfi import *
razorElectron = cmgElectronSel.clone(src = "susyElectron", cut = 'pt() > 20')
from CMGTools.Common.skims.leadingCMGElectronSelector_cfi import leadingCMGElectronSelector
razorLeadingElectron = leadingCMGElectronSelector.clone(inputCollection = "razorElectron", index = cms.int32(1))

razorElectronSequence = cms.Sequence(
    razorElectron*                                
    razorLeadingElectron
    )
razorMuonSequence = cms.Sequence(
    razorLeadingMuon
    )

# id the jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
razorPFJetSel = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>60 && abs(eta)<3.0' )
# filter out B-tagged jets for latter use 
razorPFBJetSel = cmgPFJetSel.clone( src = 'razorPFJetSel', cut = 'getSelection("cuts_btag_loose")' )

#ID at lower pt threshold - used to veto event - the number of jets that fail loose jet ID
razorPFJetSelID = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = '(pt()>30 && abs(eta)<3.0) && (!getSelection("cuts_looseJetId"))' )
razorPFJetIDCount = cmgCandCount.clone( src = 'razorPFJetSelID', minNumber = 1 ) #filter inverted below

#combine leading leptons with jets
razorPFJetsWithLeadingLeptons = cmgCandMerge.clone(
    src = cms.VInputTag(
      cms.InputTag("razorLeadingElectron"),
      cms.InputTag("razorLeadingMuon"),
      cms.InputTag("razorPFJetSel"),                    
    )
    )
#skim on leading objects - i.e. at least one ID'd jet and a lepton, or two jets
razorLeadingObjectCount = cmgCandCount.clone( src = 'razorPFJetsWithLeadingLeptons', minNumber = 2 )

#make a skim on the HLT - should match all Razor triggers
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
razorTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_.*R0[0-9]+_MR[0-9]+.*_v[0-9]+$")'
                                            )
razorTriggerCount = cmgCandCount.clone( src = 'razorTriggerSel', minNumber = 1 )

razorTriggerInfo = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("razorTriggerSel"),
    printSelections = cms.untracked.bool(True)
    )

razorTriggerSequence = cms.Sequence(
    razorTriggerSel#*
#    razorTriggerInfo
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
      #balanceAlgorithm = cms.uint32(1)#use the MassBalance algo
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
                     mr = cms.string('mR() >= 195'),
                     r = cms.string('R() >= 0.16')
    )
    )      
)
razorDiHemiHadBoxSel = cmgCandSel.clone( src = 'razorDiHemiHadBox', cut = 'getSelection("cuts_razor")' )
razorDiHemiHadBoxInfo = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("razorDiHemiHadBox"),
    printSelections = cms.untracked.bool(True)
    )

from CMGTools.Susy.Razor.razorHistograms_cff import razorDiHemiHistograms
razorDiHemiHistogramsHadBox = razorDiHemiHistograms.clone(inputCollection = 'razorDiHemiHadBox')

razorHadronicBoxSequence = cms.Sequence(
    razorHemiHadBox*
    razorDiHemiHadBox*
    razorDiHemiHadBoxSel#*
#    razorDiHemiHadBoxInfo                                
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
razorHemiMuStarBox = razorHemiHadBox.clone(
    cfg = razorHemiHadBox.cfg.clone(
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
    razorPFBJetSel+
    razorPFJetsWithLeadingLeptons
)

razorBoxesSequence = cms.Sequence(
    razorHadronicBoxSequence +
    razorMuStarBoxSequence
    )

razorObjectSequence = cms.Sequence(
    razorTriggerSequence  +                                 
    razorElectronSequence + 
    razorMuonSequence +
    razorJetSequence +
    razorBoxesSequence    
    )

from CMGTools.Susy.histograms.pfBJetHistograms_cff import pfJetHistograms
#BJet histograms
razorBJetHistograms = pfJetHistograms.clone(inputCollection = cms.InputTag("razorPFJetSel"))

razorHistogrammingSequence  = cms.Sequence(
    #Razor histograms                                       
    razorDiHemiHistogramsHadBox+
    razorDiHemiHistogramsMuStarBox+
    #btag histograms
    razorBJetHistograms
)

razorSequence = cms.Sequence(
    razorObjectSequence+
    razorHistogrammingSequence
    )

#offline based selection
razorSkimSequence = cms.Sequence(
    razorObjectSequence + 
    ~razorPFJetIDCount + #note the inversion - veto events with jets that fail the ID
    razorLeadingObjectCount+
    razorSelectedDiHemi*
    razorSelectedCount
    )

#trigger based selection - we take all Razor triggered events
razorTriggerSkimSequence = cms.Sequence(
    razorObjectSequence+
    razorTriggerCount                                
    )
