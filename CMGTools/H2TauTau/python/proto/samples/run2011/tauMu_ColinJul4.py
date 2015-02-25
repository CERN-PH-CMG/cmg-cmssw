import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.H2TauTau.proto.samples.run2011.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

from CMGTools.H2TauTau.proto.samples.run2011.ewk import *
from CMGTools.H2TauTau.proto.samples.run2011.embed import *
from CMGTools.H2TauTau.proto.samples.run2011.data import *
from CMGTools.H2TauTau.proto.samples.run2011.higgs import *

MC = copy.copy( mc_ewk )
MC.extend( mc_higgs ) 
# MC.extend( mc_diboson ) 
for sam in MC:
    sam.triggers = mc_triggers_fall11
for data in data_list_2011A:
    data.triggers = data_triggers_2011A
for data in data_list_2011B:
    data.triggers = data_triggers_2011B
    

allsamples = copy.copy(MC)
allsamples.extend( data_list_2011 )
allsamples.extend( embed_list_2011 )

aliases = {
    '/VBF_HToTauTau':'HiggsVBF',
    '/GluGluToHToTauTau':'HiggsGGH',
    '/WH_ZH_TTH_HToTauTau':'HiggsVH',
    '/DYJets':'DYJets',
    '/WJets':'WJets',
    '/W2Jets':'W2Jets',
    '/W3Jets':'W3Jets',
    '/TTJets':'TTJets',
    '/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011':'embed_Run2011A_03Oct2011_v1',
    '/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1':'embed_Run2011A_05Aug2011_v1',
    '/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1':'embed_Run2011A_May10ReReco_v1',
    '/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4':'embed_Run2011A_PromptReco_v4',
    '/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1':'embed_Run2011B_PromptReco_v1',
    '/TauPlusX/Run2011A-03Oct2011-v1':'data_Run2011A_03Oct2011_v1',
    '/TauPlusX/Run2011A-05Aug2011-v1':'data_Run2011A_05Aug2011_v1',
    '/TauPlusX/Run2011A-May10ReReco-v1':'data_Run2011A_May10ReReco_v1',
    '/TauPlusX/Run2011A-PromptReco-v4':'data_Run2011A_PromptReco_v4',
    '/TauPlusX/Run2011B-PromptReco-v1':'data_Run2011B_PromptReco_v1'
    }

connect( allsamples, '%TAUMU_Jul3_RelTauPt', 'tauMu.*root', aliases, cache=True)

TTJets.nGenEvents = 59613991 * 0.1790225 / 0.179171 * 0.99962399999999996
WJets.nGenEvents = 81345381 * 0.0236105 / 0.023636999999999998 * 0.99954999999999994


