import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg

# FIXME trigger emulation! 

period = 'Period_2011AB'
channel = 'mu_mu'

mc_vertexWeight = None
if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeightFall112invfb'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeightFall112011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeightFall112011AB'

muMuGenSel = cfg.Analyzer(
    'HZZGenSelector_MuMu',
    col_label = 'genParticlesPruned',
    channel = 'mu-mu'
    )

muMuZSkim = cfg.Analyzer(
    'ZSkim_MuMu',
    col_label = 'cmgMuonSelStdLep',
    col_type = 'std::vector< cmg::Muon >',    
    )

eleEleZSkim = cfg.Analyzer(
    'ZSkim_EleEle',
    col_label = 'cmgElectronSelStdLep',
    col_type = 'std::vector< cmg::Electron >',
    )

muEleZSkim = cfg.Analyzer(
    'ZSkim_MuEle',
    col_label = 'cmgMuonSelStdLep',
    col_type = 'std::vector< cmg::Muon >',
    col2_label = 'cmgElectronSelStdLep',
    col2_type = 'std::vector< cmg::Electron >',
    )

skimmer = cfg.Analyzer(
    'SkimOperator',
    skims = [ 'ZSkim_MuMu', 'ZSkim_EleEle', 'ZSkim_MuEle' ]
    )

skimSequence = [
    muMuZSkim,
    eleEleZSkim,
    skimmer
    ]


triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )


vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    vertexWeight = mc_vertexWeight,
    verbose = False
    )

looseIso = 0.7
# looseIso = 999
pairIso = 0.35
# pairIso = 999

muMuAna = cfg.Analyzer(
    'MuMuFourLeptonAnalyzer',
    pt1 = 5.,
    eta1 = 2.4,
    iso1 = looseIso,
    sip1 = 4,
    # 
    pt2 = 5,
    eta2 = 2.4,
    iso2 = looseIso,
    sip2 = 4, 
    #
    z1_m = (50,120),
    z1_pt1 = 20,
    z1_pt2 = 10,
    #
    z2_m = 12,
    z2_pt1 = 0.,
    z2_pt2 = 0.,
    # 
    h_m = 100.,
    h_sip = 4.,
    pair_iso = pairIso,
    pair_mass = 12.,
    nLeptonsMin = 0.,
    #
    keep = True,
    #
    PF = True,
    PFIso = True
    )


muEleAna = copy.deepcopy( muMuAna)
muEleAna.name = 'MuEleFourLeptonAnalyzer'
muEleAna.pt2 = 7,
muEleAna.eta2 = 2.5,
muEleAna.iso2 = looseIso,
# muEleAna.sip2 = 4,     

eleEleAna = copy.deepcopy( muEleAna )
eleEleAna.name = 'EleEleFourLeptonAnalyzer'
eleEleAna.pt1 = 7,
eleEleAna.eta1 = 2.5,
eleEleAna.iso1 = looseIso,
# eleEleAna.sip1 = 4,     


genMuMuAna = copy.deepcopy(muMuAna)
genMuMuAna.name = 'GenGenFourLeptonAnalyzer_MuMu'
genMuMuAna.pdgId1 = 13
genMuMuAna.pdgId2 = 13
genMuMuAna.pt1 = 0
genMuMuAna.pt2 = 0
genMuMuAna.eta1 = 999
genMuMuAna.eta2 = 999
genMuMuAna.z1_m = (0, 999)
genMuMuAna.z1_pt1 = 0
genMuMuAna.z1_pt2 = 0


genMuEleAna = copy.deepcopy(muEleAna)
genMuEleAna.pdgId1 = 13
genMuEleAna.pdgId2 = 11

genEleEleAna = copy.deepcopy(eleEleAna)
genEleEleAna.pdgId1 = 11
genEleEleAna.pdgId2 = 11


def createTreeProducer( ana ):
    tp = cfg.Analyzer( '_'.join( ['FourLeptonTreeProducer',ana.name] ),
                       anaName = ana.name
                       )
    return tp


####################################################################################

from CMGTools.HToZZTo4Leptons.samples.samples_V4_0_0 import * 

####################################################################################


jsonFilter = cfg.Analyzer(
    'JSONAnalyzer',
    json = json,
    )

# selectedComponents = Fall11
selectedComponents = copy.copy(mc_zz)
selectedComponents.append( Hig120GluGlu )
# selectedComponents.extend( Fall11 )

for comp in selectedComponents:
    comp.splitFactor = 10
DYJets.splitFactor = 300
for comp in data_DoubleMu_2011:
    comp.splitFactor = 100
    
theAna = None
if channel == 'mu_mu':
    theGenSel = muMuGenSel
    theAna = muMuAna
    for data in data_DoubleMu_2011:
        data.triggers = triggers_2011_mu_mu
    for mc in Fall11:
        mc.triggers = triggers_fall11_mu_mu
elif channel == 'mu_ele':
    theGenSel = muEleGenSel
    theAna = muEleAna
    for mc in Fall11:
        mc.triggers = triggers_fall11_mu_ele
elif channel == 'ele_ele':
    theGenSel = eleEleGenSel
    theAna = eleEleAna
    for mc in Fall11:
        mc.triggers = triggers_fall11_ele_ele

# for debugging 
stopper = cfg.Analyzer(
    'Stopper'
    )

sequence = cfg.Sequence([
    # theGenSel, # FIXME not for data
    #muMuZSkim,
    #eleEleZSkim,
    #muEleZSkim, 
    #skimmer,
    # jsonFilter, 
    # triggerAna,
    # vertexAna,
    muMuGenSel, 
    genMuMuAna,
    theAna,
    createTreeProducer( theAna ),
    createTreeProducer( genMuMuAna )
    ])


selectedComponents = [Hig120GluGlu]

test = False
if test:
    # triggerAna.verbose = 1
    # triggerAna.usePrescaled = True
    # dataset = data_DoubleMu_Run2011B_16Jan2012
    dataset = Hig120GluGlu
    # dataset = ZZTo4mu
    # Hig120GluGlu.files = getFiles('/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4/PAT_CMG_Test_444', 'botta', 'patTuple.*root')
    selectedComponents = [dataset]
    dataset.splitFactor = 1
    dataset.files = dataset.files[:10]
#    sequence = cfg.Sequence(
#        [
#        muMuGenSelector
#        ]
#        )
    


    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
