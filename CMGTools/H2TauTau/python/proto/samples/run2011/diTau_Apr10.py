from CMGTools.H2TauTau.proto.samples.run2011.data_diTau import *
from CMGTools.H2TauTau.proto.samples.run2011.embed import *
from CMGTools.H2TauTau.proto.samples.run2011.mc_fall11 import *
from CMGTools.H2TauTau.proto.samples.run2011.higgs import *
from CMGTools.H2TauTau.proto.samples.run2011.triggers_diTau import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

aod = 'V2'
pat = 'PAT_CMG_V2_5_0'
htt = 'DITAU_loosee'
filePattern = 'diTau.*fullsel.*root'

# Data --------------------------------------------------------------------------------

#data_Run2011A_May10ReReco_v1.files = getFiles('/Tau/Run2011A-May10ReReco-v1/AOD/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)

#data_Run2011A_PromptReco_v4.files = getFiles('/Tau/Run2011A-PromptReco-v4/AOD/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)

#data_Run2011A_05Aug2011_v1.files = getFiles('/Tau/Run2011A-05Aug2011-v1/AOD/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)

#data_Run2011A_PromptReco_v6.files = getFiles('/Tau/Run2011A-PromptReco-v6/AOD/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)

#data_Run2011B_PromptReco_v1.files = getFiles('/Tau/Run2011B-PromptReco-v1/AOD/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)

# Embedded samples --------------------------------------------------------------------

## embed_Run2011A_May10ReReco_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

## embed_Run2011A_PromptReco_v4.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

## embed_Run2011A_05Aug2011_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

## embed_Run2011A_03Oct2011_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

## embed_Run2011B_PromptReco_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)


# MC Fall11 ----------------------------------------------------------------------------

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
DYJets.nGenEvents = 36209629

WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
WJets.nGenEvents = 81345381

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
TTJets.nGenEvents = 3701947

#QCD15.files = getFiles('/QCD_Pt-15to30_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v3/AODSIM/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
#QCD15.nGenEvents = 10991904

#QCD30.files = getFiles('/QCD_Pt-30to50_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v3/AODSIM/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
#QCD30.nGenEvents = 6583068

#QCD50.files = getFiles('/QCD_Pt-50to80_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
#QCD50.nGenEvents = 6600000

#QCD80.files = getFiles('/QCD_Pt-80to120_TuneZ2_7TeV_pythia6/Fall11-PU_S6_START42_V14B-v3/AODSIM/V4/PAT_CMG_V4_0_0/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
#QCD80.nGenEvents = 6581772

ggHTT125.files = getFiles('/GluGluToHToTauTau_M-125_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
ggHTT125.nGenEvents = 199986

VBFHTT125.files = getFiles('/VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'hinzmann', filePattern)
VBFHTT125.nGenEvents = 198612


# Higgs Summer11 ----------------------------------------------------------------------------

# not available yet


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
