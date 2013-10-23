import copy
import CMGTools.H2TauTau.macros.Config as htt


period = 'Period_2011A'


baseDir = '2011'
filePattern = '*fullsel*root'
fixedMuWeight = False

# mc_triggers = 'HLT_IsoMu12_v1'
mc_triggers = []

# cut values 
cuts = htt.Cuts()
cuts.tauPt = 20.
cuts.lepPt = 17. 
cuts.lepEta = float('+inf')

cuts.jetPt = 30.
cuts.jetEta = (-4.5, 4.5)

cuts.VBF_Mjj = 400.
cuts.VBF_Deta = 4.

cuts.Boosted_JetPt = 150.

cuts.MT_low = 40
cuts.MT_high = 60


# global MC weighting factors
mc_lep_effCorrFactor = 0.968
mc_tau_effCorrFactor = 0.92
# mc_addtl = 0.786
# mc_effCorrFactor = mc_lep_effCorrFactor * mc_tau_effCorrFactor 
mc_effCorrFactor = 1.

# the following depends on the period... 
# event MC weighting factors

mc_vertexWeight = None
mc_tauEffWeight = None
mc_muEffWeight = None

if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeight2invfb'
    mc_tauEffWeight = 'effTau2011A'
    mc_muEffWeight = 'effMu2011A'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeight2011B'
    mc_tauEffWeight = 'effTau2011B'
    mc_muEffWeight = 'effMu2011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeight2011AB'
    mc_tauEffWeight = 'effTau2011AB'
    mc_muEffWeight = 'effMu2011AB'

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
  

DYJets = htt.MCComponent(
    name = 'DYJets',
    files ='{baseDir}/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    xSection = 3048.,
    nGenEvents = 34915945,
    triggers = mc_triggers,
    vertexWeight = mc_vertexWeight,
    tauEffWeight = mc_tauEffWeight,
    muEffWeight = mc_muEffWeight,    
    effCorrFactor = mc_effCorrFactor )


WJets = htt.MCComponent(
    name = 'WJets',
    files ='{baseDir}/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    xSection = 31314.,
    nGenEvents = 53227112,
    triggers = mc_triggers,
    vertexWeight = mc_vertexWeight,
    tauEffWeight = mc_tauEffWeight,
    muEffWeight = mc_muEffWeight,    
    effCorrFactor = mc_effCorrFactor )

TTJets = htt.MCComponent(
    name = 'TTJets',
    files ='{baseDir}/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    xSection = 165.8,
    nGenEvents = 3542770,
    triggers = mc_triggers,
    vertexWeight = mc_vertexWeight,
    tauEffWeight = mc_tauEffWeight,
    muEffWeight = mc_muEffWeight,    
    effCorrFactor = mc_effCorrFactor )


dMay10ReReco_v1 = htt.DataComponent(
    name = 'dMay10ReReco_v1',
    files ='{baseDir}/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 168.597,
    triggers = ['HLT_IsoMu12_LooseIsoPFTau10_v4'] )

dPromptReco_v4 = htt.DataComponent(
    name = 'dPromptReco_v4',
    files ='{baseDir}/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 929.748,
    triggers = ['HLT_IsoMu15_LooseIsoPFTau15_v[2,4,5,6]'] )

d05Aug2011_v1 = htt.DataComponent(
    name = 'd05Aug2011_v1',
    files ='{baseDir}/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 373.349,
    triggers = ['HLT_IsoMu15_LooseIsoPFTau15_v8'] )

dPromptReco_v6 = htt.DataComponent(
    name = 'dPromptReco_v6',
    files ='{baseDir}/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 658.886,
    triggers = ['HLT_IsoMu15_LooseIsoPFTau15_v[8,9]'] )

d03Oct2011 = htt.DataComponent(
    name = 'd03Oct2011',
    files ='{baseDir}/TauPlusX/Run2011A-03Oct2011-v1/AOD/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 658.886,
    triggers = ['HLT_IsoMu15_LooseIsoPFTau15_v[8,9]'] )

d2011B = htt.DataComponent(
    name = 'd2011B',
    files ='{baseDir}/TauPlusX/Run2011B-PromptReco-v1/AOD/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov29/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 2511.0,
    triggers = ['HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v[1,5,6]',
                'HLT_IsoMu15_LooseIsoPFTau15_v[9,10,11,12,13]'] )


MC = [DYJets, WJets, TTJets]
data_2011A = [dMay10ReReco_v1,dPromptReco_v4, d05Aug2011_v1, d03Oct2011]
data_2011B = [d2011B]
selectedComponents = list( MC )
if period == 'Period_2011A':
    selectedComponents.extend( data_2011A )
elif period == 'Period_2011B':
    selectedComponents.extend( data_2011B )
elif period == 'Period_2011AB':
    selectedComponents.extend( data_2011A )
    selectedComponents.extend( data_2011B )


# selectedComponents = [WJets]

config = htt.Config( components = selectedComponents,
                     cuts = cuts )


