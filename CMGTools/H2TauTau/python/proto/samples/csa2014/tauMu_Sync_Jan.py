import copy

from CMGTools.Production.getFiles import getFiles

from CMGTools.H2TauTau.proto.samples.csa2014.higgs import mc_higgs


# dictionarize
mc_dict = {}
for s in mc_higgs:
    mc_dict[s.name] = s

MC_list = [mc_dict['HiggsVBF125']]
for sam in MC_list:
    sam.triggers = None # No triggers yet
    sam.triggers = [] # No triggers yet
    
allsamples = copy.copy(MC_list)

pat = '/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/Spring14dr-PU20bx25_POSTLS170_V5-v1/AODSIM/SS14/TAUMU_MINIAODTEST_steggema'

mc_dict['HiggsVBF125'].files = getFiles(pat, 'steggema', '.*root')
mc_dict['HiggsVBF125'].splitFactor = 14

