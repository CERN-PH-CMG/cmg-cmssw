import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.HToZZTo4Leptons.analyzers.EffectiveAreas import effectiveAreas2011 

effectiveAreas = effectiveAreas2011()

print 'Running Cross check of the baseline'
print 'To get the results in the twiki make'
print 'sure you use the baseline analyzer in'
print 'FourLeptonAnalyzer.py'



period = 'Period_2011AB'
channel = 'mu_ele'

mc_vertexWeight = None
if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeightFall112invfb'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeightFall112011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeightFall112011AB'

###GEN LEVEL SELECTORS
muMuGenSel = cfg.Analyzer(
    'HZZGenSelector_MuMu',
    col_label = 'genParticlesPruned',
    channel = 'mu-mu'
    )
muEleGenSel = cfg.Analyzer(
    'HZZGenSelector_EMu',
    col_label = 'genParticlesPruned',
    channel = 'mu-ele'
    )

eleEleGenSel = cfg.Analyzer(
    'HZZGenSelector_EE',
    col_label = 'genParticlesPruned',
    channel = 'ele-ele'
    )

#SKIMMING
aFourSkim = cfg.Analyzer('A4Skim',
                         muons = 'cmgMuonSel',
                         electrons='cmgElectronSel'
)


skimmer = cfg.Analyzer(
    'SkimOperator',
    skims = [ 'A4Skim'],
    applySkim=True
    )

#Trigger Information
triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

#Vertex information
vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    vertexWeight = mc_vertexWeight,
    verbose = False
    )


#Analyzers
muMuAna = cfg.Analyzer(
    'MuMuFourLeptonAnalyzer',
    minPt1=5,
    maxEta1=2.4,
    minPt2=5,
    maxEta2=2.4,
    z1_m = (40.,120.),
    z2_m = (4.,120.),
    z1_pt1 = 20,
    z1_pt2 = 10,
    minMass=4.,
    minHMass=100.,
    PF=True,
    keep=False,
    effectiveAreas=effectiveAreas
    )


muEleAna = copy.deepcopy( muMuAna)
muEleAna.name = 'MuEleFourLeptonAnalyzer'
muEleAna.minPt1 = 5
muEleAna.maxEta1 = 2.4
muEleAna.minPt2 = 7
muEleAna.maxEta2 = 2.5


eleEleAna = copy.deepcopy( muEleAna )
eleEleAna.name = 'EleEleFourLeptonAnalyzer'
eleEleAna.minPt1 = 7
eleEleAna.maxEta1 = 2.5
eleEleAna.minPt2 = 7
eleEleAna.maxEta2 = 2.5


#GenLevelAnalyzers
muMuGenAna = copy.deepcopy(muMuAna)
muMuGenAna.name = 'GenGenFourLeptonAnalyzer_MuMu'
muMuGenAna.pdgId1 = 13
muMuGenAna.pdgId2 = 13

muEleGenAna = copy.deepcopy(muEleAna)
muEleGenAna.name = 'GenGenFourLeptonAnalyzer_MuEle'
muEleGenAna.pdgId1 = 13
muEleGenAna.pdgId2 = 11

eleEleGenAna = copy.deepcopy(eleEleAna)
eleEleGenAna.name = 'GenGenFourLeptonAnalyzer_EleEle'
eleEleGenAna.pdgId1 = 11
eleEleGenAna.pdgId2 = 11




def createTreeProducer( ana ):
    tp = cfg.Analyzer( '_'.join( ['FourLeptonTreeProducer',ana.name] ),
                       anaName = ana.name,
                       effectiveAreas = effectiveAreas
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
#selectedComponents = copy.copy(mc_zz)
#selectedComponents.append( Hig120GluGlu )
# selectedComponents.extend( Fall11 )

selectedComponents=[ZZTo4mu]

for comp in selectedComponents:
    comp.splitFactor = 10
DYJets.splitFactor = 300
for comp in data_DoubleMu_2011:
    comp.splitFactor = 100
    
theAna = None
if channel == 'mu_mu':
    theGenSel = muMuGenSel
    theAna = muMuAna
    theGenAna = muMuGenAna
    for data in data_DoubleMu_2011:
        data.triggers = triggers_2011_mu_mu
    for mc in Fall11:
        mc.triggers = triggers_fall11_mu_mu
elif channel == 'mu_ele':
    theGenSel = muEleGenSel
    theAna = muEleAna
    theGenAna = muEleGenAna

    for mc in Fall11:
        mc.triggers = triggers_fall11_mu_ele

elif channel == 'ele_ele':
    theGenSel = eleEleGenSel
    theAna = eleEleAna
    theGenAna = eleEleGenAna
    for mc in Fall11:
        mc.triggers = triggers_fall11_ele_ele

#Define Sequences for data and MC

mcSequence = [
#   theGenSel,    #uncomment to preselect events in gen level 
    aFourSkim,
    skimmer,
    triggerAna,
    vertexAna,
#    theGenAna,
    theAna,
    createTreeProducer( theAna ),
#   createTreeProducer( theGenAna )
    ]

dataSequence=[
    aFourSkim,
    skimmer,
    jsonFilter,
    triggerAna,
    vertexAna,
    theAna,
    createTreeProducer( theAna )
    ]




sequence = cfg.Sequence(mcSequence)


test = True
if test:
    dataset = ZZTo2e2mu
    selectedComponents = [dataset]
    dataset.splitFactor = 1
    dataset.files = dataset.files[:10]
    dataset.files = ['root://lxcms00//data3/HZZ_Pattuple/CMG/V5_1_0/cmgTuple.root']
    


    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
