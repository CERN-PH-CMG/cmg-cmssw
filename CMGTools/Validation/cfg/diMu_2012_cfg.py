import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import * 

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'offlinePrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False
    )


ZMuMuAna = cfg.Analyzer(
    'ZMuMuAnalyzer',
    pt1 = 10,
    eta1 = 2.4,
    iso1 = 0.3,
    pt2 = 10,
    eta2 = 2.4,
    iso2 = 0.3,
    m_min = 50,
    m_max = 120,
    verbose = False,
    triggerMap = None
    )




ZHistograms = cfg.Analyzer(
    'ZHistogrammer'
     )

###############################################################################


from CMGTools.ZJetsTutorial.samples.run2012.diMu_ColinNov13 import DYJets

###############################################################################


selectedComponents = [DYJets]


sequence = cfg.Sequence( [
    vertexAna,
    ZMuMuAna,
    ZHistograms
   ] )


# set test = 0 to run all jobs, in case you are using pybatch.py
test = 1
if test==1:
    # test a single component, using a single thread.
    # necessary to debug the code, until it doesn't crash anymore
    comp = DYJets
    comp.files = comp.files[:5]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:    
    # test all components (1 thread per component.
    # important to make sure that your code runs on any kind of component
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:3]

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
