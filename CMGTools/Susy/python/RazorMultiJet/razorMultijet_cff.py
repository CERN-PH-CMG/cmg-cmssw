import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter

############### Trigger
#make a skim on the HLT - should match all multi triggers
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *

#Multijet triggers from the MultiJet PD
razorMJHadTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_DiJet[0-9]+_DiJet[0-9]+_DiJet[0-9]+.*_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_QuadJet[0-9]+_DiJet[0-9]+.*_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_SixJet[0-9]+.*_v[0-9]+$")')
razorMJHadTriggerCount = cmgCandCount.clone( src = 'razorMJHadTriggerSel', minNumber = 1 )

razorMJHadTriggerInfo = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("razorMJHadTriggerSel"),
    printSelections = cms.untracked.bool(True)
    )

#the Had parking triggers
razorMJQuadJetTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_QuadJet[0-9]+_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_QuadJet[0-9]+_L1FastJet_v[0-9]+$")')
razorMJQuadJetTriggerCount = cmgCandCount.clone( src = 'razorMJQuadJetTriggerSel', minNumber = 1 )

#Multijet triggers from the MuHad PD
razorMJMuTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_Mu[0-9]+_QuadJet[0-9]+_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_IsoMu[0-9]+_QuadJet[0-9]+_v[0-9]+$")')
razorMJMuTriggerCount = cmgCandCount.clone( src = 'razorMJMuTriggerSel', minNumber = 1 )

#Multijet triggers from the ElectronHad PD
razorMJEleTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_Ele[0-9]+.*_QuadJet[0-9]+_v[0-9]+$")')
razorMJEleTriggerCount = cmgCandCount.clone( src = 'razorMJEleTriggerSel', minNumber = 1 )


#Razor triggers from the HT PD
razorMJIncTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_R0[0-9]+_MR[0-9]+_v[0-9]+$")')
razorMJIncTriggerCount = cmgCandCount.clone( src = 'razorMJIncTriggerSel', minNumber = 1 )

#Leptons
############### Muons - no isolation - definitons from H->ZZ
from CMGTools.Common.skims.cmgMuonSel_cfi import *
razorMJMuonLoose = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 10.) && (abs(eta()) < 2.4) && ( isGlobal() || (isTracker() && numberOfMatches()>0) )")
razorMJMuonTight = cmgMuonSel.clone(src = "razorMJMuonLoose", cut = "isPFMuon() && relIso(0.5) < 0.2")

#make the SAK muons to compare with
razorMJMuonSAK = cms.EDProducer(
    "DirectionalIsolationProducerMuon",
    src = cms.InputTag('patMuonsWithTrigger'),
    pfCands = cms.InputTag('pfNoPileUp'),
    vertexCollection = cms.InputTag('goodOfflinePrimaryVertices')
    )

#make a cmg lepton from it
from CMGTools.Common.factories.cmgMuon_cfi import cmgMuon
razorMJMuonSAKCMG = cmgMuon.clone()
razorMJMuonSAKCMG.cfg.inputCollection = 'razorMJMuonSAK'

#cut on the kinematics
razorMJMuonSAKSel = cmgMuonSel.clone(src = "razorMJMuonSAKCMG", cut = "(pt() > 10.) && (abs(eta()) < 2.4)")

razorMJMuonSequence = cms.Sequence(
    razorMJMuonLoose*
    razorMJMuonTight+
    razorMJMuonSAK*
    razorMJMuonSAKCMG*
    razorMJMuonSAKSel
    )
#will invert later
razorMJLooseMuonCount = cmgCandCount.clone( src = 'razorMJMuonLoose', minNumber = 2 )
razorMJTightMuonCount = cmgCandCount.clone( src = 'razorMJMuonTight', minNumber = 1 )

############### Electrons
from CMGTools.Common.skims.cmgElectronSel_cfi import *
razorMJElectronLoose = cmgElectronSel.clone(src = "cmgElectronSel", cut = '(pt()> 10.) && (abs(eta()) < 2.5) && numberOfHits()<=1 && relIso(0.5) < 0.15')
razorMJElectronTight = cmgElectronSel.clone(src = "razorMJElectronLoose", cut = '( ( (abs(eta()) < 0.8) && mvaNonTrigV0() > 0.5 ) ||'\
                                                '( (abs(eta()) >= 0.8) && (abs(eta()) < 1.479) && mvaNonTrigV0() > 0.12 ) ||'\
                                                '( (abs(eta()) >= 1.479) && (abs(eta()) < 2.5) && mvaNonTrigV0() > 0.6 ) )')

#make the SAK electrons to compare with
razorMJElectronSAK = cms.EDProducer(
    "DirectionalIsolationProducerElectron",
    src = cms.InputTag('patElectronsWithTrigger'),
    pfCands = cms.InputTag('pfNoPileUp'),
    vertexCollection = cms.InputTag('goodOfflinePrimaryVertices')
    )

#make a cmg lepton from it
from CMGTools.Common.factories.cmgElectron_cfi import cmgElectron
razorMJElectronSAKCMG = cmgElectron.clone()
razorMJElectronSAKCMG.cfg.inputCollection = 'razorMJElectronSAK'

#cut on the kinematics
razorMJElectronSAKSel = cmgElectronSel.clone(src = "razorMJElectronSAKCMG", cut = "(pt() > 10.) && (abs(eta()) < 2.5)")

razorMJElectronSequence = cms.Sequence(
    razorMJElectronLoose*
    razorMJElectronTight+
    razorMJElectronSAK*
    razorMJElectronSAKCMG*
    razorMJElectronSAKSel
    )
#will invert later
razorMJLooseElectronCount = cmgCandCount.clone( src = 'razorMJElectronLoose', minNumber = 2 )
razorMJTightElectronCount = cmgCandCount.clone( src = 'razorMJElectronTight', minNumber = 1 )

################ Taus
from CMGTools.Common.skims.cmgTauSel_cfi import *

#first remove any loose electrons or muons from the taus
razorMJTauCleaned = cms.EDProducer(
    "DeltaRVetoProducerTau",
    inputCollection = cms.InputTag('cmgTauSel'),
    MatchingParams = cms.VPSet(
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJElectronLoose"),
            minDeltaR=cms.double(0.3),
            removeMatchedObject=cms.bool(True)
            ),
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJMuonLoose"),
            minDeltaR=cms.double(0.3),
            removeMatchedObject=cms.bool(True)
            )
        ),
    verbose = cms.untracked.bool(False)
    )
razorMJTauLoose = cmgTauSel.clone(src = "razorMJTauCleaned", cut = '(pt() >= 15.) && (abs(eta()) <= 2.4) && (abs(tauID("byLooseCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3)')
razorMJTauTight = cmgTauSel.clone(src = "razorMJTauLoose", cut = '(abs(tauID("byTightCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3)')
razorMJTauSequence = cms.Sequence(
    razorMJTauCleaned*
    razorMJTauLoose*
    razorMJTauTight                                   
    )
#will invert later
razorMJLooseTauCount = cmgCandCount.clone( src = 'razorMJTauLoose', minNumber = 2 )
razorMJTightTauCount = cmgCandCount.clone( src = 'razorMJTauTight', minNumber = 1 )

razorMJLeptonSequence = cms.Sequence(
    razorMJMuonSequence+
    razorMJElectronSequence+
    razorMJTauSequence
    )

############### Jets
# count high pt jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
#require 6 jets offline
razorMJPFJetSel30 = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>=30 && abs(eta)<=2.4' )

#we start by filtering very jetty events as we can't cope with them
razorPFJetSelCount = cmgCandCount.clone( src = 'cmgPFJetSel', minNumber = 20 )
razorMJPFJetSel30Count6j = cmgCandCount.clone( src = 'razorMJPFJetSel30', minNumber = 6 )

# id the jets
#used to veto event - the number of jets that fail loose jet ID
razorMJPFJetSelID = cmgPFJetSel.clone( src = 'razorMJPFJetSel30', cut = '(!getSelection("cuts_looseJetId"))' )
razorMJPFJetIDCount = cmgCandCount.clone( src = 'razorMJPFJetSelID', minNumber = 1 ) #filter inverted below

#make the JES scaled jets while we're at it
from CMGTools.Common.factories.cmgPFJetScaler_cfi import cmgPFJetUp, cmgPFJetDown
from CMGTools.Common.factories.cmgPFMETScaler_cfi import cmgMETUp, cmgMETDown

razorPFJetsUp = cmgPFJetUp.clone()
razorPFJetsUp.cfg.inputCollection = "cmgPFJetSel"
razorPFJetsDown = cmgPFJetDown.clone()
razorPFJetsDown.cfg.inputCollection = "cmgPFJetSel"

razorMJPFJetSel30Up = razorMJPFJetSel30.clone( src = 'razorPFJetsUp' )
razorMJPFJetSel30Down = razorMJPFJetSel30.clone( src = 'razorPFJetsDown' )

#Do the MET scaling here also
razorMJMetUp = cmgMETUp.clone()
razorMJMetDown = cmgMETDown.clone()

#finally, some jet cleaning for the lepton boxes
razorMJJetCleanedLoose = cms.EDProducer(
    "DeltaRVetoProducerPFJet",
    inputCollection = cms.InputTag('razorMJPFJetSel30'),
    MatchingParams = cms.VPSet(
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJElectronTight"),
            minDeltaR=cms.double(0.3),
            removeMatchedObject=cms.bool(True)
            ),
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJMuonTight"),
            minDeltaR=cms.double(0.3),
            removeMatchedObject=cms.bool(True)
            ),
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJTauTight"),
            minDeltaR=cms.double(0.3),
            removeMatchedObject=cms.bool(True)
            )
        ),
    verbose = cms.untracked.bool(False)
    )

# id the jets
#used to veto event - the number of jets that fail loose jet ID, after we have removed our leptons
razorMJPFJetSelIDLepton = cmgPFJetSel.clone( src = 'razorMJJetCleanedLoose', cut = '(!getSelection("cuts_looseJetId"))' )
razorMJPFJetIDLeptonCount = cmgCandCount.clone( src = 'razorMJPFJetSelIDLepton', minNumber = 1 ) #filter inverted below

#used to select the events
razorMJPFJetSel30Count4j = cmgCandCount.clone( src = 'razorMJJetCleanedLoose', minNumber = 4 )

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
                     r = cms.string('Rsq() >= 0.03')
    )
    )      
)
razorMJDiHemiHadBoxSel = cmgCandSel.clone( src = 'razorMJDiHemiHadBox', cut = 'getSelection("cuts_razor")' )

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
    razorMJHadTriggerSel*
    razorMJHadTriggerInfo+
    razorMJEleTriggerSel+
    razorMJMuTriggerSel+
    razorMJIncTriggerSel
    )

razorMJJetSequence = cms.Sequence(                             
    razorMJPFJetSel30*
    razorMJPFJetSelID+
    razorMJJetCleanedLoose*
    razorMJPFJetSelIDLepton*
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
    razorMJDiHemiHadBoxDown
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
razorMJSkimSequenceHad = cms.Sequence(
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #only take 6jets
    razorMJPFJetSel30Count6j+
    #require a trigger
    razorMJHadTriggerCount+
    #filter is inverted
    ~razorMJPFJetIDCount+
    #no loose electrons
    ~razorMJLooseElectronCount+
    ~razorMJTightElectronCount+ 
    #no loose muons
    ~razorMJLooseMuonCount+
    ~razorMJTightMuonCount+
    #no loose taus
    ~razorMJLooseTauCount+
    ~razorMJTightTauCount#+  
    #apply the Razor cuts
    #razorMJDiHemiHadBoxSel 
    )

razorMJSkimSequenceEle = cms.Sequence(
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #only take 4jets, excluding leptons
    razorMJPFJetSel30Count4j+
    #require a trigger
    razorMJEleTriggerCount+
    #filter is inverted
    ~razorMJPFJetIDLeptonCount+
    #no loose electrons, but a tight one
    ~razorMJLooseElectronCount+
    razorMJTightElectronCount+ 
    #no loose muons
    ~razorMJLooseMuonCount+
    ~razorMJTightMuonCount+
    #no tight taus
    ~razorMJTightTauCount#+  
    #apply the Razor cuts
    #razorMJDiHemiHadBoxSel 
    )

razorMJSkimSequenceMu = cms.Sequence(
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #only take 4jets
    razorMJPFJetSel30Count4j+
    #require a trigger
    razorMJMuTriggerCount+
    #filter is inverted
    ~razorMJPFJetIDLeptonCount+
    #no loose electrons
    ~razorMJLooseElectronCount+
    ~razorMJTightElectronCount+ 
    #no loose muons, but a tight muon
    ~razorMJLooseMuonCount+
    razorMJTightMuonCount+
    #no tight taus
    ~razorMJTightTauCount#+  
    #apply the Razor cuts
    #razorMJDiHemiHadBoxSel 
    )

razorMJSkimSequenceTau = cms.Sequence(
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #only take 6jets
    razorMJPFJetSel30Count6j+
    #require a trigger
    razorMJHadTriggerCount+
    #filter is inverted
    ~razorMJPFJetIDCount+
    #no loose electrons
    ~razorMJLooseElectronCount+
    ~razorMJTightElectronCount+ 
    #no loose muons
    ~razorMJLooseMuonCount+
    ~razorMJTightMuonCount+
    #no loose taus, but a tight tau
    ~razorMJLooseTauCount+
    razorMJTightTauCount#+  
    #apply the Razor cuts
    #razorMJDiHemiHadBoxSel 
    )
