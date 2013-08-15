import itertools
import copy
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.RootTools.json.jsonPick import jsonPick

# to use the DCS json files:
# from CMGTools.RootTools.json.dcsJSONS import dcsJsonMap as jsonMap
# to use the official certified json files:
from CMGTools.ZJetsTutorial.officialJSONS import jsonMap

from CMGTools.ZJetsTutorial.samples.run2012.ewk import *
from CMGTools.ZJetsTutorial.samples.run2012.diboson import *
from CMGTools.ZJetsTutorial.samples.run2012.data import *

# these aliases allow to match the CMG dataset matching the regexp pattern
# to an existing component in the allsamples list provided by the connect function. 
# you should make sure that only one dataset matching the SQL pattern below
# matches a given regexp pattern in the aliases dictionary. 
aliases = {
    '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball.*':'DYJets',
    '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV.*START53.*':'TTJets',
    '/WW_TuneZ2star_8TeV.*':'WW',
    '/WZ_TuneZ2star_8TeV.*':'WZ',
    '/ZZ_TuneZ2star_8TeV.*':'ZZ',
    '/DoubleMu/Run2012A-13Jul2012-v1':'data_Run2012A',
    '/DoubleMu/Run2012A-recover-06Aug2012-v1':'data_Run2012A_aug6',
    '/DoubleMu/Run2012B-13Jul2012-v4':'data_Run2012B',
    '/DoubleMu/Run2012C-24Aug2012-v1':'data_Run2012C_v1',
    '/DoubleMu/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_10_0_runrange_start-203002':'data_Run2012C_v2',
    }



MC_list = copy.copy( mc_ewk )
MC_list.extend( mc_diboson_inc ) 
    
allsamples = copy.copy(MC_list)
allsamples.extend( data_list )

# the pattern provided to this function is a SQL pattern.
# % represents the wildcard.
# this pattern allows to select a whole group of samples.
connect( allsamples, '%PAT_CMG_V5_10_0%', 'cmgTuple.*root', aliases, cache=True, verbose=False)



# guessing the best split factor for each component,
# to get about the same number of events in each job
for c in allsamples:
    c.splitFactor = splitFactor(c, nEventsPerJob=2e5)

# picking up the json file automatically. Here, we use the dcs jsons.
for data in data_list:
    if len(data.files):
        data.json = jsonPick( data.files[0], jsonMap )





