import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter

# Current loose trigger requires pt > 40; eta < 3; R > 0.14; MR > 150

############### Muons
from CMGTools.Common.skims.cmgMuonSel_cfi import *
#for box selection
#the muon trigger requires an 8 gev muon - HLT_Mu8_R005_MR200_v* 
razorMuonLoose = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 10.) && (abs(eta()) < 2.4) &&"\
                                            "getSelection('cuts_vbtfmuon_isGlobal') && getSelection('cuts_vbtfmuon_numberOfValidTrackerHits')")
razorMuonTight = cmgMuonSel.clone(src = "razorMuonLoose", cut = "(abs(eta()) < 2.1) && relIso(0.5)<0.15 && getSelection('cuts_vbtfmuon')")

razorMuonSequence = cms.Sequence(
    razorMuonLoose*
    razorMuonTight
    )

############### Electrons
from CMGTools.Common.skims.cmgElectronSel_cfi import *
#for box selection
razorElectronLoose = cmgElectronSel.clone(src = "cmgElectronSel", cut = '(pt()> 10.) && (abs(eta()) < 2.5) &&'\
                                            '(abs(eta()) < 1.4442 || abs(eta()) > 1.566) && getSelection("cuts_vbtf95ID")')
razorElectronTight = cmgElectronSel.clone(src = "razorElectronLoose", cut = '(pt() > 20) && getSelection("cuts_vbtf80ID")'\
                                            '&& (relIso() < 0.2) && (abs(dxy()) < 0.02)')
# the electron trigger is HLT_Ele10_CaloIdL_CaloIsoVL_TrkIdVL_R005_MR200_v*
razorElectronSequence = cms.Sequence(
    razorElectronLoose*
    razorElectronTight                                   
    )

############### Jets
# id the jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
#ID at lower pt threshold - used to veto event - the number of jets that fail loose jet ID
razorPFJetSelID = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = '(pt()>20 && abs(eta)<3.0) && (!getSelection("cuts_looseJetId"))' )
razorPFJetIDCount = cmgCandCount.clone( src = 'razorPFJetSelID', minNumber = 1 ) #filter inverted below

#cut in HLTRHemisphere - works as electrons are included in jets offline also
#cut harder on eta so that jets are in tracker for tagging
razorPFJetSel = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>40 && abs(eta)<2.4' )
#clean jets of muons using deltaR
from CMGTools.Common.miscProducers.deltaRJetMuons_cfi import deltaRJetMuons
razorPFJetsMuonVeto = deltaRJetMuons.clone(
    inputCollection = cms.InputTag('razorPFJetSel'),#only require kinematics, not ID
    vetoCollection = cms.InputTag('razorMuonLoose')
)
# we need at least two jets made of real jets or electrons
razorPFJetCount = cmgCandCount.clone( src = 'razorPFJetsMuonVeto', minNumber = 2 )
# filter out B-tagged jets for latter use 
razorPFBJetSel = cmgPFJetSel.clone( src = 'razorPFJetsMuonVeto', cut = 'getSelection("cuts_btag_loose")' )
razorPFBJetCount = cmgCandCount.clone( src = 'razorPFBJetSel', minNumber = 1 )

razorJetSequence = cms.Sequence(
    razorPFJetSel+
    razorPFJetSelID+
    razorPFJetsMuonVeto*
    razorPFBJetSel    
)

############### Trigger
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

############### Hemispheres
#make the hemispheres
from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi

###first the hadronic box 
# Note that the leptons are already included, so its just a matter of which final state to apply
razorHemiHadBox = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      cms.InputTag("razorPFJetsMuonVeto")
      ),
      balanceAlgorithm = cms.uint32(1),#use the MassBalance algo
      maxCand = cms.uint32(100)
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
                     mr = cms.string('mR() >= 100'),
                     r = cms.string('R() >= 0.14')
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
razorDiHemiMuStarBox = razorDiHemiHadBox.clone(
    cfg = razorDiHemiHadBox.cfg.clone(
       metCollection = cms.InputTag('razorMuStarMet')                 
    ),       
    )
razorDiHemiMuStarBoxSel = razorDiHemiHadBoxSel.clone( src = 'razorDiHemiMuStarBox' )
razorDiHemiHistogramsMuStarBox = razorDiHemiHistogramsHadBox.clone(inputCollection = 'razorDiHemiMuStarBox')
razorMuStarBoxSequence = cms.Sequence(
    razorMuStarMet* 
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
razorBJetHistograms = pfBJetHistograms.clone(inputCollection = cms.InputTag("razorPFJetsMuonVeto"))

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
    # require no jets to fail ID
    ~razorPFJetIDCount + #note the inversion - veto events with jets that fail the ID
    # require at least two high pt jets
    razorPFJetCount*
    #at least one loose btag
    razorPFBJetCount+
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
