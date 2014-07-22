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


