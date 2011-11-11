import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter

# Current loose trigger requires pt > 40; eta < 3; R > 0.14; MR > 150
#take the leading selected electrons + muons
from CMGTools.Common.skims.leadingCMGMuonSelector_cfi import leadingCMGMuonSelector
from CMGTools.Common.skims.cmgMuonSel_cfi import *
#for box selection
#the muon trigger requires an 8 gev muon - HLT_Mu8_R005_MR200_v* 
razorMuonLoose = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 10.) && (abs(eta()) < 2.4) && getSelection('cuts_vbtfmuon_isGlobal') && getSelection('cuts_vbtfmuon_numberOfValidTrackerHits')")
razorMuonTight = cmgMuonSel.clone(src = "razorMuonLoose", cut = "(abs(eta()) < 2.1) && relIso(0.5)<0.15 && getSelection('cuts_vbtfmuon')")

razorLeadingMuon = leadingCMGMuonSelector.clone(inputCollection = "razorMuonTight", index = cms.int32(1))
razorMuonSequence = cms.Sequence(
    razorMuonLoose*
    razorMuonTight+                                 
    razorLeadingMuon
    )

from CMGTools.Common.skims.cmgElectronSel_cfi import *
#for box selection
razorElectronLoose = cmgElectronSel.clone(src = "cmgElectronSel", cut = '(pt()> 10.) && (abs(eta()) < 2.5) && (abs(eta()) < 1.4442 || abs(eta()) > 1.566) && getSelection("cuts_vbtf95ID")')
razorElectronTight = cmgElectronSel.clone(src = "razorElectronLoose", cut = '(pt() > 20) && getSelection("cuts_vbtf80ID") && (relIso() < 0.2) && (abs(dxy()) < 0.02)')
from CMGTools.Common.skims.leadingCMGElectronSelector_cfi import leadingCMGElectronSelector
razorLeadingElectron = leadingCMGElectronSelector.clone(inputCollection = "razorElectronTight", index = cms.int32(1))
# the electron trigger is HLT_Ele10_CaloIdL_CaloIsoVL_TrkIdVL_R005_MR200_v*
razorElectronSequence = cms.Sequence(
    razorElectronLoose*
    razorElectronTight+                                   
    razorLeadingElectron
    )

# id the jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
razorPFJetSel = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>60 && abs(eta)<3.0' )
# we need at least one jet
razorPFJetCount = cmgCandCount.clone( src = 'razorPFJetSel', minNumber = 1 )
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
#skim on leading objects - i.e. at least one jet and a tight lepton, or two jets
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
      balanceAlgorithm = cms.uint32(1)#use the MassBalance algo
    )
)

razorDiHemiHadBox = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorHemiHadBox'),
    leg2Collection = cms.InputTag('razorHemiHadBox'),
    metCollection = cms.InputTag('cmgPFMET')                  
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
    vetoCollection = cms.InputTag('razorMuonLoose')
)
# filter out B-tagged jets for latter use 
razorPFBJetsMuonVeto = cmgPFJetSel.clone( src = 'razorPFJetsMuonVeto', cut = 'getSelection("cuts_btag_loose")' )
#recalculate the MET
from CMGTools.Common.Tools.cmgBaseMETModifier_cfi import cmgBaseMETModifier
razorMuStarMet = cmgBaseMETModifier.clone(
    cfg = cmgBaseMETModifier.cfg.clone(
    inputCollection = cms.InputTag("razorMuonLoose"),
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
    razorPFBJetsMuonVeto*
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

from CMGTools.Susy.Razor.razorBoxDef_cff import razorBoxDef
razorBoxDef = razorBoxDef.clone(
    tightElectrons = cms.InputTag('razorElectronTight'),
    looseElectrons = cms.InputTag('razorElectronLoose'),
    tightMuons = cms.InputTag('razorMuonTight'),
    looseMuons = cms.InputTag('razorMuonLoose'),
    jets = cms.InputTag('razorPFJetSel'),
    bjets = cms.InputTag('razorPFBJetSel') 
)
#make sure we keep all EleMu events
razorBoxDefSel = cmgCandSel.clone( src = 'razorBoxDef', cut = 'getSelection("cuts_EleMu")' )
razorBoxDefCount = cmgCandCount.clone( src = 'razorBoxDefSel', minNumber = 1 )

razorBoxesSequence = cms.Sequence(
    razorBoxDef*  
    razorBoxDefSel+    
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

from CMGTools.Susy.histograms.pfBJetHistograms_cff import pfBJetHistograms
from CMGTools.Susy.histograms.pfMultiJetHistograms_cff import pfMultiJetHistograms
#BJet histograms
razorBJetHistograms = pfBJetHistograms.clone(inputCollection = cms.InputTag("razorPFJetSel"))
razorMultiJetHistograms = pfMultiJetHistograms.clone(inputCollection = cms.InputTag("razorPFJetSel"))

razorHistogrammingSequence  = cms.Sequence(
    #Razor histograms                                       
    razorDiHemiHistogramsHadBox+
    razorDiHemiHistogramsMuStarBox+
    #btag histograms
    razorBJetHistograms+
    razorMultiJetHistograms
)

razorSequence = cms.Sequence(
    razorObjectSequence+
    razorHistogrammingSequence
    )

#offline based selection
razorSkimSequence = cms.Sequence(
    razorObjectSequence + 
    # require no jets to fail ID
    ~razorPFJetIDCount + #note the inversion - veto events with jets that fail the ID
    # require at least on high pt jet
    razorPFJetCount+
    # require another jet or a tight lepton
    razorLeadingObjectCount+
    # loose cuts on R and MR
    razorSelectedDiHemi*
    razorSelectedCount
    )

#trigger based selection - we take all Razor triggered events
razorTriggerSkimSequence = cms.Sequence(
    razorObjectSequence+
    #make any Rxx_MRxx trigger
    razorTriggerCount                                
    )

#looser skim on EleMu events for btagging
razorEleMuSequence = cms.Sequence(
    razorObjectSequence*
    # Take anything that would lie in the EleMu box - just tight electron, tight muon
    razorBoxDefCount                                
    )
