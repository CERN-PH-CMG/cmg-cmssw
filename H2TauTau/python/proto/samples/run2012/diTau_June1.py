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
    xSection = 19.84*8.02e-2,
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg115 = cfg.MCComponent(
    name = 'Higgsgg115',
    files = [],
    xSection = 18.13*7.65e-2,
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg120 = cfg.MCComponent(
    name = 'Higgsgg120',
    files = [],
    xSection = 16.63*7.10e-2,
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg125 = cfg.MCComponent(
    name = 'Higgsgg125',
    files = [],
    xSection = 15.31*6.37e-2,
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg130 = cfg.MCComponent(
    name = 'Higgsgg130',
    files = [],
    xSection = 14.12*5.48e-2,
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg135 = cfg.MCComponent(
    name = 'Higgsgg135',
    files = [],
    xSection = 13.08*4.52e-2,
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg140 = cfg.MCComponent(
    name = 'Higgsgg140',
    files = [],
    xSection = 12.13*3.54e-2,
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

Higgsgg145 = cfg.MCComponent(
    name = 'Higgsgg145',
    files = [],
    xSection = 11.27*2.61e-2,
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF110 = cfg.MCComponent(
    name = 'HiggsVBF110',
    files = [],
    xSection = 1.398*8.02e-2, 
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF115 = cfg.MCComponent(
    name = 'HiggsVBF115',
    files = [],
    xSection = 1.332*7.65e-2, 
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF120 = cfg.MCComponent(
    name = 'HiggsVBF120',
    files = [],
    xSection = 1.263*7.10e-2, 
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF125 = cfg.MCComponent(
    name = 'HiggsVBF125',
    files = [],
    xSection = 1.211*6.37e-2, 
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF130 = cfg.MCComponent(
    name = 'HiggsVBF130',
    files = [],
    xSection = 1.154*5.48e-2, 
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF135 = cfg.MCComponent(
    name = 'HiggsVBF135',
    files = [],
    xSection = 1.100*4.52e-2, 
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF140 = cfg.MCComponent(
    name = 'HiggsVBF140',
    files = [],
    xSection = 1.052*3.54e-2, 
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )

HiggsVBF145 = cfg.MCComponent(
    name = 'HiggsVBF145',
    files = [],
    xSection = 1.004*2.61e-2, 
    nGenEvents = 200000,
    triggers = [],
    effCorrFactor = 1 )



mc_higgs_gg = [
    #Higgsgg110,
    #Higgsgg115,
    #Higgsgg120,
    Higgsgg125,
    #Higgsgg130,
    #Higgsgg135,
    #Higgsgg140,
    #Higgsgg145,
    ]

#pattern = re.compile('Higgs(\D+)(\d+)')
#for h in mc_higgs_vbf:
#    m = pattern.match( h.name )
#    process = m.group(1)
#    mass = float(m.group(2))
#    h.xSection = yrparser.get(mass)['gg']['sigma']
#    print h.name, 'sigma = ', h.xSection

mc_higgs_vbf = [
    #HiggsVBF110,
    #HiggsVBF115,
    #HiggsVBF120,
    HiggsVBF125,
    #HiggsVBF130,
    #HiggsVBF135,
    #HiggsVBF140,
    #HiggsVBF145,
    ]

#pattern = re.compile('Higgs(\D+)(\d+)')
#for h in mc_higgs_vbf:
#    m = pattern.match( h.name )
#    process = m.group(1)
#    mass = float(m.group(2))
#    h.xSection = yrparser.get(mass)['VBF']['sigma']
#    print h.name, 'sigma = ', h.xSection

mc_higgs = copy.copy( mc_higgs_gg ) + copy.copy( mc_higgs_vbf ) 



from CMGTools.H2TauTau.proto.samples.run2012.triggers_diTau import data_triggers_2012A, data_triggers_2012B, mc_triggers_spring12

user = 'hinzmann'
aod = 'V5'
pat = 'PAT_CMG_V5_4_0'
htt = 'H2TAUTAU_hinzmann1June12'
filePattern = 'diTau.*fullsel.*root'

# Data --------------------------------------------------------------------------------

data_Run2012A_PromptReco_v1.files = getFiles('/Tau/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2012B_PromptReco_v1.files = getFiles('/Tau/Run2012B-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_start-194479/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

# Embedded samples --------------------------------------------------------------------

## embed_Run2012A_May10ReReco_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

## embed_Run2012A_PromptReco_v4.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

## embed_Run2012A_05Aug2012_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

## embed_Run2012A_03Oct2012_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

## embed_Run2012B_PromptReco_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


# MC spring12 ----------------------------------------------------------------------------

DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
DYJets.nGenEvents = 30461028*0.9926

WJets.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WJets.nGenEvents = 18393090*0.99689

TTJets.files = getFiles('/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
TTJets.nGenEvents = 6736135

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

# Higgs Summer11 ----------------------------------------------------------------------------

Higgsgg110.files = getFiles('/GluGluToHToTauTau_M-110_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
#Higgsgg115.files = getFiles('/GluGluToHToTauTau_M-115_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg120.files = getFiles('/GluGluToHToTauTau_M-120_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg125.files = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg130.files = getFiles('/GluGluToHToTauTau_M-130_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg135.files = getFiles('/GluGluToHToTauTau_M-135_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg140.files = getFiles('/GluGluToHToTauTau_M-140_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
Higgsgg145.files = getFiles('/GluGluToHToTauTau_M-145_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
#HiggsVBF110.files = getFiles('/VBF_HToTauTau_M-110_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF115.files = getFiles('/VBF_HToTauTau_M-115_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF120.files = getFiles('/VBF_HToTauTau_M-120_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
#HiggsVBF130.files = getFiles('/VBF_HToTauTau_M-130_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
#HiggsVBF135.files = getFiles('/VBF_HToTauTau_M-135_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
#HiggsVBF140.files = getFiles('/VBF_HToTauTau_M-140_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
#HiggsVBF145.files = getFiles('/VBF_HToTauTau_M-145_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


mc_spring12 = copy.copy( mc_ewk )
mc_spring12.extend( mc_higgs ) 
mc_spring12.extend([WW, WZ, ZZ])

for data in data_2012A:
    data.triggers = data_triggers_2012A

for data in data_2012B:
    data.triggers = data_triggers_2012B

MC = []
for mc in mc_spring12:
    mc.triggers = mc_triggers_spring12
    MC.append(mc)
    
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
