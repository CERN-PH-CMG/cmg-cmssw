import itertools
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.utils.connect import connect
from CMGTools.H2TauTau.proto.samples.run2011.triggers_tauMu import data_triggers_2011A, data_triggers_2011B, mc_triggers_fall11
from CMGTools.Production.getFiles import getFiles

from CMGTools.H2TauTau.proto.samples.run2011.higgs import *

MC = copy.copy( mc_higgs )
for sam in MC:
    sam.triggers = mc_triggers_fall11
    sam.triggers = ["HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1"]
    

allsamples = copy.copy(MC)

HiggsVBF120.files = getFiles('/H2TAUTAU/Sync/VBF/AOD/PAT_CMG_V5_5_0/TAUMU_SumEtFix_Colin_Jul20', 'cmgtools', '.*root')
HiggsGGH120.files = getFiles('/H2TAUTAU/Sync/VBF/AOD/PAT_CMG_V5_5_0/TAUMU_SumEtFix_Colin_Jul20', 'cmgtools', '.*root')

