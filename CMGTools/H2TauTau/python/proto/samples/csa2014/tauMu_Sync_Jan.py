import copy

from CMGTools.Production.getFiles import getFiles

from CMGTools.H2TauTau.proto.samples.csa2014.higgs import *

MC_list = [HiggsVBF125]
for sam in MC_list:
    sam.triggers = None # No triggers yet
    
allsamples = copy.copy(MC_list)

pat = pat = '/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/Spring14dr-PU20bx25_POSTLS170_V5-v1/AODSIM/SS14/TAUMU_MINIAODTEST_steggema'

HiggsVBF125.files = getFiles(pat, 'steggema', '.*root')

HiggsVBF125.splitFactor = 14

