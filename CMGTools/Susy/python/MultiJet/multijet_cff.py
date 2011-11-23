import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter

############### Jets
btag = 'cuts_btag_medium'
# count high pt jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
#require 4 jets offline
multiPFJetSel60 = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>60 && abs(eta)<2.5' )
multiPFJetSel60Count = cmgCandCount.clone( src = 'multiPFJetSel60', minNumber = 4 )
multiPFBJetSel60 = cmgPFJetSel.clone( src = 'multiPFJetSel60', cut = 'getSelection("%s")' % btag )
multiPFBJetSel60Count = cmgCandCount.clone( src = 'multiPFBJetSel60', minNumber = 1 )

# id the jets
#ID at lower pt threshold - used to veto event - the number of jets that fail loose jet ID
multiPFJetSelID = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = '(pt()>30 && abs(eta)<3.0) && (!getSelection("cuts_looseJetId"))' )
multiPFJetIDCount = cmgCandCount.clone( src = 'multiPFJetSelID', minNumber = 1 ) #filter inverted below

############### Muons
from CMGTools.Common.skims.cmgMuonSel_cfi import *
multiMuonLoose = cmgMuonSel.clone(src = "cmgMuonSel", cut = "(pt() > 10.) && (abs(eta()) < 2.4) && isMuon()")

#find the jets that do not overlap with a jet
from CMGTools.Common.miscProducers.deltaRJetMuons_cfi import deltaRJetMuons
multiPFJetsMuonVeto = deltaRJetMuons.clone(
    inputCollection = cms.InputTag('multiPFJetSel60'),
    vetoCollection = cms.InputTag('multiMuonLoose'),
    minDeltaR = cms.double(0.5)#value from Maxime
)

#now find the jets that do overlap with a muon
from CMGTools.Susy.topprojections.pfjetprojector_cff import * 
multiPFJetsMuonRequired = pfJetOnPFJet.clone(
    topCollection = cms.InputTag('multiPFJetsMuonVeto'),
    bottomCollection = cms.InputTag('multiPFJetSel60')
)  
multiPFBJetsMuonRequired = cmgPFJetSel.clone( src = 'multiPFJetsMuonRequired', cut = 'getSelection("%s")' % btag )

############### Trigger
#make a skim on the HLT - should match all multi triggers
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
# triggers from the MultiJet, ElectronHad and MuHad PDs
multiTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_EightJet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_.*Mu[0-9]+.*Quad.*Jet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_.*Mu[0-9]+_TriCentral.*Jet[0-9]+_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_Ele[0-9]+.*Quad.*Jet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_Ele[0-9]+.*TriCentral.*Jet[0-9]+_v[0-9]+$") ||'
                                                ' getSelectionRegExp("^HLT_Ele[0-9]+.*CentralTriJet[0-9]+_v[0-9]+$")'
                                            )
multiTriggerQuadSel = cmgTriggerObjectSel.clone(
                                            src = 'multiTriggerSel',
                                            cut = 'getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$")'
                                            )
multiTriggerCount = cmgCandCount.clone( src = 'multiTriggerSel', minNumber = 1 )
multiTriggerQuadCount = cmgCandCount.clone( src = 'multiTriggerQuadSel', minNumber = 1 )

multiTriggerSequence = cms.Sequence(
    multiTriggerSel*
    multiTriggerQuadSel
    )

multiMuonSequence = cms.Sequence(
   multiMuonLoose
)

multiJetSequence = cms.Sequence(
    multiPFJetSel60+
    multiPFJetSelID*
    multiPFBJetSel60*
    multiPFJetsMuonVeto*
    multiPFJetsMuonRequired*
    multiPFBJetsMuonRequired
)

multiObjectSequence = cms.Sequence(
    multiMuonSequence +                                   
    multiJetSequence + 
    multiTriggerSequence                              
    )


multiSequence = cms.Sequence(
    multiObjectSequence
    )

#offline based selection
multijetSkimSequence = cms.Sequence(
    multiObjectSequence + 
    multiPFJetSel60Count+
    #filter is inverted
    ~multiPFJetIDCount+
    multiTriggerQuadCount+
    multiPFBJetSel60Count
    )

#trigger based selection - we take all multi triggered events
multijetTriggerSkimSequence = cms.Sequence(
    multiObjectSequence+
    multiTriggerCount                                
    )
