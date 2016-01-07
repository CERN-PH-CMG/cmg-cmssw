import PhysicsTools.HeppyCore.framework.config as cfg
import os

#Load backgrounds from common place
from CMGTools.RootTools.samples.samples_13TeV_RunIISpring15MiniAODv2 import *

####
####

TTs = [ TTJets, TTJets_SingleLeptonFromTbar, TTJets_SingleLeptonFromTbar_ext, TTJets_SingleLeptonFromT, TTJets_SingleLeptonFromT_ext]+SingleTop


background = TTs+DYJetsM50HT+WJetsToLNuHT+ZJetsToNuNuHT+GJetsHT+QCDHT+QCDPt+DiBosons


#background_50ns = [TTJets_50ns,TTJets_LO_50ns,WJetsToLNu_50ns]+QCDPt_50ns+DYJetsM50HT_50ns+DiBosons_50ns
#Load signal from here 
from CMGTools.VVResonances.samples.signal_13TeV_74X import *

mcSamples = background+signalSamples
#load triggers
from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *

#Load Data samples
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *




SingleMuon=[SingleMuon_Run2015D_Promptv4,SingleMuon_Run2015D_05Oct]
SingleElectron=[SingleElectron_Run2015D_Promptv4,SingleElectron_Run2015D_05Oct]
JetHT=[JetHT_Run2015D_Promptv4,JetHT_Run2015D_05Oct]
MET=[MET_Run2015D_Promptv4,MET_Run2015D_05Oct]




for s in SingleMuon:
    s.triggers = triggers_1mu_noniso+triggers_1mu_iso
    s.vetoTriggers = []
for s in SingleElectron:
    s.triggers = triggers_1e_noniso+triggers_1e
    s.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso
for s in JetHT:
    s.triggers = triggers_HT800+triggers_HT900+triggers_dijet_fat
    s.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso+triggers_1e_noniso+triggers_1e
for s in MET:
    s.triggers = triggers_met90_mht90+triggers_metNoMu90_mhtNoMu90+triggers_metNoMu120_mhtNoMu120
    s.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso+triggers_1e_noniso+triggers_1e+triggers_HT800+triggers_HT900+triggers_dijet_fat

dataSamples=SingleMuon+SingleElectron+JetHT+MET


from CMGTools.TTHAnalysis.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/VVResonances/data"


#Load JSON
jsonFile='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver.txt'
#Define splitting

for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250   
    comp.puFileMC=dataDir+"/pileup_MC.root"
    comp.puFileData=dataDir+"/pileup_DATA.root"
    comp.efficiency = eff2012
    comp.triggers=triggers_1mu_noniso+triggers_1mu_iso+triggers_1e+triggers_1e_noniso+triggers_HT800+triggers_HT900+triggers_dijet_fat+triggers_met90_mht90+triggers_metNoMu90_mhtNoMu90+triggers_metNoMu120_mhtNoMu120
    comp.globalTag = "Summer15_25nsV6_MC"

for comp in dataSamples:
    comp.splitFactor = 250
    comp.isMC = False
    comp.isData = True
    comp.json = jsonFile
    comp.globalTag = "Summer15_25nsV6_DATA"
