# from CMGTools.Production.getFiles import getFiles
from CMGTools.RootTools.utils.connect import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor

from CMGTools.H2TauTau.proto.samples.phys14.higgs import mc_higgs
from CMGTools.H2TauTau.proto.samples.phys14.triggers_muEle import mc_triggers

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
  sam.triggers = mc_triggers

mc_dict['HiggsGGH125'].files = [
  '/afs/cern.ch/work/m/manzoni/diTau2015/CMSSW_7_2_3/src/CMGTools/H2TauTau/prod/muEle_fullsel_tree_CMG.root'   , 
                    ]

for sample in MC_list:
    sample.splitFactor = splitFactor(sample, 10e4)

allsamples = MC_list  
connect( allsamples, '%htt_tt_27jan15_manzoni_nom', 'miniAOD*root', aliases, cache=True, verbose=False)
