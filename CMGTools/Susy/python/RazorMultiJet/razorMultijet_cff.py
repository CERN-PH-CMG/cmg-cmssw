import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *

from CMGTools.Common.skims.cmgPFJetSel_cfi import *

############### Trigger - refactored to seperate file
from CMGTools.Susy.RazorMultiJet.triggerSkim_cff import *

#Leptons

#make the SAK muons first
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
razorMJMuonSAKCMG.cfg.leptonFactory.vertexCollection = 'goodOfflinePrimaryVertices'

from CMGTools.Common.skims.cmgMuonSel_cfi import *
razorMJMuonLoose = cmgMuonSel.clone(src = "razorMJMuonSAKCMG", cut = "(pt() > 5.) && (abs(eta()) < 2.4)")
razorMJMuonTight = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 25.) && (abs(eta()) < 2.4) && isPF() && relIso(0.5) < 0.15 && getSelection('cuts_tightmuonNoVtx') && abs(dxy()) < 0.02 && abs(dz()) < 0.5")
razorMJMuonSequence = cms.Sequence(
    razorMJMuonSAK+
    razorMJMuonSAKCMG+
    razorMJMuonLoose+
    razorMJMuonTight
    )
#will invert later
razorMJLooseMuonCount = cmgCandCount.clone( src = 'razorMJMuonLoose', minNumber = 1 )
razorMJTightMuonCount = cmgCandCount.clone( src = 'razorMJMuonTight', minNumber = 1 )

############### Electrons
#start off by making the veto electrons
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
razorMJElectronSAKCMG.cfg.primaryVertexCollection = 'goodOfflinePrimaryVertices'

razorMJIsolatedElectrons = cms.EDProducer(
    "ElectronIsolationProducer",
    electronTag = cms.InputTag('cmgElectronSel'),
    rhoTag = cms.InputTag('kt6PFJets','rho'),
    isoCut = cms.double(0.15),
    coneSize = cms.double(0.3)
    )

from CMGTools.Common.skims.cmgElectronSel_cfi import *
razorMJElectronLoose = cmgElectronSel.clone(src = "razorMJElectronSAKCMG", cut = '(pt()> 5.) && (abs(eta()) < 2.5)')
razorMJElectronTight = cmgElectronSel.clone(src = "razorMJIsolatedElectrons", cut = 'pt() >= 30 && getSelection("cuts_mediumNoVtx") && abs(dxy()) < 0.02 && abs(dz()) < 0.1 && (abs(sourcePtr().superCluster().eta()) <= 1.4442 || abs(sourcePtr().superCluster().eta()) > 1.566)')

razorMJElectronSequence = cms.Sequence(
    razorMJElectronSAK+
    razorMJElectronSAKCMG+
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
razorMJTightBtaggedJets = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = '(pt() >= 15.) && (abs(eta()) <= 2.4) && getSelection("cuts_csv_tight")' )
razorMJTightBtaggedVetoJets = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = '(pt() >= 15.) && (abs(eta()) <= 2.4) && !getSelection("cuts_csv_tight")' )

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
            minDeltaR=cms.double(0.1),
            removeMatchedObject=cms.bool(True)
            )

        ),
    verbose = cms.untracked.bool(False)
    )
razorMJTauLoose = cmgTauSel.clone(src = "razorMJTauCleaned", cut = '(pt() >= 15.) && (abs(eta()) <= 2.4) && (abs(tauID("byLooseCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3)')
razorMJTauTight = cmgTauSel.clone(src = "razorMJTauLoose", cut = '(abs(tauID("byTightCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3)')

#also run the UCSB TauVeto code
razorMJTauVeto = cms.EDProducer(
    "IndirectTauVetoProducer",
    jetTag = cms.InputTag('razorMJTightBtaggedVetoJets'),
    metTag = cms.InputTag('cmgPFMET')
    )

razorMJTauSequence = cms.Sequence(
    razorMJTightBtaggedJets*
    razorMJTauCleaned*
    razorMJTauLoose*
    razorMJTauTight+
    razorMJTightBtaggedVetoJets*
    razorMJTauVeto
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
razorMJPFJetSel30 = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>=30 && abs(eta)<=2.4' )
razorMJPFJetSel70 = cmgPFJetSel.clone( src = 'razorMJPFJetSel30', cut = 'pt()>=70' )

#we start by filtering very jetty events as we can't cope with them
razorPFJetSelCount = cmgCandCount.clone( src = 'cmgPFJetSel', minNumber = 20 )
razorMJPFJetSel30Count6j = cmgCandCount.clone( src = 'razorMJPFJetSel30', minNumber = 6 )
razorMJPFJetSel70Count2j = cmgCandCount.clone( src = 'razorMJPFJetSel70', minNumber = 2 )

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
            minDeltaR=cms.double(0.4),
            removeMatchedObject=cms.bool(True)
            ),
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJMuonTight"),
            minDeltaR=cms.double(0.4),
            removeMatchedObject=cms.bool(True)
            ),
        cms.PSet(                                     
            vetoCollection=cms.InputTag("razorMJTauTight"),
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
razorMJPFJetSel30Count4j = cmgCandCount.clone( src = 'razorMJJetCleanedLoose', minNumber = 4 )

###finally, try with the pruned jets too
from CMGTools.Common.skims.cmgStructuredPFJetSel_cfi import cmgStructuredPFJetSel
razorMJStructureJetSel30 = cmgStructuredPFJetSel.clone( src = 'cmgStructuredPFJetSel', cut = 'pt()>=30 && abs(eta)<=2.4' )

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
razorMJDiHemiHadBoxSel = cmgCandSel.clone( src = 'razorMJDiHemiHadBox', cut = 'getSelection("cuts_razor_r")' )

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

#now with substructure too
razorMJHemiHadBoxSS = razorMJHemiHadBox.clone()
razorMJHemiHadBoxSS.cfg.inputCollection[0] = "razorMJStructureJetSel30"
razorMJDiHemiHadBoxSS = cmgDiHemi.clone(
    cfg = cmgDiHemi.cfg.clone(
    leg1Collection = cms.InputTag('razorMJHemiHadBoxSS'),
    leg2Collection = cms.InputTag('razorMJHemiHadBoxSS'),
    metCollection = cms.InputTag('cmgPFMET')                  
    )    
)

############### Run the sequences
razorMJTriggerSequence = cms.Sequence(
    razorMJHadTriggerSel+
    #razorMJHadTriggerInfo+
    razorMJEleTriggerSel+
    razorMJMuTriggerSel
    )

razorMJJetSequence = cms.Sequence(                             
    razorMJPFJetSel30*
    razorMJPFJetSel70*
    razorMJPFJetSelID+
    razorMJJetCleanedLoose*
    razorMJPFJetSelIDLepton*
    razorPFJetsUp*
    razorMJPFJetSel30Up+
    razorPFJetsDown*
    razorMJPFJetSel30Down+
    razorMJMetUp+
    razorMJMetDown+
    razorMJStructureJetSel30
)

razorMJHemiSequence = cms.Sequence(
    razorMJHemiHadBox*
    razorMJDiHemiHadBox+
    razorMJHemiHadBoxUp*
    razorMJDiHemiHadBoxUp+
    razorMJHemiHadBoxDown*
    razorMJDiHemiHadBoxDown+
    razorMJHemiHadBoxSS*
    razorMJDiHemiHadBoxSS
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
    # two above 70
    razorMJPFJetSel70Count2j+
    #require a trigger
    #razorMJHadTriggerCount+
    #filter is inverted
    ~razorMJPFJetIDCount+
    #no loose electrons
    ~razorMJLooseElectronCount+
    #no loose muons
    ~razorMJLooseMuonCount+
    #no tight taus - TODO - choose tau veto
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
    #razorMJEleTriggerCount+
    #filter is inverted
    ~razorMJPFJetIDLeptonCount+
    #a tight one
    razorMJTightElectronCount+ 
    #no loose muons
    ~razorMJLooseMuonCount+
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
    #razorMJMuTriggerCount+
    #filter is inverted
    ~razorMJPFJetIDLeptonCount+
    #no loose electrons
    ~razorMJLooseElectronCount+
    #no loose muons, but a tight muon
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
    # two above 70
    razorMJPFJetSel70Count2j+
    #require a trigger
    #razorMJHadTriggerCount+
    #filter is inverted
    ~razorMJPFJetIDCount+
    #no loose electrons
    ~razorMJLooseElectronCount+
    #no loose muons
    ~razorMJLooseMuonCount+
    #a tight tau
    razorMJTightTauCount#+  
    #apply the Razor cuts
    #razorMJDiHemiHadBoxSel 
    )
