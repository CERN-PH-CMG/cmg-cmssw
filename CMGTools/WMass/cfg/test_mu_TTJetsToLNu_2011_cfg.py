#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

sequence = cfg.Sequence(CoreWZsequence)


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
#TTJets.splitFactor = 1
#TTJets.files = TTJets.files[0:2]

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
