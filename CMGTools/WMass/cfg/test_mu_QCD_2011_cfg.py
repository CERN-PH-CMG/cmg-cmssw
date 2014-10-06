#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

sequence = cfg.Sequence(CoreWZsequence)


from CMGTools.H2TauTau.proto.samples.qcd import QCD15, QCD30, QCD50, QCD80
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

QCD15.files = getFiles('/QCD_Pt-0to5_TuneZ2_7TeV_pythia6/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
QCD15.triggers = triggers_mu

QCD0to5 = copy.deepcopy(QCD15)
QCD0to5.files = getFiles('/QCD_Pt-0to5_TuneZ2_7TeV_pythia6/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
QCD0to5.name = 'QCD0to5'

QCD5to15 = copy.deepcopy(QCD15)
QCD5to15.files = getFiles('/QCD_Pt-5to15_TuneZ2_7TeV_pythia6/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
QCD5to15.name = 'QCD5to15'

QCD15to30 = copy.deepcopy(QCD15)
QCD15to30.files = getFiles('/QCD_Pt-15to30_TuneZ2_7TeV_pythia6/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
QCD15to30.name = 'QCD15to30'

QCD30to50 = copy.deepcopy(QCD15)
QCD30to50.files = getFiles('/QCD_Pt-30to50_TuneZ2_7TeV_pythia6/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
QCD30to50.name = 'QCD30to50'

QCD20Mu15 = copy.deepcopy(QCD15)
QCD20Mu15.files = getFiles('/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
QCD20Mu15.name = 'QCD20Mu15'


# QCD15.triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # ]

# /QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0

selectedComponents = [QCD0to5,QCD5to15,QCD15to30,QCD30to50,QCD20Mu15]

QCD0to5.splitFactor = 200
QCD5to15.splitFactor = 200
QCD15to30.splitFactor = 200
QCD30to50.splitFactor = 200
QCD20Mu15.splitFactor = 200

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


# cmsStage QCD/QCD0to5/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD0to5/
# cmsStage QCD/QCD0to5/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD0to5/

# cmsStage QCD/QCD5to15/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD5to15/
# cmsStage QCD/QCD5to15/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD5to15/

# cmsStage QCD/QCD15to30/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD15to30/
# cmsStage QCD/QCD15to30/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD15to30/

# cmsStage QCD/QCD30to50/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD30to50/
# cmsStage QCD/QCD30to50/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD30to50/

# cmsStage QCD/QCD20Mu15/ZTreeProducer/ZTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD20Mu15/
# cmsStage QCD/QCD20Mu15/WTreeProducer/WTreeProducer_tree.root /store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/QCD/QCD20Mu15/


