import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *

from CMGTools.Common.skims.cmgPFJetSel_cfi import *

############### Trigger - refactored to seperate file
from CMGTools.Susy.RazorMultiJet.triggerSkim_cff import *

useJets = 'cmgPFJetSelCHS'
#useJets = 'cmgPFJetSel'

#Leptons
from CMGTools.Common.skims.cmgMuonSel_cfi import *

razorMJMuonLoose = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 5.) && (abs(eta()) < 2.4) && sourcePtr().userFloat('isLooseLeptonSAK')")
razorMJMuonTight = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 25.) && (abs(eta()) < 2.4) && isPF() && relIso(0.5) < 0.15 && getSelection('cuts_tightmuonNoVtx') && abs(dxy()) < 0.02 && abs(dz()) < 0.5")
razorMJMuonSequence = cms.Sequence(
    razorMJMuonLoose+
    razorMJMuonTight
    )
#will invert later

razorMJTightMuonCount = cmgCandCount.clone( src = 'razorMJMuonTight', minNumber = 1 )

############### Electrons

razorMJIsolatedElectrons = cms.EDProducer(
    "ElectronIsolationProducer",
    electronTag = cms.InputTag('cmgElectronSel'),
    rhoTag = cms.InputTag('kt6PFJetsForIso','rho'),
    isoCut = cms.double(0.15),
    coneSize = cms.double(0.3)
    )

from CMGTools.Common.skims.cmgElectronSel_cfi import *
razorMJElectronLoose = cmgElectronSel.clone(src = "cmgElectronSel", cut = "(pt()> 5.) && (abs(eta()) < 2.5)  && sourcePtr().userFloat('isLooseLeptonSAK')")
razorMJElectronTight = cmgElectronSel.clone(src = "razorMJIsolatedElectrons", cut = 'pt() >= 30 && getSelection("cuts_mediumNoVtx") && abs(dxy()) < 0.02 && abs(dz()) < 0.1 && (abs(sourcePtr().superCluster().eta()) <= 1.4442 || abs(sourcePtr().superCluster().eta()) > 1.566)')

razorMJElectronSequence = cms.Sequence(
    razorMJElectronLoose+
    razorMJIsolatedElectrons*
    razorMJElectronTight
    )

#will invert later
razorMJLooseElectronCount = cmgCandCount.clone( src = 'razorMJElectronLoose', minNumber = 1 )
razorMJTightElectronCount = cmgCandCount.clone( src = 'razorMJElectronTight', minNumber = 1 )

################ Taus
from CMGTools.Common.skims.cmgTauSel_cfi import *

#taus can't be b-tagged, so remove these first
razorMJTightBtaggedJets = cmgPFJetSel.clone( src = useJets, cut = '(pt() >= 15.) && (abs(eta()) <= 2.4) && getSelection("cuts_csv_tight")' )
razorMJTightBtaggedVetoJets = cmgPFJetSel.clone( src = useJets, cut = '(pt() >= 15.) && (abs(eta()) <= 2.4) && !getSelection("cuts_csv_tight")' )

#first remove any loose electrons or muons from the taus, as well as tightly b-tagged jets
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
            ),
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJTightBtaggedJets"),
            minDeltaR=cms.double(0.3),
            removeMatchedObject=cms.bool(True)
            )

        ),
    verbose = cms.untracked.bool(False)
    )
razorMJTauLoose = cmgTauSel.clone(src = "razorMJTauCleaned", cut = '(pt() >= 15.) && (abs(eta()) <= 2.4) && (abs(tauID("byLooseCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3)')
razorMJTauTight = cmgTauSel.clone(src = "razorMJTauLoose", cut = '(abs(tauID("byTightCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3)')

razorMJTauSequence = cms.Sequence(
    razorMJTightBtaggedJets*
    razorMJTauCleaned*
    razorMJTauLoose*
    razorMJTauTight
    )
#will invert later
razorMJLooseTauCount = cmgCandCount.clone( src = 'razorMJTauLoose', minNumber = 1 )
razorMJTightTauCount = cmgCandCount.clone( src = 'razorMJTauTight', minNumber = 1 )

razorMJLeptonSequence = cms.Sequence(
    razorMJMuonSequence+
    razorMJElectronSequence+
    razorMJTauSequence
    )

############### Jets
# count high pt jets
#require 6 jets offline
razorMJPFJetSel20 = cmgPFJetSel.clone( src = useJets, cut = 'pt()>=20 && abs(eta)<=2.4' )
razorMJPFJetSel30 = cmgPFJetSel.clone( src = 'razorMJPFJetSel20', cut = 'pt()>=30' )

#we start by filtering very jetty events as we can't cope with them
razorPFJetSelCount = cmgCandCount.clone( src = useJets, minNumber = 20 )
razorMJPFJetSel20Count6j = cmgCandCount.clone( src = 'razorMJPFJetSel20', minNumber = 6 )
razorMJPFJetSel30Count5j = cmgCandCount.clone( src = 'razorMJPFJetSel30', minNumber = 5 )

# id the jets
#used to veto event - the number of jets that fail loose jet ID
razorMJPFJetSelID = cmgPFJetSel.clone( src = 'razorMJPFJetSel30', cut = '(!getSelection("cuts_looseJetId"))' )
razorMJPFJetIDCount = cmgCandCount.clone( src = 'razorMJPFJetSelID', minNumber = 1 ) #filter inverted below

#now require either 0 loose btags, or at least one medium 
razorMJBTagFilter = cms.EDFilter(
    "BTagFilter",
    src = cms.InputTag('razorMJPFJetSel30')
    )


#make the JES scaled jets while we're at it
from CMGTools.Common.factories.cmgPFJetScaler_cfi import cmgPFJetUp, cmgPFJetDown
from CMGTools.Common.factories.cmgPFMETScaler_cfi import cmgMETUp, cmgMETDown

razorPFJetsUp = cmgPFJetUp.clone()
razorPFJetsUp.cfg.inputCollection = useJets
razorPFJetsDown = cmgPFJetDown.clone()
razorPFJetsDown.cfg.inputCollection = useJets

razorMJPFJetSel30Up = razorMJPFJetSel30.clone( src = 'razorPFJetsUp' )
razorMJPFJetSel30Down = razorMJPFJetSel30.clone( src = 'razorPFJetsDown' )

#Do the MET scaling here also
razorMJMetUp = cmgMETUp.clone()
razorMJMetDown = cmgMETDown.clone()

#finally, some jet cleaning for the lepton boxes
razorMJJetCleanedLoose = cms.EDProducer(
    "DeltaRVetoProducerPFJet",
    inputCollection = cms.InputTag('razorMJPFJetSel20'),
    MatchingParams = cms.VPSet(
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJElectronTight"),
            minDeltaR=cms.double(0.4),
            removeMatchedObject=cms.bool(True)
            ),
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJMuonTight"),
            minDeltaR=cms.double(0.4),
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
razorMJPFJetSel20Count4j = cmgCandCount.clone( src = 'razorMJJetCleanedLoose', minNumber = 4 )
razorMJJetCleanedLoose30 = cmgPFJetSel.clone( src = 'razorMJJetCleanedLoose', cut = 'pt() >= 30.0' )
razorMJPFJetSel30Count3j = cmgCandCount.clone( src = 'razorMJJetCleanedLoose30', minNumber = 3 )

############### MR and R
#make the hemispheres
#from CMGTools.Susy.factories.cmgChi2Hemi_cfi import cmgChi2Hemi
from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi

razorMJHemiHadBox = cmgHemi.clone(
    cfg = cmgHemi.cfg.clone(
    inputCollection = cms.VInputTag(
      cms.InputTag("razorMJPFJetSel30")
      ),
      balanceAlgorithm = cms.uint32(1),#use the MassBalance algo
      maxCand = cms.uint32(50),
      minObjectsPerHemi0 = cms.untracked.uint32(3),
      minObjectsPerHemi1 = cms.untracked.uint32(3),
    )
)
razorMJHemiHadBox.cfg.minObjectsPerHemi0 = cms.untracked.uint32(3)
razorMJHemiHadBox.cfg.minObjectsPerHemi1 = cms.untracked.uint32(3)

razorMJDiHemiHadBox = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBox'),
    leg2Collection = cms.InputTag('razorMJHemiHadBox'),
    metCollection = cms.InputTag('cmgPFMET')                  
    ),
    cuts = cms.PSet(
        razor = cms.PSet(
            mr = cms.string('mR() >= 300'),
            rsq = cms.string('Rsq() >= 0.02')
            )
        )      
)
razorMJDiHemiHadBoxSel = cmgCandSel.clone( src = 'razorMJDiHemiHadBox', cut = 'getSelection("cuts_razor")' )
razorMJDiHemiHadBoxCount = cmgCandCount.clone( src = 'razorMJDiHemiHadBoxSel', minNumber = 1 )

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

#now with 20 Gev jets
razorMJHemiHadBox20 = razorMJHemiHadBox.clone()
razorMJHemiHadBox20.cfg.inputCollection[0] = "razorMJPFJetSel20"
razorMJDiHemiHadBox20 = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBox20'),
    leg2Collection = cms.InputTag('razorMJHemiHadBox20'),
    metCollection = cms.InputTag('cmgPFMET')                  
    )    
)


#now for the lepton boxes - we keep everything so we can find a lepton friendly combo
razorMJHemiLepBox = razorMJHemiHadBox.clone()
razorMJHemiLepBox.cfg.minObjectsPerHemi0 = cms.untracked.uint32(0)
razorMJHemiLepBox.cfg.minObjectsPerHemi1 = cms.untracked.uint32(0)

razorMJDiHemiLepBox = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiLepBox'),
    leg2Collection = cms.InputTag('razorMJHemiLepBox'),
    metCollection = cms.InputTag('cmgPFMET')
    ),
    cuts = cms.PSet(
    razor = cms.PSet(
    mr = cms.string('mR() >= 300'),
    rsq = cms.string('Rsq() >= 0.02')
    )
    )
    )
razorMJDiHemiLepBoxSel = cmgCandSel.clone( src = 'razorMJDiHemiLepBox', cut = 'getSelection("cuts_razor")' )
razorMJDiHemiLepBoxCount = cmgCandCount.clone( src = 'razorMJDiHemiLepBoxSel', minNumber = 1 )

#also the up + down
razorMJHemiLepBoxDown = razorMJHemiLepBox.clone()
razorMJHemiLepBoxDown.cfg.inputCollection[0] = "razorMJPFJetSel30Down"
razorMJDiHemiLepBoxDown = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiLepBoxDown'),
    leg2Collection = cms.InputTag('razorMJHemiLepBoxDown'),
    metCollection = cms.InputTag('razorMJMetDown')
    )
    )

razorMJHemiLepBoxUp = razorMJHemiLepBox.clone()
razorMJHemiLepBoxUp.cfg.inputCollection[0] = "razorMJPFJetSel30Up"
razorMJDiHemiLepBoxUp = cmgDiHemi.clone(
        cfg = cmgDiHemi.cfg.clone(
        leg1Collection = cms.InputTag('razorMJHemiLepBoxUp'),
            leg2Collection = cms.InputTag('razorMJHemiLepBoxUp'),
            metCollection = cms.InputTag('razorMJMetUp')
            )
        )


#and the 20 GeV versions
razorMJHemiLepBox20 = razorMJHemiLepBox.clone()
razorMJHemiLepBox20.cfg.inputCollection[0] = "razorMJPFJetSel20"
razorMJDiHemiLepBox20 = cmgDiHemi.clone(
        cfg = cmgDiHemi.cfg.clone(
        leg1Collection = cms.InputTag('razorMJHemiLepBox20'),
            leg2Collection = cms.InputTag('razorMJHemiLepBox20'),
            metCollection = cms.InputTag('cmgPFMET')
            )
        )

############### Run the sequences
razorMJTriggerSequence = cms.Sequence(
    razorMJHadTriggerSel+
    razorMJHadBTriggerSel+
    #razorMJHadTriggerInfo+
    razorMJEleTriggerSel+
    razorMJMuTriggerSel+
    razorMJAllTriggerSel
    )

razorMJJetSequence = cms.Sequence(                             
    razorMJPFJetSel20*
    razorMJPFJetSel30*
    razorMJPFJetSelID+
    razorMJJetCleanedLoose*
    razorMJJetCleanedLoose30*
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
    razorMJDiHemiHadBox*
    razorMJDiHemiHadBoxSel+
    razorMJHemiHadBoxUp*
    razorMJDiHemiHadBoxUp+
    razorMJHemiHadBoxDown*
    razorMJDiHemiHadBoxDown+
    razorMJHemiHadBox20*
    razorMJDiHemiHadBox20+
    razorMJHemiLepBox*
    razorMJDiHemiLepBox*
    razorMJDiHemiLepBoxSel+
    razorMJHemiLepBoxUp*
    razorMJDiHemiLepBoxUp+
    razorMJHemiLepBoxDown*
    razorMJDiHemiLepBoxDown+
    razorMJHemiLepBox20*
    razorMJDiHemiLepBox20
)

###Isolated Tracks veto, hadronic

from CMGTools.Susy.common.trackIsolationMaker_cfi import trackIsolationMaker
razorMJTrackIsolationMaker = trackIsolationMaker.clone()
razorMJTrackIsolationMaker.vertexInputTag = cms.InputTag("goodOfflinePrimaryVertices")
razorMJTrackIsolationMaker.minPt_PFCandidate = cms.double(10.0)
razorMJTrackIsolationMaker.pfCandidatesTag = cms.InputTag("pfNoPileUp")
razorMJTrackIsolationMaker.dz_CutValue = cms.double(101.)
razorMJTrackIsolationMaker.vetoCollections = cms.VInputTag()

### Isolated Tracks veto, leptonic
from CMGTools.Common.skims.leadingCMGElectronSelector_cfi import *
from CMGTools.Common.skims.leadingCMGMuonSelector_cfi import *
from CMGTools.Susy.common.trackIsolationMaker_cfi         import trackIsolationMaker

##muons
razorMJMuonTightLead = leadingCMGMuonSelector.clone(
        inputCollection = 'razorMJMuonTight',
            index = 1
            )

##electrons
razorMJElectronTightLead = leadingCMGElectronSelector.clone(
        inputCollection = 'razorMJElectronTight', index = 1
            )

razorMJLeptonTrackIsolationMaker                   = razorMJTrackIsolationMaker.clone()
razorMJLeptonTrackIsolationMaker.vetoCollections   = cms.VInputTag(cms.InputTag("razorMJMuonTightLead"),
                                                                   cms.InputTag("razorMJElectronTightLead"))

razorMJTrackIsolationMakerForLepBoxSequence = cms.Sequence(
    razorMJMuonTightLead            +
    razorMJElectronTightLead        +
    razorMJLeptonTrackIsolationMaker
    )

###
razorMJObjectSequence = cms.Sequence(
    razorMJTrackIsolationMaker+
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
    #require *some* trigger fired
    razorMJTriggerSequence+
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #require at least 5 jets above 30 
    razorMJPFJetSel30Count5j + 
    #only take 6jets above 20
    razorMJPFJetSel20Count6j+
    #filter is inverted
    ~razorMJPFJetIDCount+
    # we need a btag, or a veto
    razorMJBTagFilter+
    #loose razor cut
    razorMJDiHemiHadBoxCount
    )

razorMJSkimSequenceEle = cms.Sequence(
    #require *some* trigger fired
    razorMJTriggerSequence+
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #at least 3 are hard
    razorMJPFJetSel30Count3j+ 
    #only take 4jets, excluding leptons
    razorMJPFJetSel20Count4j+
    #filter is inverted
    ~razorMJPFJetIDLeptonCount+
    # we need a btag, or a veto
    razorMJBTagFilter+
    #a tight electron
    razorMJTightElectronCount+
    #loose razor cut
    razorMJDiHemiLepBoxCount
    )

razorMJSkimSequenceMu = cms.Sequence(
    #require *some* trigger fired
    razorMJTriggerSequence+
    #veto events with too many jets
    ~razorPFJetSelCount+ 
    #now make the sequence
    razorMJObjectSequence +
    #at least 3 are hard
    razorMJPFJetSel30Count3j+ 
    #only take 4jets
    razorMJPFJetSel20Count4j+
    #filter is inverted
    ~razorMJPFJetIDLeptonCount+
    # we need a btag, or a veto
    razorMJBTagFilter+
    #a tight muon
    razorMJTightMuonCount+
    #loose razor cut
    razorMJDiHemiLepBoxCount
    )

trkVetoLeptonSequence = cms.Sequence(
    ~razorPFJetSelCount       +
    razorMJTrackIsolationMakerForLepBoxSequence
    )
