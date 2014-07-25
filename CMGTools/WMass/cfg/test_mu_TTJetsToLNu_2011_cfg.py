import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap import triggers_mu

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
     'triggerBitFilter',
     # keepFailingEvents = False    
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    allVertices = 'slimmedPrimaryVertices',
    goodVertices = 'slimmedPrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False,
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
    triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = False
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
    triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = False
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

from CMGTools.H2TauTau.proto.samples.ewk import TTJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
TTJets.triggers = triggers_mu
# TTJets.triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # ]

selectedComponents = [TTJets]

TTJets.splitFactor = 750

# TEST
# TTJets.splitFactor = 1
# TTJets.files = TTJets.files[0:2]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


# cmsStage Tbars/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/Tbar_s
# cmsStage Tbars/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/Tbar_s
# cmsStage Tbart/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/Tbar_t
# cmsStage Tbart/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/Tbar_t
# cmsStage TbartW/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/Tbar_tW
# cmsStage TbartW/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/Tbar_tW
# cmsStage Ts/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/T_s
# cmsStage Ts/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/T_s
# cmsStage Tt/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/T_t
# cmsStage Tt/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/T_t
# cmsStage TtW/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/T_tW
# cmsStage TtW/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/SingleTop/T_tW
