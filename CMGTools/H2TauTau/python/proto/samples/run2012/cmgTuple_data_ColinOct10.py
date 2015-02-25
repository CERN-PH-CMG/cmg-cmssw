import copy
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.H2TauTau.proto.samples.computeLumi import computeLumi
from CMGTools.H2TauTau.tools.jsonPick import jsonPick

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
# from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
# from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers

aliases = {
    '/TauPlusX/Run2012A-13Jul2012.*':'data_Run2012A',
    '/TauPlusX/Run2012A-recover-06Aug2012-v1.*':'data_Run2012A_aug6',
    '/TauPlusX/Run2012B-13Jul2012.*':'data_Run2012B',
    '/TauPlusX/Run2012C-24Aug2012-v1.*':'data_Run2012C_v1',
    '/TauPlusX/Run2012C-PromptReco-v2.*':'data_Run2012C_v2'
    }

allsamples = copy.copy(data_list)

connect( allsamples, '%Run2012%PAT_CMG_V5_8_0', 'cmgTuple.*root', aliases, cache=True, verbose=False)

allsamples = []

for data in data_list:
    data.json = jsonPick( data.files[0])
    data.triggers = data_triggers
    allsamples.append(data)
for c in allsamples:
    c.splitFactor = splitFactor(c)



# for c in data_list:
#     computeLumi(c, test=True)
