from CMGTools.H2TauTau.proto.samples.run2012.data_diTau import *
# from CMGTools.H2TauTau.proto.samples.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *

import copy
import re 
import CMGTools.RootTools.fwlite.Config as cfg
#from CMGTools.RootTools.yellowreport.YRParser import yrparser

Higgsgg110 = cfg.MCComponent(
    name = 'Higgsgg110',
    files = [],
    xSection = 25.04*8.02e-2,
    nGenEvents =999907*1.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg115 = cfg.MCComponent(
    name = 'Higgsgg115',
    files = [],
    xSection = 22.96*7.65e-2,
    nGenEvents = 917825*1.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg120 = cfg.MCComponent(
    name = 'Higgsgg120',
    files = [],
    xSection = 21.13*7.10e-2,
    nGenEvents = 4999561*0.9937,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg125 = cfg.MCComponent(
    name = 'Higgsgg125',
    files = [],
    xSection = 19.52*6.37e-2,
    nGenEvents = 968134*1.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg130 = cfg.MCComponent(
    name = 'Higgsgg130',
    files = [],
    xSection = 18.07*5.48e-2,
    nGenEvents = 999921,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg135 = cfg.MCComponent(
    name = 'Higgsgg135',
    files = [],
    xSection = 16.79*4.52e-2,
    nGenEvents = 860729*1.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg140 = cfg.MCComponent(
    name = 'Higgsgg140',
    files = [],
    xSection = 15.63*3.54e-2,
    nGenEvents = 999932*1.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg145 = cfg.MCComponent(
    name = 'Higgsgg145',
    files = [],
    xSection = 14.59*2.61e-2,
    nGenEvents = 962119,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF110 = cfg.MCComponent(
    name = 'HiggsVBF110',
    files = [],
    xSection = 1.791*8.02e-2, 
    nGenEvents = 499385*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF115 = cfg.MCComponent(
    name = 'HiggsVBF115',
    files = [],
    xSection = 1.709*7.65e-2, 
    nGenEvents = 998870*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF120 = cfg.MCComponent(
    name = 'HiggsVBF120',
    files = [],
    xSection = 1.632*7.10e-2, 
    nGenEvents = 4798759*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF125 = cfg.MCComponent(
    name = 'HiggsVBF125',
    files = [],
    xSection = 1.559*6.37e-2, 
    nGenEvents = 998836*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF130 = cfg.MCComponent(
    name = 'HiggsVBF130',
    files = [],
    xSection = 1.490*5.48e-2, 
    nGenEvents = 998790*0.99190,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF135 = cfg.MCComponent(
    name = 'HiggsVBF135',
    files = [],
    xSection = 1.425*4.52e-2, 
    nGenEvents = 998817*0.9900,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF140 = cfg.MCComponent(
    name = 'HiggsVBF140',
    files = [],
    xSection = 1.365*3.54e-2, 
    nGenEvents = 998894*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF145 = cfg.MCComponent(
    name = 'HiggsVBF145',
    files = [],
    xSection = 1.306*2.61e-2, 
    nGenEvents = 998883*1.0,
    triggers = [],
    effCorrFactor = 1 )



# xsec WH + ZH + TTH
# https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt8TeV
HiggsVH110 = cfg.MCComponent(
    name = 'HiggsVH110',
    files = [],
    xSection = (1.060+0.5869+0.1887)*8.02e-2,
    nGenEvents = 200408*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH115 = cfg.MCComponent(
    name = 'HiggsVH115',
    files = [],
    xSection = (0.9165+0.5117+0.1663)*7.65e-2, 
    nGenEvents = 200460*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH120 = cfg.MCComponent(
    name = 'HiggsVH120',
    files = [],
    xSection = (0.7859+0.4483+0.1470)*7.10e-2, 
    nGenEvents = 200448*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH125 = cfg.MCComponent(
    name = 'HiggsVH125',
    files = [],
    xSection = (0.6966+0.3943+0.1302)*6.37e-2, 
    nGenEvents = 200124*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH130 = cfg.MCComponent(
    name = 'HiggsVH130',
    files = [],
    xSection = (0.6095+0.3473+0.1157)*5.48e-2, 
    nGenEvents = 200080*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH135 = cfg.MCComponent(
    name = 'HiggsVH135',
    files = [],
    xSection = (0.5351+0.3074+0.1031)*4.52e-2, 
    nGenEvents = 200128*0.97169,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH140 = cfg.MCComponent(
    name = 'HiggsVH140',
    files = [],
    xSection = (0.4713+0.2728+0.09207)*3.54e-2, 
    nGenEvents = 200186*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVH145 = cfg.MCComponent(
    name = 'HiggsVH145',
    files = [],
    xSection = (0.4164+0.2424+0.08246)*2.61e-2, 
    nGenEvents = 200384*1.0,
    triggers = [],
    effCorrFactor = 1 )


HiggsSUSYBB120 = cfg.MCComponent(
    name = 'HiggsSUSYBB120',
    files = [],
    xSection = 1.0,
    nGenEvents = 991913*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB180 = cfg.MCComponent(
    name = 'HiggsSUSYBB180',
    files = [],
    xSection = 1.0, 
    nGenEvents = 967197*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB300 = cfg.MCComponent(
    name = 'HiggsSUSYBB300',
    files = [],
    xSection = 1.0, 
    nGenEvents = 999900*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB600 = cfg.MCComponent(
    name = 'HiggsSUSYBB600',
    files = [],
    xSection = 1.0, 
    nGenEvents = 987577*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB1000 = cfg.MCComponent(
    name = 'HiggsSUSYBB1000',
    files = [],
    xSection = 1.0, 
    nGenEvents = 996838*1.0,
    triggers = [],
    effCorrFactor = 1 )


HiggsSUSYGluGlu120 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu120',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000620*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu180 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu180',
    files = [],
    xSection = 1.0, 
    nGenEvents = 997200*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu300 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu300',
    files = [],
    xSection = 1.0, 
    nGenEvents = 990976*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu600 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu600',
    files = [],
    xSection = 1.0, 
    nGenEvents = 980316*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu1000 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu1000',
    files = [],
    xSection = 1.0, 
    nGenEvents = 994294*1.0,
    triggers = [],
    effCorrFactor = 1 )



mc_higgs_gg = [
    Higgsgg110,
    Higgsgg115,
    Higgsgg120,
    Higgsgg125,
    Higgsgg130,
    Higgsgg135,
    Higgsgg140,
    Higgsgg145,
    ]

#pattern = re.compile('Higgs(\D+)(\d+)')
#for h in mc_higgs_vbf:
#    m = pattern.match( h.name )
#    process = m.group(1)
#    mass = float(m.group(2))
#    h.xSection = yrparser.get(mass)['gg']['sigma']
#    print h.name, 'sigma = ', h.xSection

mc_higgs_vbf = [
    HiggsVBF110,
    HiggsVBF115,
    HiggsVBF120,
    HiggsVBF125,
    HiggsVBF130,
    HiggsVBF135,
    HiggsVBF140,
    HiggsVBF145,
    ]

#pattern = re.compile('Higgs(\D+)(\d+)')
#for h in mc_higgs_vbf:
#    m = pattern.match( h.name )
#    process = m.group(1)
#    mass = float(m.group(2))
#    h.xSection = yrparser.get(mass)['VBF']['sigma']
#    print h.name, 'sigma = ', h.xSection

mc_higgs_VH = [
    HiggsVH110,
    HiggsVH115,
    HiggsVH120,
    HiggsVH125,
    HiggsVH130,
    HiggsVH135,
    HiggsVH140,
    HiggsVH145,
    ]

mc_higgs_SUSYBB = [
    HiggsSUSYBB120,
    HiggsSUSYBB180,
    HiggsSUSYBB300,
    HiggsSUSYBB600,
    HiggsSUSYBB1000,
    ]

mc_higgs_SUSYGluGlu = [
    HiggsSUSYGluGlu120,
    HiggsSUSYGluGlu180,
    HiggsSUSYGluGlu300,
    HiggsSUSYGluGlu600,
    HiggsSUSYGluGlu1000,
    ]

mc_higgs = copy.copy( mc_higgs_gg ) + copy.copy( mc_higgs_vbf ) + copy.copy( mc_higgs_VH ) + copy.copy( mc_higgs_SUSYBB )  + copy.copy( mc_higgs_SUSYGluGlu ) 



from CMGTools.H2TauTau.proto.samples.run2012.triggers_diTau import data_triggers_2012A, data_triggers_2012B, data_triggers_2012C, mc_triggers_spring12

user = 'hinzmann'
aod  = 'V5_B'
pat  = 'PAT_CMG_V5_6_0_B'
htt  = 'HTT_Sep12_hinzmann'
filePattern = 'diTau.*fullsel.*root'

# Data --------------------------------------------------------------------------------

#data_Run2012A_PromptReco_v1.files = getFiles('/Tau/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

#data_Run2012B_PromptReco_v1.files = getFiles('/Tau/Run2012B-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_start-194479/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
#                                    getFiles('/Tau/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
#                                    getFiles('/Tau/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195017-195396/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
#                                    getFiles('/Tau/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195397-195947/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
#                                    getFiles('/Tau/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195948-196531/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2012A_PromptReco_v1.files = getFiles('/Tau/Run2012A-13Jul2012-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
data_Run2012B_PromptReco_v1.files = getFiles('/Tau/Run2012B-13Jul2012-v1/AOD/V5/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
data_Run2012C_PromptReco_v1.files = getFiles('/Tau/Run2012C-PromptReco-v1/AOD/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
data_Run2012C_PromptReco_v2.files = getFiles('/Tau/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_7_0_runrange_start-202305/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
                                    getFiles('/Tau/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_7_0_runrange_202306-end/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

# Embedded samples --------------------------------------------------------------------

embed_Run2012A_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012A_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
            getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    triggers = [],
    )

embed_Run2012B_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012B_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    triggers = [],
    )

embed_Run2012C_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012C_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012C_24Aug2012_v1_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    triggers = [],
    )

embed_Run2012C_PromptReco_v2 = cfg.EmbedComponent(
    name = 'embed_Run2012C_PromptReco_v2',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012C_PromptReco_v2_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    triggers = [],
    )

# MC spring12 ----------------------------------------------------------------------------

DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
DYJets.nGenEvents = 30459503*0.99749

WJets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
              getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WJets.nGenEvents = 57709905*0.99126+18393090*0.98926

W3Jets = cfg.MCComponent(
    name = 'W3Jets',
    files = getFiles('/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12-PU_S7_START52_V9-v2/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    xSection = 612.0,
    nGenEvents = 15539503*0.9954,
    triggers = [],
    effCorrFactor = 1 )

TTJets.files = getFiles('/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
TTJets.nGenEvents = 6736135*0.9696

WW = cfg.MCComponent(
    name = 'WW',
    files = [],
    xSection = 43.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WW.files = getFiles('/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12_PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WW.nGenEvents = 10000431*0.9981

WZ = cfg.MCComponent(
    name = 'WZ',
    files = [],
    xSection = 18.2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WZ.files = getFiles('/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WZ.nGenEvents = 10000283*0.9988

ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
    xSection = 5.9,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZ.files = getFiles('/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
ZZ.nGenEvents = 9799908*1.0

QCD = cfg.DataComponent(
    name = 'QCD',
    files = [],
    #files = getFiles('/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM/{aod}/{pat}/H2TAUTAU_hinzmann1June12'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    intLumi = 1.0, # dummy
    triggers = [] )

QCD50 = cfg.DataComponent(
    name = 'QCD50',
    files = [],
    #files = getFiles('/QCD_Pt-50to80_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/H2TAUTAU_hinzmann1June12'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    intLumi = 1.0, # dummy
    triggers = [] )

QCD80 = cfg.DataComponent(
    name = 'QCD80',
    files = [],
    #files = getFiles('/QCD_Pt-80to120_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/H2TAUTAU_hinzmann1June12'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    intLumi = 1.0, # dummy
    triggers = [] )

# Higgs Summer11 ----------------------------------------------------------------------------

Higgsgg110.files = getFiles('/GluGluToHToTauTau_M-110_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg115.files = getFiles('/GluGluToHToTauTau_M-115_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg120.files = getFiles('/GluGluToHToTauTau_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg125.files = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

#Higgsgg125.files = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}_rawMET'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
#Higgsgg125.files = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}_rawMET'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

Higgsgg130.files = getFiles('/GluGluToHToTauTau_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg135.files = getFiles('/GluGluToHToTauTau_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg140.files = getFiles('/GluGluToHToTauTau_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg145.files = getFiles('/GluGluToHToTauTau_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF110.files = getFiles('/VBF_HToTauTau_M-110_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF115.files = getFiles('/VBF_HToTauTau_M-115_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF120.files = getFiles('/VBF_HToTauTau_M-120_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
#HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{pat}/HTT_Oct2_hinzmann'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF130.files = getFiles('/VBF_HToTauTau_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF135.files = getFiles('/VBF_HToTauTau_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF140.files = getFiles('/VBF_HToTauTau_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF145.files = getFiles('/VBF_HToTauTau_M-145_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH110.files = getFiles('/WH_ZH_TTH_HToTauTau_M-110_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v3/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH115.files = getFiles('/WH_ZH_TTH_HToTauTau_M-115_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH120.files = getFiles('/WH_ZH_TTH_HToTauTau_M-120_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH125.files = getFiles('/WH_ZH_TTH_HToTauTau_M-125_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH130.files = getFiles('/WH_ZH_TTH_HToTauTau_M-130_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH135.files = getFiles('/WH_ZH_TTH_HToTauTau_M-135_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH140.files = getFiles('/WH_ZH_TTH_HToTauTau_M-140_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH145.files = getFiles('/WH_ZH_TTH_HToTauTau_M-145_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

HiggsSUSYBB120.files = getFiles('/SUSYBBHToTauTau_M-120_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsSUSYBB180.files = getFiles('/SUSYBBHToTauTau_M-180_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsSUSYBB300.files = getFiles('/SUSYBBHToTauTau_M-300_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsSUSYBB600.files = getFiles('/SUSYBBHToTauTau_M-600_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsSUSYBB1000.files = getFiles('/SUSYBBHToTauTau_M-1000_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

HiggsSUSYGluGlu120.files = getFiles('/SUSYGluGluToHToTauTau_M-120_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsSUSYGluGlu180.files = getFiles('/SUSYGluGluToHToTauTau_M-180_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsSUSYGluGlu300.files = getFiles('/SUSYGluGluToHToTauTau_M-300_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsSUSYGluGlu600.files = getFiles('/SUSYGluGluToHToTauTau_M-120_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsSUSYGluGlu1000.files = getFiles('/SUSYGluGluToHToTauTau_M-120_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

mc_spring12 = copy.copy( mc_ewk )
mc_spring12.extend( mc_higgs ) 
mc_spring12.extend([W3Jets, WW, WZ, ZZ])
mc_spring12.extend([QCD])

for data in data_2012A:
    data.triggers = data_triggers_2012A

for data in data_2012B:
    data.triggers = data_triggers_2012B

MC = []
for mc in mc_spring12:
    mc.triggers = mc_triggers_spring12
    MC.append(mc)

embedded_2012 = [embed_Run2012A_PromptReco_v1, embed_Run2012B_PromptReco_v1, embed_Run2012C_PromptReco_v1, embed_Run2012C_PromptReco_v2]
    
if __name__ == '__main__':

    def printSamples(sams):
        for sam in sams:
            print
            print sam

    print 'DATA 2012:'
    print 
    printSamples(data_2012)

    print 'MC spring12:'
    print
    printSamples(mc_spring12)
