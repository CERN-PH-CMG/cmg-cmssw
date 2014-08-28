import copy

from CMGTools.RootTools.utils.connect import connect
from CMGTools.Production.getFiles import getFiles
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.splitFactor import splitFactor

from CMGTools.H2TauTau.proto.samples.csa2014.higgs import *

aliases = {
    '/VBF_HToTauTau.*POSTLS170.*':'HiggsVBF'
}

MC_list = [HiggsVBF125]

for sam in MC_list:
    sam.triggers = None # No triggers yet
    
allsamples = copy.copy(MC_list)


pat = '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_14_0/TAUMU_OSVFit_NoRecoil_May1'

connect( allsamples, '%TAUMU_MINIAODTEST_steggema', 'miniAOD*root', aliases, cache=True, verbose=False)
