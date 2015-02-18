import copy

# from CMGTools.Production.getFiles import getFiles
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor

from CMGTools.H2TauTau.proto.samples.phys14.higgs import mc_higgs

aliases = {
    '/GluGluToHToTauTau.*Phys14DR.*' : 'HiggsGGH',
    '/VBF_HToTauTau_M-125_13TeV-powheg-pythia6.*Phys14DR.*' : 'HiggsVBF',
}

# dictionarize
mc_dict = {}
for s in mc_higgs:
    mc_dict[s.name] = s

MC_list = [v for k, v in mc_dict.items()]#[mc_dict['HiggsGGH125'], mc_dict['HiggsVBF125']]
for sam in MC_list:
    # Test for mu-tau channel
    sam.triggers = ['HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v1', 'HLT_IsoMu24_eta2p1_IterTrk02_v1']
    

connect(MC_list, '%TAUMU_MINIAOD_SVFIT25ns_steggema', '.*root', aliases, cache=True, verbose=False)

MC_list = [m for m in MC_list if m.files]

for sample in MC_list:
    sample.splitFactor = splitFactor(sample, 10e4)

