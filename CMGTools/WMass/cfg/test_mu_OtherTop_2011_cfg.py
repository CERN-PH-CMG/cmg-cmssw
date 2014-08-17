Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

sequence = cfg.Sequence(CoreWZsequence)


from CMGTools.H2TauTau.proto.samples.ewk import TTJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

TTJets.files = getFiles('/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
TTJets.triggers = triggers_mu
TTJets.splitFactor = 100

##################################################################3
TtW = copy.deepcopy(TTJets)
TtW.files = getFiles('/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
TtW.name = 'TtW'

TbartW = copy.deepcopy(TTJets)
TbartW.files = getFiles('/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
TbartW.name = 'TbartW'

Tt = copy.deepcopy(TTJets)
Tt.files = getFiles('/T_TuneZ2_t-channel_7TeV-powheg-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
Tt.name = 'Tt'

Tbart = copy.deepcopy(TTJets)
Tbart.files = getFiles('/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
Tbart.name = 'Tbart'

Ts = copy.deepcopy(TTJets)
Ts.files = getFiles('/T_TuneZ2_s-channel_7TeV-powheg-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
Ts.name = 'Ts'

Tbars = copy.deepcopy(TTJets)
Tbars.files = getFiles('/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
Tbars.name = 'Tbars'


selectedComponents = [TtW,TbartW,Tt,Tbart,Ts,Tbars]


# TEST
# TTJets.splitFactor = 1
# TTJets.files = TTJets.files[0:2]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


