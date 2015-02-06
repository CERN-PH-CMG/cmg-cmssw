from CMGTools.H2TauTau.proto.samples.run2011.data_diTau import *
# from CMGTools.H2TauTau.proto.samples.run2011.embed import *
from CMGTools.H2TauTau.proto.samples.run2011.ewk import *

import copy
import re 
import CMGTools.RootTools.fwlite.Config as cfg
#from CMGTools.RootTools.yellowreport.YRParser import yrparser

Higgsgg110 = cfg.MCComponent(
    name = 'Higgsgg110',
    files = [],
    xSection = 19.84*6.37e-2,
    nGenEvents = 199988,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg115 = cfg.MCComponent(
    name = 'Higgsgg115',
    files = [],
    xSection = 18.13*6.37e-2,
    nGenEvents = 196002,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg120 = cfg.MCComponent(
    name = 'Higgsgg120',
    files = [],
    xSection = 16.63*6.37e-2,
    nGenEvents = 199981,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg125 = cfg.MCComponent(
    name = 'Higgsgg125',
    files = [],
    xSection = 15.31*6.37e-2,
    nGenEvents = 199986,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg130 = cfg.MCComponent(
    name = 'Higgsgg130',
    files = [],
    xSection = 14.12*6.37e-2,
    nGenEvents = 199985,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg135 = cfg.MCComponent(
    name = 'Higgsgg135',
    files = [],
    xSection = 13.08*6.37e-2,
    nGenEvents = 198571,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg140 = cfg.MCComponent(
    name = 'Higgsgg140',
    files = [],
    xSection = 12.13*6.37e-2,
    nGenEvents = 191549,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg145 = cfg.MCComponent(
    name = 'Higgsgg145',
    files = [],
    xSection = 11.27*6.37e-2,
    nGenEvents = 199981,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF110 = cfg.MCComponent(
    name = 'HiggsVBF110',
    files = [],
    xSection = 1.398*6.37e-2, 
    nGenEvents = 198435,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF115 = cfg.MCComponent(
    name = 'HiggsVBF115',
    files = [],
    xSection = 1.332*6.37e-2, 
    nGenEvents = 198051,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF120 = cfg.MCComponent(
    name = 'HiggsVBF120',
    files = [],
    xSection = 1.263*6.37e-2, 
    nGenEvents = 198632,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF125 = cfg.MCComponent(
    name = 'HiggsVBF125',
    files = [],
    xSection = 1.211*6.37e-2, 
    nGenEvents = 198612,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF130 = cfg.MCComponent(
    name = 'HiggsVBF130',
    files = [],
    xSection = 1.154*6.37e-2, 
    nGenEvents = 197236,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF135 = cfg.MCComponent(
    name = 'HiggsVBF135',
    files = [],
    xSection = 1.100*6.37e-2, 
    nGenEvents = 198682,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF140 = cfg.MCComponent(
    name = 'HiggsVBF140',
    files = [],
    xSection = 1.052*6.37e-2, 
    nGenEvents = 193846,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF145 = cfg.MCComponent(
    name = 'HiggsVBF145',
    files = [],
    xSection = 1.004*6.37e-2, 
    nGenEvents = 197095,
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

mc_higgs = copy.copy( mc_higgs_gg ) + copy.copy( mc_higgs_vbf ) 

from CMGTools.H2TauTau.proto.samples.run2011.triggers_diTau import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

user = 'hinzmann'
aod = 'V5'
pat = 'PAT_CMG_V5_2_0'
htt = 'H2TAUTAU_hinzmann18May12'
filePattern = 'diTau.*fullsel.*root'

# Data --------------------------------------------------------------------------------

data_Run2011A_May10ReReco_v1.files = getFiles('/Tau/Run2011A-May10ReReco-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2011A_PromptReco_v4.files = getFiles('/Tau/Run2011A-PromptReco-v4/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2011A_05Aug2011_v1.files = getFiles('/Tau/Run2011A-05Aug2011-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

# data_Run2011A_PromptReco_v6.files = getFiles('/Tau/Run2011A-PromptReco-v6/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

# data_Run2011B_PromptReco_v1.files = getFiles('/Tau/Run2011B-PromptReco-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


# Embedded samples --------------------------------------------------------------------

## embed_Run2011A_May10ReReco_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

## embed_Run2011A_PromptReco_v4.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

## embed_Run2011A_05Aug2011_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

## embed_Run2011A_03Oct2011_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

## embed_Run2011B_PromptReco_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


# MC Fall11 ----------------------------------------------------------------------------

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
DYJets.nGenEvents = 36209629

WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WJets.nGenEvents = 81345381

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
TTJets.nGenEvents = 59613991

WW = cfg.MCComponent(
    name = 'WW',
    files = [],
    xSection = 43.,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WW.files = getFiles('/WW_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WW.nGenEvents = 4225916

WZ = cfg.MCComponent(
    name = 'WZ',
    files = [],
    xSection = 18.2,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

WZ.files = getFiles('/WZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WZ.nGenEvents = 4265243

ZZ = cfg.MCComponent(
    name = 'ZZ',
    files = [],
    xSection = 5.9,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

ZZ.files = getFiles('/ZZ_TuneZ2_7TeV_pythia6_tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
ZZ.nGenEvents = 4191045

# Higgs Summer11 ----------------------------------------------------------------------------

Higgsgg110.files = getFiles('/GluGluToHToTauTau_M-110_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg115.files = getFiles('/GluGluToHToTauTau_M-115_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg120.files = getFiles('/GluGluToHToTauTau_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg125.files = getFiles('/GluGluToHToTauTau_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg130.files = getFiles('/GluGluToHToTauTau_M-130_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg135.files = getFiles('/GluGluToHToTauTau_M-135_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg140.files = getFiles('/GluGluToHToTauTau_M-140_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg145.files = getFiles('/GluGluToHToTauTau_M-145_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF110.files = getFiles('/VBF_HToTauTau_M-110_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF115.files = getFiles('/VBF_HToTauTau_M-115_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF120.files = getFiles('/VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF130.files = getFiles('/VBF_HToTauTau_M-130_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF135.files = getFiles('/VBF_HToTauTau_M-135_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF140.files = getFiles('/VBF_HToTauTau_M-140_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF145.files = getFiles('/VBF_HToTauTau_M-145_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


mc_fall11 = copy.copy( mc_ewk )
mc_fall11.extend( mc_higgs ) 
mc_fall11.extend([WW, WZ, ZZ])

for data in data_list_2011A:
    data.triggers = data_triggers_2011A

for data in data_list_2011B:
    data.triggers = data_triggers_2011B

MC = []
for mc in mc_fall11:
    mc.triggers = mc_triggers_fall11
    MC.append(mc)
    
if __name__ == '__main__':

    def printSamples(sams):
        for sam in sams:
            print
            print sam

    print 'DATA 2011:'
    print 
    printSamples(data_list_2011)

    print 'MC FALL11:'
    print
    printSamples(mc_fall11)
