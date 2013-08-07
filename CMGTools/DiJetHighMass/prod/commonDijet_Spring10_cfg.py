from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# changing process name, as PAT is already taken 
processName = 'SKIM'
runOnData = False
postfix="PFlow"
ext = 'MC'

import os
path = os.getcwd()
runAtFNAL = False

if path.find('uscms') > -1:
    runAtFNAL=True
    print "Run at FNAL"
else:
    print "Run probably at CERN"
    
process.setName_(processName)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000))

if (not runOnData):
    process.source.duplicateCheckMode = cms.untracked.string("noDuplicateCheck")


if runOnData:
    process.source.fileNames = cms.untracked.vstring('/store/data/Run2011A/HT/AOD/PromptReco-v1/000/161/312/FCDA9BD1-0358-E011-BB89-003048F024C2.root')
    if runAtFNAL:
        process.source.fileNames = cms.untracked.vstring('file:/uscms/home/mgouzevi/work/TEST_SAMPLES/Data_skim.root')
else:
    process.source.fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/D6929334-4247-DF11-8ECD-00215E93EC84.root')   
    if runAtFNAL:
        process.source.fileNames = cms.untracked.vstring('file:/uscms/home/mgouzevi/work/TEST_SAMPLES/Reco_RSGraviton_500_skim.root')



print 'processing: '
print process.source.fileNames

process.out.fileName = cms.untracked.string('AOD_diJetAnalysis_%s.root' % ext )

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("diJetAnalysis_Histograms_%s.root" % ext ) )


# Event Content
from CMGTools.DiJetHighMass.EventContent.commonDiJetAnalysis_cff import commonDiJetHighMass

process.out.outputCommands = commonDiJetHighMass

# Pat Sequence definition
from CMGTools.DiJetHighMass.myPatSequence_cff import myPatSequence
myPatSequence(process, runOnData)
process.load("Configuration.StandardSequences.Generator_cff")
process.gen = cms.Sequence(
    process.genJetMET
    )

if not runOnData:
	process.pat = cms.Sequence(
    	process.gen +	
    	process.patDefaultSequence
    	)
else :	
	process.pat = cms.Sequence(
        	process.patDefaultSequence
        )
	


# Analysis Sequence definition
process.load('CMGTools.DiJetHighMass.baseJets_cff')
process.load('CMGTools.DiJetHighMass.pfJets_cff')
process.load("CMGTools.Common.runInfoAccounting_cfi")

from CMGTools.DiJetHighMass.myJetSequence_cff import myJetSequence
myJetSequence(process)


process.analysisSequence = cms.Sequence(
    process.jetSequence
    )

if not runOnData:
	process.analysisSequence += process.runInfoAccounting


# Skimming
process.load('CMGTools.DiJetHighMass.skims.selHighMass_cff')

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoJets_cff')


############# turn-on the fastjet area calculation needed for the L1Fastjet ##############
############# applied only to PFJets because if CaloJets are re-recoed the JetID map will be lost #####
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)
process.ak7PFJets.doAreaFastjet = True
process.ak7PFJets.Rho_EtaMax = cms.double(5.0)
############# slimming the PFJet collection by raising the pt cut #################
process.ak5PFJets.jetPtMin = cms.double(10.0)
process.ak7PFJets.jetPtMin = cms.double(10.0)


process.QCDAnalysis = cms.Sequence(
############# first run the kt6PFJets reconstruction to calculate the fastjet density rho #####
process.kt6PFJets * 
############# then reconstruct the PFJets with jet area calculation ############
process.ak5PFJets *
process.ak7PFJets)



run36xOn35x = True






# Total process
process.p = cms.Path(
    process.QCDAnalysis +
    process.pat +
    process.analysisSequence +
    process.select
    )

process.out.SelectEvents.SelectEvents.append('p')

if runOnData:
	process.p.remove(process.patTriggerSequence)
	process.p.remove(process.patTriggerEventSequence)

if not runOnData:
	process.p.remove(process.btaggingAK5PF)
	process.p.remove(process.btaggingAK7PF)
	process.patJets.addTagInfos     = cms.bool(False)
	process.patJets.addBTagInfo     = cms.bool(False)
	getattr(process, "patJetsAK5PF").addTagInfos     = cms.bool(False)
	getattr(process, "patJetsAK7PF").addTagInfos     = cms.bool(False)
	getattr(process, "patJetsAK5PF").addBTagInfo     = cms.bool(False)
	getattr(process, "patJetsAK7PF").addBTagInfo     = cms.bool(False)
	

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
