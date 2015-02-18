import copy

from CMGTools.H2TauTau.proto.samples.phys14.higgs import *

from CMGTools.Production.getFiles         import getFiles
from CMGTools.RootTools.utils.connect     import connect
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.RootTools.fwlite.Config     import printComps

aliases = {
  '/GluGluToHToTauTau.*Phys14DR.*' : 'HiggsGGH',
          }

MC_list = [ HiggsGGH125 ]

for sam in MC_list:
  sam.triggers = ['HLT_Ele22_eta2p1_WP85_Gsf_LooseIsoPFTau20_v1','HLT_Ele27_eta2p1_WP85_Gsf_v1']
  
# HLT_Ele27_eta2p1_WP85_Gsf_v1
# HLT_Ele32_eta2p1_WP85_Gsf_v1
  
allsamples = copy.copy(MC_list)
connect( allsamples, '%htt_tt_27jan15_manzoni_nom', 'miniAOD*root', aliases, cache=True, verbose=False)

HiggsGGH125.files = [
  '/afs/cern.ch/work/m/manzoni/diTau2015/CMSSW_7_2_3/src/CMGTools/H2TauTau/prod/tauEle_fullsel_tree_CMG.root'  , 
                    ]