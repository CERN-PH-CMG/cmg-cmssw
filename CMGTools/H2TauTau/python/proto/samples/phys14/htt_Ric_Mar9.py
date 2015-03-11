# from CMGTools.RootTools.utils.connect import connect, connectSample
from CMGTools.RootTools.utils.splitFactor import splitFactor

from CMGTools.H2TauTau.proto.samples.phys14.higgs import mc_higgs
from CMGTools.H2TauTau.proto.samples.phys14.ewk import mc_ewk
from CMGTools.H2TauTau.proto.samples.phys14.diboson import mc_diboson
from CMGTools.H2TauTau.proto.samples.phys14.triggers_tauMu  import mc_triggers as mc_triggers_mt
from CMGTools.H2TauTau.proto.samples.phys14.triggers_tauEle import mc_triggers as mc_triggers_et
from CMGTools.H2TauTau.proto.samples.phys14.triggers_tauTau import mc_triggers as mc_triggers_tt
from CMGTools.H2TauTau.proto.samples.phys14.triggers_muEle  import mc_triggers as mc_triggers_em

aliases = {
    '/GluGluToHToTauTau.*Phys14DR.*' : 'HiggsGGH'         ,
    '/VBF_HToTauTau.*Phys14DR.*'     : 'HiggsVBF'         ,
    '/DYJetsToLL.*Phys14DR.*'        : 'DYJets'           ,
    '/TTJets.*Phys14DR.*'            : 'TTJets'           ,
    '/T_tW.*Phys14DR.*'              : 'T_tW'             ,
    '/Tbar_tW.*Phys14DR.*'           : 'Tbar_tW'          ,
    '/WZJetsTo3LNu.*Phys14DR.*'      : 'WZJetsTo3LNu'     ,
    '/TTbarH.*Phys14DR.*'            : 'HiggsTTHInclusive',
    '/WJetsToLNu.*Phys14DR.*'        : 'WJets'            ,
}

# dictionarize
mc_dict = {}
for s in mc_higgs + mc_ewk + mc_diboson:
    mc_dict[s.name] = s

from CMGTools.RootTools.utils.getFiles import getFiles
mc_dict['DYJets'              ].files = getFiles('/DYJetsToLL_M-50_13TeV-madgraph-pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM/htt_6mar15_manzoni_nom'                 , 'htautau_group', '.*root', useCache=True)
mc_dict['TTJets'              ].files = getFiles('/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM/htt_6mar15_manzoni_nom', 'htautau_group', '.*root', useCache=True)
mc_dict['T_tW'                ].files = getFiles('/T_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM/htt_6mar15_manzoni_nom'       , 'htautau_group', '.*root', useCache=True)
mc_dict['Tbar_tW'             ].files = getFiles('/Tbar_tW-channel-DR_Tune4C_13TeV-CSA14-powheg-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM/htt_6mar15_manzoni_nom'    , 'htautau_group', '.*root', useCache=True)
mc_dict['WZJetsTo3LNu'        ].files = getFiles('/WZJetsTo3LNu_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM/htt_6mar15_manzoni_nom'              , 'htautau_group', '.*root', useCache=True)
mc_dict['HiggsVBF125'         ].files = getFiles('/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM/htt_6mar15_manzoni_nom'           , 'htautau_group', '.*root', useCache=True)
mc_dict['HiggsGGH125'         ].files = getFiles('/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v1/MINIAODSIM/htt_6mar15_manzoni_nom'       , 'htautau_group', '.*root', useCache=True)
mc_dict['HiggsTTHInclusive125'].files = getFiles('/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM/htt_6mar15_manzoni_nom'         , 'htautau_group', '.*root', useCache=True)
mc_dict['WJets'               ].files = getFiles('/WJetsToLNu_13TeV-madgraph-pythia8-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM/htt_6mar15_manzoni_nom'               , 'htautau_group', '.*root', useCache=True)


MC_list = [v for k, v in mc_dict.items()]
for sam in MC_list:
    # split as much as possible
    sam.splitFactor = 100 #splitFactor(sam, 10e4)
    sam.fineSplitFactor = 4


'''
# connect(MC_list, '%htt_6mar15_manzoni_nom', '.*root', aliases, cache=True, verbose=False)

# connect to the CMGDB only when really necessary
import os
import pickle
import glob
# Ric: trick, else I get mad with jobs
from CMGTools.RootTools.utils.getFiles import getFiles

home = os.getenv('HOME')
tier       = 'htt_6mar15_manzoni_nom'
pattern    = '.*root'

for alias_k, alias_v in aliases.items():
    sample_pkl = '*'.join(['',alias_k.replace('/','').replace('.','*'),tier,pattern+'.pck'])
    cached_sample = glob.glob('/'.join([home,'.cmgdataset',sample_pkl]))
    if len(cached_sample) == 0:
        connect([v for k, v in mc_dict.items() if alias_v in k], '%'+tier, pattern, aliases, cache=True, verbose=False)
    elif len(cached_sample) >1:
        print 'better specify which sample, many found'
        print cached_sample
        raise
    else:
        file = open(cached_sample[0])
        mycomp = pickle.load(file)
        [v for k, v in mc_dict.items() if alias_v in k][0].files = getFiles('/'.join( ['']+mycomp.lfnDir.split('/')[mycomp.lfnDir.split('/').index('CMG')+1:] ), mycomp.user, pattern, useCache=True)
        [v for k, v in mc_dict.items() if alias_v in k][0].splitFactor = splitFactor(sam, 10e4)
        [v for k, v in mc_dict.items() if alias_v in k][0].fineSplitFactor = 3
        #connectSample([mycomp], [mycomp.dbsInstance,mycomp.name,mycomp.user], pattern, aliases, cache=True, verbose=False)
        #                         dataset_id, path_name, file_owner
'''
