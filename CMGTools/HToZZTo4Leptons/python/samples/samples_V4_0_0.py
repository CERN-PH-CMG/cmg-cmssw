from CMGTools.HToZZTo4Leptons.samples.getFiles import getFiles
from CMGTools.HToZZTo4Leptons.samples.mc_higgs import *
from CMGTools.HToZZTo4Leptons.samples.mc_big import *
from CMGTools.HToZZTo4Leptons.samples.mc_zz import *

pat = 'PAT_CMG_Test_444'
filePattern = 'tree.*root'

triggers_fall11_mu_mu = ["HLT_Mu17_Mu8_v*"] 
triggers_fall11_ele_ele = ["HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"]
triggers_fall11_mu_ele = ["HLT_Mu17_Mu8_v*",
                          "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"] 


Hig120GluGlu.files = getFiles('/GluGluToHToZZTo4L_M-120_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4/{pat}'.format(pat=pat), 'botta', filePattern)

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4/PAT_CMG_4_0_0', 'cmgtools', filePattern)
DYJets.nGenEvents = 31843769


#/ZZTo4mu_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0 ||    2.0 ||  954860
ZZTo4mu.files = getFiles('/ZZTo4mu_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0',
                         'cmgtools', filePattern)
ZZTo4mu.nGenEvents = 954860 / 2.


#/ZZTo4e_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0 ||     1.932798 ||     966252
ZZTo4e.files = getFiles('/ZZTo4e_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0',
                         'cmgtools', filePattern)
ZZTo4e.nGenEvents = 966252 * 1.932798 / 2.


#/ZZTo4tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0 ||   2.0 ||  998052
ZZTo4tau.files = getFiles('/ZZTo4tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0',
                          'cmgtools', filePattern)
ZZTo4tau.nGenEvents = 998052 / 2.


#/ZZTo2e2mu_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0 ||  2.0 ||  748680
ZZTo2e2mu.files = getFiles('/ZZTo2e2mu_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0',
                          'cmgtools', filePattern)
ZZTo2e2mu.nGenEvents = 748680 / 2.


#/ZZTo2e2tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0 || 2.0 ||  987236
ZZTo2e2tau.files = getFiles('/ZZTo2e2tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0',
                          'cmgtools', filePattern)
ZZTo2e2tau.nGenEvents = 987236 / 2.


#/ZZTo2mu2tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0 ||        2.0 ||  999854
ZZTo2mu2tau.files = getFiles('/ZZTo2mu2tau_7TeV-powheg-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V4/PAT_CMG_4_0_0',
                          'cmgtools', filePattern)
ZZTo2mu2tau.nGenEvents = 999854 / 2.

Fall11 = [ Hig120GluGlu,
           DYJets ]
Fall11.extend( mc_zz )
