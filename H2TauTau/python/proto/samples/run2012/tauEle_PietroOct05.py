import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.H2TauTau.proto.samples.connect import connect

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauEle import data_triggers, mc_triggers

aliases = {
    '/VBF_HToTauTau.*START53.*':'HiggsVBF',
    '/GluGluToHToTauTau.*START53.*':'HiggsGGH',
    '/WH_ZH_TTH_HToTauTau.*START53.*':'HiggsVH',
    '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START53_V9-v2':'WJets',
    '/DYJets.*START53.*':'DYJets',
    # '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START53_V9-v1.*':'WJets',
    '/W1Jets.*START53.*':'W1Jets',
    '/W2Jets.*START53.*':'W2Jets',
    '/W3Jets.*START53.*':'W3Jets',
    '/W4Jets.*START53.*':'W4Jets',
    '/TTJets.*START53.*':'TTJets',
    '/TauPlusX/Run2012A-recover-06Aug2012-v1.*':'data_Run2012A_aug6',
    '/TauPlusX/Run2012A-13Jul2012-v1.*':'data_Run2012A',
    '/TauPlusX/Run2012B-13Jul2012-v1.*':'data_Run2012B',
    '/TauPlusX/Run2012C-PromptReco-v2.*':'data_Run2012C_v2',
    '/TauPlusX/Run2012C-24Aug2012-v1.*':'data_Run2012C_v1',
#    '/DoubleMu/StoreResults-DoubleMu_2012A_PromptReco_v1.*':'embed_Run2012A',
#    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run193752to195135.*':'embed_Run2012B_193752_195135',
#    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run195147to196070.*':'embed_Run2012B_195147_196070',
#    '/DoubleMu/StoreResults-DoubleMu_2012B_PromptReco_v1_Run196090to196531.*':'embed_Run2012B_196090_196531',
    '/T_tW.*START53.*':'T_tW',
    '/Tbar_tW.*START53.*':'Tbar_tW',
#    '/T_t-.*START53.*':'T_t',
#    '/Tbar_t-.*START53.*':'Tbar_t',
#    '/T_s-.*START53.*':'T_s',
#    '/Tbar_s-.*START53.*':'Tbar_s'
    '/WW_TuneZ2star.*START53.*':'WW',
    '/WZ_TuneZ2star.*START53.*':'WZ',
    '/ZZ_TuneZ2star.*START53.*':'ZZ',
    '/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph.*START53.*':'WWJetsTo2L2Nu',
    '/WZJetsTo2L2Q_TuneZ2star_8TeV-madgraph.*START53.*':'WZJetsTo2L2Q',
    '/WZJetsTo3LNu_TuneZ2_8TeV-madgraph.*START53.*':'WZJetsTo3LNu',
    '/ZZJetsTo2L2Q_TuneZ2star_8TeV-madgraph.*START53.*':'ZZJetsTo2L2Q',
    '/ZZJetsTo2L2Nu_TuneZ2star_8TeV-madgraph.*START53.*':'ZZJetsTo2L2Nu',
    '/ZZJetsTo4L_TuneZ2star_8TeV-madgraph.*START53.*':'ZZJetsTo4L'
    }

MC = copy.copy( mc_ewk )

# MC.extend( mc_higgs )
# the following for 52X sync purposes only:
MC.append (HiggsGGH125)
MC.append (HiggsVBF125)

MC.extend( mc_diboson ) 

allsamples = copy.copy(MC)
allsamples.extend( data_list )
# allsamples.extend( embed_list )

print 'start connecting... \n\n'

connect( allsamples, '%H2TauTau_tauEle_06oct_PG_newSVFit_MVAMET', 'tauEle.*root', aliases, cache=False, verbose=True)

print
for sam in MC:
    print 'setting trigger', mc_triggers,'for sample',sam.name
    sam.triggers = mc_triggers
for data in data_list:
    print 'setting trigger', data_triggers,'for sample',data.name
    data.triggers = data_triggers
print
