import FWCore.ParameterSet.Config as cms
import os, sys

## using VarParsing taken from here:
## https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideAboutPythonConfigFile#Passing_Command_Line_Arguments_T
import FWCore.ParameterSet.VarParsing as VarParsing


process = cms.Process("TOP")


# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')



# setup any defaults you want
options.maxEvents = -1 
## options.outputFile = '/uscms/home/cplager/nobackup/outputFiles/try_3.root'
## options.inputFiles= 'file1.root', 'file2.root'

# add additional inputs
options.register ('sampleLocation',
                  '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Location of the sample")

options.register ('sampleName',
                  'ttbar', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Name of the sample")

## options.register ('directory',
##                   'cmgtools', # default value
##                   VarParsing.VarParsing.multiplicity.singleton, # singleton or list
##                   VarParsing.VarParsing.varType.string,          # string, int, or float
##                   "Name of the directory")



# get and parse the command line arguments
options.parseArguments()

## check if data
if 'Run201' in options.sampleLocation:
    runOnData = True
else:
    runOnData = False


lumiFile = 'None'


# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )



print "\n Sample locations:"
print options.sampleLocation

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource('cmgtools', options.sampleLocation, 'cmgTuple_.*root')
 

#print "WARNING!!!!!!!!!!!!!!!! remove the following line (see .cfg) before running on the batch!"
#process.source.fileNames = [process.source.fileNames[0]]

print "\n Processing files:"
print process.source.fileNames

print "\n Processing events:"
print process.maxEvents


#https://twiki.cern.ch/twiki/bin/view/CMS/PdmV2012Analysis#DATA
if runOnData:
   if '13Jul2012' in options.sampleLocation:
      lumiFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt'
   if '06Aug2012' in options.sampleLocation:
      lumiFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt'
   if '24Aug2012' in options.sampleLocation:
      lumiFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt'
   if 'Run2012C-PromptReco-v2' in options.sampleLocation:
      lumiFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON_v2.txt'
   if '11Dec2012' in options.sampleLocation:
      lumiFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_201191-201191_8TeV_11Dec2012ReReco-recover_Collisions12_JSON.txt'
   if 'Run2012D-PromptReco-v1' in options.sampleLocation:
      lumiFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON_PixAlignRecovery.txt'


print "\n Using json file:"
print lumiFile
print "\n"

 
if runOnData:
   from CMGTools.Common.Tools.applyJSON_cff import *
   applyJSON(process, lumiFile)



from CMGTools.Common.skims.cmgMuonSel_cfi import *
from CMGTools.Common.skims.cmgElectronSel_cfi import *
from CMGTools.Common.skims.cmgTauSel_cfi import *
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
from CMGTools.Common.skims.cmgDiMuonSel_cfi import *
from CMGTools.Common.skims.cmgWMuNuSel_cfi import *
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *




process.primaryVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),    
   filter = cms.bool(True),							    # otherwise it won't filter the events, just produce an empty vertex collection.
   debugOn = cms.untracked.bool(True),
   )

# Filters defined in CMGTools/Common/python/PAT/addFilterPaths_cff.py
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.scrapingFilter = hltHighLevel.clone()
process.scrapingFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","PAT")
process.scrapingFilter.andOr = cms.bool(False)
process.scrapingFilter.HLTPaths = cms.vstring('noscrapingFilterPath')

process.metFilter = hltHighLevel.clone()
process.metFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","PAT")
process.metFilter.andOr = cms.bool(False)
process.metFilter.HLTPaths = cms.vstring('HBHENoiseFilterPath', 'CSCTightHaloFilterPath', 'hcalLaserEventFilterPath', 'EcalDeadCellTriggerPrimitiveFilterPath', 'trackingFailureFilterPath', 'eeBadScFilterPath')
 

# Check triggers here: http://fwyzard.web.cern.ch/fwyzard/hlt/2012/summary
#Path HLT_Ele27_WP80:
# - first seen online on run 190456 (/cdaq/physics/Run2012/5e33/v4.4/HLT/V5)
# - last  seen online on run 209151 (/cdaq/special/25ns/v1.1/HLT/V2)
# - V8: (runs 190456 - 190738) seeded by: L1_SingleEG20 OR L1_SingleEG22
# - V9: (runs 190782 - 191411) seeded by: L1_SingleEG20 OR L1_SingleEG22
# - V10: (runs 191691 - 196531) seeded by: L1_SingleEG20 OR L1_SingleEG22
# - V11: (runs 198022 - 209151) seeded by: L1_SingleEG20 OR L1_SingleEG22
#Path HLT_IsoMu24_eta2p1:
# - first seen online on run 190456 (/cdaq/physics/Run2012/5e33/v4.4/HLT/V5)
# - last  seen online on run 209151 (/cdaq/special/25ns/v1.1/HLT/V2)
# - V11: (runs 190456 - 190738) seeded by: L1_SingleMu16er
# - V12: (runs 190782 - 193621) seeded by: L1_SingleMu16er
# - V13: (runs 193834 - 196531) seeded by: L1_SingleMu16er
# - V14: (runs 198022 - 199608) seeded by: L1_SingleMu16er
# - V15: (runs 199698 - 209151) seeded by: L1_SingleMu16er

muTriggerList  = "( getSelectionRegExp(\"HLT_IsoMu24_eta2p1_v*\") )"   
eleTriggerList = "( getSelectionRegExp(\"HLT_Ele27_WP80_v*\")	  )"   

#if runOnData:
#    muTriggerList  = "( getSelectionRegExp(\"HLT_IsoMu20_eta2p1_TriCentralPFJet30_v*\") || getSelectionRegExp(\"HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet30_v*\") || getSelectionRegExp(\"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v*\") || getSelectionRegExp(\"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v*\") || getSelectionRegExp(\"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet45_35_25_v*\")  )"   
#    eleTriggerList = "( getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30_v*\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_v*\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_v*\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20_v*\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet45_35_25_v*\") )"   
#else:
#    muTriggerList  = "( getSelectionRegExp(\"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet50_40_30_v1\") )"
#    eleTriggerList = "( getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet50_40_30_v5\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet50_40_30_v1\") )"


process.cmgTopEleTriggerSel = cmgTriggerObjectSel.clone(
   src = 'cmgTriggerObjectSel',
   cut = cms.string( eleTriggerList ),
   filter = cms.bool(True),
   debugOn = cms.untracked.bool(True),
   )

process.cmgTopMuoTriggerSel = cmgTriggerObjectSel.clone(
   src = 'cmgTriggerObjectSel',
   cut = cms.string( muTriggerList ),
   filter = cms.bool(True),
   debugOn = cms.untracked.bool(True),
   )



#######################################
#### Muon + jets selection
#######################################

# Apply top tight muon selection (as VBTF with looser dxy) + db() cut + kinematics + isolation
## Muon definitions in CMGTools/Common/python/selections/muonIDs_cfi.py
tightMuon = "(isPF() && isGlobal() && pt()>26 && abs(eta())<2.1 && normalizedChi2()<10  && trackerLayersWithMeasurement()>5  && numberOfValidMuonHits()>0 && abs(dxy())<0.02  && abs(dz())<0.5 && relIso(0.5, 0, 0.4)<0.12 && numberOfValidPixelHits()>0 && numberOfMatches()>1)"
process.cmgTopTightMuonMuJetSel = cmgMuonSel.clone(
   src = 'cmgMuonSel',
   cut = cms.string( tightMuon ),
   )
process.oneTightMuonMuJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (1),
     maxNumber = cms.uint32  (1),
     src       = cms.InputTag("cmgTopTightMuonMuJetSel"),
)


# Apply selection for loose muon veto
process.cmgTopLooseMuonMuJetSel = cmgMuonSel.clone(
   src = 'cmgMuonSel',
   cut = cms.string( "isPF() && (isGlobal() || isTracker()) && pt()>10 && abs(eta())<2.5 && relIso(0.5, 0, 0.4)<0.2 && !" + tightMuon  )
   )
process.zeroLooseMuonsMuJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (0),
     maxNumber = cms.uint32  (0),
     src       = cms.InputTag("cmgTopLooseMuonMuJetSel"),
)


#Apply electron selection for electron veto
process.cmgTopLooseElecMuJetSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string( "pt()>20 && abs(eta())<2.5 && relIso(0.5, 0, 0.3)<0.15 && mvaTrigV0()>0.0")
   )
process.zeroLooseElecsMuJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (0),
     maxNumber = cms.uint32  (0),
     src       = cms.InputTag("cmgTopLooseElecMuJetSel"),
)


# Apply deltaR jet-muon cleaning 
# http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/Common/plugins/DeltaRVetoProducer.cc?revision=1.3&view=markup
# Producer that makes a copy of the inputCollection, vetoing objects that lie within minDeltaR of *any* object in the vetoCollection.
# Algorithm is order N^2, so please use reduced collections as input
process.cleanMuonJets = cms.EDProducer("DeltaRVetoProducerPFJet",
   inputCollection = cms.InputTag('cmgPFJetSelCHS'),
   MatchingParams = cms.VPSet(cms.PSet( 				    
      vetoCollection=cms.InputTag("cmgTopTightMuonMuJetSel"),
      minDeltaR=cms.double(0.3),
      removeMatchedObject=cms.bool(True)
      )
   ),
   verbose = cms.untracked.bool(False)
   )


#Apply Jet selection: loose jet Id (from Common/python/selections/jetid_cfi.py and CMGTools/Common/src/PFJetFactory.cc) and kinematics
process.cmgTopJetMuJetSel     = cmgPFJetSel.clone(
   src = 'cleanMuonJets',
   cut = cms.string( " pt()>15 && abs(eta())<2.5 && abs(phi()) < 3.2  &&  getSelection(\"cuts_looseJetId\") " )
   )
process.fourJetsMuJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (4),
     maxNumber = cms.uint32  (9999),
     src       = cms.InputTag("cmgTopJetMuJetSel"),
)




#######################################
#### Electron + jets selection
#######################################

tightElectron = "(pt()>30 && abs(eta())<2.5&& (abs(sourcePtr().superCluster().eta())>1.5660 || abs(sourcePtr().superCluster().eta())<1.4442) && abs(dxy())<0.02 && passConversionVeto()==1 && mvaTrigV0()>0.0 && numberOfHits()<=0 && relIso(0.5, 0, 0.3)<0.1)"
process.cmgTopTightElecEleJetSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string(tightElectron)
   ) 
process.oneTightElecEleJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (1),
     maxNumber = cms.uint32  (1),
     src       = cms.InputTag("cmgTopTightElecEleJetSel"),
)


process.cmgTopLooseElecEleJetSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string( "pt()>20 && abs(eta())<2.5 && relIso(0.5, 0, 0.3)<0.15 && mvaTrigV0()>0.0&& !" + tightElectron)
   )
process.zeroLooseElecsEleJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (0),
     maxNumber = cms.uint32  (0),
     src       = cms.InputTag("cmgTopLooseElecEleJetSel"),
)


process.cmgTopLooseMuonEleJetSel = cmgMuonSel.clone(
   src = 'cmgMuonSel',
   cut = cms.string( "isPF() && (isGlobal() || isTracker()) && pt()>10 && abs(eta())<2.5 && relIso(0.5, 0, 0.4)<0.2" )
   )
process.zeroLooseMuonsEleJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (0),
     maxNumber = cms.uint32  (0),
     src       = cms.InputTag("cmgTopLooseMuonEleJetSel"),
)


process.cleanElectronJets = cms.EDProducer("DeltaRVetoProducerPFJet",
   inputCollection = cms.InputTag('cmgPFJetSelCHS'),
   MatchingParams = cms.VPSet(cms.PSet( 				    
      vetoCollection=cms.InputTag("cmgTopTightElecEleJetSel"),
      minDeltaR=cms.double(0.3),
      removeMatchedObject=cms.bool(True)
      )
   ),
   verbose = cms.untracked.bool(False)
   )


process.cmgTopJetEleJetSel = cmgPFJetSel.clone(
   src = 'cleanElectronJets',
   cut = cms.string( " pt()>15 && abs(eta())<2.5 && abs(phi()) < 3.2  &&  getSelection(\"cuts_looseJetId\") " )
   )
process.fourJetsEleJetSel = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (4),
     maxNumber = cms.uint32  (9999),
     src       = cms.InputTag("cmgTopJetEleJetSel"),
)


# Run the weights for PU
process.load('CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff')
#process.genSequence = cms.Sequence(
#   process.vertexWeightSequence 
#)

#you need to do this: cvs up -d -A CMGTools/RootTools/data/vertexWeight/Pileup_2012ABCD.true.root
from CMGTools.RootTools.utils.vertexWeight.vertexWeights2012_cfi import *
process.vertexWeight2012ABCDtrue = cms.EDProducer(
    "PileUpWeightProducer",
    verbose = cms.untracked.bool( False ),
    src = cms.InputTag('addPileupInfo'),
    type = cms.int32(2),
    inputHistMC   = cms.string( '../../../CMGTools/RootTools/data/vertexWeight/Pileup_Summer12MC53X.true.root'),
    inputHistData = cms.string( '../../../CMGTools/RootTools/data/vertexWeight/Pileup_2012ABCD.true.root'),
    )


from CMGTools.Common.physicsObjectPrinter_cfi import *
process.printerTTbar = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("cmgTriggerObjectSel"),
    #inputCollection = cms.untracked.InputTag("cmgTopMuonSel"),
    #inputCollection = cms.untracked.InputTag("cmgTopPFJetSel"),
    printSelections = cms.untracked.bool(True),
    reportEvery     = cms.untracked.uint32(500)
)


# Produce PDF weights (maximum is 3)
# http://cmslxr.fnal.gov/lxr/source/ElectroWeakAnalysis/Utilities/src/PdfWeightProducer.cc
# http://cmslxr.fnal.gov/lxr/source/ElectroWeakAnalysis/Utilities/test/PdfSystematicsAnalyzer.py
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
      PdfInfoTag = cms.untracked.InputTag("generator"),
      PdfSetNames = cms.untracked.vstring(
	      "cteq66.LHgrid", # 44 members
	      #"cteq65.LHgrid", # 21 members
	      #"MRST2006nnlo.LHgrid", # 31 members
	      #"MRST2007lomod.LHgrid" # 1 member
      )
)


######################################################################################################################################
## std sequence to produce the kinematic fit for semi-leptonic events
process.load("TopQuarkAnalysis.TopHitFit.TtSemiLepHitFitProducer_Muons_cfi")
process.hitFitTtSemiLepEventMJ = process.hitFitTtSemiLepEvent.clone(
   jets	     = cms.InputTag("cmgTopJetMuJetSel"	     , "", ""),
   leps	     = cms.InputTag("cmgTopTightMuonMuJetSel", "", ""),
   mets	     = cms.InputTag("cmgPFMET"		     , "", ""),
   maxNJets  = cms.int32(4),  # maximum number of jets to be considered in the jet combinatorics (has to be >= 4, can be set to -1 if you want to take all) 
   maxNComb  = cms.int32(-1), # maximum number of jet combinations finally written into the event, starting from the "best" (has to be >= 1, can be set to -1 if you want to take all)
   #bTagAlgo  = cms.string("trackCountingHighEffBJetTags")
)

process.load("TopQuarkAnalysis.TopHitFit.TtSemiLepHitFitProducer_Electrons_cfi")
process.hitFitTtSemiLepEventEJ = process.hitFitTtSemiLepEvent.clone(
   jets	     = cms.InputTag("cmgTopJetEleJetSel"      , "", ""),
   leps	     = cms.InputTag("cmgTopTightElecEleJetSel", "", ""),
   mets	     = cms.InputTag("cmgPFMET"		      , "", ""),
   maxNJets  = cms.int32(4),  # maximum number of jets to be considered in the jet combinatorics (has to be >= 4, can be set to -1 if you want to take all) 
   maxNComb  = cms.int32(-1), # maximum number of jet combinations finally written into the event, starting from the "best" (has to be >= 1, can be set to -1 if you want to take all)
)


print process.primaryVertexFilter.dumpPython() 	 
print process.scrapingFilter.dumpPython() 	 
print process.metFilter.dumpPython() 	 

#print process.cmgTopMuoTriggerSel.dumpPython() 	 
print process.cmgTopTightMuonMuJetSel.dumpPython() 		 		 
print process.cmgTopLooseMuonMuJetSel.dumpPython() 	 	 
print process.cmgTopLooseElecMuJetSel.dumpPython() 	 	 
print process.cmgTopJetMuJetSel.dumpPython()   	 	 
print process.oneTightMuonMuJetSel.dumpPython()
print process.zeroLooseMuonsMuJetSel.dumpPython()
print process.zeroLooseElecsMuJetSel.dumpPython()
print process.fourJetsMuJetSel.dumpPython()
print process.hitFitTtSemiLepEventEJ.dumpPython()


#print process.cmgTopEleTriggerSel.dumpPython() 	 
print process.cmgTopTightElecEleJetSel.dumpPython() 		 		 
print process.cmgTopLooseMuonEleJetSel.dumpPython() 	 	 
print process.cmgTopLooseElecEleJetSel.dumpPython() 	 	 
print process.cmgTopJetEleJetSel.dumpPython()   	 	 
print process.oneTightElecEleJetSel.dumpPython()
print process.zeroLooseElecsEleJetSel.dumpPython()
print process.zeroLooseMuonsEleJetSel.dumpPython()
print process.fourJetsEleJetSel.dumpPython()
print process.hitFitTtSemiLepEventMJ.dumpPython()

print process.pdfWeights.dumpPython()

process.pEle = cms.Path( 
   process.primaryVertexFilter      +
   process.scrapingFilter           + 	 
   process.metFilter                + 	 
   process.cmgTopTightElecEleJetSel +		    
   process.cmgTopLooseMuonEleJetSel +	    
   process.cmgTopLooseElecEleJetSel +	    
   process.cleanElectronJets        + 
   process.cmgTopJetEleJetSel       +       	    	    
   process.cmgTopEleTriggerSel      +
   process.oneTightElecEleJetSel    +       	    	    
   process.zeroLooseElecsEleJetSel  +       	    	    
   process.zeroLooseMuonsEleJetSel  +       	    	    
   process.fourJetsEleJetSel        +   	    	    
   process.hitFitTtSemiLepEventEJ        	       	    
   )

process.pMu = cms.Path( 
   process.primaryVertexFilter      +
   process.scrapingFilter           + 	 
   process.metFilter                + 	 
   process.cmgTopTightMuonMuJetSel  +		    
   process.cmgTopLooseMuonMuJetSel  +	    
   process.cmgTopLooseElecMuJetSel  +	    
   process.cleanMuonJets            + 
   process.cmgTopJetMuJetSel        +	       	    
   process.cmgTopMuoTriggerSel      +
   process.oneTightMuonMuJetSel     +	       	    
   process.zeroLooseElecsMuJetSel   +	       	    
   process.zeroLooseMuonsMuJetSel   +	       	    
   process.fourJetsMuJetSel         +       	       	    
   process.hitFitTtSemiLepEventMJ        	       	    
   )

if not runOnData:
   process.pEle += process.vertexWeightSequence
   process.pEle += process.vertexWeight2012ABCDtrue
   process.pMu  += process.vertexWeightSequence
   process.pMu  += process.vertexWeight2012ABCDtrue

# Calculate the PDF weights only for the central ttbar sample (it takes ~2 mins per event)
#if 'TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola' in options.sampleLocation and not runOnData:
#   process.pEle += process.pdfWeights
#   process.pMu  += process.pdfWeights
   
process.outLepton = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(options.sampleName+'_treeCMG_leptonJetsSkim.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('pMu || pEle')),
                               outputCommands = cms.untracked.vstring( 'drop *',
                               				               'keep *_TriggerResults*_*_*'	     ,
                                				       'keep *_cmgTriggerObjectSel*_*_*'     ,
                              				               'keep *_cmgPFMET*_*_*'		     ,
                                				       'keep *_offlinePrimaryVertices*_*_*'  ,
                                				       'keep *_addPileupInfo_*_*'            ,
                                				       'keep *_generator_*_*'                ,
                                				       'keep *_cmgTop*_*_*'                  ,
								       'keep *_vertexWeightSummer12*_*_*'    ,
								       'keep *_vertexWeight2012ABCDtrue*_*_*',
								       'keep *_*Filter*_*_*'                 ,
								       'keep *_primaryVertexFilter*_*_*'     ,
                                                                       'keep *_pdfWeights_*_*'               ,
								       'keep *_genJetSel*_*_*'               ,
								       'keep cmgPFJets_cmgPFJetSel_*_*'      ,
                                                                       'keep *_hitFitTtSemiLepEvent*_*_*'
 								      ) 
)



process.outpath = cms.EndPath(process.outLepton)

print process.outLepton.dumpPython()

print "Output file Lepton: '%s'"  % process.outLepton.fileName

######################################################################



