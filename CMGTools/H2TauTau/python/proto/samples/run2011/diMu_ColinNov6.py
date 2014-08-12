import itertools
import copy
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.H2TauTau.tools.jsonPick import jsonPick

from CMGTools.H2TauTau.proto.samples.run2011.data_dimuon import *
from CMGTools.H2TauTau.proto.samples.run2011.ewk import *
from CMGTools.H2TauTau.proto.samples.run2011.diboson import *

aliases = {
    '/DYJetsToLL_TuneZ2_M-50_7TeV.*START42.*/VertexWeight':'DYJets',
##     '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola.*START42.*':'WJets',
##     '/W1Jet.*START42.*':'W1Jets',
##     '/W2Jets.*START42.*':'W2Jets',
##     '/W3Jets.*START42.*':'W3Jets',
##     '/W4Jets.*START42.*':'W4Jets',
    '/DoubleMu/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_V5_6_0_B$':'data_Run2011B',
    '/DoubleMu/Run2011A-16Jan2012-v1/AOD/V5/PAT_CMG_V5_6_0_B$':'data_Run2011A',
    '/TTJets.*START42.*/VertexWeight':'TTJets',
    '/WWJetsTo2L2Nu.*START42.*/VertexWeight':'WWJetsTo2L2Nu',
    '/WZJetsTo2L2Q.*START42.*/VertexWeight':'WZJetsTo2L2Q',
    '/WZJetsTo3LNu.*START42.*/VertexWeight':'WZJetsTo3LNu',
    '/ZZJetsTo2L2Nu.*START42.*/VertexWeight':'ZZJetsTo2L2Nu',
    '/ZZJetsTo2L2Q.*START42.*/VertexWeight':'ZZJetsTo2L2Q',
    '/ZZJetsTo4L.*START42.*/VertexWeight':'ZZJetsTo4L',
    }


data_list = data_list_2011

MC_list = copy.copy( mc_ewk )
MC_list.extend( mc_diboson ) 
    
allsamples = copy.copy(MC_list)
allsamples.extend( data_list_2011 )

connect( allsamples, '%PAT_CMG_V5_6_0_B%', 'cmgTuple.*root', aliases, cache=True, verbose=False)

for c in allsamples:
    c.splitFactor = splitFactor(c, nEventsPerJob=2e5)

for data in data_list:
    if len(data.files):
        data.json = jsonPick( data.files[0])


