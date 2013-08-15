import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg

from CMGTools.HToZZTo4Leptons.setup.EffectiveAreas import effectiveAreas2012 as effectiveAreas
from CMGTools.HToZZTo4Leptons.setup.FSR import FSRConfig as fsr


#Trigger Information
triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

#Vertex information
vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1,
    verbose = False
    )


#Pu reweighting
puAna = cfg.Analyzer(
    "PileUpAnalyzer",
    # build unweighted pu distribution using number of pile up interactions if False
    # otherwise, use fill the distribution using number of true interactions
    true = True
    )

fakeAna = cfg.Analyzer(
    'FakeRateAnalyzer',
    minPt1=3,
    maxEta1=2.4,
    minPt2=7,
    maxEta2=2.5,
    z1_m = (40.,120.),
    z1_pt1 = 20,
    z1_pt2 = 10,
    minMass=4.,
    keep=False,
    effectiveAreas=effectiveAreas,
    rhoMuon     = 'kt6PFJetsCentralNeutral',
    rhoElectron = 'kt6PFJets',
    FSR=fsr
    )



fakeTree = cfg.Analyzer('FakeRateTreeProducer',
                       effectiveAreas=effectiveAreas
)



####################################################################################

from CMGTools.HToZZTo4Leptons.samples.samples_8TeV import * 

####################################################################################

jsonFilter = cfg.Analyzer(
    'JSONAnalyzer'
    )

    



for data in dataSamplesE:
    data.triggers = triggers_ee
for data in dataSamplesMu:
    data.triggers = triggers_mumu
    data.vetoTriggers = triggers_ee

for data in dataSamplesMuE:
    data.triggers = triggers_mue
    data.vetoTriggers=triggers_ee+triggers_mumu

selectedComponents=dataSamplesMu+dataSamplesE+dataSamplesMuE




dataSequence=[
    jsonFilter,
    puAna,
    triggerAna,
    vertexAna,
    fakeAna,
    fakeTree
    ]



#for full production use data sequence
sequence = cfg.Sequence(dataSequence)





test = 0
if test==1:
    dataset = DoubleMuAB
    selectedComponents = [dataset]
    dataset.splitFactor = 1



   
if test ==2:
    selectedComponents=selectedComponents[:1]
    print selectedComponents
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:10]

if test ==3:
    selectedComponents=selectedComponents[-1:]
    print selectedComponents
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:10]



    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
