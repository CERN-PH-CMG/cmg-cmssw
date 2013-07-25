import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.H2TauTau.proto.samples.connect import connect
from CMGTools.H2TauTau.proto.samples.splitFactor import splitFactor
from CMGTools.RootTools.json.jsonPick import jsonPick
from CMGTools.H2TauTau.officialJSONS import jsonMap

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers, embed_triggers

aliases = {
    '/VBF_HToTauTau.*START53.*':'HiggsVBF',
    '/VBFHToTauTau.*START53.*':'HiggsVBF',
    '/GluGluToHToTauTau.*START53.*':'HiggsGGH',
    '/WH_ZH_TTH_HToTauTau.*START53.*':'HiggsVH',
    '/DYJets.*START53.*':'DYJets',
    '/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph.*START53.*':'DY1Jets',
    '/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph.*START53.*':'DY2Jets',
    '/DY3JetsToLL_M-50_TuneZ2Star_8TeV-madgraph.*START53.*':'DY3Jets',
    '/DY4JetsToLL_M-50_TuneZ2Star_8TeV-madgraph.*START53.*':'DY4Jets',
    # '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START53_V9-v1.*':'WJets',
    '/WJets.*START53.*':'WJets',
    '/W1Jets.*START53.*':'W1Jets',
    '/W2Jets.*START53.*':'W2Jets',
    '/W3Jets.*START53.*':'W3Jets',
    '/W4Jets.*START53.*':'W4Jets',
    '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV.*START53.*':'TTJets',
    '/TTJets_FullLeptMGDecays_8TeV.*START53.*':'TTJetsFullLept',
    '/TTJets_SemiLeptMGDecays_8TeV.*START53.*':'TTJetsSemiLept',
    '/TTJets_HadronicMGDecays_8TeV.*START53.*':'TTJetsHadronic',
    '/TauPlusX/Run2012A-22Jan2013-v1.*':'data_Run2012A',
    '/TauPlusX/Run2012B-22Jan2013-v1.*':'data_Run2012B',
    '/TauPlusX/Run2012C-22Jan2013-v1.*':'data_Run2012C',
    '/TauPlusX/Run2012D-22Jan2013-v1.*':'data_Run2012D',
    '/T_tW-channel.*START53.*':'T_tW',
    '/Tbar_tW-channel.*START53.*':'Tbar_tW',
    '/WWJetsTo2L2Nu.*START53.*':'WWJetsTo2L2Nu',
    '/WZJetsTo2L2Q.*START53.*':'WZJetsTo2L2Q',
    '/WZJetsTo3LNu.*START53.*':'WZJetsTo3LNu',
    '/WW_TuneZ2star.*START53.*':'WW',
    '/WZ_TuneZ2star.*START53.*':'WZ',
    '/ZZ_TuneZ2star.*START53.*':'ZZ',
    '/ZZJetsTo2L2Nu.*START53.*':'ZZJetsTo2L2Nu',
    '/ZZJetsTo2L2Q.*START53.*':'ZZJetsTo2L2Q',
    '/ZZJetsTo4L.*START53.*':'ZZJetsTo4L',
    '/DoubleMu/StoreResults-Run2012A_22Jan2013_v1_PFembedded_trans1_tau116_ptmu1_16had1_18_v1.*':'embed_Run2012A_22Jan',
    '/DoubleMuParked/StoreResults-Run2012D_22Jan2013_v1_PFembedded_trans1_tau116_ptmu1_16had1_18_v1.*':'embed_Run2012B_22Jan',
    '/DoubleMuParked/StoreResults-Run2012C_22Jan2013_v1_PFembedded_trans1_tau116_ptmu1_16had1_18_v1.*':'embed_Run2012C_22Jan',
    '/DoubleMuParked/StoreResults-Run2012B_22Jan2013_v1_PFembedded_trans1_tau116_ptmu1_16had1_18_v1.*':'embed_Run2012D_22Jan',
    # '/DoubleMu/StoreResults-Run2012A_22Jan2013_v1_RHembedded_trans1_tau116_ptmu1_16had1_18_v1.*':'embed_Run2012A_22Jan',
    # '/DoubleMuParked/StoreResults-Run2012D_22Jan2013_v1_RHembedded_trans1_tau116_ptmu1_16had1_18_v1.*':'embed_Run2012B_22Jan',
    # '/DoubleMuParked/StoreResults-Run2012C_22Jan2013_v1_RHembedded_trans1_tau116_ptmu1_16had1_18_v1.*':'embed_Run2012C_22Jan',
    # '/DoubleMuParked/StoreResults-Run2012B_22Jan2013_v1_RHembedded_trans1_tau116_ptmu1_16had1_18_v1.*':'embed_Run2012D_22Jan',
    }


mc_ewk = []
mc_ewk += mc_dy
# mc_ewk += t_mc_ewk

MC_list = copy.copy( mc_ewk )

MC_list.extend( mc_higgs )
# MC_list.extend( mc_diboson ) 
    
allsamples = copy.copy( MC_list )
# allsamples.extend( data_list )
allsamples.extend( embed_list )

connect( allsamples, '%TAUMU_SVFitNoESTauESUp_Jul23_steggema', 'tauMu.*root', aliases, cache=True, verbose=False)

# Attach number of generated events for stitching
dy_nevents = [ DYJets.nGenEvents,
               DY1Jets.nGenEvents,
               DY2Jets.nGenEvents,
               DY3Jets.nGenEvents,
               DY4Jets.nGenEvents
               ]
for dy in mc_dy:
    dy.nevents = dy_nevents

# Attach number of generated events for stitching
w_nevents = [ WJets.nGenEvents,
               W1Jets.nGenEvents,
               W2Jets.nGenEvents,
               W3Jets.nGenEvents,
               W4Jets.nGenEvents
               ]
for w in mc_w:
    w.nevents = w_nevents


print [(s.name, s.dataset_entries) for s in allsamples]
for mc in MC_list:
    mc.triggers = mc_triggers
    # allsamples.append(mc)
for data in data_list:
    if len(data.files):
        data.json = jsonPick( data.files[0], jsonMap)
        data.triggers = data_triggers
        # allsamples.append(data)
for embed in embed_list:
    if len(embed.files):
        embed.json = jsonPick( embed.files[0], jsonMap)
        embed.triggers = embed_triggers
        # No trigger requirements for embedded samples
for c in allsamples:
    c.splitFactor = splitFactor(c, 5e4)
