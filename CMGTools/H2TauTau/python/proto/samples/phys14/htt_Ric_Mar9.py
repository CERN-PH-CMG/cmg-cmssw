from CMGTools.RootTools.utils.connect import connect
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

MC_list = [v for k, v in mc_dict.items()]
for sam in MC_list:
    # split as much as possible
    sam.splitFactor = splitFactor(sam, 10e4)

connect(MC_list, '%htt_6mar15_manzoni_nom', '.*root', aliases, cache=True, verbose=False)
