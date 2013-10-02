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
    nGenEvents = 4999561*1.99183/2.0,
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
    nGenEvents = 998817*1.0,
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

mc_higgs = copy.copy( mc_higgs_gg ) + copy.copy( mc_higgs_vbf ) + copy.copy( mc_higgs_VH ) 



from CMGTools.H2TauTau.proto.samples.run2012.triggers_diTau import data_triggers_2012A, data_triggers_2012B, mc_triggers_spring12

user = 'hinzmann'
aod = 'V5'
pat = 'PAT_CMG_V5_4_0'
htt = 'H2TAUTAU_hinzmann1June12'
filePattern = 'diTau.*fullsel.*root'

# Data --------------------------------------------------------------------------------

data_Run2012A_PromptReco_v1.files = getFiles('/Tau/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2012B_PromptReco_v1.files = getFiles('/Tau/Run2012B-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_start-194479/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
                                    getFiles('/Tau/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
                                    getFiles('/Tau/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195017-195396/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
                                    getFiles('/Tau/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195397-195947/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
                                    getFiles('/Tau/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_195948-196531/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

# Embedded samples --------------------------------------------------------------------

embed_Run2012A_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012A_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1_embedded_trans1_tau132_pttau1_17had2_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    triggers = [],
    )

embed_Run2012B_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012B_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135_embedded_trans1_tau132_pttau1_17had2_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
            getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070_embedded_trans1_tau132_pttau1_17had2_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
            getFiles('/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run196090to196531_embedded_trans1_tau132_pttau1_17had2_17_v2-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    triggers = [],
    )

# MC spring12 ----------------------------------------------------------------------------

DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
DYJets.nGenEvents = 30461028*0.9926   ### all but June1 v1
#DYJets.nGenEvents = 1082838

WJets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WJets.nGenEvents = 81345381
#WJets.nGenEvents = 18393090*0.99689  ### all but June1 v1

W3Jets = cfg.MCComponent(
    name = 'W3Jets',
    files = getFiles('/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12-PU_S7_START52_V9-v2/AODSIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    xSection = 304.0,
    nGenEvents = 7541595*1.99724/2.0,
#    nGenEvents = 14725074*1.98661/2.0,
    triggers = [],
    effCorrFactor = 1 )

TTJets.files = getFiles('/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
TTJets.nGenEvents = 6736135*1.0

WW = cfg.MCComponent(
    name = 'WW',
    files = [],
    xSection = 43.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WW.files = getFiles('/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WW.nGenEvents = 10000431

WZ = cfg.MCComponent(
    name = 'WZ',
    files = [],
    xSection = 18.2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WZ.files = getFiles('/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WZ.nGenEvents = 9996622

ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
    xSection = 5.9,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZ.files = getFiles('/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12-PU_S7_START50_V15-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
ZZ.nGenEvents = 9799908

QCD = cfg.MCComponent(
    name = 'QCD',
    files = getFiles('/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    xSection = 1.0,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

QCD50 = cfg.MCComponent(
    name = 'QCD50',
    files = getFiles('/QCD_Pt-50to80_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    xSection = 1.0,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

QCD80 = cfg.MCComponent(
    name = 'QCD80',
    files = getFiles('/QCD_Pt-80to120_TuneZ2star_8TeV_pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern),
    xSection = 1.0,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

# Higgs Summer11 ----------------------------------------------------------------------------

Higgsgg110.files = getFiles('/GluGluToHToTauTau_M-110_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg115.files = getFiles('/GluGluToHToTauTau_M-115_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg120.files = getFiles('/GluGluToHToTauTau_M-120_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg125.files = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg130.files = getFiles('/GluGluToHToTauTau_M-130_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg135.files = getFiles('/GluGluToHToTauTau_M-135_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg140.files = getFiles('/GluGluToHToTauTau_M-140_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg145.files = getFiles('/GluGluToHToTauTau_M-145_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF110.files = getFiles('/VBF_HToTauTau_M-110_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF115.files = getFiles('/VBF_HToTauTau_M-115_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF120.files = getFiles('/VBF_HToTauTau_M-120_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF130.files = getFiles('/VBF_HToTauTau_M-130_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF135.files = getFiles('/VBF_HToTauTau_M-135_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF140.files = getFiles('/VBF_HToTauTau_M-140_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF145.files = getFiles('/VBF_HToTauTau_M-145_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH110.files = getFiles('/WH_ZH_TTH_HToTauTau_M-110_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v3/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH115.files = getFiles('/WH_ZH_TTH_HToTauTau_M-115_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH120.files = getFiles('/WH_ZH_TTH_HToTauTau_M-120_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH125.files = getFiles('/WH_ZH_TTH_HToTauTau_M-125_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH130.files = getFiles('/WH_ZH_TTH_HToTauTau_M-130_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH135.files = getFiles('/WH_ZH_TTH_HToTauTau_M-135_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH140.files = getFiles('/WH_ZH_TTH_HToTauTau_M-140_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVH145.files = getFiles('/WH_ZH_TTH_HToTauTau_M-145_8TeV-pythia6-tauola/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

VBF_HToWWTo2LAndTau2Nu_M_125 = cfg.MCComponent(
    name = 'VBF_HToWWTo2LAndTau2Nu_M_125',
    files = [],
    xSection = 1.559*2.34e-2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

VBF_HToWWTo2LAndTau2Nu_M_125.files = getFiles('/VBF_HToWWTo2LAndTau2Nu_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
VBF_HToWWTo2LAndTau2Nu_M_125.nGenEvents = 9799908

GluGluToHToWWTo2LAndTau2Nu_M_125 = cfg.MCComponent(
    name = 'GluGluToHToWWTo2LAndTau2Nu_M_125',
    files = [],
    xSection = 19.52*2.34e-2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

GluGluToHToWWTo2LAndTau2Nu_M_125.files = getFiles('/GluGluToHToWWTo2LAndTau2Nu_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
GluGluToHToWWTo2LAndTau2Nu_M_125.nGenEvents = 9799908


# Higgs WW/ZZ Fall11 ----------------------------------------------------------------------------

GluGluToHToWWTo2Tau2Nu_M_120 = cfg.MCComponent(
    name = 'GluGluToHToWWTo2Tau2Nu_M_120',
    files = [],
    xSection = 16.63*(1.54e-2-6.94e-3),
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

GluGluToHToWWTo2Tau2Nu_M_120.files = getFiles('/GluGluToHToWWTo2Tau2Nu_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/PAT_CMG_V5_4_1/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
GluGluToHToWWTo2Tau2Nu_M_120.nGenEvents = 54996

GluGluToHToWWToTauNuQQ_M_120 = cfg.MCComponent(
    name = 'GluGluToHToWWToTauNuQQ_M_120',
    files = [],
    xSection = 16.63*1.49e-3/3.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

GluGluToHToWWToTauNuQQ_M_120.files = getFiles('/GluGluToHToWWToTauNuQQ_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/PAT_CMG_V5_4_1/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
GluGluToHToWWToTauNuQQ_M_120.nGenEvents = 109994

VBF_HToWWToTauNuQQ_M_120 = cfg.MCComponent(
    name = 'VBF_HToWWToTauNuQQ_M_120',
    files = [],
    xSection = 1.263*1.49e-3/3.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

VBF_HToWWToTauNuQQ_M_120.files = getFiles('/VBF_HToWWToTauNuQQ_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/PAT_CMG_V5_4_1/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
VBF_HToWWToTauNuQQ_M_120.nGenEvents = 109857

VBF_HToWWTo2Tau2Nu_M_120 = cfg.MCComponent(
    name = 'VBF_HToWWTo2Tau2Nu_M_120',
    files = [],
    xSection = 1.263*(1.54e-2-6.94e-3),
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

VBF_HToWWTo2Tau2Nu_M_120.files = getFiles('/VBF_HToWWTo2Tau2Nu_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/PAT_CMG_V5_4_1/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
VBF_HToWWTo2Tau2Nu_M_120.nGenEvents = 52803

GluGluToHToZZTo2L2Nu_M_125 = cfg.MCComponent(
    name = 'GluGluToHToZZTo2L2Nu_M_125',
    files = [],
    xSection = 15.31*2.36e-2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

GluGluToHToZZTo2L2Nu_M_125.files = getFiles('/GluGluToHToZZTo2L2Nu_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/PAT_CMG_V5_4_1/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
GluGluToHToZZTo2L2Nu_M_125.nGenEvents = 292174

GluGluToHToZZTo2L2Q_M_125 = cfg.MCComponent(
    name = 'GluGluToHToZZTo2L2Q_M_125',
    files = [],
    xSection = 15.31*3.73e-3,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

GluGluToHToZZTo2L2Q_M_125.files = getFiles('/GluGluToHToZZTo2L2Q_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/PAT_CMG_V5_4_1/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
GluGluToHToZZTo2L2Q_M_125.nGenEvents = 263973

VBF_ToHToZZTo2l2q_M_130 = cfg.MCComponent(
    name = 'VBF_ToHToZZTo2l2q_M_130',
    files = [],
    xSection = 1.211*3.73e-3,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

VBF_ToHToZZTo2l2q_M_130.files = getFiles('/VBF_ToHToZZTo2l2q_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/PAT_CMG_V5_4_1/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
VBF_ToHToZZTo2l2q_M_130.nGenEvents = 49944

VBF_ToHToZZTo2L2NU_M_125 = cfg.MCComponent(
    name = 'VBF_ToHToZZTo2L2NU_M_125',
    files = [],
    xSection = 1.211*2.36e-2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

VBF_ToHToZZTo2L2NU_M_125.files = getFiles('/VBF_ToHToZZTo2L2NU_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/PAT_CMG_V5_4_1/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
VBF_ToHToZZTo2L2NU_M_125.nGenEvents = 49347


mc_spring12 = copy.copy( mc_ewk )
mc_spring12.extend( mc_higgs ) 
mc_spring12.extend([W3Jets, WW, WZ, ZZ])
mc_spring12.extend([QCD,QCD50,QCD80])
mc_spring12.extend([GluGluToHToWWTo2LAndTau2Nu_M_125,VBF_HToWWTo2LAndTau2Nu_M_125])

for data in data_2012A:
    data.triggers = data_triggers_2012A

for data in data_2012B:
    data.triggers = data_triggers_2012B

MC = []
for mc in mc_spring12:
    mc.triggers = mc_triggers_spring12
    MC.append(mc)

embedded_2012 = [embed_Run2012A_PromptReco_v1, embed_Run2012B_PromptReco_v1]
    
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
