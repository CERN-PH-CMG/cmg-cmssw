import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg

# FIXME trigger emulation! 

period = 'Period_2012'
channel = 'mu_mu'

mc_vertexWeight = 1

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

# for 2012 analysis (need to get more MC samples!)
from CMGTools.H2TauTau.proto.samples.run2012.zmumu_13Apr import * 
    

####################################################################################


# jsonFilter = cfg.Analyzer(
#    'JSONAnalyzer',
#    json = json,
#    )

selectedComponents = copy.copy(MC)
selectedComponents.extend(data)

for comp in selectedComponents:
    comp.splitFactor = 10
 
theAna = None
if channel == 'mu_mu':
    theGenSel = muMuGenSel
    theAna = muMuAna
elif channel == 'mu_ele':
    pass
elif channel == 'ele_ele':
    pass
    
# for debugging 
stopper = cfg.Analyzer(
    'Stopper'
    )

sequence = cfg.Sequence([
    theAna,
    createTreeProducer( theAna ),
    ])


test = True
if test:
    dataset = DYJets
    selectedComponents = [dataset]
    dataset.splitFactor = 1
    dataset.files = dataset.files[:10]


    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
