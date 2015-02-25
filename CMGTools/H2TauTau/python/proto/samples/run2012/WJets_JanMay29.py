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
    '/WJets.*START53.*':'WJets',
    '/W1Jets.*START53.*':'W1Jets',
    '/W2Jets.*START53.*':'W2Jets',
    '/W3Jets.*START53.*':'W3Jets',
    '/W4Jets.*START53.*':'W4Jets',
    }


MC_list = copy.copy( mc_ewk )
MC_list.extend( mc_higgs )
MC_list.extend( mc_diboson ) 
    
allsamples = copy.copy( MC_list )
allsamples.extend( data_list )
allsamples.extend( embed_list )


connect( allsamples, '/W%Jet%PAT_CMG_V5_14_0', 'cmgTuple.*root', aliases, cache=True, verbose=False)

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
for c in allsamples:
    c.splitFactor = splitFactor(c, 5e4)
