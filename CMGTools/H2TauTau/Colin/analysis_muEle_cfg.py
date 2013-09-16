#TODO NO TRIGGER MAPPING YET!
#NO MC TRIGGER SELECTION YET!
#NEED TO TRY MIT MVA
#GET EMBEDDED SAMPLES

import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters

def getFiles(dataset, user, pattern):
    from CMGTools.Production.datasetToSource import datasetToSource
    # print 'getting files for', dataset,user,pattern
    ds = datasetToSource( user, dataset, pattern, True )
    files = ds.fileNames
    return ['root://eoscms//eos/cms%s' % f for f in files]



period = 'Period_2011A'

baseDir = '2011'
pat = 'PAT_CMG_TestMVAs'
htt = 'H2TAUTAU_TestMVAs_Mar09'
filePattern = 'muEle.*fullsel.*root'


H2TauTauPackage = '/'.join( [ os.environ['CMSSW_BASE'],
                              'src/CMGTools/H2TauTau' ] ) 



# mc_triggers = 'HLT_IsoMu12_v1'
mc_triggers = []
mc_triggers_fall11 = [
    ]

mc_jet_scale = 1.
mc_jet_smear = 0.
mc_vertexWeight = None
mc_tauEffWeight = None
mc_eleEffWeight = None

if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeight2invfb'
    mc_muEffWeight = 'effMu2011A'
    mc_eleEffWeight = 'effEle2011A'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeight2011B'
    mc_muEffWeight = 'effMu2011B'
    mc_eleEffWeight = 'effEle2011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeight2011AB'
    mc_muEffWeight = 'effMu2011AB'
    mc_eleEffWeight = 'effEle2011AB'


# global MC weighting factors
# mc_lep_effCorrFactor = 0.968
# mc_tau_effCorrFactor = 0.92
# mc_addtl = 0.786
# mc_effCorrFactor = mc_lep_effCorrFactor * mc_tau_effCorrFactor 
mc_effCorrFactor = 1.

data_triggers_11A = [
    'HLT_Mu8_Ele17_CaloIdL_v3', # May10ReReco
    'HLT_Mu17_Ele8_CaloIdL_v3', # May10ReReco
    'HLT_Mu8_Ele17_CaloIdL_v4', # v4
    'HLT_Mu8_Ele17_CaloIdL_v5', # v4
    'HLT_Mu8_Ele17_CaloIdL_v6', # v4
    'HLT_Mu17_Ele8_CaloIdL_v4', # v4
    'HLT_Mu17_Ele8_CaloIdL_v5', # v4
    'HLT_Mu17_Ele8_CaloIdL_v6', # v4
    'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v3', # Aug5 
    'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v4', # Oct3
    'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v3', # Aug5 
    'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v4', # Oct3
    ]

data_triggers_11B = [
    'HLT_Mu8_Ele17_CaloIdL_v9',
    'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v4',
    'HLT_Mu17_Ele8_CaloIdL_v12',
    'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v7'
    ]

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

muEleAna = cfg.Analyzer(
    'MuEleAnalyzer',
    pt1 = 10,
    pt2 = 20,
    iso1 = 0.1,
    iso2 = 0.1,
    eta1 = 2.1,
    eta2 = 2.3,
    # eta2 = 1.4,
    m_min = 10,
    m_max = 99999,
    diLeptonCutString = 'cuts_baseline'
    )

muWeighter = cfg.Analyzer(
    'LeptonWeighter_mu',
    effWeight = mc_muEffWeight,
    lepton = 'leg1',
    verbose = False
    )

eleWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    lepton = 'leg2',
    verbose = False
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    vertexWeight = mc_vertexWeight,
    verbose = False
    )

# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetPt = 30,
    jetEta = 4.5,
    **vbfKwargs
    )

eventSorter = cfg.Analyzer(
    'H2TauTauEventSorter',
    leg1 = 'mu',
    leg2 = 'ele',
    # vertexWeight = mc_vertexWeight,
    MT_low = 40,
    MT_high = 60,
    Boosted_JetPt = 150,
    **vbfKwargs
    )

#########################################################################################

data_Run2011A_May10ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011A_May10ReReco_v1',
    files = getFiles('/TauPlusX/Run2011A-May10ReReco-v1/AOD/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 168.597,
    triggers = data_triggers_11A,
    json = '{H2TauTauPackage}/json/finalTauPlusXMay.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )


data_Run2011A_PromptReco_v4 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v4',
    files = getFiles('/TauPlusX/Run2011A-PromptReco-v4/AOD/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 929.748,
    triggers = data_triggers_11A,
    json = '{H2TauTauPackage}/json/finalTauPlusXv4.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011A_05Aug2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_05Aug2011_v1',
    files = getFiles('/TauPlusX/Run2011A-05Aug2011-v1/AOD/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 373.349,
    triggers = data_triggers_11A,
    json = '{H2TauTauPackage}/json/finalTauPlusXAug5.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011A_03Oct2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_03Oct2011_v1',
    files = getFiles('/TauPlusX/Run2011A-03Oct2011-v1/AOD/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 658.886,
    triggers = data_triggers_11A,
    json = '{H2TauTauPackage}/json/finalTauPlusXOct3.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011B_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2011B_PromptReco_v1',
    files = getFiles('/TauPlusX/Run2011B-PromptReco-v1/AOD/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 2511.0,
    triggers = data_triggers_11B,
    json = '{H2TauTauPackage}/json/finalTauPlusX11B.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )


#########################################################################################


DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    xSection = 3048.,
    nGenEvents = 36209629,
    #     nGenEvents = 28086242,    
    triggers = mc_triggers_fall11,
    effCorrFactor = mc_effCorrFactor)


WJets = cfg.MCComponent(
    name = 'WJets',
    # files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}/{htt}/Merge_TauMu'.format(pat=pat, htt=htt), 'cbern', filePattern),
    files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/TAUTAU/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    xSection = 31314.,
    nGenEvents = 81345381, # this number is probably slightly overestimated (3/1000 missing files, not taken into account)
    triggers = mc_triggers_fall11,
    effCorrFactor = mc_effCorrFactor )


TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    xSection = 165.8,
    nGenEvents = 3701947,
    triggers = mc_triggers_fall11,
    effCorrFactor = mc_effCorrFactor )



#########################################################################################


MC = [DYJets, WJets, TTJets]
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear


data_2011A = [
    data_Run2011A_May10ReReco_v1,
    data_Run2011A_PromptReco_v4,
    data_Run2011A_05Aug2011_v1,
    data_Run2011A_03Oct2011_v1,
    # data_Run2011A_PromptReco_v6, # equivalent to 03Oct, if I remember correctly
    ]
data_2011B = [
    data_Run2011B_PromptReco_v1
    ]



selectedComponents =  copy.copy(MC)
if period == 'Period_2011A':
    selectedComponents.extend( data_2011A )
elif period == 'Period_2011B':
    selectedComponents.extend( data_2011B )
elif period == 'Period_2011AB':
    selectedComponents.extend( data_2011A )
    selectedComponents.extend( data_2011B )

sequence = cfg.Sequence( [
    triggerAna,
    vertexAna,
    muEleAna,
    muWeighter, 
    eleWeighter, 
    vbfAna,
    eventSorter
   ] )


DYJets.fakes = True
DYJets.splitFactor = 5
TTJets.splitFactor = 8
data_Run2011B_PromptReco_v1.splitFactor = 2
data_Run2011A_PromptReco_v4.splitFactor = 2 

test = 0
if test==1:
    comp = DYJets
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:2]
    # TTJets.files = TTJets.files[:1]
elif test==2:
    for comp in selectedComponents:
     comp.splitFactor = 1
     comp.files = comp.files[:2]


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
