import copy
import CMGTools.RootTools.fwlite.Config as cfg


period = 'Period_2011B'

fraction = 1

baseDir = '2011'
filePattern = 'tree*10*root'
fixedMuWeight = False

# mc_triggers = 'HLT_IsoMu12_v1'
mc_triggers = []

# cut values 
## cuts = cfg.Cuts()
## cuts.minMass = 75
## cuts.maxMass = 105
## cuts.muPt = 20
## cuts.muIso = 0.1
## cuts.muEta = 1.0

## cuts.jetPt = 30.
## cuts.jetEta = (-4.5, 4.5)

## cuts.VBF_Mjj = 400.
## cuts.VBF_Deta = 4.

## cuts.Boosted_JetPt = 150.

## cuts.MT_low = 40
## cuts.MT_high = 60

ZMuMuAna = cfg.Analyzer(
    'ZMuMuAnalyzer',
    pt1 = 20,
    pt2 = 20,
    iso1 = 0.1,
    iso2 = 0.1,
    eta1 = 2,
    eta2 = 2,
    m_min = 0,
    m_max = 200
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    verbose = False
    )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetPt = 30,
    jetEta = 4.5
    )

# MC smearing / scaling

mc_jet_scale = 1.
mc_jet_smear = 0.

# global MC weighting factors
# mc_lep_effCorrFactor = 0.968
# mc_tau_effCorrFactor = 0.92
# mc_addtl = 0.786
# mc_effCorrFactor = mc_lep_effCorrFactor * mc_tau_effCorrFactor 
mc_effCorrFactor = 1.

# the following depends on the period... 
# event MC weighting factors


mc_vertexWeight = 'vertexWeight2011B'
mc_tauEffWeight = None
mc_muEffWeight = None

# vertexWeight2011AB
# vertexWeight05AugReReco
# vertexWeight2invfb  For v4 + v6 + may10 + aug5
# vertexWeightEPSJul8
# vertexWeightLeptonPhoton
# vertexWeightMay10ReReco
# vertexWeightPromptRecov4
# vertexWeightPromptRecov6


# here, the place to hack around
if fixedMuWeight:
    mc_muEffWeight = None
    mc_effCorrFactor = mc_lep_effCorrFactor
  

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files ='{baseDir}/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0/ZMUMU_Dec14/tree*.root'.format(baseDir=baseDir),
    # files = 'tree_dimu_CMG.root',
    xSection = 3048.,
    nGenEvents = 34915945,
    triggers = mc_triggers,
    vertexWeight = mc_vertexWeight,
    # tauEffWeight = mc_tauEffWeight,
    # muEffWeight = mc_muEffWeight,    
    effCorrFactor = mc_effCorrFactor,
    # fraction = fraction
    )


WJets = cfg.MCComponent(
    name = 'WJets',
    files ='{baseDir}/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0/ZMUMU_Dec14/tree*.root'.format(baseDir=baseDir),
    xSection = 31314.,
    nGenEvents = 53227112,
    triggers = mc_triggers,
    vertexWeight = mc_vertexWeight,
    # tauEffWeight = mc_tauEffWeight,
    # muEffWeight = mc_muEffWeight,    
    effCorrFactor = mc_effCorrFactor )

TTJets = cfg.MCComponent(
    name = 'TTJets',
    files ='{baseDir}/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0/ZMUMU_Dec14/tree*.root'.format(baseDir=baseDir),
    xSection = 165.8,
    nGenEvents = 3542770,
    triggers = mc_triggers,
    vertexWeight = mc_vertexWeight,
    # tauEffWeight = mc_tauEffWeight,
    # muEffWeight = mc_muEffWeight,    
    effCorrFactor = mc_effCorrFactor )


data_2011B = cfg.DataComponent(
    name = 'data_2011B',
    files ='{baseDir}/DoubleMu/Run2011B-ZMu-PromptSkim-v1/RAW-RECO/PAT_CMG_V2_4_0/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 2511.0,
    triggers = [],
    # fraction = fraction
    )



MC = [DYJets, WJets, TTJets]
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear


selectedComponents =  list( MC )
selectedComponents.append( data_2011B )

# selectedComponents = [ data_2011B ]
selectedComponents = [ DYJets ]

## config = cfg.cfg( components = selectedComponents,
##                      cuts = cuts )


sequence = cfg.Sequence( [
    vertexAna,
    ZMuMuAna,
    vbfAna
    ] )

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
