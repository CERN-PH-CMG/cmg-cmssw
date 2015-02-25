import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.RootTools.json.jsonPick import jsonPick
from CMGTools.H2TauTau.officialJSONS import jsonMap

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers

aliases = {
    # '/WJets.*START53.*NewRecoil.*':'WJets',
    '/DYJets.*START53.*NewRecoil.*':'DYJets',
    '/DY1Jets.*START53.*NewRecoil.*':'DY1Jets',
    '/DY2Jets.*START53.*NewRecoil.*':'DY2Jets',
    '/DY3Jets.*START53.*NewRecoil.*':'DY3Jets',
    '/DY4Jets.*START53.*NewRecoil.*':'DY4Jets',
    # '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START53_V9-v1.*':'WJets',

    '/TauPlusX/Run2012A-22Jan2013-v1.*':'data_Run2012A',
    '/TauPlusX/Run2012B-22Jan2013-v1.*':'data_Run2012B',
    '/TauPlusX/Run2012C-22Jan2013-v1.*':'data_Run2012C',
    '/TauPlusX/Run2012D-22Jan2013-v1.*':'data_Run2012D',
    }


MC_list = copy.copy( mc_ewk )
MC_list.extend( mc_higgs )
MC_list.extend( mc_diboson ) 
    
allsamples = copy.copy( MC_list )
allsamples.extend( data_list )
allsamples.extend( embed_list )


connect( allsamples, '%TAUMU_MuRm_%Colin_%Jun13', 'tauMu.*root', aliases, cache=True, verbose=False)

connect( [WJets], '%TAUMU_NewRecoil_RelTauIso_Colin_1Jul13', 'tauMu.*root',
         {'/WJets.*START53.*NewRecoil.*':'WJets'}, cache=True, verbose=False)

dy_nevents = [ DYJets.nGenEvents,
               DY1Jets.nGenEvents,
               DY2Jets.nGenEvents,
               DY3Jets.nGenEvents,
               DY4Jets.nGenEvents
               ]
for dy in mc_dy:
    dy.nevents = dy_nevents



puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/Data_Pileup_2012_HCP-600bins.root'
    
for mc in MC_list:
    mc.triggers = mc_triggers
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData
    
for data in data_list:
    if len(data.files):
        data.json = jsonPick( data.files[0], jsonMap)
        data.triggers = data_triggers

for embed in embed_list:
    if len(embed.files):
        embed.json = jsonPick( data.files[0], jsonMap)
        # No trigger requirements for embedded samples

for c in allsamples:
    c.splitFactor = splitFactor(c, 5e4)

