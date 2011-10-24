import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.object_cff import *
from CMGTools.H2TauTau.skims.skim_cff import *
from CMGTools.H2TauTau.histograms.histogram_cff import *
from CMGTools.H2TauTau.histograms.histogramSkim_cff import *

tauMuPath = cms.Path(
    objectSequence + 
    tauMuSkimSequence +
    histogramSkimSequence 
    )


tauMuBaselinePath = cms.Path(
    objectSequence + 
    tauMuBaselineSkimSequence
    )



tauEPath = cms.Path(
    objectSequence + 
    tauESkimSequence     
    # beware! can we put histogramSkimSequence here as well? aren't the histograms goign to be filled twice?
    )
