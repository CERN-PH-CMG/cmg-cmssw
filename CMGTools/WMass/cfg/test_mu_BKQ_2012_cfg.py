import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap8TeV import pathsAndFilters

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
     keepFailingEvents = False        
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1.,
     keepFailingEvents = True        
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True,
    triggerMap = pathsAndFilters
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

ZAna = cfg.Analyzer(
    'ZAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True,
    triggerMap = pathsAndFilters
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer'
    )

genAna = cfg.Analyzer(
    'GenParticleAnalyzerFSR',
    src = 'genParticlesPruned'
    )

sequence = cfg.Sequence( [
    genAna,
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )


from CMGTools.H2TauTau.proto.samples.getFiles import getFiles



TT_bar = cfg.MCComponent(
    name = 'TT_bar_l',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

TT_barv2 = cfg.MCComponent(
    name = 'TT_barv_ll',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)
WW = cfg.MCComponent(
    name = 'WW',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WZ = cfg.MCComponent(
    name = 'WZ',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


TTpow  = cfg.MCComponent(
    name = 'TTpow',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

TTpowv2  = cfg.MCComponent(
    name = 'TTpowv2',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

TWpow = cfg.MCComponent(
    name = 'TWpow',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)



TbarWpow = cfg.MCComponent(
    name = 'TbarWpow',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

Wjets= cfg.MCComponent(
    name = 'Wjets',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

Wjetsv2= cfg.MCComponent(
    name = 'Wjetsv2',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)


Wjets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')
Wjets.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]
 

Wjetsv2.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')
Wjetsv2.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]




TTpow.files = getFiles('/TT_CT10_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')

TTpow.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]

TTpowv2.files = getFiles('/TT_CT10_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')
                 
TTpowv2.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]


TWpow.files = getFiles('/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0', 'cmgtools', '.*root')
TWpow.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]
TbarWpow.files = getFiles('/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0', 'cmgtools', '.*root')
TbarWpow.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]

TT_bar.files = getFiles('/TTJets_SemiLeptMGDecays_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')
TT_bar.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]
TT_barv2.files = getFiles('/TTJets_FullLeptMGDecays_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7C-v2/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')
TT_barv2.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]


WW.files = getFiles('/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')
WW.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]

ZZ.files = getFiles('/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')
ZZ.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]


WZ.files = getFiles('/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0', 'cmgtools', '.*root')
WZ.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]


TT_barv2.splitFactor = 1000
TT_bar.splitFactor = 1000
WW.splitFactor = 1000
WZ.splitFactor = 1000
ZZ.splitFactor = 1000
TTpow.splitFactor = 1000
TTpowv2.splitFactor = 1000
TWpow.splitFactor = 100
TbarWpow.splitFactor = 100
Wjets.splitFactor = 1000
Wjetsv2.splitFactor = 1000
#selectedComponents = [TT_bar,WW,WZ,ZZ,TT_barv2]
#selectedComponents = [TWpow,TbarWpow]
selectedComponents = [Wjets,Wjetsv2]
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


