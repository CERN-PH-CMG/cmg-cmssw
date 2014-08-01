import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.H2TauTau.tools.jsonPick import jsonPick

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers

aliases = {
    '/VBF_HToTauTau.*START53.*':'HiggsVBF',
    '/GluGluToHToTauTau.*START53.*':'HiggsGGH',
    '/WH_ZH_TTH_HToTauTau.*START53.*':'HiggsVH',
    '/DYJets.*START53.*':'DYJets',
    '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START53_V9-v1':'WJets',
    # '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START53_V9-v1.*':'WJets',
    '/WJets.*START53.*-v2.*':'WJets',
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
    }


MC_list = copy.copy( mc_ewk )
MC_list.extend( mc_higgs )
MC_list.extend( mc_diboson ) 
    
allsamples = copy.copy( MC_list )
allsamples.extend( data_list )
allsamples.extend( embed_list )

connect( allsamples, '%TAUMU_2012_ColinOct10%', 'tauMu.*root', aliases, cache=True, verbose=False)

# this number is not in the DB (dbs says this sample is still in production)
# WJets.nGenEvents = 61444940
    
for mc in MC_list:
    mc.triggers = mc_triggers
    # allsamples.append(mc)
for data in data_list:
    if len(data.files):
        data.json = jsonPick( data.files[0])
        data.triggers = data_triggers
        # allsamples.append(data)
for c in allsamples:
    c.splitFactor = splitFactor(c)
