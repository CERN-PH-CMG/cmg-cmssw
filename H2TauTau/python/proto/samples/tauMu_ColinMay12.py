from CMGTools.H2TauTau.proto.samples.data import *
from CMGTools.H2TauTau.proto.samples.embed import *
from CMGTools.H2TauTau.proto.samples.ewk import *
from CMGTools.H2TauTau.proto.samples.higgs import *

from CMGTools.H2TauTau.proto.samples.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

user = 'cbern'
aod = 'V5'
pat = 'PAT_CMG_V5_2_0'
htt = 'H2TAUTAU_TauMu_MetRaw_ColinMay12'
filePattern = 'tauMu.*fullsel.*root'

# Data --------------------------------------------------------------------------------

data_Run2011A_May10ReReco_v1.files = getFiles('/TauPlusX/Run2011A-May10ReReco-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2011A_PromptReco_v4.files = getFiles('/TauPlusX/Run2011A-PromptReco-v4/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2011A_05Aug2011_v1.files = getFiles('/TauPlusX/Run2011A-05Aug2011-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2011A_03Oct2011_v1.files = getFiles('/TauPlusX/Run2011A-03Oct2011-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

data_Run2011B_PromptReco_v1.files = getFiles('/TauPlusX/Run2011B-PromptReco-v1/AOD/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


# Embedded samples --------------------------------------------------------------------

embed_Run2011A_May10ReReco_v1.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

embed_Run2011A_PromptReco_v4.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

embed_Run2011A_05Aug2011_v1.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

embed_Run2011A_03Oct2011_v1.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)

embed_Run2011B_PromptReco_v1.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau116_ptmu1_13had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


# MC Fall11 ----------------------------------------------------------------------------

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
DYJets.nGenEvents = 36209629

WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/HTTSKIM/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
WJets.nGenEvents = 81345381

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/{aod}/HTTSKIM1/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)
TTJets.nGenEvents = 59613991


# Higgs Summer11 ----------------------------------------------------------------------------

# not available yet

## HiggsVBF120.files = getFiles('/VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htt}'.format(aod=aod, pat=pat, htt=htt), user, filePattern)


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
