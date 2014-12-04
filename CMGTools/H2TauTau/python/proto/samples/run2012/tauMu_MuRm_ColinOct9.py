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

walias = {
    '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_8_0/TAUMU_2012_TauIsoRel_ColinOct16':'WJets',
}
aliases = {
    '/DYJets.*':'DYJets',
    '/TauPlusX/Run2012A-13Jul2012.*':'data_Run2012A',
    '/TauPlusX/Run2012A-recover-06Aug2012-v1.*':'data_Run2012A_aug6',
    '/TauPlusX/Run2012B-13Jul2012.*':'data_Run2012B',
    '/TauPlusX/Run2012C-24Aug2012-v1.*':'data_Run2012C_v1',
    '/TauPlusX/Run2012C-PromptReco-v2.*':'data_Run2012C_v2'
    }

MC_list = [DYJets, WJets]
allsamples = copy.copy(MC_list)
allsamples.extend(data_list)

connect( [WJets], '/WJets%TAUMU_2012%', 'tauMu.*root', walias, cache=True, verbose=False)

# connect( allsamples, '%TAUMU_2012_MuRm_SVFit_Recoil_ZtoW_C_15Oct%', 'tauMu.*root', aliases, cache=True, verbose=False)
connect( allsamples, '%TAUMU_MuRm_OSVFit_Colin18Oct', 'tauMu.*root', aliases, cache=True, verbose=False)


allsamples = []
for mc in MC_list:
    mc.triggers = mc_triggers
    allsamples.append(mc)
for data in data_list:
    if len(data.files):
        data.json = jsonPick( data.files[0])
        data.triggers = data_triggers
        allsamples.append(data)
for c in allsamples:
    c.splitFactor = splitFactor(c)



## for c in data_list:
##     computeLumi(c, test=True)
