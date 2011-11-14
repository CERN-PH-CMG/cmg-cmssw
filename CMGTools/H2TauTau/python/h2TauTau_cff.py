import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.objects.object_cff import *
from CMGTools.H2TauTau.skims.skim_cff import *
from CMGTools.H2TauTau.histograms.histogram_cff import *
from CMGTools.H2TauTau.histograms.histogramSkim_cff import *

# preselection

tauMuPreSelPath = cms.Path(
    objectSequence + 
    tauMuPreSelSkimSequence +
    histogramSkimSequence 
    )


# full selection
tauMuFullSelPath = cms.Path(
    objectSequence + 
    tauMuFullSelSkimSequence
    )



tauEPath = cms.Path(
    objectSequence + 
    tauESkimSequence     
    # beware! can we put histogramSkimSequence here as well? aren't the histograms goign to be filled twice?
    )
