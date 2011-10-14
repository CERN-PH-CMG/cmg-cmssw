import FWCore.ParameterSet.Config as cms

from CMGTools.H2TauTau.Colin.objects.object_cff import *
from CMGTools.H2TauTau.Colin.skims.skim_cff import *
from CMGTools.H2TauTau.Colin.histograms.histogram_cff import *
from CMGTools.H2TauTau.Colin.histograms.histogramSkim_cff import *

tauMuPath = cms.Path(
    objectSequence + 
    # histogramSequence +  
    tauMuSkimSequence +
    histogramSkimSequence 
    )

tauEPath = cms.Path(
    objectSequence + 
    tauESkimSequence     
    # beware! can we put histogramSkimSequence here as well? aren't the histograms goign to be filled twice?
    )
