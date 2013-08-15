import os
from copy import copy
#####COMPONENT CREATOR
from CMGTools.HToZZTo4Leptons.samples.ComponentCreator import ComponentCreator
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath
from CMGTools.HToZZTo4Leptons.setup.FakeRates import *
from CMGTools.HToZZTo4Leptons.setup.Efficiencies import *

suffix='/SKIM_LEGACY_I'
userName='bachtis'
filePattern='skim.*root'



################### Triggers
triggers_ee   = ['HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
                 'HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v*',
                 'HLT_TripleEle10_CaloIdL_TrkIdVL_v*']


triggers_mumu = ["HLT_Mu17_Mu8_v*","HLT_DoubleMu7_v*","HLT_Mu13_Mu8_v*"]

triggers_mue   = [
              'HLT_Mu17_Ele8_CaloIdL_v*',
              'HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v*',
              'HLT_Mu8_Ele17_CaloIdL_v*',
              'HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v*'
              ]



triggersMC_mumu = ["HLT_Mu17_Mu8_v*"]

triggersMC_ee   = [
    'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
    'HLT_TripleEle10_CaloIdL_TrkIdVL_v*'
    ]



triggersMC_mue   = [
    'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
    'HLT_TripleEle10_CaloIdL_TrkIdVL_v*',
    "HLT_Mu17_Mu8_v*",
    "HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v*",
    "HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v*"
    ]




json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11//7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'



kreator = ComponentCreator()
mcSamples =kreator.makeMCComponentFromList(getFullPath('data/samples_MC_7TeV.txt'),suffix,userName,filePattern,triggersMC_mue)
dataSamples =kreator.makeDataComponentFromList(getFullPath('data/samples_DATA_7TeV.txt'),suffix,userName,filePattern,json,triggers_mumu,triggers_ee,triggers_mue)

dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/HToZZTo4Leptons/data"




from CMGTools.HToZZTo4Leptons.setup.FakeRates import *
from CMGTools.HToZZTo4Leptons.setup.Efficiencies import *


#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 10
    comp.puFileMC=dataDir+"/puProfile_Fall11.root"
    comp.puFileData=dataDir+"/puProfile_Data11.root"
    comp.efficiency = eff2011

    

for comp in dataSamples:
    comp.splitFactor = 100
    comp.fakeRates=fakeRates2011
    comp.isMC = False
    comp.isData = True
