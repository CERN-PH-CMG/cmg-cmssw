import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters



triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

ZEleEleAna = cfg.Analyzer(
    'ZEleEleAnalyzer',
    pt1 = 10,
    pt2 = 10,
    iso1 = 0.3,
    iso2 = 0.3,
    eta1 = 2.5,
    eta2 = 2.5,
    m_min = 0,
    m_max = 200
    )


## tauWeighter = cfg.Analyzer(
##     'LeptonWeighter_tau',
##     effWeight = mc_tauEffWeight,
##     effWeightMC = mc_tauEffWeight_mc,
##     lepton = 'leg1',
##     verbose = False
##     )

## muonWeighter = cfg.Analyzer(
##     'LeptonWeighter_mu',
##     effWeight = mc_muEffWeight,
##     effWeightMC = mc_muEffWeight_mc,
##     lepton = 'leg2',
##     verbose = False
##     )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    # vertexWeight = mc_vertexWeight,
    fixedWeight = 1,
    verbose = False
    )


# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    # jetCol = 'cmgPFJetSelAK5NoPUSubCMG',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 4.5,
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducer'
    )

#########################################################################################

# from CMGTools.H2TauTau.proto.samples.run2012.zmumu_13Apr import * 
from CMGTools.H2TauTau.proto.samples.run2012.zeleele_13Apr import * 

#########################################################################################


sequence = cfg.Sequence( [
    triggerAna,
    ZEleEleAna,
    vbfAna,
    vertexAna,
    # tauWeighter, 
    # muonWeighter, 
    # eventSorter,
    treeProducer
   ] )

data_Run2012A_PromptReco_v1.splitFactor = 20
DYJets.splitFactor = 50

selectedComponents = [
    data_Run2012A_PromptReco_v1,
    DYJets
    ]

test = False
if test:
    theComp = DYJets
    theComp.splitFactor = 1
    selectedComponents = [theComp]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
