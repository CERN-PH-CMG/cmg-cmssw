import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.H2TauTau.proto.samples.connect import connect
from CMGTools.H2TauTau.proto.samples.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11

from CMGTools.H2TauTau.proto.samples.ewk import *
from CMGTools.H2TauTau.proto.samples.embed import *
from CMGTools.H2TauTau.proto.samples.data import *
from CMGTools.H2TauTau.proto.samples.higgs import *

MC = copy.copy( mc_ewk )
MC.extend( mc_higgs ) 
# MC.extend( mc_diboson ) 
for sam in MC:
    sam.triggers = mc_triggers_fall11
for data in data_2011A:
    data.triggers = data_triggers_2011A
for data in data_2011B:
    data.triggers = data_triggers_2011A
    

allsamples = copy.copy(MC)
allsamples.extend( data_2011 )
allsamples.extend( embed_2011 )

connect( allsamples, '%TAUMU_Jul3_RelTauPt', 'tauMu.*root', cache=True)

TTJets.nGenEvents = 59613991 * 0.1790225 / 0.179171 * 0.99962399999999996
WJets.nGenEvents = 81345381 * 0.0236105 / 0.023636999999999998 * 0.99954999999999994


