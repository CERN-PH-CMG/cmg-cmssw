import copy
import CMGTools.RootTools.fwlite.Config as cfg


period = 'Period_2011B'

baseDir = '2011'
filePattern = 'tauEle*fullsel*.root'
fixedMuWeight = False

# mc_triggers = 'HLT_IsoMu12_v1'
mc_triggers = []

mc_jet_scale = 1.
mc_jet_smear = 0.

mc_vertexWeight = None
mc_tauEffWeight = None
mc_eleEffWeight = None

# For Fall11 need to use vertexWeightFall11 for WJets and DYJets and TTJets 
# For Fall11 : trigger is applied in MC:
#   "HLT_IsoMu15_LooseIsoPFTau15_v9"

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

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

TauEleAna = cfg.Analyzer(
    'TauEleAnalyzer',
    pt1 = 20,
    pt2 = 20,
    iso1 = 999,
    iso2 = 0.1,
    eta1 = 999,
    # eta2 = 2.1,
    eta2 = 1.4,
    m_min = 10,
    m_max = 99999,
    diLeptonCutString = 'cuts_baseline'
    )

electronWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    lepton = 'leg2',
    verbose = False
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    lepton = 'leg1',
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
    # vertexWeight = mc_vertexWeight,
    MT_low = 40,
    MT_high = 60,
    Boosted_JetPt = 150,
    **vbfKwargs
    )

#########################################################################################

blah='CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT'
data_Run2011A_May10ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011A_May10ReReco_v1',
    files ='{baseDir}/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 168.597,
    #TODO: quite a low efficiency...
    triggers = ['HLT_Ele15_{blah}_LooseIsoPFTau15_v4'.format(blah=blah)] )


data_Run2011A_PromptReco_v4 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v4',
    files ='{baseDir}/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 929.748,
    triggers = ['HLT_Ele15_{blah}_LooseIsoPFTau20_v[6,7,8,9]'.format(blah=blah)],
    )

data_Run2011A_05Aug2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_05Aug2011_v1',
    files ='{baseDir}/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 373.349,
    triggers = ['HLT_Ele15_{blah}_TightIsoPFTau20_v2'.format(blah=blah)] )

data_Run2011A_PromptReco_v6 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v6',
    files ='{baseDir}/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 658.886,
    triggers = ['HLT_Ele15_{blah}_TightIsoPFTau20_v2'.format(blah=blah),
                'HLT_Ele18_{blah}_MediumIsoPFTau20_v1'.format(blah=blah) ] )

data_Run2011A_03Oct2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_03Oct2011_v1',
    files ='{baseDir}/TauPlusX/Run2011A-03Oct2011-v1/AOD/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = data_Run2011A_PromptReco_v6.intLumi,
    triggers = data_Run2011A_PromptReco_v6.triggers )

data_Run2011B_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2011B_PromptReco_v1',
    files ='{baseDir}/TauPlusX/Run2011B-PromptReco-v1/AOD/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 2511.0,
    triggers = [ 'HLT_Ele18_{blah}_MediumIsoPFTau20_v1'.format(blah=blah),
                 'HLT_Ele20_{blah}_MediumIsoPFTau20_v5'.format(blah=blah),
                 'HLT_Ele20_{blah}_MediumIsoPFTau20_v6'.format(blah=blah) ])


#########################################################################################

embed_Run2011A_May10ReReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_May10ReReco_v1',
    files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011A_May10thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    # tauEffWeight = mc_tauEffWeight,
    # eleEffWeight = mc_eleEffWeight    
    )

embed_Run2011A_PromptReco_v4 = cfg.EmbedComponent(
    name = 'embed_Run2011A_PromptReco_v4',
    files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    # tauEffWeight = mc_tauEffWeight,
    # eleEffWeight = mc_eleEffWeight    
    ) 

embed_Run2011A_05Aug2011_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_05Aug2011_v1',
    files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011A_Aug05thRR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    # tauEffWeight = mc_tauEffWeight,
    # eleEffWeight = mc_eleEffWeight    
    ) 

embed_Run2011A_03Oct2011_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_03Oct2011_v1',
    files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    # tauEffWeight = mc_tauEffWeight,
    # eleEffWeight = mc_eleEffWeight    
    ) 

embed_Run2011B_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011B_PromptReco_v1',
    files = '{baseDir}/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    triggers = mc_triggers,
    # tauEffWeight = mc_tauEffWeight,
    # eleEffWeight = mc_eleEffWeight    
    ) 

#########################################################################################


DYJets = cfg.MCComponent(
    name = 'DYJets',
    files ='{baseDir}/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/AntiEle/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    xSection = 3048.,
    nGenEvents = 34915945,
    triggers = mc_triggers,
    # vertexWeight = mc_vertexWeight,
    # tauEffWeight = mc_tauEffWeight,
    # eleEffWeight = mc_eleEffWeight,    
    effCorrFactor = mc_effCorrFactor )

WJets = cfg.MCComponent(
    name = 'WJets',
    files ='{baseDir}/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/AntiEle/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    xSection = 31314.,
    nGenEvents = 53227112,
    triggers = mc_triggers,
    # vertexWeight = mc_vertexWeight,
    # tauEffWeight = mc_tauEffWeight,
    # eleEffWeight = mc_eleEffWeight,    
    effCorrFactor = mc_effCorrFactor )


TTJets = cfg.MCComponent(
    name = 'TTJets',
    files ='{baseDir}/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/AntiEle/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    xSection = 165.8,
    nGenEvents = 3542770,
    triggers = mc_triggers,
    # vertexWeight = mc_vertexWeight,
    # tauEffWeight = mc_tauEffWeight,
    # eleEffWeight = mc_eleEffWeight,    
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
embed_2011A = [
    embed_Run2011A_May10ReReco_v1,
    embed_Run2011A_PromptReco_v4,
    embed_Run2011A_05Aug2011_v1,
    embed_Run2011A_03Oct2011_v1,
    ]
data_2011B = [
    data_Run2011B_PromptReco_v1
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


# selectedComponents = MC

# selectedComponents = data_2011A
# selectedComponents.extend(data_2011B)
# selectedComponents  = [ data_Run2011B_PromptReco_v1  ]
# selectedComponents = [ DYJets ]

# selectedComponents = [embed_Run2011A_May10ReReco_v1]

sequence = cfg.Sequence( [
    triggerAna,
    vertexAna,
    TauEleAna,
    electronWeighter, 
    tauWeighter, 
    vbfAna,
    eventSorter
   ] )

# sequence = sequence[:1]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
