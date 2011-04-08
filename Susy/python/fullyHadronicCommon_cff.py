import FWCore.ParameterSet.Config as cms

from CMGTools.Common.analysis_cff import *
from CMGTools.Susy.RA1.RA1_cff import *
from CMGTools.Susy.RA2.RA2_cff import *
from CMGTools.Susy.Razor.razor_cff import *

jetPtCut = 'pt()>20'
cmgPFJetSel.cut = jetPtCut
cmgPFBaseJetSel.cut = jetPtCut
cmgCaloBaseJetSel.cut = jetPtCut

fullyHadronicCommonSequence  = cms.Sequence(
    analysisSequence +
    RA1Sequence +
    RA2Sequence +
    razorSequence
    )
