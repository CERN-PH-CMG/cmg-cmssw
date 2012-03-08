import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.MultiJet.multijet_cff import *
from CMGTools.Susy.Razor.razor_cff import *
from CMGTools.Susy.RazorMultiJet.razorMultijet_cff import *
from CMGTools.Common.countingSequences_cff import *

#full selection with trigger
multijetPath = cms.Path(
    razorMJSkimSequence
    )

#full selection, but no trigger
multijetPathNoTrigger = cms.Path(
    razorMJSkimSequence6jLeptonVeto
    )

#full selection, with trigger and only 5 jets
multijetPathMultijetTrigger = cms.Path(
    razorMJSkimSequenceMultiJetTrigger
    )

#full selection, with Razor trigger
multijetPathRazorTrigger = cms.Path(
    razorMJSkimSequenceRazorTrigger
    )

#full selection, with HT trigger
multijetPathHTTrigger = cms.Path(
    razorMJSkimSequenceHTTrigger
    )

#full selection, with L1 passthrough and only 5 jets
multijetPathL1Passthrough = cms.Path(
    razorMJSkimSequenceL1Passthrough
    )

#full selection, with L1 passthrough and only 5 jets
multijetPathL1Seed = cms.Path(
    razorMJSkimSequenceL1Seed
    )

