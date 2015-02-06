from CMGTools.H2TauTau.proto.samples.run2012.data_diTau import *
# from CMGTools.H2TauTau.proto.samples.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *

import copy
import re 
import CMGTools.RootTools.fwlite.Config as cfg
#from CMGTools.RootTools.yellowreport.YRParser import yrparser

Higgsgg110 = cfg.MCComponent(
    name = 'Higgsgg110',
    files = [],
    xSection = 25.04*8.02e-2,
    nGenEvents =999907*1.959996/2.0,
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
    nGenEvents = 4999561*1.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg125 = cfg.MCComponent(
    name = 'Higgsgg125',
    files = [],
    xSection = 19.52*6.37e-2,
    nGenEvents = 968134*1.979342/2.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg130 = cfg.MCComponent(
    name = 'Higgsgg130',
    files = [],
    xSection = 18.07*5.48e-2,
    nGenEvents = 999921*1.979998/2.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg135 = cfg.MCComponent(
    name = 'Higgsgg135',
    files = [],
    xSection = 16.79*4.52e-2,
    nGenEvents = 860729*1.953528/2.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg140 = cfg.MCComponent(
    name = 'Higgsgg140',
    files = [],
    xSection = 15.63*3.54e-2,
    nGenEvents = 999932*1.85999/2.0,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg145 = cfg.MCComponent(
    name = 'Higgsgg145',
    files = [],
    xSection = 14.59*2.61e-2,
    nGenEvents = 962119*1.87778/2.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF110 = cfg.MCComponent(
    name = 'HiggsVBF110',
    files = [],
    xSection = 1.791*8.02e-2, 
    nGenEvents = 499385*1.920811/2.0,
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
    nGenEvents = 998790*1.0,
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
    nGenEvents = 998883*1.975395/2.0,
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
    nGenEvents = 200128*1.0,
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


HiggsSUSYBB80 = cfg.MCComponent(
    name = 'HiggsSUSYBB80',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB90 = cfg.MCComponent(
    name = 'HiggsSUSYBB90',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB100 = cfg.MCComponent(
    name = 'HiggsSUSYBB100',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB110 = cfg.MCComponent(
    name = 'HiggsSUSYBB110',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB120 = cfg.MCComponent(
    name = 'HiggsSUSYBB120',
    files = [],
    xSection = 1.0,
    nGenEvents = 991913*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB130 = cfg.MCComponent(
    name = 'HiggsSUSYBB130',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB140 = cfg.MCComponent(
    name = 'HiggsSUSYBB140',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB160 = cfg.MCComponent(
    name = 'HiggsSUSYBB160',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB180 = cfg.MCComponent(
    name = 'HiggsSUSYBB180',
    files = [],
    xSection = 1.0, 
    nGenEvents = 967197*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB200 = cfg.MCComponent(
    name = 'HiggsSUSYBB200',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB250 = cfg.MCComponent(
    name = 'HiggsSUSYBB250',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB300 = cfg.MCComponent(
    name = 'HiggsSUSYBB300',
    files = [],
    xSection = 1.0, 
    nGenEvents = 999900*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB350 = cfg.MCComponent(
    name = 'HiggsSUSYBB350',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB400 = cfg.MCComponent(
    name = 'HiggsSUSYBB400',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB450 = cfg.MCComponent(
    name = 'HiggsSUSYBB450',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB500 = cfg.MCComponent(
    name = 'HiggsSUSYBB500',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB600 = cfg.MCComponent(
    name = 'HiggsSUSYBB600',
    files = [],
    xSection = 1.0, 
    nGenEvents = 987577*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB700 = cfg.MCComponent(
    name = 'HiggsSUSYBB700',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB800 = cfg.MCComponent(
    name = 'HiggsSUSYBB800',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB900 = cfg.MCComponent(
    name = 'HiggsSUSYBB900',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYBB1000 = cfg.MCComponent(
    name = 'HiggsSUSYBB1000',
    files = [],
    xSection = 1.0, 
    nGenEvents = 996838*1.0,
    triggers = [],
    effCorrFactor = 1 )


HiggsSUSYGluGlu80 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu80',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu90 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu90',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu100 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu100',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu110 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu110',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu120 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu120',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000620*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu130 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu130',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu140 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu140',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu160 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu160',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu180 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu180',
    files = [],
    xSection = 1.0, 
    nGenEvents = 997200*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu200 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu200',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu250 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu250',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu300 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu300',
    files = [],
    xSection = 1.0, 
    nGenEvents = 990976*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu350 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu350',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu400 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu400',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu450 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu450',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu500 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu500',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu600 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu600',
    files = [],
    xSection = 1.0, 
    nGenEvents = 980316*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu700 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu700',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
    triggers = [],
    effCorrFactor = 1 )

HiggsSUSYGluGlu800 = cfg.MCComponent(
    name = 'HiggsSUSYGluGlu800',
    files = [],
    xSection = 1.0,
    nGenEvents = 1000000*1.0,
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
    HiggsSUSYBB80,
    HiggsSUSYBB90,
    HiggsSUSYBB100,
    HiggsSUSYBB110,
    HiggsSUSYBB120,
    HiggsSUSYBB130,
    HiggsSUSYBB140,
    HiggsSUSYBB160,
    HiggsSUSYBB180,
    HiggsSUSYBB200,
    HiggsSUSYBB250,
    HiggsSUSYBB300,
    HiggsSUSYBB350,
    HiggsSUSYBB400,
    HiggsSUSYBB450,
    HiggsSUSYBB500,
    HiggsSUSYBB600,
    HiggsSUSYBB700,
    HiggsSUSYBB800,
    HiggsSUSYBB1000,
    ]

mc_higgs_SUSYGluGlu = [
    HiggsSUSYGluGlu80,
    HiggsSUSYGluGlu90,
    HiggsSUSYGluGlu100,
    HiggsSUSYGluGlu110,
    HiggsSUSYGluGlu120,
    HiggsSUSYGluGlu130,
    HiggsSUSYGluGlu140,
    HiggsSUSYGluGlu160,
    HiggsSUSYGluGlu180,
    HiggsSUSYGluGlu200,
    HiggsSUSYGluGlu250,
    HiggsSUSYGluGlu300,
    HiggsSUSYGluGlu350,
    HiggsSUSYGluGlu400,
    HiggsSUSYGluGlu450,
    HiggsSUSYGluGlu500,
    HiggsSUSYGluGlu600,
    HiggsSUSYGluGlu700,
    HiggsSUSYGluGlu800,
    HiggsSUSYGluGlu1000,
    ]

mc_higgs = copy.copy( mc_higgs_gg ) + copy.copy( mc_higgs_vbf ) + copy.copy( mc_higgs_VH ) + copy.copy( mc_higgs_SUSYBB )  + copy.copy( mc_higgs_SUSYGluGlu ) 



from CMGTools.H2TauTau.proto.samples.run2012.triggers_diTau import *

user  = 'hinzmann'
user2 = 'manzoni'
user3 = 'cbern'
aod   = 'V5_B'
pat   = 'PAT_CMG_V5_8_0'
htt   = 'HTT_Sep12_hinzmann'
htt2  = 'HTT_29Jan_metType1Recoil'
htt3  = 'TAUTAU_Nom_Jan30'
htt4  = 'TAUTAU_Up_Jan30'
htt5  = 'TAUTAU_Down_Jan30'
htt_nom = 'HTT_Feb5_hinzmann_Nom'
htt_up = 'HTT_Feb5_hinzmann_Up'
htt_down = 'HTT_Feb5_hinzmann_Down'
filePattern = 'diTau.*fullsel.*root'

# Data --------------------------------------------------------------------------------

data_Run2012A_PromptReco_v1.files = getFiles('/Tau/Run2012A-13Jul2012-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern) + \
                                    getFiles('/Tau/Run2012A-recover-06Aug2012-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
data_Run2012B_PromptReco_v1.files = getFiles('/Tau/Run2012B-13Jul2012-v1/AOD/V5/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
data_Run2012C_PromptReco_v1.files = getFiles('/Tau/Run2012C-24Aug2012-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
data_Run2012C_PromptReco_v2.files = getFiles('/Tau/Run2012C-PromptReco-v2/AOD/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
data_Run2012D_PromptReco_v1.files = getFiles('/Tau/Run2012D-PromptReco-v1/AOD/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

# Embedded samples --------------------------------------------------------------------

cache = True

embed_Run2012A_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012A_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache) + \
            getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    triggers = [],
    )

embed_Run2012B_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012B_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    triggers = [],
    )

embed_Run2012C_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012C_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012C_24Aug2012_v1_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    triggers = [],
    )

embed_Run2012C_PromptReco_v2 = cfg.EmbedComponent(
    name = 'embed_Run2012C_PromptReco_v2',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_Run2012C_PromptReco_v2_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    triggers = [],
    )

embed_Run2012D_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012D_PromptReco_v1',
    files = getFiles('/DoubleMu/StoreResults-DoubleMu_2012D_PromptReco_v1_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    triggers = [],
    )

# MC spring12 ----------------------------------------------------------------------------

#DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt3), user3, filePattern, cache)
DYJets.nGenEvents = 30459503*0.99836254 * 253./5. ## computing efficiency 
DYJets.xSection = 3503.71

DY1Jets = cfg.MCComponent(
    name = 'DY1Jets',
    #files = getFiles('/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    files = getFiles('/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt3), user3, filePattern, cache),
    xSection = 561.0,
    nGenEvents = 24045248*1.0,
    triggers = [],
    effCorrFactor = 1 )

DY2Jets = cfg.MCComponent(
    name = 'DY2Jets',
    #files = getFiles('/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    files = getFiles('/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7C-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt3), user3, filePattern, cache),
    xSection = 181.0,
    nGenEvents = 21852156*0.999445,
    triggers = [],
    effCorrFactor = 1 )

DY3Jets = cfg.MCComponent(
    name = 'DY3Jets',
    #files = getFiles('/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    files = getFiles('/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt3), user3, filePattern, cache),
    xSection = 51.1,
    nGenEvents = 11015445*1.0,
    triggers = [],
    effCorrFactor = 1 )

DY4Jets = cfg.MCComponent(
    name = 'DY4Jets',
    #files = getFiles('/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    files = getFiles('/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt3), user3, filePattern, cache),
    xSection = 23.04,
    nGenEvents = 6402827*1.0,
    triggers = [],
    effCorrFactor = 1 )

#WJets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache) + \
#              getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
WJets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt='HTT_29Jan_metType1Recoil'), 'manzoni', filePattern, cache) + \
              getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt='HTT_29Jan_metType1Recoil'), 'manzoni', filePattern, cache)
WJets.nGenEvents = 57709905*0.99037+18393090*0.993474

W1Jets = cfg.MCComponent(
    name = 'W1Jets',
    files = getFiles('/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    #files = getFiles('/W1JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt='HTT_29Jan_metType1Recoil'), 'manzoni', filePattern, cache),
    xSection = 6440.4,
    nGenEvents = 23141598*0.981021,
    triggers = [],
    effCorrFactor = 1 )

W2Jets = cfg.MCComponent(
    name = 'W2Jets',
    files = getFiles('/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    #files = getFiles('/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt='HTT_29Jan_metType1Recoil'), 'manzoni', filePattern, cache),
    xSection = 2087.2,
    nGenEvents = 34044921*0.981852,
    triggers = [],
    effCorrFactor = 1 )

W3Jets = cfg.MCComponent(
    name = 'W3Jets',
    files = getFiles('/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    #files = getFiles('/W3JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt='HTT_29Jan_metType1Recoil'), 'manzoni', filePattern, cache),
    xSection = 619.0,
    nGenEvents = 15539503*0.979117,
    triggers = [],
    effCorrFactor = 1 )

W4Jets = cfg.MCComponent(
    name = 'W4Jets',
    files = getFiles('/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt2), user2, filePattern, cache),
    #files = getFiles('/W4JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt='HTT_29Jan_metType1Recoil'), 'manzoni', filePattern, cache),
    xSection = 255.2,
    nGenEvents = 13382803*0.981096,
    triggers = [],
    effCorrFactor = 1 )

T_s = cfg.MCComponent(
    name = 'T_s',
    files = [],#getFiles('/T_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = 3.79,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

T_t = cfg.MCComponent(
    name = 'T_t',
    files = [],#getFiles('/T_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = 56.4,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

T_tW = cfg.MCComponent(
    name = 'T_tW',
    files = getFiles('/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = 11.1,
    nGenEvents = 497658*1.0,
    triggers = [],
    effCorrFactor = 1 )

Tbar_s = cfg.MCComponent(
    name = 'Tbar_s',
    files = [],#getFiles('/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PATbar_CMG_V5_10_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = 1.76,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

Tbar_t = cfg.MCComponent(
    name = 'Tbar_t',
    files = [],#getFiles('/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PATbar_CMG_V5_10_0/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = 30.7,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1 )

Tbar_tW = cfg.MCComponent(
    name = 'Tbar_tW',
    files = getFiles('/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = 11.1,
    nGenEvents = 986920*1.0,
    triggers = [],
    effCorrFactor = 1 )

TTJets.files = getFiles('/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
TTJets.nGenEvents = 6923750*0.99747

WW = cfg.MCComponent(
    name = 'WW',
    files = [],
    xSection = 43.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WW.files = getFiles('/WW_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
WW.nGenEvents = 10000431*0.99793

WZ = cfg.MCComponent(
    name = 'WZ',
    files = [],
    xSection = 18.2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WZ.files = getFiles('/WZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
WZ.nGenEvents = 10000283*1.0

ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
    xSection = 5.9,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZ.files = getFiles('/ZZ_TuneZ2star_8TeV_pythia6_tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
ZZ.nGenEvents = 9799908*0.99878

QCD = cfg.DataComponent(
    name = 'QCD',
    files = [],
    #files = getFiles('/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v5/AODSIM/{aod}/{pat}/H2TAUTAU_hinzmann1June12'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    intLumi = 1.0, # dummy
    triggers = [] )

QCD80 = cfg.DataComponent(
    name = 'QCD80',
    #files = [],
    files = getFiles('/QCD_Pt-80to120_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/{aod}/V5_B/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    intLumi = 1.0, # dummy
    triggers = [] )

QCD120 = cfg.DataComponent(
    name = 'QCD120',
    #files = [],
    files = getFiles('/QCD_Pt-120to170_TuneZ2star_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v3/AODSIM/{aod}/V5_B/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    intLumi = 1.0, # dummy
    triggers = [] )

# Higgs Summer11 ----------------------------------------------------------------------------

#Higgsgg110.files  = getFiles('/GluGluToHToTauTau_M-110_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#Higgsgg115.files  = getFiles('/GluGluToHToTauTau_M-115_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#Higgsgg120.files  = getFiles('/GluGluToHToTauTau_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#Higgsgg125.files  = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#Higgsgg130.files  = getFiles('/GluGluToHToTauTau_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#Higgsgg135.files  = getFiles('/GluGluToHToTauTau_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#Higgsgg140.files  = getFiles('/GluGluToHToTauTau_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#Higgsgg145.files  = getFiles('/GluGluToHToTauTau_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVBF110.files = getFiles('/VBF_HToTauTau_M-110_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVBF115.files = getFiles('/VBF_HToTauTau_M-115_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVBF120.files = getFiles('/VBF_HToTauTau_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVBF130.files = getFiles('/VBF_HToTauTau_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVBF135.files = getFiles('/VBF_HToTauTau_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVBF140.files = getFiles('/VBF_HToTauTau_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVBF145.files = getFiles('/VBF_HToTauTau_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVH110.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-110_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVH115.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-115_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVH120.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-120_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVH125.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-125_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVH130.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-130_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVH135.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-135_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVH140.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-140_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
#HiggsVH145.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-145_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)


#####################


Higgsgg110.files  = getFiles('/GluGluToHToTauTau_M-110_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
Higgsgg115.files  = getFiles('/GluGluToHToTauTau_M-115_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
Higgsgg120.files  = getFiles('/GluGluToHToTauTau_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt_nom), user, filePattern, cache)
Higgsgg125.files  = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
Higgsgg130.files  = getFiles('/GluGluToHToTauTau_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
Higgsgg135.files  = getFiles('/GluGluToHToTauTau_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
Higgsgg140.files  = getFiles('/GluGluToHToTauTau_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
Higgsgg145.files  = getFiles('/GluGluToHToTauTau_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVBF110.files = getFiles('/VBF_HToTauTau_M-110_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVBF115.files = getFiles('/VBF_HToTauTau_M-115_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVBF120.files = getFiles('/VBF_HToTauTau_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVBF130.files = getFiles('/VBF_HToTauTau_M-130_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVBF135.files = getFiles('/VBF_HToTauTau_M-135_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVBF140.files = getFiles('/VBF_HToTauTau_M-140_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVBF145.files = getFiles('/VBF_HToTauTau_M-145_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVH110.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-110_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVH115.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-115_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVH120.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-120_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVH125.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-125_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVH130.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-130_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVH135.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-135_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)
HiggsVH140.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-140_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt5), user3, filePattern, cache)

###
HiggsVH145.files  = getFiles('/WH_ZH_TTH_HToTauTau_M-145_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt4), user3, filePattern, cache)





#####################





HiggsSUSYBB80.files = getFiles('/SUSYBBHToTauTau_M-80_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB90.files = getFiles('/SUSYBBHToTauTau_M-90_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB100.files = getFiles('/SUSYBBHToTauTau_M-100_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB110.files = getFiles('/SUSYBBHToTauTau_M-110_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB120.files = getFiles('/SUSYBBHToTauTau_M-120_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB130.files = getFiles('/SUSYBBHToTauTau_M-130_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB140.files = getFiles('/SUSYBBHToTauTau_M-140_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB160.files = getFiles('/SUSYBBHToTauTau_M-160_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB180.files = getFiles('/SUSYBBHToTauTau_M-180_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB200.files = getFiles('/SUSYBBHToTauTau_M-200_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB250.files = getFiles('/SUSYBBHToTauTau_M-250_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB300.files = getFiles('/SUSYBBHToTauTau_M-300_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB350.files = getFiles('/SUSYBBHToTauTau_M-350_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB400.files = getFiles('/SUSYBBHToTauTau_M-400_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB450.files = getFiles('/SUSYBBHToTauTau_M-450_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB500.files = getFiles('/SUSYBBHToTauTau_M-500_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB600.files = getFiles('/SUSYBBHToTauTau_M-600_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB700.files = getFiles('/SUSYBBHToTauTau_M-700_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB800.files = getFiles('/SUSYBBHToTauTau_M-800_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYBB1000.files = getFiles('/SUSYBBHToTauTau_M-1000_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)

HiggsSUSYGluGlu80.files = getFiles('/SUSYGluGluToHToTauTau_M-80_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu90.files = getFiles('/SUSYGluGluToHToTauTau_M-90_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu100.files = getFiles('/SUSYGluGluToHToTauTau_M-100_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu110.files = getFiles('/SUSYGluGluToHToTauTau_M-110_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu120.files = getFiles('/SUSYGluGluToHToTauTau_M-120_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu130.files = getFiles('/SUSYGluGluToHToTauTau_M-130_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu140.files = getFiles('/SUSYGluGluToHToTauTau_M-140_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu160.files = getFiles('/SUSYGluGluToHToTauTau_M-160_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu180.files = getFiles('/SUSYGluGluToHToTauTau_M-180_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu200.files = getFiles('/SUSYGluGluToHToTauTau_M-200_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu250.files = getFiles('/SUSYGluGluToHToTauTau_M-250_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu300.files = getFiles('/SUSYGluGluToHToTauTau_M-300_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu350.files = getFiles('/SUSYGluGluToHToTauTau_M-350_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu400.files = getFiles('/SUSYGluGluToHToTauTau_M-400_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu450.files = getFiles('/SUSYGluGluToHToTauTau_M-450_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu500.files = getFiles('/SUSYGluGluToHToTauTau_M-500_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu600.files = getFiles('/SUSYGluGluToHToTauTau_M-600_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu700.files = getFiles('/SUSYGluGluToHToTauTau_M-700_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu800.files = getFiles('/SUSYGluGluToHToTauTau_M-800_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)
HiggsSUSYGluGlu1000.files = getFiles('/SUSYGluGluToHToTauTau_M-1000_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache)

GluGluToHToBB125 = cfg.MCComponent(
    name = 'GluGluToHToBB125',
    #files = [],
    files = getFiles('/GluGluToHToBB_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0_tauSkim/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = 19.52*5.77e-1, # dummy
    triggers = [],
    nGenEvents = 999908,
    effCorrFactor = 1 )

VBF_HToBB125 = cfg.MCComponent(
    name = 'VBF_HToBB125',
    #files = [],
    files = getFiles('/VBF_HToBB_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0_tauSkim/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = 1.559*5.77e-1, # dummy
    triggers = [],
    nGenEvents = 998844,
    effCorrFactor = 1 )

TTH_HToBB125 = cfg.MCComponent(
    name = 'TTH_HToBB125',
    #files = [],
    files = getFiles('/TTH_HToBB_M-125_8TeV-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0_tauSkim/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern, cache),
    xSection = (0.1302)*5.77e-1, # VH 0.6966+0.3943+
    triggers = [],
    nGenEvents = 1000008,
    effCorrFactor = 1 )

mc_spring12 = copy.copy( mc_ewk )
mc_spring12.extend( mc_higgs ) 
mc_spring12.extend([DY1Jets, DY2Jets, DY3Jets, DY4Jets, W1Jets, W2Jets, W3Jets, W4Jets, WW, WZ, ZZ])
mc_spring12.extend([T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW])
mc_spring12.extend([QCD])
mc_spring12.extend([GluGluToHToBB125, VBF_HToBB125, TTH_HToBB125])

for data in data_2012A:
    data.triggers = data_triggers_2012A
for data in data_2012B:
    data.triggers = data_triggers_2012B
for data in data_2012C:
    data.triggers = data_triggers_2012C
for data in data_2012D:
    data.triggers = data_triggers_2012D

MC_list = copy.copy( mc_ewk )
MC_list.extend( mc_higgs )
MC_list.extend( mc_diboson )

######
for sam in MC_list:
    sam.triggers = mc_triggers
######

MC = []
for mc in mc_spring12:
    mc.triggers = mc_triggers
    MC.append(mc)

embedded_2012 = [embed_Run2012A_PromptReco_v1, embed_Run2012B_PromptReco_v1, embed_Run2012C_PromptReco_v1, embed_Run2012C_PromptReco_v2, embed_Run2012D_PromptReco_v1]
    
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
