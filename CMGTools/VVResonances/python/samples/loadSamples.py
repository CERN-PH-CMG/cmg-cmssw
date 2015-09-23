import PhysicsTools.HeppyCore.framework.config as cfg
import os

#Load backgrounds from common place
from CMGTools.RootTools.samples.samples_13TeV_74X import *
background_25ns = QCDPt+WJetsToLNuHT+GJetsHT+TTs+DYJetsM50HT+DiBosons
background_50ns = [TTJets_50ns,TTJets_LO_50ns,WJetsToLNu_50ns]+QCDPt_50ns+DYJetsM50HT_50ns+DiBosons_50ns

#Load signal from here 
from CMGTools.VVResonances.samples.signal_13TeV_74X import signalSamples,RSGravToWWToLNQQ_kMpl01_2500

###
signalSamples=[RSGravToWWToLNQQ_kMpl01_2500]
###

mcSamples_25ns = background_25ns+signalSamples
mcSamples_50ns=background_50ns


mcSamples=mcSamples_25ns+mcSamples_50ns

#load triggers
from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *


#Load Data samples
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import JetHT_Run2015B,SingleElectron_Run2015B,SingleMuon_Run2015B

dataSamples_50ns= [JetHT_Run2015B,SingleElectron_Run2015B,SingleMuon_Run2015B]

#assign triggers to data 
SingleMuon_Run2015B.triggers = triggers_1mu_noniso+triggers_1mu_iso_50ns
SingleMuon_Run2015B.vetoTriggers = []

SingleElectron_Run2015B.triggers = triggers_1e_50ns+triggers_1e_noniso
SingleElectron_Run2015B.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso_50ns

JetHT_Run2015B.triggers = triggers_HT800+triggers_HT900+triggers_dijet_fat
JetHT_Run2015B.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso_50ns+triggers_1e_50ns+triggers_1e_noniso

from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import JetHT_Run2015C,SingleElectron_Run2015C,SingleMuon_Run2015C

dataSamples_25ns= [JetHT_Run2015C,SingleElectron_Run2015C,SingleMuon_Run2015C]

#assign triggers to data 
SingleMuon_Run2015C.triggers = triggers_1mu_noniso+triggers_1mu_iso
SingleMuon_Run2015C.vetoTriggers = []

SingleElectron_Run2015C.triggers = triggers_1e+triggers_1e_noniso
SingleElectron_Run2015C.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso

JetHT_Run2015C.triggers = triggers_HT800+triggers_HT900+triggers_dijet_fat
JetHT_Run2015C.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso+triggers_1e+triggers_1e_noniso

dataSamples=dataSamples_50ns+dataSamples_25ns



from CMGTools.TTHAnalysis.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"







#Load JSON
jsonFile_50ns = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON.txt'





#Define splitting
for comp in mcSamples_25ns:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 500   
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012
    comp.triggers=triggers_1mu_noniso+triggers_1mu_iso_50ns+triggers_1e_50ns+triggers_1e_noniso+triggers_HT800+triggers_HT900+triggers_dijet_fat
    comp.globalTag = "Summer15_25nsV2_MC"

for comp in mcSamples_50ns:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 500   
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012
    comp.triggers=triggers_1mu_noniso+triggers_1mu_iso_50ns+triggers_1e_50ns+triggers_1e_noniso+triggers_HT800+triggers_HT900+triggers_dijet_fat
    comp.globalTag = "Summer15_50nsV5_MC"




for comp in dataSamples_50ns:
    comp.splitFactor = 1000
    comp.isMC = False
    comp.isData = True
    comp.json = jsonFile_50ns
    comp.globalTag = "Summer15_50nsV5_DATA"

for comp in dataSamples_25ns:
    comp.splitFactor = 2000
    comp.isMC = False
    comp.isData = True
    comp.json = jsonFile_50ns
    comp.globalTag = "Summer15_25nsV2_DATA"

