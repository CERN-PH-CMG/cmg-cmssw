
import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.H2TauTau.proto.weights.weighttable import mu_id_taumu_2012, mu_iso_taumu_2012
from CMGTools.H2TauTau.proto.samples.sampleShift import selectShift
from CMGTools.RootTools.RootTools import * 

# 'Nom', 'Up', 'Down', or None
shift = None
# 1.0, 1.03, 0.97
tauScaleShift = 1.0
syncntuple = True

puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'

# mine: 
# puFileMC = '/'.join([puFileDir, 'MyMCPileupHistogram_true.root'])
# puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_AB_start_196509.root'])

# andrew ICHEP
# puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/12-09-12/MC_Summer12_PU_S7.root'
# puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/12-09-12/Data_Pileup_2012.root'
# andrew HCP
puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/Data_Pileup_2012_HCP-600bins.root'


vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'
vertexFileData = '/'.join([vertexFileDir, 'vertices_data_2012A_2012B_start_195947.root'])

mc_vertexWeight = None

mc_tauEffWeight_mc = 'effTau2012MC53X'
mc_muEffWeight_mc = 'eff_2012_Rebecca_TauMu_IsoMu1753XMC'
mc_tauEffWeight = 'effTau2012ABC'
mc_muEffWeight = 'effMu2012_Rebecca_TauMu_ABC'
    
    
eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [
##    376085 ,
##    576766 ,
##    585849 ,
##    138712 ,
##    462372 ,
##    647749 ,
##    794711 ,
##    397814 ,
##    168577 ,
##    752021 ,
##    955981 ,
##    144732 ,
##    253983 ,
##    967335 ,
##    593390 ,
##    312587 ,
##     95728 ,
##     78807 ,
##    433140 ,
##    501604 ,
##    990370 ,
##    496868 ,
##     65293 ,
##    479335 ,
##     19791 ,
   # selected by me and not by Jose
   343405 ,
   900535 ,
    55498 ,
   600255 ,
   523634 ,
   755769 ,
   192915 ,
   569949 ,
   268302 ,
   840192 ,
   746810 ,
   776229 ,
   992574 ,
   387821 ,
   523969 ,
   878407 ,
   661467 ,
   145475 ,

    ]
    )


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'goodPVFilter',
    vertexWeight = mc_vertexWeight,
    fixedWeight = 1,
    verbose = False,
    )

embedWeighter = cfg.Analyzer(
    'EmbedWeighter',
    verbose = False
    )

pileUpAna = cfg.Analyzer(
    'PileUpAnalyzer',
    true = True
    )

genErsatzAna = cfg.Analyzer(
    'GenErsatzAnalyzer',
    verbose = False
    )

TauMuAna = cfg.Analyzer(
    'TauMuAnalyzer',
    scaleShift1 = tauScaleShift,
    pt1 = 20,
    eta1 = 2.3,
    iso1 = 999,
    pt2 = 20,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    triggerMap = pathsAndFilters,
    mvametsigs = 'mvaMETTauMu',
    verbose = False
    )

dyJetsFakeAna = cfg.Analyzer(
    'DYJetsFakeAnalyzer',
    leptonType = 13
    )

WNJetsAna = cfg.Analyzer(
    'WNJetsAnalyzer',
    verbose = False
    )

higgsWeighter = cfg.Analyzer(
    'HiggsPtWeighter',
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False,
    disable = False,
    )

muonWeighter = cfg.Analyzer(
    'LeptonWeighter_mu',
    effWeight = mc_muEffWeight,
    effWeightMC = mc_muEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    disable = False,
    idWeight = mu_id_taumu_2012,
    isoWeight = mu_iso_taumu_2012    
    )



# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_52X.weights.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 20,
    jetEta = 4.7,
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauMu'
    )

treeProducerXCheck = cfg.Analyzer(
    'H2TauTauSyncTree'    
    )

#########################################################################################


# from CMGTools.H2TauTau.proto.samples.run2012.tauMu_MuRm_ColinOct9 import * 
# from CMGTools.H2TauTau.proto.samples.run2012.tauMu_ColinOct10 import * 
from CMGTools.H2TauTau.proto.samples.run2012.tauMu_Sync_Colin import * 

#########################################################################################

for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData

for emb in embed_list:
    emb.puFileData = None
    emb.puFileMC = None


selectedComponents = allsamples
# selectedComponents = [data_Run2012A_aug6, data_Run2012A, data_Run2012C_v1, ]
# selectedComponents = [DYJets]

WJets.splitFactor = 50

sequence = cfg.Sequence( [
    # eventSelector,
    jsonAna,
    triggerAna,
    vertexAna,
    genErsatzAna,
    TauMuAna,
    dyJetsFakeAna,
    WNJetsAna,
    higgsWeighter, 
    vbfAna,
    pileUpAna,
    embedWeighter, 
    tauWeighter, 
    muonWeighter, 
    treeProducer
   ] )

if syncntuple:
    sequence.append( treeProducerXCheck)


test = 0
if test==1:
    comp = HiggsVBF125
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:10]
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:5]


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)

