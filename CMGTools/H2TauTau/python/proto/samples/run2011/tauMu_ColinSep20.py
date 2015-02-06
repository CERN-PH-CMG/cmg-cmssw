import itertools
import copy
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor

from CMGTools.H2TauTau.proto.samples.run2011.data import *
from CMGTools.H2TauTau.proto.samples.run2011.embed import *
from CMGTools.H2TauTau.proto.samples.run2011.ewk import *
from CMGTools.H2TauTau.proto.samples.run2011.diboson import *
from CMGTools.H2TauTau.proto.samples.run2011.higgs import *

from CMGTools.H2TauTau.proto.samples.run2011.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers

aliases = {
    '/VBF_HToTauTau.*START42.*':'HiggsVBF',
    '/GluGluToHToTauTau.*START42.*':'HiggsGGH',
    '/WH_ZH_TTH_HToTauTau.*START42.*':'HiggsVH',
    '/DYJets.*START42.*':'DYJets',
    '/WJetsToLNu.*START42.*':'WJets',
    '/W2Jets.*START42.*':'W2Jets',
    '/W3Jets.*START42.*':'W3Jets',
    '/TTJets.*START42.*':'TTJets',
    '/T_TuneZ2_s-channel.*START42.*':'T_s',
    '/T_TuneZ2_t-channel.*START42.*':'T_t',    
    '/T_TuneZ2_tW-channel.*START42.*':'T_tW',    
    '/Tbar_TuneZ2_s-channel.*START42.*':'Tbar_s',
    '/Tbar_TuneZ2_t-channel.*START42.*':'Tbar_t',    
    '/Tbar_TuneZ2_tW-channel.*START42.*':'Tbar_tW',    
    '/TauPlusX/Run2011A-03Oct2011-v1.*':'data_Run2011A_03Oct2011_v1',
    '/TauPlusX/Run2011A-05Aug2011-v1.*':'data_Run2011A_05Aug2011_v1',
    '/TauPlusX/Run2011A-May10ReReco-v1.*':'data_Run2011A_May10ReReco_v1',
    '/TauPlusX/Run2011A-PromptReco-v4.*':'data_Run2011A_PromptReco_v4',
    '/TauPlusX/Run2011B-PromptReco-v1':'data_Run2011B_PromptReco_v1',
    '/DoubleMu/StoreResults-DoubleMu_2011A_03Oct2011_v1.*':'embed_Run2011A_03Oct2011_v1',
    '/DoubleMu/StoreResults-DoubleMu_2011A_05Aug2011_v1.*':'embed_Run2011A_05Aug2011_v1',
    '/DoubleMu/StoreResults-DoubleMu_2011A_10May2011_v1.*':'embed_Run2011A_May10ReReco_v1',
    '/DoubleMu/StoreResults-DoubleMu_2011A_PR_v4.*':'embed_Run2011A_PromptReco_v4',
    '/DoubleMu/StoreResults-DoubleMu_2011B_PR_v1.*':'embed_Run2011B_PromptReco_v1',
    '/WW_TuneZ2.*START42.*':'WW',
    '/WZ_TuneZ2.*START42.*':'WZ',
    '/ZZ_TuneZ2.*START42.*':'ZZ',
    '/WWJetsTo2L2Nu.*START42.*':'WWJetsTo2L2Nu',
    '/WZJetsTo2L2Q.*START42.*':'WZJetsTo2L2Q',
    '/WZJetsTo3LNu.*START42.*':'WZJetsTo3LNu',
    '/ZZJetsTo2L2Nu.*START42.*':'ZZJetsTo2L2Nu',
    '/ZZJetsTo2L2Q.*START42.*':'ZZJetsTo2L2Q',
    }


MC = copy.copy( mc_ewk )
MC.extend( mc_higgs )
MC.extend( mc_diboson ) 
for sam in MC:
    sam.triggers = mc_triggers
for data in data_list_2011A:
    data.triggers = data_triggers_2011A
for data in data_list_2011B:
    data.triggers = data_triggers_2011B

    
allsamples = copy.copy(MC)
allsamples.extend( data_list_2011 )
allsamples.extend( embed_list_2011 )

connect( allsamples, '%/TAUMU_2011_ColinSep21_NewSVFit', 'tauMu.*root', aliases, cache=True, verbose=False)

for c in allsamples:
    c.splitFactor = splitFactor(c)

