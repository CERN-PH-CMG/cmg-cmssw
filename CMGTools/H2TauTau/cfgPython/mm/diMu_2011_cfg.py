import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.H2TauTau.proto.weights.weighttable import mu_id_taumu_2012, mu_iso_taumu_2012
from CMGTools.H2TauTau.proto.samples.sampleShift import selectShift
from CMGTools.RootTools.RootTools import * 

mc_vertexWeight = 'vertexWeightFall112012AB'

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'offlinePrimaryVertices',
    vertexWeight = mc_vertexWeight,
    # fixedWeight = 1,
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
    verbose = False
    )


leg1Weighter = cfg.Analyzer(
    'LeptonWeighter_leg1',
    effWeight = None,
    effWeightMC = None,
    lepton = 'leg1',
    verbose = False,
    disable = True,
    idWeight = mu_id_taumu_2012,
    isoWeight = mu_iso_taumu_2012    
    )

leg2Weighter = cfg.Analyzer(
    'LeptonWeighter_leg2',
    effWeight = None,
    effWeightMC = None,
    lepton = 'leg2',
    verbose = False,
    disable = True,
    idWeight = mu_id_taumu_2012,
    isoWeight = mu_iso_taumu_2012    
    )


# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 500,
                  deltaEta = 3.5    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_42X.weights.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 20.,
    jetEta = 4.7,
    cjvPtCut = 30.,
    btagSFseed = 123456,
    relaxJetId = True, 
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'ZJetsTreeProducer'
    )


#########################################################################################


from CMGTools.H2TauTau.proto.samples.run2011.diMu_ColinNov6 import *

#########################################################################################

# selectedComponents = copy.copy(data_list_2012)
selectedComponents = data_list_2012
# selectedComponents.append( DYJets )
# selectedComponents = []
selectedComponents.extend([TTJets, DYJets])
selectedComponents.extend(mc_diboson)

VVgroup = [comp.name for comp in mc_diboson]

sequence = cfg.Sequence( [
    #     eventSelector,
    jsonAna,
    vertexAna,
    ZMuMuAna,
    leg1Weighter,
    leg2Weighter,
    # WNJetsAna, 
    vbfAna,
    treeProducer
   ] )


test = 0
if test==1:
    comp = DYJets
    comp.files = comp.files[:10]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    selectedComponents = copy.copy(data_list_2012)
    selectedComponents.extend(embed_list_2012)
    selectedComponents.extend(MC_list[0:3])
    
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:3]



config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
