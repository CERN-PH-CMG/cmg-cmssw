import itertools
import copy
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.H2TauTau.proto.samples.connect import connect

from CMGTools.H2TauTau.proto.samples.data import *
from CMGTools.H2TauTau.proto.samples.embed import *
from CMGTools.H2TauTau.proto.samples.ewk import *
# from CMGTools.H2TauTau.proto.samples.diboson import *
# from CMGTools.H2TauTau.proto.samples.higgs import *

from CMGTools.H2TauTau.proto.samples.triggers_tauMu import mc_triggers

aliases = {
    '/VBF_HToTauTau.*START42.*':'HiggsVBF',
    '/GluGluToHToTauTau.*START42.*':'HiggsGGH',
    '/WH_ZH_TTH_HToTauTau.*START42.*':'HiggsVH',
    '/DYJets.*START42.*':'DYJets',
    # '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START42_V9-v1':'WJets',
    '/WJetsToLNu.*START42.*':'WJets',
    '/W2Jets.*START42.*':'W2Jets',
    '/W3Jets.*START42.*':'W3Jets',
    '/TTJets.*START42.*':'TTJets',
    '/TauPlusX/Run2012A-PromptReco-v1':'data_Run2012A',
    '/TauPlusX/Run2012B-PromptReco-v1':'data_Run2012B',
    '/WW_TuneZ2star.*START42.*':'WW',
    '/WZ_TuneZ2star.*START42.*':'WZ',
    '/ZZ_TuneZ2star.*START42.*':'ZZ' 
    }


MC = copy.copy( mc_ewk )
# MC.extend( mc_higgs )
# MC.extend( mc_diboson ) 
for sam in MC:
    sam.triggers = mc_triggers
# for data in data_list_2012:
#    data.triggers = data_triggers

    
allsamples = copy.copy(MC)
#allsamples.extend( data_list_2012 )
#allsamples.extend( embed_list_2012 )

connect( allsamples, '%TAUMU_NewSVFit_TauIsoRelaxed_Colin19Sep', 'tauMu.*root', aliases, cache=True, verbose=False)

# this number is not in the DB (dbs says this sample is still in production)
WJets.nGenEvents = 61444940
