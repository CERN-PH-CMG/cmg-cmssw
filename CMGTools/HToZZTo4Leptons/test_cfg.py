import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg

period = 'Period_2011AB'

mc_vertexWeight = None
if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeightFall112invfb'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeightFall112011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeightFall112011AB'


triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    vertexWeight = mc_vertexWeight,
    verbose = False
    )

muMuEleEleAna = cfg.Analyzer(
    'MuEleFourLeptonAnalyzer',
    pt1 = 5.,
    eta1 = 2.4,
    iso1 = 999,
    pt2 = 7.,
    eta2 = 2.5,
    iso2 = 999,
    z1_m = 60,
    z1_pt1 = 20,
    z1_pt2 = 10,
    nLeptonsMin = 2
    )

treeProducer = cfg.Analyzer(
     'FourLeptonTreeProducer'
     )

####################################################################################

# from CMGTools.HToZZTo4Leptons.samples.phedex_march20 import * 

from CMGTools.H2TauTau.proto.samples.tauMu_march12 import DYJets

####################################################################################


sequence = cfg.Sequence( [
    triggerAna,
    vertexAna,
    muMuEleEleAna,
    treeProducer
   ] )
DYJets.files = DYJets.files[:5]
DYJets.triggers = []

# selectedComponents = [Hig120GluGlu]
selectedComponents = [DYJets]
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
