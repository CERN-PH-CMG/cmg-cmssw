import copy
import CMGTools.H2TauTau.proto.framework.Config as cfg


period = 'Period_2011A'

baseDir = '2011'
filePattern = '*fullsel*.root'
fixedMuWeight = False

mc_triggers = 'HLT_IsoMu12_v1'

mc_jet_scale = 1.
mc_jet_smear = 0.

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


# global MC weighting factors
mc_lep_effCorrFactor = 0.968
mc_tau_effCorrFactor = 0.92
# mc_addtl = 0.786
# mc_effCorrFactor = mc_lep_effCorrFactor * mc_tau_effCorrFactor 
mc_effCorrFactor = 1.


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

TauMuAna = cfg.Analyzer(
    'TauMuAnalyzer',
    pt1 = 20,
    pt2 = 17,
    iso1 = 999,
    iso2 = 999,
    eta1 = 999,
    eta2 = 999,
    m_min = 10,
    m_max = 99999,
    diLeptonCutString = 'cuts_baseline'
    )

muonWeighter = cfg.Analyzer(
    'LeptonWeighter_mu',
    effWeight = mc_muEffWeight,
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
    verbose = False
    )

vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetPt = 30,
    jetEta = 4.5,
    **vbfKwargs
    # Mjj = 400,
    # deltaEta = 4.0
    )

eventSorter = cfg.Analyzer(
    'H2TauTauEventSorter',
    vertexWeight = mc_vertexWeight,
    MT_low = 40,
    MT_high = 60,
    Boosted_JetPt = 150,
    **vbfKwargs
    )




data_Run2011A_PromptReco_v4 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v4',
    files ='{baseDir}/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_4_1/H2TAUTAU_Jan23/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 929.748,
    triggers = ['HLT_IsoMu15_LooseIsoPFTau15_v[2,4,5,6]'],
    )

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files ='{baseDir}/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_1/H2TAUTAU_Jan23/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    xSection = 3048.,
    nGenEvents = 34915945,
    triggers = mc_triggers,
    vertexWeight = mc_vertexWeight,
    tauEffWeight = mc_tauEffWeight,
    muEffWeight = mc_muEffWeight,    
    effCorrFactor = mc_effCorrFactor )


MC = [DYJets]
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear

data = [data_Run2011A_PromptReco_v4]

selectedComponents =  MC
# selectedComponents.extend( data )


sequence = cfg.Sequence( [
   vertexAna,
   TauMuAna,
   muonWeighter, 
   tauWeighter, 
#    vbfAna,
#    eventSorter
    ] )

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
