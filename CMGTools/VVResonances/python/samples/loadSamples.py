import PhysicsTools.HeppyCore.framework.config as cfg
import os

#Load backgrounds from common place
from CMGTools.RootTools.samples.samples_13TeV_74X import QCDPt,WJetsToLNuHT,GJetsHT,TTs,TTJets_50ns,TTJets_LO_50ns,WJetsToLNu_50ns,QCDPt_50ns
background_25ns = QCDPt+WJetsToLNuHT+GJetsHT+TTs
background_50ns = [TTJets_50ns,TTJets_LO_50ns,WJetsToLNu_50ns]+QCDPt_50ns

#Load signal from here 
from CMGTools.VVResonances.samples.signal_13TeV_74X import signalSamples,RSGravToWWToLNQQ_kMpl01_2500

###
signalSamples=[RSGravToWWToLNQQ_kMpl01_2500]
###

mcSamples = background_25ns+background_50ns+signalSamples



#Load Data samples
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import JetHT_Run2015B,SingleElectron_Run2015B,SingleMuon_Run2015B

dataSamples = [JetHT_Run2015B,SingleElectron_Run2015B,SingleMuon_Run2015B]


from CMGTools.TTHAnalysis.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"



#load triggers
from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *


#assign triggers to data 
SingleMuon_Run2015B.triggers = triggers_1mu_noniso+triggers_1mu_iso_50ns
SingleMuon_Run2015B.vetoTriggers = []

SingleElectron_Run2015B.triggers = triggers_1e_50ns+triggers_1e_noniso
SingleElectron_Run2015B.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso_50ns

JetHT_Run2015B.triggers = triggers_HT800+triggersHT900+triggers_dijet_fat
JetHT_Run2015B.vetoTriggers = triggers_1mu_noniso+triggers_1mu_iso_50ns+triggers_1e_50ns+triggers_1e_noniso


#Load JSON
jsonFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-255031_13TeV_PromptReco_Collisions15_50ns_JSON.txt'




#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 500   
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012
    comp.triggers=triggers_1mu_noniso+triggers_1mu_iso_50ns+triggers_1e_50ns+triggers_1e_noniso+triggers_HT800+triggers_HT900+triggers_dijet_fat

for comp in dataSamples:
    comp.splitFactor = 2000
    comp.isMC = False
    comp.isData = True
    comp.json = jsonFile

