from CMGTools.H2TauTau.proto.samples.getFiles import getFiles
from CMGTools.H2TauTau.proto.samples.triggers_tauEle import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

aod = 'V5'
pat = 'PAT_CMG_V5_4_1'
htautau = 'TAUELE_OldSVFit_ColinJun26'
user = 'cmgtools'
filePattern = 'tauEle_fullsel_tree_CMG.*root'

## --- simulation: bkg --- + --- + --- + --- + --- + --- + --- + --- + ---

from CMGTools.H2TauTau.proto.samples.ewk import *
# NB this does not contain diboson samples

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)
# https://savannah.cern.ch/task/?30143
# 16 bad files out of 2751 total (0.6%), mostly pattuples, by now I don't care
DYJets.nGenEvents = 36209629

WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/{aod}/HTTSKIM5/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)
# https://savannah.cern.ch/task/?30145
# 2 bad pattuples over 907 files, the cmg tuples are there => ok
WJets.nGenEvents = 81345381

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/{aod}/HTTSKIM1/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)
## # https://savannah.cern.ch/task/?30144
## # 3 bad files out of 5365 => I don't care
TTJets.nGenEvents = 59613991

## --- simulation: sig --- + --- + --- + --- + --- + --- + --- + --- + ---

## from CMGTools.H2TauTau.proto.samples.higgs import *



## --- embedded --- + --- + --- + --- + --- + --- + --- + --- + ---

from CMGTools.H2TauTau.proto.samples.embed import *

embed_Run2011A_May10ReReco_v1.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)
embed_Run2011A_PromptReco_v4.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)
embed_Run2011A_05Aug2011_v1.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)
embed_Run2011A_03Oct2011_v1.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)
embed_Run2011B_PromptReco_v1.files = getFiles('/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1_embedded_trans1_tau115_ptelec1_17had1_17_v3-f456bdbb960236e5c696adfe9b04eaae/USER/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)


## --- data --- + --- + --- + --- + --- + --- + --- + --- + ---


from CMGTools.H2TauTau.proto.samples.data import *

data_Run2011A_May10ReReco_v1.files = getFiles('/TauPlusX/Run2011A-May10ReReco-v1/AOD/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)

data_Run2011A_PromptReco_v4.files = getFiles('/TauPlusX/Run2011A-PromptReco-v4/AOD/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)

data_Run2011A_05Aug2011_v1.files = getFiles('/TauPlusX/Run2011A-05Aug2011-v1/AOD/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)

data_Run2011A_03Oct2011_v1.files = getFiles('/TauPlusX/Run2011A-03Oct2011-v1/AOD/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)

data_Run2011B_PromptReco_v1.files = getFiles('/TauPlusX/Run2011B-PromptReco-v1/AOD/{aod}/{pat}/{htautau}'.format(aod=aod, pat=pat, htautau=htautau), user, filePattern)
data_Run2011B_PromptReco_v1.triggers = data_triggers_2011B


## --- summary --- + --- + --- + --- + --- + --- + --- + --- + ---


MC = copy.copy( mc_ewk )
#MC.extend( mc_higgs ) 

for data in data_list_2011A:
    data.triggers = data_triggers_2011A
    
for mc in MC:
    mc.triggers = mc_triggers_fall11
