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

muEleAna = cfg.Analyzer(
    'MuEleFourLeptonAnalyzer',
    pt1 = 5.,
    eta1 = 2.4,
    iso1 = 0.7,
    sip1 = 4,
    pt2 = 7.,
    eta2 = 2.5,
    iso2 = 0.7,
    sip2 = 4, 
    z1_m = (50,120),
    z1_pt1 = 20,
    z1_pt2 = 10,
    z2_m = 12,
    z2_pt1 = 0,
    z2_pt2 = 0,
    h_m = 100,
    nLeptonsMin = 4
    )



muMuAna = cfg.Analyzer(
    'MuMuFourLeptonAnalyzer',
    pt1 = 5.,
    eta1 = 2.4,
    iso1 = 0.7,
    sip1 = 4,
    # 
    pt2 = 5,
    eta2 = 2.4,
    iso2 = 0.7,
    sip2 = 4, 
    #
    z1_m = (50,120),
    z1_pt1 = 20,
    z1_pt2 = 10,
    #
    z2_m = 12,
    z2_pt1 = 0,
    z2_pt2 = 0,
    # 
    h_m = 100,
    pair_iso = 0.35, 
    nLeptonsMin = 2
    )

eleEleAna = cfg.Analyzer(
    'EleEleFourLeptonAnalyzer',
    pt1 = 7.,
    eta1 = 2.5,
    iso1 = 0.7,
    sip1 = 4,
    # 
    pt2 = 7,
    eta2 = 2.5,
    iso2 = 0.7,
    sip2 = 4, 
    #
    z1_m = (50,120),
    z1_pt1 = 20,
    z1_pt2 = 10,
    #
    z2_m = 12,
    z2_pt1 = 0,
    z2_pt2 = 0,
    # 
    h_m = 100,
    pair_iso = 0.35, 
    nLeptonsMin = 2
    )


treeProducer = cfg.Analyzer(
     'FourLeptonTreeProducer'
     )

####################################################################################

from CMGTools.HToZZTo4Leptons.samples.samples_march21 import * 

# from CMGTools.H2TauTau.proto.samples.tauMu_march12 import DYJets

####################################################################################


sequence = cfg.Sequence( [
    triggerAna,
    vertexAna,
    # muEleAna,
    # muMuAna,
    eleEleAna,
    treeProducer
   ] )

# Hig120GluGlu.files = Hig120GluGlu.files[:1]
# DYJets.triggers = []

Hig120GluGlu.splitFactor = 1
selectedComponents = [Hig120GluGlu]
# selectedComponents = [DYJets]
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
