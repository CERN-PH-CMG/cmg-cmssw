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

pat = 'PAT_CMG_TestMVAs'
htt = 'H2TAUTAU_TestMVAs_Mar12'
H2TauTauPackage = '/'.join( [ os.environ['CMSSW_BASE'],
                              'src/CMGTools/H2TauTau' ] ) 
filePattern = 'diTau.*fullsel.*root'
fixedMuWeight = False

mc_triggers = []
mc_triggers_fall11 = []

mc_jet_scale = 1.
mc_jet_smear = 0.

# the code below is using the tau trigger weights
# from the muon channel... beware! 
mc_vertexWeight = None
mc_tauEffWeight = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeightFall112invfb'
    mc_tauEffWeight = 'effTau2011A'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeightFall112011B'
    mc_tauEffWeight = 'effTau2011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeightFall112011AB'
    mc_tauEffWeight = 'effTau2011AB'


triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )


TauTauAna = cfg.Analyzer(
    'TauTauAnalyzer',
    pt1 = 20,
    pt2 = 20,
    iso1 = 999,
    iso2 = 999,
    eta1 = 999,
    eta2 = 999,
    m_min = 10,
    m_max = 99999,
    diLeptonCutString = 'cuts_baseline',
    triggerMap = pathsAndFilters
    )



tauWeighter1 = cfg.Analyzer(
    'LeptonWeighter_tau1',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False
    )

tauWeighter2 = cfg.Analyzer(
    'LeptonWeighter_tau2',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
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
    # vertexWeight = mc_vertexWeight,
    leg1 = 'tau_1',
    leg2 = 'tau_2',
    MT_low = 40,
    MT_high = 60,
    Boosted_JetPt = 150,
    **vbfKwargs
    )


#########################################################################################

DYJets = cfg.MCComponent(
    name = 'DYJets',
    files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    xSection = 3048.,
    nGenEvents = 36209629,
    #     nGenEvents = 28086242,    
    triggers = mc_triggers_fall11,
    effCorrFactor = 1)


WJets = cfg.MCComponent(
    name = 'WJets',
    # files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}/{htt}/Merge_TauMu'.format(pat=pat, htt=htt), 'cbern', filePattern),
    files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/PAT_CMG_TestMVAs_f0/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    xSection = 31314.,
    nGenEvents = 16257108, # this number is probably slightly overestimated (3/1000 missing files, not taken into account)
    triggers = mc_triggers_fall11,
    effCorrFactor = 1 )


TTJets = cfg.MCComponent(
    name = 'TTJets',
    files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern),
    xSection = 165.8,
    nGenEvents = 3701947,
    triggers = mc_triggers_fall11,
    effCorrFactor = 1 )



#########################################################################################


MC = [DYJets, WJets, TTJets]
# MC.extend( Higgs )

for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear


selectedComponents = MC

sequence = cfg.Sequence( [
    triggerAna,
    TauTauAna,
    vbfAna,
    vertexAna,
    tauWeighter1, 
    tauWeighter2, 
    eventSorter
   ] )


# sequence = sequence[:1]

DYJets.fakes = True
DYJets.splitFactor = 20
WJets.splitFactor = 2

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
        comp.files = comp.files[:1]

    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

