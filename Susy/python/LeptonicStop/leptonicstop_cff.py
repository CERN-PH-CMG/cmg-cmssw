import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter
from CMGTools.Common.miscProducers.collectionSize.collectionSize_cff import candidateSize

# count high pt jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *

#cf the HLT TriCentralJet30 trigger - only cut on eta to get inside the tracker volume
leptonicStopPFJetSel = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt() >= 30 && abs(eta)<2.4' )
leptonicStopPFJetSelSize = candidateSize.clone(src = cms.InputTag('leptonicStopPFJetSel'))
leptonicStopPFJetSelCount = cmgCandCount.clone( src = 'leptonicStopPFJetSel', minNumber = 3 )

#count the number of btags with a medium tag
leptonicStopPFBJetSel = cmgPFJetSel.clone( src = 'leptonicStopPFJetSel', cut = 'getSelection("cuts_btag_medium")' )
leptonicStopPFBJetSelSize = candidateSize.clone(src = cms.InputTag('leptonicStopPFBJetSel'))

#ID at lower pt threshold - used to veto event - the number of jets that fail loose jet ID
leptonicStopSelID = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = '(pt()>20 && abs(eta)<3.0) && (!getSelection("cuts_looseJetId"))' )
leptonicStopIDCount = cmgCandCount.clone( src = 'leptonicStopSelID', minNumber = 1 ) #filter inverted below

leptonicStopJetSequence = cms.Sequence(
    leptonicStopPFJetSel*
    leptonicStopPFJetSelSize*
    leptonicStopSelID*
    leptonicStopPFBJetSel*
    leptonicStopPFBJetSelSize
)

#ID the leptons and require at least one
from CMGTools.Common.skims.leadingCMGMuonSelector_cfi import leadingCMGMuonSelector
from CMGTools.Common.skims.cmgMuonSel_cfi import *
#the tight selection is a subset of the loose selection
leptonicStopMuonLoose = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 10.) && (abs(eta()) < 2.4) && getSelection('cuts_vbtfmuon_isGlobal') && getSelection('cuts_vbtfmuon_numberOfValidTrackerHits')")
leptonicStopMuonTight = cmgMuonSel.clone(src = "leptonicStopMuonLoose", cut = "(pt() > 17.) && (abs(eta()) < 2.1) && relIso(0.5)<0.15 && getSelection('cuts_vbtfmuon')")
leptonicStopLeadingMuon = leadingCMGMuonSelector.clone(inputCollection = "leptonicStopMuonTight", index = cms.int32(1))
#HLT - isoMu17 eta cut at eta 2.5
leptonicStopMuonSequence = cms.Sequence(
    leptonicStopMuonLoose+
    leptonicStopMuonTight+ 
    leptonicStopLeadingMuon
    )

from CMGTools.Common.skims.cmgElectronSel_cfi import *
from CMGTools.Common.skims.leadingCMGElectronSelector_cfi import leadingCMGElectronSelector
#the tight selection is a subset of the loose selection
leptonicStopElectronLoose = cmgElectronSel.clone(src = "cmgElectronSel", cut = '(pt()> 10.) && (abs(eta()) < 2.5) && (abs(eta()) < 1.4442 || abs(eta()) > 1.566) && getSelection("cuts_vbtf95ID")')
leptonicStopElectronTight = cmgElectronSel.clone(src = "leptonicStopElectronLoose", cut = 'pt() > 25. && getSelection("cuts_vbtf80ID") && (relIso() < 0.2) && (abs(dxy()) < 0.02)')
leptonicStopLeadingElectron = leadingCMGElectronSelector.clone(inputCollection = "leptonicStopElectronTight", index = cms.int32(1))
# the electron trigger is HLT Ele25, eta 2.5
leptonicStopElectronSequence = cms.Sequence(
    leptonicStopElectronLoose+
    leptonicStopElectronTight+
    leptonicStopLeadingElectron
    )

#now require that there is least one selected lepton
leptonicStopLeadingLeptons = cmgCandMerge.clone(
    src = cms.VInputTag(
      cms.InputTag("leptonicStopLeadingElectron"),
      cms.InputTag("leptonicStopLeadingMuon"),
    )
    )
#require one tight lepton - don't care which
leptonicStopLeadingObjectCount = cmgCandCount.clone( src = 'leptonicStopLeadingLeptons', minNumber = 1 )

from CMGTools.Susy.topprojections.electronprojector_cff import * 
from CMGTools.Susy.topprojections.muonprojector_cff import * 

leptonicStopNoLeadingElectron = electronOnElectron.clone(
    topCollection = cms.InputTag('leptonicStopLeadingElectron'),
    bottomCollection = cms.InputTag('leptonicStopElectronLoose')
)  

leptonicStopNoLeadingMuon = muonOnMuon.clone(
    topCollection = cms.InputTag('leptonicStopLeadingMuon'),
    bottomCollection = cms.InputTag('leptonicStopMuonLoose')
)                                        

leptonicStopLeptonSequence = cms.Sequence(
    leptonicStopMuonSequence+
    leptonicStopElectronSequence*
    leptonicStopLeadingLeptons*
    leptonicStopNoLeadingElectron+
    leptonicStopNoLeadingMuon
    )

#add a MET cut - but not too tight for the moment
leptonicStopMET = cmgCandSel.clone(src = 'cmgPFMET', cut = 'et() >= 50')
leptonicStopMETCount = cmgCandCount.clone( src = 'leptonicStopMET', minNumber = 1 )

#recalculate the MET, removing the leading tight lepton
from CMGTools.Common.Tools.cmgBaseMETModifier_cfi import cmgBaseMETModifier
leptonicStopMETNoMu = cmgBaseMETModifier.clone(
    cfg = cmgBaseMETModifier.cfg.clone(
    inputCollection = cms.InputTag("leptonicStopNoLeadingMuon"),
    metCollection = cms.InputTag("cmgPFMET"),
    operator = cms.string('+') #Add the muons to the MET                                               
    )
)
leptonicStopMETNoEle = cmgBaseMETModifier.clone(
    cfg = cmgBaseMETModifier.cfg.clone(
    inputCollection = cms.InputTag("leptonicStopNoLeadingElectron"),
    metCollection = cms.InputTag("cmgPFMET"),
    operator = cms.string('+') #Add the electrons to the MET                                               
    )
)

leptonicStopMETSequence = cms.Sequence(
    leptonicStopMET+
    leptonicStopMETNoMu+
    leptonicStopMETNoEle              
    )

#now build the W candidates
from CMGTools.Common.factories.cmgWENu_cfi import *
from CMGTools.Common.factories.cmgWMuNu_cfi import *

from CMGTools.Common.skims.cmgWENuSel_cfi import *
from CMGTools.Common.skims.cmgWENuCount_cfi import *
from CMGTools.Common.skims.cmgWMuNuSel_cfi import *
from CMGTools.Common.skims.cmgWMuNuCount_cfi import *

#use the tight leptons and throw away the low MT region
leptonicStopWENu = cmgWENu.clone()
leptonicStopWENu.cfg.leg1Collection = "leptonicStopElectronTight"
leptonicStopWENu.cuts.mt = cms.string('mT() >= 30')
leptonicStopWENuSel = cmgWENuSel.clone(src = "leptonicStopWENu", cut = "getSelection('cuts_mt')")

#use the tight leptons and throw away the low MT region
leptonicStopWMuNu = cmgWMuNu.clone()
leptonicStopWMuNu.cfg.leg1Collection = "leptonicStopMuonTight"
leptonicStopWMuNu.cuts.mt = cms.string('mT() >= 30')
leptonicStopWMuNuSel = cmgWMuNuSel.clone(src = "leptonicStopWMuNu", cut = "getSelection('cuts_mt')")

#require at least one W candidate to pass the selection
leptonicStopW = cmgCandMerge.clone(
    src = cms.VInputTag(
      cms.InputTag("leptonicStopWENuSel"),
      cms.InputTag("leptonicStopWMuNuSel"),
    )
    )
leptonicStopWCount = cmgCandCount.clone( src = 'leptonicStopW', minNumber = 1 )

#remake the W's with the modified MET
leptonicStopWMuNuNoMu = leptonicStopWMuNu.clone()
leptonicStopWMuNuNoMu.cfg.leg2Collection = "leptonicStopMETNoMu"
leptonicStopWMuNuNoEle = leptonicStopWMuNu.clone()
leptonicStopWMuNuNoEle.cfg.leg2Collection = "leptonicStopMETNoEle"

leptonicStopWEleNuNoMu = leptonicStopWENu.clone()
leptonicStopWEleNuNoMu.cfg.leg2Collection = "leptonicStopMETNoMu"
leptonicStopWEleNuNoEle = leptonicStopWENu.clone()
leptonicStopWEleNuNoEle.cfg.leg2Collection = "leptonicStopMETNoEle"

leptonicStopWSequence = cms.Sequence(
    leptonicStopWENu*
    leptonicStopWENuSel+
    leptonicStopWMuNu*
    leptonicStopWMuNuSel*
    leptonicStopW+
    leptonicStopWMuNuNoMu+
    leptonicStopWMuNuNoEle+
    leptonicStopWEleNuNoMu+
    leptonicStopWEleNuNoEle    
    )

#now veto the Z mass window
from CMGTools.Common.factories.cmgDiElectron_cfi import *
from CMGTools.Common.factories.cmgDiMuon_cfi import *

#we know we have one tight lepton here, so just veto on mass
leptonicStopDiElectron = cmgDiElectron.clone()
leptonicStopDiElectron.cfg.leg1Collection = "leptonicStopElectronLoose"
leptonicStopDiElectron.cfg.leg2Collection = "leptonicStopElectronLoose"

#we know we have one tight lepton here, so just veto on mass
leptonicStopDiMuon = cmgDiMuon.clone()
leptonicStopDiMuon.cfg.leg1Collection = "leptonicStopMuonLoose"
leptonicStopDiMuon.cfg.leg2Collection = "leptonicStopMuonLoose"

leptonicStopZSequence = cms.Sequence(
    leptonicStopDiElectron+
    leptonicStopDiMuon
    )

#add the lepton vetos now
from CMGTools.Common.miscProducers.deltaRJetMuons_cfi import deltaRJetMuons
leptonicStopPFJetsMuonVeto = deltaRJetMuons.clone(
    inputCollection = cms.InputTag('leptonicStopPFJetSel'),#only require kinematics, not ID
    vetoCollection = cms.InputTag('leptonicStopMuonTight')
)
from CMGTools.Common.miscProducers.deltaRJetElectrons_cfi import deltaRJetElectrons
leptonicStopPFJetsLeptonVeto = deltaRJetElectrons.clone(
    inputCollection = cms.InputTag('leptonicStopPFJetsMuonVeto'),#only require kinematics, not ID
    vetoCollection = cms.InputTag('leptonicStopElectronTight')
)
# filter out B-tagged jets for latter use
leptonicStopPFBJetsLeptonVeto = cmgPFJetSel.clone( src = 'leptonicStopPFJetsLeptonVeto', cut = 'getSelection("cuts_btag_medium")' )
leptonicStopPFBJetsLeptonVetoSize = candidateSize.clone(src = cms.InputTag('leptonicStopPFBJetsLeptonVeto'))
leptonicStopPFJetsLeptonVetoCount = cmgCandCount.clone( src = 'leptonicStopPFJetsLeptonVeto', minNumber = 3 )

leptonicStopVetoSequence = cms.Sequence(
    leptonicStopPFJetsMuonVeto*
    leptonicStopPFJetsLeptonVeto*
    leptonicStopPFBJetsLeptonVeto*
    leptonicStopPFBJetsLeptonVetoSize
    )

#make a skim on the HLT - should match all leptonicStop triggers
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
# triggers from the ElectronHad and MuHad PDs
leptonicStopTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_.*Mu[0-9]+.*Quad.*Jet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_.*Mu[0-9]+_TriCentral.*Jet[0-9]+_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_Ele[0-9]+.*Quad.*Jet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_Ele[0-9]+.*TriCentral.*Jet[0-9]+_v[0-9]+$") ||'
                                                ' getSelectionRegExp("^HLT_Ele[0-9]+.*CentralTriJet[0-9]+_v[0-9]+$")'
                                            )
leptonicStopTriggerCount = cmgCandCount.clone( src = 'leptonicStopTriggerSel', minNumber = 1 )
leptonicStopTriggerInfo = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("leptonicStopTriggerSel"),
    printSelections = cms.untracked.bool(True)
    )

leptonicStopTriggerSequence = cms.Sequence(
    leptonicStopTriggerSel#*
#    leptonicStopTriggerInfo
    )

leptonicStopObjectSequence = cms.Sequence(
    leptonicStopJetSequence + 
    leptonicStopLeptonSequence +
    leptonicStopMETSequence +    
    leptonicStopWSequence +
    leptonicStopZSequence +
    leptonicStopVetoSequence + 
    leptonicStopTriggerSequence                              
    )

leptonicStopSequence = cms.Sequence(
    leptonicStopObjectSequence
    )

#offline based selection
leptonicStopSkimSequence = cms.Sequence(
    leptonicStopObjectSequence+ 
    #at least one tight lepton
    leptonicStopLeadingObjectCount +
    # at least 3 jets
    leptonicStopPFJetSelCount+
    # no ID failures
    ~leptonicStopIDCount+
    # cut on MET
    leptonicStopMETCount+
    # cut the low Mt QCD
    leptonicStopWCount+
    # at least 3 jets after the veto
    leptonicStopPFJetsLeptonVetoCount
    )

#trigger based selection - we take all leptonicStop triggered events
leptonicStopTriggerSkimSequence = cms.Sequence(
    leptonicStopObjectSequence+
    leptonicStopTriggerCount                                
    )
