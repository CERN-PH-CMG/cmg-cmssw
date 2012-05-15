from CMGTools.H2TauTau.proto.samples.data_diTau import *
# from CMGTools.H2TauTau.proto.samples.embed import *
from CMGTools.H2TauTau.proto.samples.ewk import *
from CMGTools.H2TauTau.proto.samples.higgs import *

from CMGTools.H2TauTau.proto.samples.triggers_diTau import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

user = 'hinzmann'
aod = 'V5'
pat = 'PAT_CMG_V5_2_0'
htt = 'H2TAUTAU_hinzmann14May12'
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


# Higgs Summer11 ----------------------------------------------------------------------------

Higgsgg125.files = getFiles('/GluGluToHToTauTau_M-125_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


mc_fall11 = copy.copy( mc_ewk )
mc_fall11.extend( mc_higgs ) 

for data in data_2011A:
    data.triggers = data_triggers_2011A

for data in data_2011B:
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
    printSamples(data_2011)

    print 'MC FALL11:'
    print
    printSamples(mc_fall11)
