import FWCore.ParameterSet.Config as cms

from CMGTools.Common.analysis_cff import *
from CMGTools.Susy.RA2.RA2_cff import *

jetPtCut = 'pt()>20'
cmgPFJetSel.cut = jetPtCut
cmgPFBaseJetSel.cut = jetPtCut
cmgCaloBaseJetSel.cut = jetPtCut

fullyHadronicCommonSequence  = cms.Sequence(
    analysisSequence +
    RA2Sequence
    # add RA1 and Razor object + histogram sequences in the same way.
    ) 
