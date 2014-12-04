from CMGTools.H2TauTau.proto.samples.run2011.data import *
from CMGTools.H2TauTau.proto.samples.run2011.embed import *
from CMGTools.H2TauTau.proto.samples.run2011.mc_fall11 import *
from CMGTools.H2TauTau.proto.samples.run2011.higgs import *
from CMGTools.H2TauTau.proto.samples.run2011.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

aod = 'V3'
pat = 'PAT_CMG_TestMVAs'
htt = 'H2TAUTAU_TestMVAs_Mar12'
filePattern = 'tauMu.*fullsel.*root'

# Data --------------------------------------------------------------------------------

data_Run2011A_May10ReReco_v1.files = getFiles('/TauPlusX/Run2011A-May10ReReco-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

data_Run2011A_PromptReco_v4.files = getFiles('/TauPlusX/Run2011A-PromptReco-v4/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

data_Run2011A_05Aug2011_v1.files = getFiles('/TauPlusX/Run2011A-05Aug2011-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

data_Run2011A_03Oct2011_v1.files = getFiles('/TauPlusX/Run2011A-03Oct2011-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

data_Run2011B_PromptReco_v1.files = getFiles('/TauPlusX/Run2011B-PromptReco-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)


# Embedded samples --------------------------------------------------------------------

## embed_Run2011A_May10ReReco_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

## embed_Run2011A_PromptReco_v4.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

## embed_Run2011A_05Aug2011_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

## embed_Run2011A_03Oct2011_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)

## embed_Run2011B_PromptReco_v1.files = getFiles('/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), 'cbern', filePattern)


# MC Fall11 ----------------------------------------------------------------------------

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern)
DYJets.nGenEvents = 36209629

WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/TAUTAU/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern)
WJets.nGenEvents = 81345381

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/{pat}/{htt}'.format(pat=pat, htt=htt), 'cbern', filePattern)
TTJets.nGenEvents = 3701947


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
