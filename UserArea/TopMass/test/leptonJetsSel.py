import FWCore.ParameterSet.Config as cms
import os, sys

process = cms.Process("TOP")

runOnData = False

lumiFile = 'None'

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

if(len(sys.argv)!=3):
   print 'Wrong parameters. Exiting... Usage: cmsRun leptonJetsSel.py ttbar \n'
   exit(0)

sample = sys.argv[2]
print 'Selected dataset: {0}'.format(sample)

sampleLocation = ""

#sample = "test" 
#sample = "muDataRun2012A" 
#sample = "muDataRun2012B" 
#sample = "ttbar" 
#sample = "wjets" 
#sample = "zjets" 
#sample = "qcd" 
#sample = "stop_s"
#sample = "stop_t"
#sample = "stop_tW"
#sample = "stbar_s"
#sample = "stbar_t"
#sample = "stbar_tW"
#sample = "ttbar1615" 
#sample = "ttbar1635" 
#sample = "ttbar1665" 
#sample = "ttbar1695" 
#sample = "ttbar1755" 
#sample = "ttbar1785" 
#sample = "ttbar1815" 
#sample = "ttbar1845" 
#sample = "ttbarMatchingUp" 
#sample = "ttbarMatchingDown" 
#sample = "ttbarScaleUp" 
#sample = "ttbarScaleDown" 



## getInfo.py -s "select path_name,dataset_fraction,dataset_entries,NUMBER_FILES_GOOD from dataset_details where path_name like '%all11%5_10_0' order by path_name"
## PATH_NAME                                                                                               DATASET_FRACTION   DATASET_ENTRIES  NUMBER_FILES_GOOD
## /SingleMu/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_10_0                                                                 ||      1.0 || 59538958 ||    4294
## /TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0 ||      2.0 || 13847500 ||    1444
## /DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0       || 0.999175 || 30434389 ||    2575
## /WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0            || 0.993301 || 18269870 ||    1601
## /WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_10_0            || 0.990817 || 57179973 ||    4691
## /T_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0              ||      1.0 ||   259961 ||      52
## /T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0          ||      1.0 ||   497658 ||     100
## /Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0           ||      1.0 ||   139974 ||      28
## /Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0       ||      1.0 ||   493460 ||      53
## /Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0           ||      1.0 ||  1935072 ||     388
## /T_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0              ||      1.0 ||  3758227 ||     753

## /TTJets_mass166_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0       ||   1.0 ||  4469095 ||      496
## /TTJets_mass169_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0       ||   1.0 ||  5202817 ||      523
## /TTJets_mass178_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0       ||   1.0 ||  4733483 ||      514
## /TTJets_mass181_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0       ||   0.998084 ||     5135282 ||      521

## /TTJets_scaledown_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0       ||      2.0 || 10774362 ||    1232
## /TTJets_scaleup_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0         ||      2.0 || 10018976 ||    1098
## /TTJets_matchingdown_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0    ||      2.0 || 10953456 ||    1212
## /TTJets_matchingup_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0      ||      2.0 || 10830020 ||    1314 

if(sample == "muDataRun2012A"):
   sampleLocation = '//AOD/V5_B/PAT_CMG_V5_10_0'
   runOnData = True
elif(sample == "muDataRun2012B"):
   sampleLocation = '/SingleMu/Run2012B-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'
   runOnData = True
if(sample == "muDataRun2012C"):
   sampleLocation = '//AOD/V5_B/PAT_CMG_V5_10_0'
   runOnData = True
elif(sample == "ttbar" or sample == "ttbarPDF" or sample == "ttbarJESup" or sample == "ttbarJESdown" or sample == "test"):
   sampleLocation = '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbar1615" ):
   sampleLocation = '//AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbar1635" ):
   sampleLocation = '//AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbar1665" ):
   sampleLocation = '/TTJets_mass166_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbar1695" ):
   sampleLocation = '/TTJets_mass169_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbar1755" ):
   sampleLocation = '//AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbar1785" ):
   sampleLocation = '/TTJets_mass178_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbar1815" ):
   sampleLocation = '/TTJets_mass181_5_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbar1845" ):
   sampleLocation = '//AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbarScaleUp" ):
   sampleLocation = '/TTJets_scaleup_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbarScaleDown" ):
   sampleLocation = '/TTJets_scaledown_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbarMatchingUp" ):
   sampleLocation = '/TTJets_matchingup_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "ttbarMatchingDown" ):
   sampleLocation = '/TTJets_matchingdown_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "wjetsV1"):
   sampleLocation = '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "wjetsV2"):
   sampleLocation = '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "zjets"):
   sampleLocation = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "qcd"):
   sampleLocation = '/'
elif(sample == "stop_s"):
   sampleLocation = '/T_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "stop_t"):
   sampleLocation = '/T_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "stop_tW"):
   sampleLocation = '/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "stbar_s"):
   sampleLocation = '/Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "stbar_t"):
   sampleLocation = '/Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
elif(sample == "stbar_tW"):
   sampleLocation = '/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'


print "\n Sample locations:"
print sampleLocation

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource('cmgtools', sampleLocation, 'cmgTuple_.*root')
 

#print "WARNING!!!!!!!!!!!!!!!! remove the following line (see .cfg) before running on the batch!"
#process.source.fileNames = [process.source.fileNames[0]]

print "\n Processing files:"
print process.source.fileNames

print "\n Processing events:"
print process.maxEvents

if runOnData:
   lumiFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt'

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
# 2012A: HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet30_v*                                                        ---> 45, 45, 45, 20 jet pT cuts
# 2012B: HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v* || HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v*   ---> 45, 45, 45, 20 jet pT cuts
# 2012C and D: HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet45_35_25_v*                                            ---> 55, 45, 35, 20 jet pT cuts

if runOnData:
   muTriggerList  = "( getSelectionRegExp(\"HLT_IsoMu20_eta2p1_TriCentralPFNoPUJet30_v*\") || getSelectionRegExp(\"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v*\") || getSelectionRegExp(\"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v*\") )"   
   eleTriggerList = "( getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_v*\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30_30_20_v*\") )"   
else:
   #muTriggerList  = "( getSelectionRegExp(\"HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet50_40_30_v1\") )"
   #eleTriggerList = "( getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet50_40_30_v5\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet50_40_30_v1\") )"
   # Last recommendation is to not apply HLT in MC
   muTriggerList  = "( getSelectionRegExp(\"*\") )"
   eleTriggerList = "( getSelectionRegExp(\"*\") )"

process.cmgTopTriggerSel = cmgTriggerObjectSel.clone(
   src = 'cmgTriggerObjectSel',
   cut = cms.string( muTriggerList +' || ' + eleTriggerList),
   filter = cms.bool(True),
   debugOn = cms.untracked.bool(True),
   )

## Muon definitions in CMGTools/Common/python/selections/muonIDs_cfi.py
tightMuon     = "(isPF() && isGlobal() && pt()>26 && abs(eta())<2.1 && normalizedChi2()<10  && trackerLayersWithMeasurement()>5  && numberOfValidMuonHits()>0 && abs(dxy())<0.02  && abs(dz())<0.5 && relIso(0.5, 0, 0.4)<0.12 && numberOfValidPixelHits()>0 && numberOfMatches()>1)"
tightElectron = "(pt()>30 && abs(eta())<2.5 && (abs(sourcePtr().superCluster().eta())>1.5660 || abs(sourcePtr().superCluster().eta())<1.4442) && abs(dxy())<0.02 && passConversionVeto()==1 && mvaTrigV0()>0.0 && numberOfHits()<=0 && relIso(0.5, 0, 0.3)<0.1)"


# Apply top muon selection (as VBTF with looser dxy) + db() cut + kinematics + isolation
process.cmgTopTightMuonMuJetSel = cmgMuonSel.clone(
   src = 'cmgMuonSel',
   cut = cms.string( tightMuon ),
   )

process.cmgTopTightElecEleJetSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string(tightElectron)
   ) 

# Apply selection for loose muon veto
process.cmgTopLooseMuonMuJetSel = cmgMuonSel.clone(
   src = 'cmgMuonSel',
   cut = cms.string( "isPF() && (isGlobal() || isTracker()) && pt()>10 && abs(eta())<2.5 && relIso(0.5, 0, 0.4)<0.2 && !" + tightMuon  )
   )

process.cmgTopLooseMuonEleJetSel = cmgMuonSel.clone(
   src = 'cmgMuonSel',
   cut = cms.string( "isPF() && (isGlobal() || isTracker()) && pt()>10 && abs(eta())<2.5 && relIso(0.5, 0, 0.4)<0.2" )
   )


#Apply electron selection for electron veto
process.cmgTopLooseElecMuJetSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string( "pt()>20 && abs(eta())<2.5 && relIso(0.5, 0, 0.3)<0.15 && mvaTrigV0()>0.0")
   )

process.cmgTopLooseElecEleJetSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string( "pt()>20 && abs(eta())<2.5 && relIso(0.5, 0, 0.3)<0.15 && mvaTrigV0()>0.0&& !" + tightElectron)
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

#Apply Jet selection: loose jet Id (from Common/python/selections/jetid_cfi.py and CMGTools/Common/src/PFJetFactory.cc) and kinematics
process.cmgTopJetMuJetSel = cmgPFJetSel.clone(
   src = 'cleanMuonJets',
   cut = cms.string( " pt()>15 && abs(eta())<2.5 && abs(phi()) < 3.2  &&  getSelection(\"cuts_looseJetId\") " )
   )


process.cmgTopJetEleJetSel = cmgPFJetSel.clone(
   src = 'cleanElectronJets',
   cut = cms.string( " pt()>15 && abs(eta())<2.5 && abs(phi()) < 3.2  &&  getSelection(\"cuts_looseJetId\") " )
   )


#Apply Jet selection: loose jet Id, kinematics and btagging (combinedSecondaryVertexBJetTags, medium working point); https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
process.cmgTopPFbTagCSVMJetSel  = cmgPFJetSel.clone(
   src = 'cmgTopPFJetSel',
   cut = cms.string( " btag(6)>0.679 " )
   )



process.load('CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff')
process.genSequence = cms.Sequence(
   process.vertexWeightSequence 
)



from CMGTools.Common.physicsObjectPrinter_cfi import *
process.printerTTbar = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("cmgTriggerObjectSel"),
    #inputCollection = cms.untracked.InputTag("cmgTopMuonSel"),
    #inputCollection = cms.untracked.InputTag("cmgTopPFJetSel"),
    printSelections = cms.untracked.bool(True),
    reportEvery     = cms.untracked.uint32(5000)
)




print process.primaryVertexFilter.dumpPython() 	 
print process.scrapingFilter.dumpPython() 	 
print process.metFilter.dumpPython() 	 
print process.cmgTopTriggerSel.dumpPython() 	 
print process.cmgTopTightMuonMuJetSel.dumpPython() 		 		 
print process.cmgTopLooseMuonMuJetSel.dumpPython() 	 	 
print process.cmgTopLooseElecMuJetSel.dumpPython() 	 	 
print process.cmgTopJetMuJetSel.dumpPython()   	 	 
print process.cmgTopTightElecEleJetSel.dumpPython() 		 		 
print process.cmgTopLooseMuonEleJetSel.dumpPython() 	 	 
print process.cmgTopLooseElecEleJetSel.dumpPython() 	 	 
print process.cmgTopJetEleJetSel.dumpPython()   	 	 


process.ana = cms.EDAnalyzer('TtbarLeptonJets')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(sample+'Histos.root')
    )


process.p = cms.Path( 
   #process.printerTTbar             +
   process.primaryVertexFilter      +
   process.scrapingFilter           + 	 
   process.metFilter                + 	 
   #process.cmgTopTriggerSel         +
   process.cmgTopTightMuonMuJetSel  +		    
   process.cmgTopLooseMuonMuJetSel  +	    
   process.cmgTopLooseElecMuJetSel  +	    
   process.cleanMuonJets            + 
   process.cmgTopJetMuJetSel	    +   	    
   process.cmgTopTightElecEleJetSel +		    
   process.cmgTopLooseMuonEleJetSel +	    
   process.cmgTopLooseElecEleJetSel +	    
   process.cleanElectronJets        + 
   process.cmgTopJetEleJetSel       	    	    
   )

if not runOnData:
   process.p += process.vertexWeightSequence

process.p += process.ana
   
# Setup event content.
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(sample+'_treeCMG_leptonJetsSel.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('p')),
                               #outputCommands = cms.untracked.vstring('keep *')
                               outputCommands = cms.untracked.vstring( 'drop *',
                               				               'keep *_TriggerResults*_*_*'	  ,
                               				               'keep *_cmgPFMET*_*_*'		   ,
                                				       'keep *_offlinePrimaryVertices*_*_*',
                                				       'keep *_addPileupInfo_*_*',
                                				       'keep *_generator_*_*',
                                				       'keep *_cmgTop*_*_*',
								       'keep *_vertexWeightSummer12*_*_*',
								       'keep *_*Filter*_*_*',
								       'keep *_primaryVertexFilter*_*_*'  ,
								       'keep *_genJetSel*_*_*'  ,
								       'keep cmgPFJets_cmgPFJetSel_*_*'
 								      ) 
)

# uncomment the line below to keep the CMG tree
#process.outpath = cms.EndPath(process.out)   

print process.out.dumpPython()

print "Output file: '%s'" % process.out.fileName

######################################################################



