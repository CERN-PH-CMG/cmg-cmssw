import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter

############### Jets
# count high pt jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
#require 6 jets offline, of which 2 must be hard
#QuadJet trigger cuts at pt >80, eta < 5.0
razorMJPFJetSel30 = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>30 && abs(eta)<3.0' )
razorMJPFJetSel80 = cmgPFJetSel.clone( src = 'razorMJPFJetSel30', cut = 'pt()>80' )

#we start by filtering very jetty events as we can't cope with them
razorPFJetSelCount = cmgCandCount.clone( src = 'cmgPFJetSel', minNumber = 20 )
#
razorMJPFJetSel30Count5j = cmgCandCount.clone( src = 'razorMJPFJetSel30', minNumber = 5 )
razorMJPFJetSel30Count6j = cmgCandCount.clone( src = 'razorMJPFJetSel30', minNumber = 6 )
razorMJPFJetSel80Count = cmgCandCount.clone( src = 'razorMJPFJetSel80', minNumber = 2 )

# id the jets
#used to veto event - the number of jets that fail loose jet ID, or have a high electron or muon fraction
razorMJPFJetSelID = cmgPFJetSel.clone( src = 'razorMJPFJetSel30', cut = '(!getSelection("cuts_looseJetId")) ||'\
                                           '(component(2).fraction() > 0.90 && abs(eta()) < 2.4) ||'\
                                           '(component(3).fraction() > 0.90 && abs(eta()) < 2.4)'
                                       )
razorMJPFJetIDCount = cmgCandCount.clone( src = 'razorMJPFJetSelID', minNumber = 1 ) #filter inverted below

#make the JES scaled jets while we're at it
from CMGTools.Common.factories.cmgPFJetScaler_cfi import cmgPFJetUp, cmgPFJetDown
from CMGTools.Common.factories.cmgPFMETScaler_cfi import cmgMETUp, cmgMETDown

razorPFJetsUp = cmgPFJetUp.clone()
razorPFJetsUp.cfg.inputCollection = "cmgPFJetSel"
razorPFJetsDown = cmgPFJetDown.clone()
razorPFJetsDown.cfg.inputCollection = "cmgPFJetSel"

razorMJPFJetSel30Up = cmgPFJetSel.clone( src = 'razorPFJetsUp', cut = 'pt()>30 && abs(eta)<3.0' )
razorMJPFJetSel30Down = cmgPFJetSel.clone( src = 'razorPFJetsDown', cut = 'pt()>30 && abs(eta)<3.0' )

#Do the MET scaling here also
razorMJMetUp = cmgMETUp.clone()
razorMJMetDown = cmgMETDown.clone()

############### Trigger
#make a skim on the HLT - should match all multi triggers
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *

#Multijet triggers from the MultiJet PD
razorMJTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_SixJet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_EightJet[0-9]+.*_v[0-9]+$")')
razorMJTriggerCount = cmgCandCount.clone( src = 'razorMJTriggerSel', minNumber = 1 )

#Razor triggers from the HT PD
razorMJIncTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_R0[0-9]+_MR[0-9]+_v[0-9]+$")')
razorMJIncTriggerCount = cmgCandCount.clone( src = 'razorMJIncTriggerSel', minNumber = 1 )


#look at the MultiJet L1 passthrough
razorMJL1TriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_L1MultiJet_v[0-9]+$")'\
                                            )
razorMJL1TriggerCount = cmgCandCount.clone( src = 'razorMJL1TriggerSel', minNumber = 1 )

#HLT_HT250, HLT_HT300
razorMJHTTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_HT250_v[0-9]+$") || getSelectionRegExp("^HLT_HT300_v[0-9]+$")')
razorMJHTTriggerCount = cmgCandCount.clone( src = 'razorMJHTTriggerSel', minNumber = 1 )


#take the L1 seed for the multijet triggers
import HLTrigger.HLTfilters.triggerResultsFilter_cfi as hlt
razorMRFilterL1QuadJet = hlt.triggerResultsFilter.clone(
    triggerConditions = cms.vstring(
        'L1_QuadJet20_Central',
        'L1_QuadJet28_Central',
        'L1_HTT100'
        ),
    l1tResults = cms.InputTag('gtDigis')
    )


#Leptons
############### Muons
from CMGTools.Common.skims.cmgMuonSel_cfi import *
razorMJMuonLoose = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 10.) && (abs(eta()) < 2.4) &&"\
                                            "getSelection('cuts_vbtfmuon_isGlobal') && getSelection('cuts_vbtfmuon_numberOfValidTrackerHits')")
razorMJMuonTight = cmgMuonSel.clone(src = "razorMJMuonLoose", cut = "(abs(eta()) < 2.1) && relIso(0.5)<0.15 && getSelection('cuts_vbtfmuon')")

razorMJMuonSequence = cms.Sequence(
    razorMJMuonLoose*
    razorMJMuonTight
    )
#will invert later
razorMJTightMuonCount = cmgCandCount.clone( src = 'razorMJMuonTight', minNumber = 1 )
############### Electrons
from CMGTools.Common.skims.cmgElectronSel_cfi import *
razorMJElectronLoose = cmgElectronSel.clone(src = "cmgElectronSel", cut = '(pt()> 10.) && (abs(eta()) < 2.5) &&'\
                                            '(abs(eta()) < 1.4442 || abs(eta()) > 1.566) && getSelection("cuts_vbtf95ID")')
razorMJElectronTight = cmgElectronSel.clone(src = "razorMJElectronLoose", cut = '(pt() > 20) && getSelection("cuts_vbtf80ID")'\
                                            '&& (relIso() < 0.2) && (abs(dxy()) < 0.02)')
razorMJElectronSequence = cms.Sequence(
    razorMJElectronLoose*
    razorMJElectronTight                                   
    )
#will invert later
razorMJTightElectronCount = cmgCandCount.clone( src = 'razorMJElectronTight', minNumber = 1 )
################ Taus
from CMGTools.Common.skims.cmgTauSel_cfi import *
razorMJTauLoose = cmgTauSel.clone(src = "cmgTauSel", cut = '(pt()> 15.) && (abs(eta()) < 2.3) && (abs(tauID("byLooseCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3)')
razorMJTauTight = cmgTauSel.clone(src = "razorMJTauLoose", cut = '(abs(tauID("byMediumCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3)')
razorMJTauSequence = cms.Sequence(
    razorMJTauLoose*
    razorMJTauTight                                   
    )
#will invert later
razorMJTightTauCount = cmgCandCount.clone( src = 'razorMJTauTight', minNumber = 1 )

razorMJLeptonSequence = cms.Sequence(
    razorMJMuonSequence+
    razorMJElectronSequence+
    razorMJTauSequence
    )
############### MR and R
#make the hemispheres
from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi

razorMJHemiHadBox = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      cms.InputTag("razorMJPFJetSel30")
      ),
      balanceAlgorithm = cms.uint32(1),#use the MassBalance algo
      maxCand = cms.uint32(50)
    )
)

razorMJDiHemiHadBox = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBox'),
    leg2Collection = cms.InputTag('razorMJHemiHadBox'),
    metCollection = cms.InputTag('cmgPFMET')                  
    ),
    cuts = cmgDiHemi.cuts.clone(
    razor = cms.PSet(
                     mr = cms.string('mR() >= 500'),
                     r = cms.string('Rsq() >= 0.03')
    )
    )      
)
razorMJDiHemiHadBoxSel = cmgCandSel.clone( src = 'razorMJDiHemiHadBox', cut = 'getSelection("cuts_razor")' )


razorMJHemiHadBoxTop = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      cms.InputTag("razorMJPFJetSel30")
      ),
      balanceAlgorithm = cms.uint32(2),#use the TopMassBalance algo
      maxCand = cms.uint32(50)
    )
)

razorMJDiHemiHadBoxTop = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBoxTop'),
    leg2Collection = cms.InputTag('razorMJHemiHadBoxTop'),
    metCollection = cms.InputTag('cmgPFMET')                  
    )    
)

#Now the JES scaled hemispheres
razorMJHemiHadBoxUp = razorMJHemiHadBox.clone()
razorMJHemiHadBoxUp.cfg.inputCollection[0] = "razorMJPFJetSel30Up"
razorMJDiHemiHadBoxUp = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBoxUp'),
    leg2Collection = cms.InputTag('razorMJHemiHadBoxUp'),
    metCollection = cms.InputTag('razorMJMetUp')                  
    )    
)

razorMJHemiHadBoxDown = razorMJHemiHadBox.clone()
razorMJHemiHadBoxDown.cfg.inputCollection[0] = "razorMJPFJetSel30Down"
razorMJDiHemiHadBoxDown = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBoxDown'),
    leg2Collection = cms.InputTag('razorMJHemiHadBoxDown'),
    metCollection = cms.InputTag('razorMJMetDown')                  
    )    
)

############### Run the sequences
razorMJTriggerSequence = cms.Sequence(
    razorMJTriggerSel+
    razorMJL1TriggerSel+
    razorMJIncTriggerSel+
    razorMJHTTriggerSel
    )

razorMJJetSequence = cms.Sequence(                             
    razorMJPFJetSel30*
    razorMJPFJetSelID*
    razorMJPFJetSel80+
    razorPFJetsUp*
    razorMJPFJetSel30Up+
    razorPFJetsDown*
    razorMJPFJetSel30Down+
    razorMJMetUp+
    razorMJMetDown
)

razorMJHemiSequence = cms.Sequence(
    razorMJHemiHadBox*
    razorMJDiHemiHadBox+
    razorMJHemiHadBoxUp*
    razorMJDiHemiHadBoxUp+
    razorMJHemiHadBoxDown*
    razorMJDiHemiHadBoxDown+    
    razorMJHemiHadBoxTop*
    razorMJDiHemiHadBoxTop 
)

razorMJObjectSequence = cms.Sequence(
    razorMJLeptonSequence+
    razorMJJetSequence + 
    razorMJTriggerSequence+
    razorMJHemiSequence                            
    )


razorMJSequence = cms.Sequence(
    razorMJObjectSequence
    )

###Selections for analysis and side bands

#offline selection, but with 5jets and no lepton veto
#Baseline selection for all sidebands
razorMJSkimSequence5j = cms.Sequence(
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #the selection 
    razorMJPFJetSel80Count+
    #only take 5jets to study sidebands
    razorMJPFJetSel30Count5j+
    #filter is inverted
    ~razorMJPFJetIDCount+
    #apply the Razor cuts
    razorMJDiHemiHadBoxSel 
    )

#offline selection, but with 5jets and lepton veto
razorMJSkimSequence5jLeptonVeto = cms.Sequence(
    razorMJSkimSequence5j+
    #veto tight leptons
    ~razorMJTightElectronCount+
    ~razorMJTightMuonCount+
    ~razorMJTightTauCount
    )

#offline selection, no trigger
razorMJSkimSequence6jLeptonVeto = cms.Sequence(
    razorMJSkimSequence5jLeptonVeto+
    razorMJPFJetSel30Count6j
    )

#offline based selection with 6 jets, the lepton veto and the trigger
#used in the analysis for the main fits
razorMJSkimSequence = cms.Sequence(
    razorMJSkimSequence6jLeptonVeto+
    razorMJTriggerCount
    )

#require a multijet trigger
#Used for sidebands: Only 5j, the trigger and no lepton veto
razorMJSkimSequenceMultiJetTrigger = cms.Sequence(
    razorMJSkimSequence5j+
    razorMJTriggerCount
)

#require a razor trigger
#Used for sidebands: Full selection with Razor trigger
razorMJSkimSequenceRazorTrigger = cms.Sequence(
    razorMJSkimSequence6jLeptonVeto+
    razorMJIncTriggerCount
)

#require a razor trigger
#Used for sidebands: Full selection with an HT trigger
razorMJSkimSequenceHTTrigger = cms.Sequence(
    razorMJSkimSequence6jLeptonVeto+
    razorMJHTTriggerCount
)

#require the L1 passthough
#Used for sidebands: Full selection with L1 passthrough
razorMJSkimSequenceL1Passthrough = cms.Sequence(
    razorMJSkimSequence6jLeptonVeto+
    razorMJL1TriggerCount
)

#require the L1 seed
#Used for sidebands: Full selection with L1 seed
razorMJSkimSequenceL1Seed = cms.Sequence(
    razorMRFilterL1QuadJet+
    razorMJSkimSequence6jLeptonVeto
)
