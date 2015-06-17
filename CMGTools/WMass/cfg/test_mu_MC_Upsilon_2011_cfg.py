import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap import triggers_jpsi,triggers_upsilon,triggers_mu

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
     'triggerBitFilter',
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    allVertices = 'slimmedPrimaryVertices',
    goodVertices = 'slimmedPrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False,
    keepFailingEvents = True,
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    recoilcut = 1000, # 20
    pfmetcut = 0, # 25
    jetptcut = 1000, # 30
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True,
    triggerBits = {'Dimuon10_Jpsi' : triggers_upsilon},
    # triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = True,    
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

ZAna = cfg.Analyzer(
    'JPsiAnalyzer',
    recoilcut = 1000, # 20
    pfmetcut = 0, # 25
    jetptcut = 1000, # 30
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    # savegenp = False,
    savegenp = True,
    verbose = True,
    triggerBits = {'Dimuon10_Jpsi' : triggers_upsilon},
    # triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = True,
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer',
    # storeNeutralCMGcandidates = True,
    # storeCMGcandidates = True,
    # storeSlimRecoInfo = True,
    # storeSlimGenInfo = True,    
    )

genAna = cfg.Analyzer(
    'GenParticleAnalyzerFSR',
    src = 'genParticlesPruned'
)

sequence = cfg.Sequence( [
    genAna,
    # jsonAna,
    # triggerAna,
    vertexAna,
    # WAna,
    # WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )


from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

# sequence = cfg.Sequence(CoreWZsequence)

from CMGTools.H2TauTau.proto.samples.ewk import WJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles



UpsilonMC = copy.deepcopy(WJets)
# UpsilonMC.files = getFiles('/JPsiToMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen-v2/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
UpsilonMC.files = [
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_1.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_10.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_11.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_12.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_13.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_14.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_15.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_16.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_17.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_18.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_19.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_2.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_20.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_21.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_22.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_23.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_24.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_25.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_26.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_27.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_28.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_29.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_3.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_30.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_31.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_32.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_33.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_34.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_35.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_36.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_37.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_38.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_39.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_4.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_40.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_41.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_42.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_43.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_44.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_45.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_46.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_47.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_48.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_49.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_5.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_50.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_51.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_52.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_53.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_54.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_55.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_56.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_57.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_58.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_59.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_6.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_60.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_61.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_62.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_63.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_64.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_65.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_66.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_67.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_68.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_69.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_7.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_70.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_71.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_72.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_73.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_74.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_75.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_76.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_77.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_78.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_79.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_8.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_80.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_81.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_82.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_83.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_84.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_85.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_86.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_87.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_88.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_89.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_9.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_90.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_91.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_92.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_93.root',
                  'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/Upsilon1SToMuMu_2MuEtaFilter_tuneD6T_7TeV-pythia6-evtgen/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/PAT_CMG_V5_15_0/cmgTuple_94.root',

               ]
# UpsilonMC.files = [
                # 'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/JPsiToMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen-v2/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0/cmgTuple_203.root',
                # 'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/JPsiToMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen-v2/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0/cmgTuple_355.root',
                # 'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/JPsiToMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen-v2/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0/cmgTuple_685.root',
                # ]
UpsilonMC.name = 'UpsilonMC'
# UpsilonMC.triggers = triggers_upsilon
UpsilonMC.splitFactor = 100

# ######## TEST ##################
# UpsilonMC.files = UpsilonMC.files[:10]
# UpsilonMC.splitFactor = 1
# ################################

selectedComponents = [UpsilonMC]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


