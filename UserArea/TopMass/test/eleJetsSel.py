import FWCore.ParameterSet.Config as cms
import os, sys

process = cms.Process("TOP")

runOnData = False

lumiFile = 'None'

#############################################################################################################
### edmFileUtil -P -f file:/data/PF2PAT/fall387_ttbarAOD_v2/Fall10-START38_V12-v2_ttbar_387_10_0_yep.root ###
#############################################################################################################

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

#if(len(sys.argv)!=3):
#   print 'Wrong parameters. Exiting... Usage: cmsRun eleHitFit.py ttbar \n'
#   exit(0)

#sample = sys.argv[2]
#print 'Selected dataset: {0}'.format(sample)


sample = "test"
#sample = "eleDataRun2011A"
#sample = "eleDataRun2011B" 
#sample = "ttbar" 
#sample = "wjets" 
#sample = "zjets" 
#sample = "qcd20to30_BCtoE" 
#sample = "qcd20to30_EM" 
#sample = "qcd30to80_BCtoE" 
#sample = "qcd30to80_EM" 
#sample = "qcd80to170_BCtoE" 
#sample = "qcd80to170_EM" 
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


sampleLocation = ""
directory = 'cmgtools'


##/ElectronHad/Run2011A-May10ReReco-v1/AOD
##/ElectronHad/Run2011A-PromptReco-v4/AOD
##/ElectronHad/Run2011A-05Aug2011-v1/AOD
##/ElectronHad/Run2011A-PromptReco-v6/AOD
##/ElectronHad/Run2011B-PromptReco-v1/AOD

## getInfo.py -s "select path_name,dataset_fraction,dataset_entries,NUMBER_FILES_GOOD from dataset_details where path_name like '%all11%5_6_0_B' order by path_name"
## PATH_NAME                                                                                               DATASET_FRACTION   DATASET_ENTRIES  NUMBER_FILES_GOOD
## /ElectronHad/Run2011A-08Nov2011-v1/AOD/V5_B/PAT_CMG_V5_6_0_B                                             ||        1.0 ||  46221658       ||     2001
## /ElectronHad/Run2011B-19Nov2011-v1/AOD/V5_B/PAT_CMG_V5_6_0_B                                             ||        1.0 ||  28260662       ||     2039
## /TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5_B/PAT_CMG_V5_6_0_B            ||   0.994978 ||  59314581       ||     5341
## /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B        ||   0.996416 ||  81053834       ||     4524
## /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B   ||        1.0 ||  36209629       ||     2754
## /QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B       ||        1.0 ||   2071133       ||      140
## /QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||   0.999574 ||  35706604       ||     2273
## /QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B       ||        1.0 ||   2030033       ||      173
## /QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B   ||   0.999858 ||  70382054       ||     6766
## /QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B       ||        1.0 ||   1082691       ||       74
## /QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5_B/PAT_CMG_V5_6_0_B ||   0.998023 ||   8134559       ||      533
## /T_TuneZ2_s-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B               ||        1.0 ||    259971       ||       14
## /T_TuneZ2_t-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B               ||   0.994872 ||   3880171       ||      196
## /T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B           ||        1.0 ||    814390       ||       43
## /Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B            ||        1.0 ||    137980       ||        8
## /Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B            ||        1.0 ||   1944826       ||      100
## /Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B        ||       -1.0 ||    323401       ||       18
## /TTJets_TuneZ2_mass161_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v3/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||        1.0 ||   1620072       ||      146
## /TTJets_TuneZ2_mass163_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||        1.0 ||   1633197       ||      142
## /TTJets_TuneZ2_mass166_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||        1.0 ||   1669034       ||      146
## /TTJets_TuneZ2_mass169_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||        1.0 ||   1606570       ||      154
## /TTJets_TuneZ2_mass175_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||        1.0 ||   1538301       ||      134
## /TTJets_TuneZ2_mass178_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||        1.0 ||   1650682       ||      158
## /TTJets_TuneZ2_mass181_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||        1.0 ||   1662116       ||      158
## /TTJets_TuneZ2_mass184_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B  ||        1.0 ||   1671859       ||      153

if(sample == "eleDataRun2011A"):
   sampleLocation = '/ElectronHad/Run2011A-08Nov2011-v1/AOD/V5_B/PAT_CMG_V5_6_0_B'
   runOnData = True
elif(sample == "eleDataRun2011B"):
   sampleLocation = '/ElectronHad/Run2011B-19Nov2011-v1/AOD/V5_B/PAT_CMG_V5_6_0_B'
   runOnData = True
elif(sample == "ttbar" or sample == "ttbarPDF" or sample == "ttbarJESup" or sample == "ttbarJESdown" or sample == "test"):
   sampleLocation = '/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
   #process.source = cms.Source("PoolSource",
   #    fileNames = cms.untracked.vstring('/store/cmst3/user/cmgtools/CMG/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5_B/PAT_CMG_V5_6_0_B/cmgTuple_998.root'),
   #    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/p/palencia/work/cmg/V5_4_0/CMGTools/CMSSW_5_2_5/src/CMGTools/Common/prod/cmgTuple_forSync.root'),
   #)
elif(sample == "ttbar1615" ):
   sampleLocation = '/TTJets_TuneZ2_mass161_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v3/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "ttbar1635" ):
   sampleLocation = '/TTJets_TuneZ2_mass163_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "ttbar1665" ):
   sampleLocation = '/TTJets_TuneZ2_mass166_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "ttbar1695" ):
   sampleLocation = '/TTJets_TuneZ2_mass169_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "ttbar1755" ):
   sampleLocation = '/TTJets_TuneZ2_mass175_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "ttbar1785" ):
   sampleLocation = '/TTJets_TuneZ2_mass178_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "ttbar1815" ):
   sampleLocation = '/TTJets_TuneZ2_mass181_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "ttbar1845" ):
   sampleLocation = '/TTJets_TuneZ2_mass184_5_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "wjets"):
   sampleLocation = '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "zjets"):
   sampleLocation = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "qcd20to30_BCtoE"):
   sampleLocation = '/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "qcd20to30_EM"):
   sampleLocation = '/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "qcd30to80_BCtoE"):
   sampleLocation = '/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "qcd30to80_EM"):
   sampleLocation = '/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "qcd80to170_BCtoE"):
   sampleLocation = '/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "qcd80to170_EM"):
   sampleLocation = '/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
elif(sample == "stop_s"):
   sampleLocation = '/T_TuneZ2_s-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B'
   directory = 'cmgtools_group'
elif(sample == "stop_t"):
   sampleLocation = '/T_TuneZ2_t-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B'
   directory = 'cmgtools_group'
elif(sample == "stop_tW"):
   sampleLocation = '/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B'
   directory = 'cmgtools_group'
elif(sample == "stbar_s"):
   sampleLocation = '/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B'
   directory = 'cmgtools_group'
elif(sample == "stbar_t"):
   sampleLocation = '/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B'
   directory = 'cmgtools_group'
elif(sample == "stbar_tW"):
   sampleLocation = '/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_5_6_0_B'
   directory = 'cmgtools_group'

# Check triggers here: http://fwyzard.web.cern.ch/fwyzard/hlt/summary. 
if runOnData:
   triggerList = "( getSelectionRegExp(\"HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30_v1\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30_v2\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30_v3\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30_v3\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30_v1\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30_v2\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30_v3\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30_v4\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30_v5\") ||  getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30_v2\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30_v3\") )"    
else:
   triggerList = "( getSelectionRegExp(\"HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30_v1\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30_v2\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30_v3\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30_v+\") || getSelectionRegExp(\"HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30_v5\") )"

print "\n Sample locations:"
print sampleLocation

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(directory, sampleLocation, 'cmgTuple_.*root')

#print "WARNING!!!!!!!!!!!!!!!! remove the following line (see .cfg) before running on the batch!"
#process.source.fileNames = [process.source.fileNames[10]]


print "\n Processing files:"
print process.source.fileNames

print "\n Processing events:"
print process.maxEvents

if runOnData:
   lumiFile = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'

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
   #cut = cms.string("!isFake && ndof > 90 && abs(z) <= 24 && position.Rho <= 2"),    
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
#process.metFilter.HLTPaths = cms.vstring('HBHENoiseFilterPath', 'CSCTightHaloFilterPath', 'hcalLaserEventFilterPath', 'EcalDeadCellTriggerPrimitiveFilterPath', 'trackingFailureFilterPath', 'eeBadScFilterPath')
process.metFilter.HLTPaths = cms.vstring('HBHENoiseFilterPath', 'CSCTightHaloFilterPath', 'hcalLaserEventFilterPath', 'EcalDeadCellTriggerPrimitiveFilterPath', 'trackingFailureFilterPath')


process.cmgTopEleTriggerSel = cmgTriggerObjectSel.clone(
   src = 'cmgTriggerObjectSel',
   cut = cms.string( triggerList ),
   filter = cms.bool(True),
   debugOn = cms.untracked.bool(True),
   )

tightElectron = "(pt()>30 && abs(eta())<2.5&& (abs(sourcePtr().superCluster().eta())>1.5660 || abs(sourcePtr().superCluster().eta())<1.4442) && abs(dxy())<0.02 && passConversionVeto()==1 && mvaTrigV0()>0.0 && numberOfHits()<=0 && relIso(0.5)<0.1)"

#Apply electron selection for electron veto
process.cmgTopElectronSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string(tightElectron)
   ) 
process.oneElectron = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (1),
     maxNumber = cms.uint32  (1),
     src       = cms.InputTag("cmgTopElectronSel"),
)


# Apply selection for loose muon veto
process.cmgTopLooseMuonSel = cmgMuonSel.clone(
   src = 'cmgMuonSel',
   cut = cms.string( "isPF() && (isGlobal() || isTracker()) && pt()>10 && abs(eta())<2.5 && relIso(0.5)<0.2" )
   )
process.zeroLooseMuons = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (0),
     maxNumber = cms.uint32  (0),
     src       = cms.InputTag("cmgTopLooseMuonSel"),
)


#Apply electron selection for loose electron veto
process.cmgTopLooseElectronSel = cmgElectronSel.clone(
   src = 'cmgElectronSel',
   cut = cms.string( "pt()>20 && abs(eta())<2.5 && relIso(0.5)<0.2 && mvaTrigV0()>0.0&& !" + tightElectron)
   )
process.zeroLooseElectrons = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (0),
     maxNumber = cms.uint32  (0),
     src       = cms.InputTag("cmgTopLooseElectronSel"),
)





# Apply deltaR jet-muon cleaning 
# http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/Common/plugins/DeltaRVetoProducer.cc?revision=1.3&view=markup
# Producer that makes a copy of the inputCollection, vetoing objects that lie within minDeltaR of *any* object in the vetoCollection.
# Algorithm is order N^2, so please use reduced collections as input
process.cleanElectronJets = cms.EDProducer("DeltaRVetoProducerPFJet",
   inputCollection = cms.InputTag('cmgPFJetSelCHS'),
   MatchingParams = cms.VPSet(cms.PSet( 				    
      vetoCollection=cms.InputTag("cmgTopElectronSel"),
      minDeltaR=cms.double(0.3),
      removeMatchedObject=cms.bool(True)
      )
   ),
   verbose = cms.untracked.bool(False)
   )


#Apply Jet selection: loose jet Id (from Common/python/selections/jetid_cfi.py and CMGTools/Common/src/PFJetFactory.cc) and kinematics
process.cmgTopPFJetSel     = cmgPFJetSel.clone(
   src = 'cleanElectronJets',
   cut = cms.string( " pt()>35 && abs(eta())<2.5 && abs(phi()) < 3.2  &&  getSelection(\"cuts_looseJetId\") " )
   )

process.fourJets = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (4),
     maxNumber = cms.uint32  (9999),
     src       = cms.InputTag("cmgTopPFJetSel"),
)



#Apply Jet selection: loose jet Id, kinematics and btagging (combinedSecondaryVertexBJetTags, medium working point); https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
process.cmgTopPFbTagCSVMJetSel  = cmgPFJetSel.clone(
   src = 'cmgTopPFJetSel',
   cut = cms.string( " btag(6)>0.679 " )
   )

process.onebTag = cms.EDFilter("PATCandViewCountFilter",
     minNumber = cms.uint32  (1),
     maxNumber = cms.uint32  (9999),
     src       = cms.InputTag("cmgTopPFbTagCSVMJetSel"),
)


process.load('CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff')
process.genSequence = cms.Sequence(
   process.vertexWeightSequence 
)


from CMGTools.RootTools.utils.vertexWeight.vertexWeightsFall11_cfi import *
process.vertexWeightFall112011ABtrue = cms.EDProducer(
    "PileUpWeightProducer",
    verbose = cms.untracked.bool( False ),
    src = cms.InputTag('addPileupInfo'),
    type = cms.int32(2),
    inputHistMC  = cms.string( '../data/pileUp_Fall11MC.root'),
    inputHistData = cms.string( '../data/PU2011dataTrueCERN_68mb.root'),
    )

from CMGTools.RootTools.utils.vertexWeight.vertexWeightsFall11_cfi import *
process.vertexWeightFall112011ABtrueUp = cms.EDProducer(
    "PileUpWeightProducer",
    verbose = cms.untracked.bool( False ),
    src = cms.InputTag('addPileupInfo'),
    type = cms.int32(2),
    inputHistMC  = cms.string( '../data/pileUp_Fall11MC.root'),
    inputHistData = cms.string( '../data/PU2011dataTrueCERN_71mb.root'),
    )

from CMGTools.RootTools.utils.vertexWeight.vertexWeightsFall11_cfi import *
process.vertexWeightFall112011ABtrueDown = cms.EDProducer(
    "PileUpWeightProducer",
    verbose = cms.untracked.bool( False ),
    src = cms.InputTag('addPileupInfo'),
    type = cms.int32(2),
    inputHistMC  = cms.string( '../data/pileUp_Fall11MC.root'),
    inputHistData = cms.string( '../data/PU2011dataTrueCERN_65mb.root'),
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
print process.cmgTopEleTriggerSel.dumpPython() 	 
print process.cmgTopElectronSel.dumpPython() 		 		 
print process.cmgTopLooseMuonSel.dumpPython() 	 	 
print process.cmgTopLooseElectronSel.dumpPython() 	 	 
print process.cmgTopPFJetSel.dumpPython()   	 	 
print process.cmgTopPFbTagCSVMJetSel.dumpPython()     
print process.oneElectron.dumpPython() 	 
print process.zeroLooseMuons.dumpPython()  	 
print process.zeroLooseElectrons.dumpPython()  	 
print process.fourJets.dumpPython() 		 
print process.onebTag.dumpPython()
#print process.vertexWeightSequence.dumpPython()
print process.vertexWeightFall112011ABtrueDown.dumpPython()
print process.vertexWeightFall112011ABtrue.dumpPython()
print process.vertexWeightFall112011ABtrueUp.dumpPython()




process.p = cms.Path( 
   #process.printerTTbar             +
   #process.vertexWeightSequence     +
   process.primaryVertexFilter      +
   process.scrapingFilter           + 	 
   process.metFilter                + 	 
   process.cmgTopEleTriggerSel      +
   process.cmgTopElectronSel	    +		    
   process.cmgTopLooseMuonSel	    +	    
   process.cmgTopLooseElectronSel   +	    
   process.cleanElectronJets        * 
   process.cmgTopPFJetSel	    +	    
   process.cmgTopPFbTagCSVMJetSel   +	
   process.oneElectron              +
   process.zeroLooseMuons           +
   process.zeroLooseElectrons       +
   process.fourJets		    #+    
   #process.onebTag		    
   )

if not runOnData:
   process.p += process.vertexWeightFall112011ABtrueDown 
   process.p += process.vertexWeightFall112011ABtrue 
   process.p += process.vertexWeightFall112011ABtrueUp 


# Setup event content.
process.out = cms.OutputModule("PoolOutputModule",
                               #fileName = cms.untracked.string('/data/CMG/topMass/'+sample+'_tree_CMG_muonSel.root'),
                               fileName = cms.untracked.string(sample+'_treeCMG_eleJetsSel.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('p')),
                               #outputCommands = cms.untracked.vstring('keep *')
                               outputCommands = cms.untracked.vstring( 'drop *',
                                				       ##'keep *_selectedPatMuonsAK5LC*_*_*' ,
                                				       ##'keep *_selectedPatElectronsAK5LC*_*_*' ,
                                				       ##'keep *_cmgTriggerObjectSelAK5LCCMG*_*_*' ,
                                				       'keep *_TriggerResults*_*_*'	  ,
                                				       ##'keep *_cmgMuonSel*_*_*'	  ,
                                				       ##'keep *_cmgElectronSel*_*_*'	  ,
                                				       ##'keep *_cmgPFJetSel_*_*'	  ,
                                 				       ##'keep *_cmgDiMuonSel_*_*'	  ,
                                				       'keep *_cmgPFMET*_*_*'		   ,
                                				       'keep *_offlinePrimaryVertices*_*_*',
                                				       'keep *_addPileupInfo_*_*',
                                				       'keep *_generator_*_*',
                                				       'keep *_cmgTop*_*_*',
								       'keep *_vertexWeight*_*_*',
								       'keep *_*Filter*_*_*',
								       'keep *_primaryVertexFilter*_*_*'  ,
								       'keep *_genJetSel*_*_*'  ,
								       'keep cmgPFJets_cmgPFJetSel_*_*'
								       #'drop *',
 								      ) 
)


process.outpath = cms.EndPath(process.out)

print process.out.dumpPython()

print "Output file: '%s'" % process.out.fileName

######################################################################




