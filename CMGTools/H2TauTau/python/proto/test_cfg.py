import copy
import CMGTools.H2TauTau.proto.framework.Config as cfg



baseDir = '2011'
filePattern = '*fullsel*.root'
fixedMuWeight = False

mc_triggers = 'HLT_IsoMu12_v1'

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

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    verbose = False
    )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetPt = 30,
    jetEta = 4.5
    )


data_Run2011A_PromptReco_v4 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v4',
    files ='{baseDir}/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_4_1/H2TAUTAU_Jan23/{filePattern}'.format(baseDir=baseDir, filePattern=filePattern),
    intLumi = 929.748,
    triggers = ['HLT_IsoMu15_LooseIsoPFTau15_v[2,4,5,6]'],
    )


selectedComponents =  [ data_Run2011A_PromptReco_v4 ]

sequence = cfg.Sequence( [
    vertexAna,
    TauMuAna,
    vbfAna, 
    ] )

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
