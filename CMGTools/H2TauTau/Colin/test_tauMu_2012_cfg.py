import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.RootTools.RootTools import * 

period = 'Period_2012AB'
shift = True

puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'
puFileData = None
puFileMC = '/'.join([puFileDir, 'MyMCPileupHistogram_true.root'])
puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_AB_190456_196531.root'])

vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'
vertexFileData = '/'.join([vertexFileDir, 'vertices_data_2012A_2012B_start_195947.root'])

mc_vertexWeight = None
mc_tauEffWeight = None
mc_muEffWeight = None

mc_tauEffWeight_mc = 'effTau2012MC'
mc_muEffWeight_mc = 'effMu2012MC'
mc_tauEffWeight = 'effTau2012AB'
mc_muEffWeight = 'effMu2012AB'
    

## if period == 'Period_2012A':
##     puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_A.root'])
##     mc_tauEffWeight = 'effTau2012A'
##     mc_muEffWeight = 'effMu2012A'
## elif period == 'Period_2012B':
##     puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_B.root'])
##     mc_tauEffWeight = 'effTau2012B'
##     mc_muEffWeight = 'effMu2012B'
## elif period == 'Period_2012AB':


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    # fixme pick it up automatically
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    # goodVertices = 'offlinePrimaryVertices', # hum... collection not available in old tuples
    goodVertices = 'goodPVFilter',
    vertexWeight = mc_vertexWeight,
    fixedWeight = 1,
    verbose = False
    )

embedWeighter = cfg.Analyzer(
    'EmbedWeighter',
    verbose = False
    )

pileUpAna = cfg.Analyzer(
    'PileUpAnalyzer',
    true = True
    )


TauMuAna = cfg.Analyzer(
    'TauMuAnalyzer',
    pt1 = 20,
    eta1 = 2.3,
    iso1 = 999,
    pt2 = 18,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    triggerMap = pathsAndFilters
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False,
    disable = False,
    recEffVersion = None
    )

muonWeighter = cfg.Analyzer(
    'LeptonWeighter_mu',
    effWeight = mc_muEffWeight,
    effWeightMC = mc_muEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    disable = False,
    recEffVersion = None
    )



# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG.weights.44X.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 5.0,
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauMu'
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.run2012.tauMu_ColinMay30 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay18_CHS import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay18 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay15 import * 

#########################################################################################

MC_list = copy.copy(MC)
data_list = copy.copy(data_list_2012)
embed_list = copy.copy(embed_list_2012)

if shift: 
    MC_list = copy.copy(MC_up)
    MC_list.extend(MC_down)
    embed_list = copy.copy(embed_list_2012_up)
    embed_list.extend(embed_list_2012_down)
    data_list = []

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC_list:
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC
    mc.splitFactor = 10
    if mc.name.find('DYJets')!=-1:
        mc.splitFactor = 100
        mc.fakes = True
    elif mc.name.find('WJets')!=-1:
        mc.splitFactor = 20
    elif mc.name.find('TTJets')!=-1:
        mc.splitFactor = 80
    elif mc.name.find('HiggsVBF120')!=-1 or \
         mc.name.find('HiggsGGH120')!=-1:
        mc.splitFactor = 50
    elif mc.name.find('HiggsVBF150')!=-1 or \
         mc.name.find('HiggsGGH150')!=-1:
        mc.splitFactor = 30
for emb in embed_list:
    emb.puFileData = None
    emb.puFileMC = None
    emb.splitFactor = 10

data_Run2012A.splitFactor = 40
data_Run2012B_start_194479.splitFactor = 40
data_Run2012B_194480_195016.splitFactor = 40
data_Run2012B_195017_195947.splitFactor = 40 

selectedComponents =  copy.copy(MC_list)
selectedComponents.extend( data_list )
selectedComponents.extend( embed_list )
    

sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    TauMuAna,
    vbfAna,
    pileUpAna,
    embedWeighter, 
    tauWeighter, 
    muonWeighter, 
    treeProducer
   ] )


test = 0
if test==1:
    # comp = embed_Run2012B_195147_196070
    comp = data_Run2012A
    comp.files = comp.files[:10]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:5]


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

def printComps():
    nJobs = 0
    for c in config.components:
        print c.name, c.splitFactor, len(c.files)
        if len(c.files):
            print c.files[0]
            nJobs += c.splitFactor
    print '-'*70
    print '# components = ', len(config.components)
    print '# jobs       = ', nJobs
