from CMGTools.H2TauTau.proto.samples.getFiles import getFiles
from CMGTools.H2TauTau.proto.samples.triggers_tauEle import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

user = 'govoni'
filePattern = 'tauEle_fullsel_tree_CMG.*root'

## --- simulation: bkg --- + --- + --- + --- + --- + --- + --- + --- + ---

from CMGTools.H2TauTau.proto.samples.ewk import *
# NB this does not contain diboson samples

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1/H2TAUTAU_PGJun02/', user, filePattern)
# https://savannah.cern.ch/task/?30143
# 16 bad files out of 2751 total (0.6%), mostly pattuples, by now I don't care
DYJets.nGenEvents = 36209629
DYJets.triggers = mc_triggers_fall11
# DYJets.fakes = True # what is this?
# DYJets.splitFactor = 40

WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/HTTSKIM5/PAT_CMG_V5_4_1/H2TAUTAU_PGJun02/', user, filePattern)
# https://savannah.cern.ch/task/?30145
# 2 bad pattuples over 907 files, the cmg tuples are there => ok
WJets.nGenEvents = 81345381
WJets.triggers = mc_triggers_fall11
# WJets.splitFactor = 10

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/HTTSKIM1/PAT_CMG_V5_4_1/H2TAUTAU_PGJun02/', user, filePattern)
# https://savannah.cern.ch/task/?30144
# 3 bad files out of 5365 => I don't care
TTJets.nGenEvents = 59613991
TTJets.triggers = mc_triggers_fall11
# TTJets.splitFactor = 100 

## --- simulation: sig --- + --- + --- + --- + --- + --- + --- + --- + ---

## from CMGTools.H2TauTau.proto.samples.higgs import *



## --- embedded --- + --- + --- + --- + --- + --- + --- + --- + ---

## from CMGTools.H2TauTau.proto.samples.embed import *



## --- data --- + --- + --- + --- + --- + --- + --- + --- + ---


from CMGTools.H2TauTau.proto.samples.data import *

data_Run2011A_May10ReReco_v1.files = getFiles('/TauPlusX/Run2011A-May10ReReco-v1/AOD/V5/PAT_CMG_V5_4_1/H2TAUTAU_PGJun02/', user, filePattern)
data_Run2011A_May10ReReco_v1.triggers = data_triggers_2011A

data_Run2011A_PromptReco_v4.files = getFiles('/TauPlusX/Run2011A-PromptReco-v4/AOD/V5/PAT_CMG_V5_4_1/H2TAUTAU_PGJun02/', user, filePattern)
data_Run2011A_PromptReco_v4.triggers = data_triggers_2011A

data_Run2011A_05Aug2011_v1.files = getFiles('/TauPlusX/Run2011A-05Aug2011-v1/AOD/V5/PAT_CMG_V5_4_1/H2TAUTAU_PGJun02/', user, filePattern)
data_Run2011A_05Aug2011_v1.triggers = data_triggers_2011A

data_Run2011A_03Oct2011_v1.files = getFiles('/TauPlusX/Run2011A-03Oct2011-v1/AOD/V5/PAT_CMG_V5_4_1/H2TAUTAU_PGJun02/', user, filePattern)
data_Run2011A_03Oct2011_v1.triggers = data_triggers_2011A

data_Run2011B_PromptReco_v1.files = getFiles('/TauPlusX/Run2011B-PromptReco-v1/AOD/V5/PAT_CMG_V5_4_1/H2TAUTAU_PGJun02/', user, filePattern)
data_Run2011B_PromptReco_v1.triggers = data_triggers_2011B


## --- summary --- + --- + --- + --- + --- + --- + --- + --- + ---


MC = copy.copy( mc_ewk )
#MC.extend( mc_higgs ) 


