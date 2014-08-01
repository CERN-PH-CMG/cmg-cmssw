import itertools
import copy
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.H2TauTau.tools.jsonPick import jsonPick
from CMGTools.H2TauTau.proto.samples.run2011.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers

from CMGTools.H2TauTau.proto.samples.run2011.ewk import DYJets as DYJetsMuRm
from CMGTools.H2TauTau.proto.samples.run2011.ewk import WJets 


DYJetsMuRm.name = 'DYJetsMuRm'

data_Run2011A_MuRm = cfg.DataComponent(
    name = 'data_Run2011A_MuRm',
    files = [],
    intLumi = 0.,
    triggers = []
    )

data_Run2011B_MuRm = cfg.DataComponent(
    name = 'data_Run2011B_MuRm',
    files = [],
    intLumi = 0.,
    triggers = []
    )

aliases = {
    '/DYJets.*START42.*':'DYJetsMuRm',
    '/WJetsToLNu.*START42.*':'WJets',
    '/DoubleMu/Run2011A-16Jan2012-v1/AOD.*':'data_Run2011A_MuRm',
    '/DoubleMu/Run2011B-16Jan2012-v1/AOD.*':'data_Run2011B_MuRm',
    }



    
allsamples = [DYJetsMuRm, WJets, data_Run2011A_MuRm, data_Run2011B_MuRm]

# connect( allsamples, '%TAUMU_%MuRm_ColinSep28', 'tauMu.*root', aliases, cache=True, verbose=False)
connect( allsamples, '%TAUMU_MuRm_rlxTauIso_ColinOct6', 'tauMu.*root', aliases, cache=True, verbose=False)

MC_list = [DYJetsMuRm, WJets]
data_list_2011 = [data_Run2011A_MuRm, data_Run2011B_MuRm]
embed_list = []

for mc in MC_list:
    mc.triggers = mc_triggers
for data in data_list_2011:
    data.json = jsonPick( data.files[0] )
    
