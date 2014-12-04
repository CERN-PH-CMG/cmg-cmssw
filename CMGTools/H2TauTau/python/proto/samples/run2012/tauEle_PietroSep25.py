import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauEle import data_triggers, mc_triggers

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
    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run196090to196531':'embed_Run2012B_196090_196531',
    '/WW_TuneZ2star.*START52.*':'WW',
    '/WZ_TuneZ2star.*START52.*':'WZ',
    '/ZZ_TuneZ2star.*START52.*':'ZZ',
    '/T_tW.*START52.*':'T_tW',
    '/Tbar_tW.*START52.*':'Tbar_tW',
#    '/T_t-.*START52.*':'T_t',
#    '/Tbar_t-.*START52.*':'Tbar_t',
#    '/T_s-.*START52.*':'T_s',
#    '/Tbar_s-.*START52.*':'Tbar_s'
    '/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph.*START52.*':'WW2l2v',
    '/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph.*START52.*':'WZ2l2q',
    '/WZJetsTo3LNu_TuneZ2_8TeV-madgraph.*START52.*':'WZ3lv',
    '/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph.*START52.*':'ZZ2l2q',
    '/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph.*START52.*':'ZZ2l2v',
    '/ZZJetsTo4L_TuneZ2star_8TeV-madgraph.*START52.*':'ZZ4l'
    }


MC = copy.copy( mc_ewk )

#MC.extend( mc_higgs )
# the following for 52X sync purposes only:
MC.append (HiggsGGH125)
MC.append (HiggsVBF125)

MC.extend( mc_diboson ) 
MC.extend( mc_diboson_xcl ) 

allsamples = copy.copy(MC)
allsamples.extend( data_list_2012 )
allsamples.extend( embed_list_2012 )

# allsamples = copy.copy(data_list_2012)

connect( allsamples, '%H2TauTau_tauEle_20Sep_PG_newSVFit_MVAMET', 'tauEle.*root', aliases, cache=False, verbose=False)

print
for sam in MC:
    print 'setting trigger', mc_triggers,'for sample',sam.name
    sam.triggers = mc_triggers
for data in data_list_2012:
    print 'setting trigger', data_triggers,'for sample',data.name
    data.triggers = data_triggers
print

# this number is not in the DB (dbs says this sample is still in production)
WJets.nGenEvents = 61444940
