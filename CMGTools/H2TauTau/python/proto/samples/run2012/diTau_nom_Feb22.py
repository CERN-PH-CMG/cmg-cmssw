import itertools
import copy
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor

from CMGTools.H2TauTau.proto.samples.run2012.data_diTau import * 
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs_susy import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_diTau import *

aliases = {
    '/VBF_.*START53.*'      :'HiggsVBF',
    '/GluGluTo.*START53.*'  :'HiggsGGH',
    '/SUSYBB.*START53.*'      :'HiggsSUSYBB',
    '/SUSYGluGluTo.*START53.*':'HiggsSUSYGluGlu',
    '/WH_ZH_TTH_.*START53.*':'HiggsVH',
    '/DYJets.*START53.*' :'DYJets',
    '/DY1Jets.*START53.*':'DY1Jets',
    '/DY2Jets.*START53.*':'DY2Jets',
    '/DY3Jets.*START53.*':'DY3Jets',
    '/DY4Jets.*START53.*':'DY4Jets',
    '/WJetsToLNu.*START53.*':'WJets',
    '/W1Jets.*START53.*'    :'W1Jets',
    '/W2Jets.*START53.*'    :'W2Jets',
    '/W3Jets.*START53.*'    :'W3Jets',
    '/W4Jets.*START53.*'    :'W4Jets',
    '/TTJets.*START53.*'         :'TTJets',
    '/T_tW-channel.*START53.*'   :'T_tW',
    '/Tbar_tW-channel.*START53.*':'Tbar_tW',
    '/Tau/Run2012A-13Jul2012-v1.*'         :'data_Run2011A_PromptReco_v1',
    '/Tau/Run2012A-recover-06Aug2012-v1.*' :'data_Run2011A_PromptReco_v1',
    '/Tau/Run2012B-13Jul2012-v1.*'         :'data_Run2011B_PromptReco_v1',
    '/Tau/Run2012C-24Aug2012-v1.*'         :'data_Run2011C_PromptReco_v1',
    '/Tau/Run2012C-PromptReco-v2.*'        :'data_Run2011C_PromptReco_v2',
    '/Tau/Run2012D-PromptReco-v1.*'        :'data_Run2011D_PromptReco_v1',    
    '/DoubleMu/StoreResults-DoubleMu_Run2012A_13Jul2012_v1.*'        :'embed_Run2012A_13Jul2012_v1',
    '/DoubleMu/StoreResults-DoubleMu_Run2012A-recover_06Aug2012_v1.*':'embed_Run2012A_recover_06Aug2012_v1',
    '/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4.*'        :'embed_Run2012B_13Jul2012_v4',
    '/DoubleMu/StoreResults-DoubleMu_Run2012C_24Aug2012_v1.*'        :'embed_Run2012C_24Aug2012_v1',
    '/DoubleMu/StoreResults-DoubleMu_Run2012C_PromptReco_v2.*'       :'embed_Run2012C_PromptReco_v2',
    '/DoubleMu/StoreResults-DoubleMu_2012D_PromptReco_v1.*'          :'embed_2012D_PromptReco_v1',
    '/WW_TuneZ2.*START53.*':'WW',
    '/WZ_TuneZ2.*START53.*':'WZ',
    '/ZZ_TuneZ2.*START53.*':'ZZ',
    '/WWJetsTo2L2Nu.*START53.*':'WWJetsTo2L2Nu',
    '/WZJetsTo2L2Q.*START53.*' :'WZJetsTo2L2Q',
    '/WZJetsTo3LNu.*START53.*' :'WZJetsTo3LNu',
    '/ZZJetsTo2L2Nu.*START53.*':'ZZJetsTo2L2Nu',
    '/ZZJetsTo2L2Q.*START53.*' :'ZZJetsTo2L2Q',
    '/ZZJetsTo4L.*START53.*'   :'ZZJetsTo4L',
    }

MC_list = copy.copy( mc_ewk )
MC_list.extend( mc_higgs )
MC_list.extend( mc_diboson )
MC_list.extend( mc_higgs_susy )

for sam in MC_list:
    sam.triggers = mc_triggers

for sam in data_2012 :
    if '2012A' in sam.name:
        sam.triggers = data_triggers_2012A
    elif '2012B' in sam.name:
        sam.triggers = data_triggers_2012B
    elif '2012C' in sam.name:
        sam.triggers = data_triggers_2012C
    elif '2012D' in sam.name:
        sam.triggers = data_triggers_2012D

allsamples1 = copy.copy( mc_higgs )
allsamples1.extend( ztt_mc_ewk )
allsamples1.extend( embed_list )
allsamples1.extend( mc_higgs_susy )

pat1 = '%HTT_Feb6_hinzmann_Nom'
connect( allsamples1, pat1, 'diTau.*root', aliases, cache=True, verbose=False)

allsamples2 = copy.copy( data_2012 )
allsamples2.extend( t_mc_ewk )
allsamples2.extend( mc_diboson_incl )
allsamples2.extend( w_mc_ewk )
pat2 = '%V5_8_0/HTT_Sep12_hinzmann'
connect( allsamples2, pat2, 'diTau.*root', aliases, cache=True, verbose=False)

allsamples = copy.copy(allsamples1)
allsamples.extend(allsamples2)

DYJets.nGenEvents /= 0.8 ## overall trigger efficiency in MC

for h in mc_higgs :
  h.nGenEvents /= 0.8 ## overall trigger efficiency in MC

for c in allsamples:
    c.splitFactor = splitFactor(c)

