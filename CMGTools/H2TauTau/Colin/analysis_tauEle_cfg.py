#TODO NO TRIGGER MAPPING YET! 
#NO MC TRIGGER SELECTION YET! should now be ok
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



period = 'Period_2011B'

baseDir = '2011'
# aod = 'V3'
# pat = 'PAT_CMG_TestMVAs'
# htt = 'H2TAUTAU_TestMVAs_Mar09'
aod = 'V2'
pat = 'PAT_CMG_V2_5_0'
htt = 'H2TAUTAU_Feb2'
filePattern = 'tauEle.*fullsel.*root'


H2TauTauPackage = '/'.join( [ os.environ['CMSSW_BASE'],
                              'src/CMGTools/H2TauTau' ] ) 



# mc_triggers = 'HLT_IsoMu12_v1'
mc_triggers = []
mc_triggers_fall11 = [
    'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1'
    ]

mc_jet_scale = 1.
mc_jet_smear = 0.
mc_vertexWeight = None
mc_tauEffWeight = None
mc_eleEffWeight = None

mc_tauEffWeight_mc = 'effMediumIsoTau20'
mc_eleEffWeight_mc = 'effEle18MC'
if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeight2invfb'
    mc_tauEffWeight = 'effTau2011A_TauEle'
    mc_eleEffWeight = 'effEle2011A'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeight2011B'
    mc_tauEffWeight = 'effTau2011B_TauEle'
    mc_eleEffWeight = 'effEle2011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeight2011AB'
    mc_tauEffWeight = 'effTau2011AB_TauEle'
    mc_eleEffWeight = 'effEle2011AB'


# global MC weighting factors
# mc_lep_effCorrFactor = 0.968
# mc_tau_effCorrFactor = 0.92
# mc_addtl = 0.786
# mc_effCorrFactor = mc_lep_effCorrFactor * mc_tau_effCorrFactor 
mc_effCorrFactor = 1.

blah = 'CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT'
data_triggers_11A = [
    # the first 2 triggers correspond to runs before the good run list
    # why do we get them??
    # 'HLT_Ele15_{blah}_LooseIsoPFTau15_v1'.format(blah=blah), 
    # 'HLT_Ele15_{blah}_LooseIsoPFTau15_v2'.format(blah=blah),  
    'HLT_Ele15_{blah}_LooseIsoPFTau15_v4'.format(blah=blah),  
    'HLT_Ele15_{blah}_LooseIsoPFTau20_v6'.format(blah=blah),  
    'HLT_Ele15_{blah}_LooseIsoPFTau20_v8'.format(blah=blah),  
    'HLT_Ele15_{blah}_LooseIsoPFTau20_v9'.format(blah=blah),  
    'HLT_Ele15_{blah}_TightIsoPFTau20_v2'.format(blah=blah),
    'HLT_Ele18_{blah}_MediumIsoPFTau20_v1'.format(blah=blah)
    ]
data_triggers_11B = [
    'HLT_Ele18_{blah}_MediumIsoPFTau20_v1'.format(blah=blah), 
    'HLT_Ele20_{blah}_MediumIsoPFTau20_v5'.format(blah=blah), 
    'HLT_Ele20_{blah}_MediumIsoPFTau20_v6'.format(blah=blah)
    ]


triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

tauEleAna = cfg.Analyzer(
    'TauEleAnalyzer',
    pt1 = 20,
    pt2 = 20,
    iso1 = 999,
    iso2 = 0.1,
    eta1 = 999,
    eta2 = 2.1,
    # cutString2 = 'cuts_vbtf80ID',
    # eta2 = 1.4,
    m_min = 10,
    m_max = 99999,
    diLeptonCutString = 'cuts_baseline'
    )


tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    # effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False
    )

eleWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    # effWeightMC = mc_eleEffWeight_mc,
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
    leg1 = 'tau',
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
    files = getFiles('/TauPlusX/Run2011A-May10ReReco-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 168.597,
    triggers = data_triggers_11A,
    json = '{H2TauTauPackage}/json/finalTauPlusXMay.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )


data_Run2011A_PromptReco_v4 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v4',
    files = getFiles('/TauPlusX/Run2011A-PromptReco-v4/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 929.748,
    triggers = data_triggers_11A,
    json = '{H2TauTauPackage}/json/finalTauPlusXv4.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011A_05Aug2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_05Aug2011_v1',
    files = getFiles('/TauPlusX/Run2011A-05Aug2011-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 373.349,
    triggers = data_triggers_11A,
    json = '{H2TauTauPackage}/json/finalTauPlusXAug5.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011A_03Oct2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_03Oct2011_v1',
    files = getFiles('/TauPlusX/Run2011A-03Oct2011-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 658.886,
    triggers = data_triggers_11A,
    json = '{H2TauTauPackage}/json/finalTauPlusXOct3.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011B_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2011B_PromptReco_v1',
    files = getFiles('/TauPlusX/Run2011B-PromptReco-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
    intLumi = 2511.0,
    triggers = data_triggers_11B,
    json = '{H2TauTauPackage}/json/finalTauPlusX11B.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )


#########################################################################################


DYJets = None
WJets = None
TTJets = None

if aod=='V3':
    DYJets = cfg.MCComponent(
        name = 'DYJets',
        files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
        xSection = 3048.,
        nGenEvents = 36209629,
        #     nGenEvents = 28086242,    
        triggers = mc_triggers_fall11,
        effCorrFactor = mc_effCorrFactor)


    WJets = cfg.MCComponent(
        name = 'WJets',
        # files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}/Merge_TauMu'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
        files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/TAUTAU/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
        xSection = 31314.,
        nGenEvents = 81345381, # this number is probably slightly overestimated (3/1000 missing files, not taken into account)
        triggers = mc_triggers_fall11,
        effCorrFactor = mc_effCorrFactor )


    TTJets = cfg.MCComponent(
        name = 'TTJets',
        files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern),
        xSection = 165.8,
        nGenEvents = 3701947,
        triggers = mc_triggers_fall11,
        effCorrFactor = mc_effCorrFactor )

if aod == 'V2':
    DYJets = cfg.MCComponent(
        name = 'DYJets',
        files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/AntiEle', 'cbern', filePattern),
        xSection = 3048.,
        nGenEvents = 34915945,
        triggers = mc_triggers,
        effCorrFactor = mc_effCorrFactor )

    WJets = cfg.MCComponent(
        name = 'WJets',
        files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/AntiEle', 'cbern', filePattern),
        # files ='{baseDir}/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/AntiEle/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
        xSection = 31314.,
        nGenEvents = 53227112,
        triggers = mc_triggers,
        effCorrFactor = mc_effCorrFactor )


    TTJets = cfg.MCComponent(
        name = 'TTJets',
        files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/AntiEle', 'cbern', filePattern),
        # files ='{baseDir}/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/AntiEle/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
        xSection = 165.8,
        nGenEvents = 3542770,
        triggers = mc_triggers,
        effCorrFactor = mc_effCorrFactor )


embed_Run2011A_May10ReReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_May10ReReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2', 'cbern', filePattern),
    # files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    )

embed_Run2011A_PromptReco_v4 = cfg.EmbedComponent(
    name = 'embed_Run2011A_PromptReco_v4',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2', 'cbern', filePattern),
    # files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    ) 

embed_Run2011A_05Aug2011_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_05Aug2011_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2', 'cbern', filePattern),
    # files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    ) 

embed_Run2011A_03Oct2011_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_03Oct2011_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2', 'cbern', filePattern),
    # files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    ) 

embed_Run2011B_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011B_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2', 'cbern', filePattern),
    # files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    ) 





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

embed_2011A = [
    embed_Run2011A_May10ReReco_v1,
    embed_Run2011A_PromptReco_v4,
    embed_Run2011A_05Aug2011_v1,
    embed_Run2011A_03Oct2011_v1,
    ]
embed_2011B = [
    embed_Run2011B_PromptReco_v1
    ]


selectedComponents =  copy.copy(MC)
if period == 'Period_2011A':
    selectedComponents.extend( data_2011A )
    selectedComponents.extend( embed_2011A )    
elif period == 'Period_2011B':
    selectedComponents.extend( data_2011B )
    selectedComponents.extend( embed_2011B )    
elif period == 'Period_2011AB':
    selectedComponents.extend( data_2011A )
    selectedComponents.extend( data_2011B )
    selectedComponents.extend( embed_2011A )    
    selectedComponents.extend( embed_2011B )    

sequence = cfg.Sequence( [
    triggerAna,
    vertexAna,
    tauEleAna,
    tauWeighter, 
    eleWeighter, 
    vbfAna,
    eventSorter
   ] )

DYJets.fakes = True
TTJets.splitFactor = 2 
DYJets.splitFactor = 8
WJets.splitFactor = 3
data_Run2011B_PromptReco_v1.splitFactor = 8
data_Run2011A_PromptReco_v4.splitFactor = 2 

embed_Run2011A_May10ReReco_v1.splitFactor = 2
embed_Run2011A_PromptReco_v4.splitFactor = 4
embed_Run2011A_05Aug2011_v1.splitFactor = 2
embed_Run2011A_03Oct2011_v1.splitFactor = 2
embed_Run2011B_PromptReco_v1.splitFactor = 8

# selectedComponents = embed_2011A
# selectedComponents.extend( embed_2011B )

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
