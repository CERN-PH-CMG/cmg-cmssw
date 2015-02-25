import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.H2TauTau.tools.jsonPick import jsonPick

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
#    '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1.*':'WJets',
    '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2.*':'WJets',
    '/DYJets.*START53.*':'DYJets',
    '/W1Jets.*START53.*':'W1Jets',
    '/W2Jets.*START53.*':'W2Jets',
    '/W3Jets.*START53.*':'W3Jets',
    '/W4Jets.*START53.*':'W4Jets',
    '/TTJets.*START53.*':'TTJets',
    '/TauPlusX/Run2012A-recover-06Aug2012-v1.*':'data_Run2012A_aug6',
    '/TauPlusX/Run2012A-13Jul2012-v1.*':'data_Run2012A',
    '/TauPlusX/Run2012B-13Jul2012-v1.*':'data_Run2012B',
    '/TauPlusX/Run2012C-24Aug2012-v1.*':'data_Run2012C_v1',
    '/TauPlusX/Run2012C-PromptReco-v2.*':'data_Run2012C_v2',
    '/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1_embedded_trans1_tau115_ptelec1.*':'embed_Run2012A_13Jul2012_v1',
    '/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1_embedded_trans1_tau115_ptelec1.*':'embed_Run2012A_recover_06Aug2012_v1',
    '/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau115_ptelec1.*':'embed_Run2012B_13Jul2012_v4',
    '/DoubleMu/StoreResults-DoubleMu_Run2012C_24Aug2012_v1_embedded_trans1_tau115_ptelec1.*':'embed_Run2012C_24Aug2012_v1',
    '/DoubleMu/StoreResults-DoubleMu_Run2012C_PromptReco_v2_embedded_trans1_tau115_ptelec1.*':'embed_Run2012C_PromptReco_v2',
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

MC_list = copy.copy( mc_ewk )
MC_list.extend( mc_diboson ) 
MC_list.extend( mc_higgs )

# the following for 52X sync purposes only:
#MC_list.append (HiggsGGH125)
#MC_list.append (HiggsVBF125)

allsamples = copy.copy(MC_list)
allsamples.extend( data_list )
allsamples.extend( embed_list )


print 'start connecting... \n\n'

connect( allsamples, '%H2TauTau_tauEle_06oct_PG_newSVFit_MVAMET', 'tauEle.*root', aliases, 
         cache=True, verbose=False)

print
for sam in MC_list:
    print 'setting trigger', mc_triggers,'for sample',sam.name
    sam.triggers = mc_triggers
for data in data_list:
    data.triggers = data_triggers
    print 'setting trigger', data.triggers,'for sample',data.name
    data.json = jsonPick (data.files[0])
    print 'setting json', data.json, 'for sample', data.name
for emb in embed_list:
    emb.json = jsonPick (data.files[0])
    print 'setting json', emb.json, 'for sample', emb.name
print
