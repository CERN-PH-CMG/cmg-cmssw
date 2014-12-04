import CMGTools.RootTools.fwlite.Config as cfg
import copy
import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.RootTools.json.jsonPick import jsonPick
from CMGTools.H2TauTau.officialJSONS import jsonMap

# from CMGTools.H2TauTau.proto.samples.run2012.data import *
# from CMGTools.H2TauTau.proto.samples.run2012.embed import *
# from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
# from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
# from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers

aliases = {
    '/VBF_HToTauTau.*START53.*':'HiggsVBF',
    '/GluGluToHToTauTau.*START53.*':'HiggsGGH',
    '/WH_ZH_TTH_HToTauTau.*START53.*':'HiggsVH',
    '/DYJets.*START53.*':'DYJets',
    # '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START53_V9-v1.*':'WJets',
    '/WJets.*START53.*':'WJets',
    '/W1Jets.*START53.*':'W1Jets',
    '/W2Jets.*START53.*':'W2Jets',
    '/W3Jets.*START53.*':'W3Jets',
    '/W4Jets.*START53.*':'W4Jets',
    '/TTJets.*START53.*':'TTJets',
    '/TauPlusX/Run2012A-13Jul2012-v1.*':'data_Run2012A',
    '/TauPlusX/Run2012A-recover-06Aug2012-v1.*':'data_Run2012A_aug6',
    '/TauPlusX/Run2012B-13Jul2012-v1.*':'data_Run2012B',
    '/TauPlusX/Run2012C-24Aug2012-v1.*':'data_Run2012C_v1',
    '/TauPlusX/Run2012C-PromptReco-v2.*':'data_Run2012C_v2',
    '/TauPlusX/Run2012D-PromptReco-v1.*':'data_Run2012D_v1',
    '/T_tW-channel.*START53.*':'T_tW',
    '/Tbar_tW-channel.*START53.*':'Tbar_tW',
    '/WWJetsTo2L2Nu.*START53.*':'WWJetsTo2L2Nu',
    '/WZJetsTo2L2Q.*START53.*':'WZJetsTo2L2Q',
    '/WZJetsTo3LNu.*START53.*_C$':'WZJetsTo3LNu',
    '/WW_TuneZ2star.*START53.*':'WW',
    '/WZ_TuneZ2star.*START53.*':'WZ',
    '/ZZ_TuneZ2star.*START53.*':'ZZ',
    '/ZZJetsTo2L2Nu.*START53.*':'ZZJetsTo2L2Nu',
    '/ZZJetsTo2L2Q.*START53.*':'ZZJetsTo2L2Q',
    '/ZZJetsTo4L.*START53.*_C$':'ZZJetsTo4L',
    '/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1.*':'embed_Run2012A_13Jul2012_v1',
    '/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1.*':'embed_Run2012B_13Jul2012_v4',
    '/DoubleMu/StoreResults-DoubleMu_Run2012C_PromptReco_v2_embedded_trans1.*':'embed_Run2012C_PromptReco_v2',
    '/DoubleMu/StoreResults-DoubleMu_Run2012C_24Aug2012_v1_embedded_trans1.*':'embed_Run2012C_24Aug2012_v1',
    '/DoubleMu/StoreResults-DoubleMu_2012D_PromptReco_v1_embedded_trans1.*':'embed_2012D_PromptReco_v1',
    }

LOCALS = locals()
allsamples = []
for name in aliases.values():
    LOCALS[name] = cfg.MCComponent(name=name,
                                   files = [],
                                   xSection = 1, # correction factor from Valentina through Jose (18/10)
                                   nGenEvents = 1,
                                   triggers = [],
                                   effCorrFactor = 1 
                                   )
    allsamples.append(LOCALS[name])

connect( allsamples, '%TAUMU_OSVFit_May27%', 'tauMu.*root', aliases, cache=False, verbose=False)

# this number is not in the DB (dbs says this sample is still in production)
# WJets.nGenEvents = 61444940
    
## for mc in MC_list:
##     mc.triggers = mc_triggers
##     # allsamples.append(mc)
## for data in data_list:
##     if len(data.files):
##         data.json = jsonPick( data.files[0], jsonMap)
##         data.triggers = data_triggers
##         # allsamples.append(data)
## for embed in embed_list:
##     if len(embed.files):
##         embed.json = jsonPick( data.files[0], jsonMap)
##         # No trigger requirements for embedded samples
## for c in allsamples:
##     c.splitFactor = splitFactor(c, 5e4)
