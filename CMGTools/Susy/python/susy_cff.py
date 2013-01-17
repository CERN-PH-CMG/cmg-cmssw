import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.MultiJet.multijet_cff import *
from CMGTools.Susy.Razor.razor_cff import *
from CMGTools.Susy.RazorMultiJet.razorMultijet_cff import *
from CMGTools.Common.countingSequences_cff import *

razorMJSkimSequenceHadPath = cms.Path(
    razorMJSkimSequenceHad
)

razorMJSkimSequenceElePath = cms.Path(
    razorMJSkimSequenceEle
)

razorMJSkimSequenceMuPath = cms.Path(
    razorMJSkimSequenceMu
)

trkVetoLeptonSequencePath = cms.Path(
    trkVetoLeptonSequence
)
