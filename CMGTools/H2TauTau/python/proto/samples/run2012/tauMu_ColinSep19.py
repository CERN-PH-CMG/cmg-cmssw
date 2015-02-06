import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers

aliases = {
    '/VBF_HToTauTau.*START52.*':'HiggsVBF',
    '/GluGluToHToTauTau.*START52.*':'HiggsGGH',
    '/WH_ZH_TTH_HToTauTau.*START52.*':'HiggsVH',
    '/DYJets.*START52.*':'DYJets',
    '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1':'WJets',
    # '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1.*':'WJets',
    '/W2Jets.*START52.*':'W2Jets',
    '/W3Jets.*START52.*':'W3Jets',
    '/TTJets.*START52.*':'TTJets',
    '/TauPlusX/Run2012A-PromptReco-v1':'data_Run2012A',
    '/TauPlusX/Run2012B-PromptReco-v1':'data_Run2012B',
    '/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1':'embed_Run2012A',
    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135':'embed_Run2012B_193752_195135',
    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070':'embed_Run2012B_195147_196070',
    '/WW_TuneZ2star.*START52.*':'WW',
    '/WZ_TuneZ2star.*START52.*':'WZ',
    '/ZZ_TuneZ2star.*START52.*':'ZZ' 
    }


MC = copy.copy( mc_ewk )
MC.extend( mc_higgs )
MC.extend( mc_diboson ) 
for sam in MC:
    sam.triggers = mc_triggers
for data in data_list_2012:
    data.triggers = data_triggers

    
allsamples = copy.copy(MC)
allsamples.extend( data_list_2012 )
allsamples.extend( embed_list_2012 )

connect( allsamples, '%TAUMU_NewSVFit_Colin17Sep', 'tauMu.*root', aliases, cache=True, verbose=False)

# this number is not in the DB (dbs says this sample is still in production)
WJets.nGenEvents = 61444940
