import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("DIMU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
# numberOfFilesToProcess = 100
numberOfFilesToProcess = -1




# Input  & JSON             -------------------------------------------------

# process.lumiJSON = cms.EDAnalyzer("JsonLogger",
                                  # fileName = cms.untracked.string("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt")
                                  # )


# process.setName_('H2TAUTAU')


dataset_user = 'cmgtools' 
# dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
# dataset_name = '/DoubleMu/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_V5_6_0_B'
# dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START44_V9B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
dataset_name = '/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Fall11-PU_S6_START44_V9B-v1/AODSIM/V5_B/PAT_CMG_5_6_0_B_pfcandidates'
dataset_files = 'cmgTuple.*root'

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

# restricting the number of files to process to a given number
if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

import pprint
pprint.pprint(process.source.fileNames[:10])
print '.. and so on.'


###ProductionTaskHook$$$
    
process.load('CMGTools.Common.skims.cmgDiMuonSel_cfi')
process.load('CMGTools.Common.skims.cmgDiMuonCount_cfi')

process.cmgDiMuonSel.src = 'cmgDiMuonSel'
process.cmgDiMuonSel.cut = ('mass()>40 && (leg1().pt()>25 && leg2().pt()>6)')
process.cmgDiMuonCount.minNumber = 1


from CMGTools.WMass.myMetFlavors_cff import *
# COMMA SEPARATED AMONG "X,h,e,mu,gamma,h0,hHF,egammaHF"
process.myPFmet         =  myMetFlavor.clone(  
                                            MetFlavor = cms.int32(0),
                                            candFlavor      = cms.string(""), 
                                            candPtMin       = cms.double(0.0),
                                            candEtaMin      = cms.double(-100.0),
                                            candEtaMax      = cms.double(100.0),
                                            )
myMetEtaCuts=[-5.0,-3.0,-2.4,-2.1,-1.4,1.4,2.1,2.4,3.0,5.0]
myMetPtCuts=[0.0,0.5,1.0,1.5,2.0]
#INCLUSIVE
process.mygammaPFmet         =  process.myPFmet.clone(  
                                                  candFlavor      = cms.string("gamma"), 
                                            )
process.myh0PFmet         =  process.myPFmet.clone(  
                                                  candFlavor      = cms.string("h0"), 
                                            )
process.myhegammaHFPFmet         =  process.myPFmet.clone(  
                                                  candFlavor      = cms.string("h_HF,egamma_HF"), 
                                            )
process.mytkmethemuPFmet         =  process.myPFmet.clone(  
                                                  MetFlavor = cms.int32(1),
                                                  candFlavor      = cms.string("h,e,mu"), 
                                            )

#GAMMA
# PT > 0.0
process.mygammaPFmetPt0p0EtaMinm5p0EtaMaxm3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mygammaPFmetPt0p0EtaMinm3p0EtaMaxm2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mygammaPFmetPt0p0EtaMinm2p4EtaMaxm2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mygammaPFmetPt0p0EtaMinm2p1EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mygammaPFmetPt0p0EtaMinm1p4EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mygammaPFmetPt0p0EtaMin1p4EtaMax2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mygammaPFmetPt0p0EtaMin2p1EtaMax2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mygammaPFmetPt0p0EtaMin2p4EtaMax3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mygammaPFmetPt0p0EtaMin3p0EtaMax5p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 0.5
process.mygammaPFmetPt0p5EtaMinm5p0EtaMaxm3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mygammaPFmetPt0p5EtaMinm3p0EtaMaxm2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mygammaPFmetPt0p5EtaMinm2p4EtaMaxm2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mygammaPFmetPt0p5EtaMinm2p1EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mygammaPFmetPt0p5EtaMinm1p4EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mygammaPFmetPt0p5EtaMin1p4EtaMax2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mygammaPFmetPt0p5EtaMin2p1EtaMax2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mygammaPFmetPt0p5EtaMin2p4EtaMax3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mygammaPFmetPt0p5EtaMin3p0EtaMax5p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 1.0
process.mygammaPFmetPt1p0EtaMinm5p0EtaMaxm3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mygammaPFmetPt1p0EtaMinm3p0EtaMaxm2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mygammaPFmetPt1p0EtaMinm2p4EtaMaxm2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mygammaPFmetPt1p0EtaMinm2p1EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mygammaPFmetPt1p0EtaMinm1p4EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mygammaPFmetPt1p0EtaMin1p4EtaMax2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mygammaPFmetPt1p0EtaMin2p1EtaMax2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mygammaPFmetPt1p0EtaMin2p4EtaMax3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mygammaPFmetPt1p0EtaMin3p0EtaMax5p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 0.5
process.mygammaPFmetPt1p5EtaMinm5p0EtaMaxm3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mygammaPFmetPt1p5EtaMinm3p0EtaMaxm2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mygammaPFmetPt1p5EtaMinm2p4EtaMaxm2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mygammaPFmetPt1p5EtaMinm2p1EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mygammaPFmetPt1p5EtaMinm1p4EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mygammaPFmetPt1p5EtaMin1p4EtaMax2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mygammaPFmetPt1p5EtaMin2p1EtaMax2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mygammaPFmetPt1p5EtaMin2p4EtaMax3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mygammaPFmetPt1p5EtaMin3p0EtaMax5p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 2.0
process.mygammaPFmetPt2p0EtaMinm5p0EtaMaxm3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mygammaPFmetPt2p0EtaMinm3p0EtaMaxm2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mygammaPFmetPt2p0EtaMinm2p4EtaMaxm2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mygammaPFmetPt2p0EtaMinm2p1EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mygammaPFmetPt2p0EtaMinm1p4EtaMaxm1p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mygammaPFmetPt2p0EtaMin1p4EtaMax2p1  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mygammaPFmetPt2p0EtaMin2p1EtaMax2p4  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mygammaPFmetPt2p0EtaMin2p4EtaMax3p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mygammaPFmetPt2p0EtaMin3p0EtaMax5p0  =  process.mygammaPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
#H0
# PT > 0.0
process.myh0PFmetPt0p0EtaMinm5p0EtaMaxm3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myh0PFmetPt0p0EtaMinm3p0EtaMaxm2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myh0PFmetPt0p0EtaMinm2p4EtaMaxm2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myh0PFmetPt0p0EtaMinm2p1EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myh0PFmetPt0p0EtaMinm1p4EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myh0PFmetPt0p0EtaMin1p4EtaMax2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myh0PFmetPt0p0EtaMin2p1EtaMax2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myh0PFmetPt0p0EtaMin2p4EtaMax3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myh0PFmetPt0p0EtaMin3p0EtaMax5p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 0.5
process.myh0PFmetPt0p5EtaMinm5p0EtaMaxm3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myh0PFmetPt0p5EtaMinm3p0EtaMaxm2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myh0PFmetPt0p5EtaMinm2p4EtaMaxm2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myh0PFmetPt0p5EtaMinm2p1EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myh0PFmetPt0p5EtaMinm1p4EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myh0PFmetPt0p5EtaMin1p4EtaMax2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myh0PFmetPt0p5EtaMin2p1EtaMax2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myh0PFmetPt0p5EtaMin2p4EtaMax3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myh0PFmetPt0p5EtaMin3p0EtaMax5p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 1.0
process.myh0PFmetPt1p0EtaMinm5p0EtaMaxm3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myh0PFmetPt1p0EtaMinm3p0EtaMaxm2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myh0PFmetPt1p0EtaMinm2p4EtaMaxm2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myh0PFmetPt1p0EtaMinm2p1EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myh0PFmetPt1p0EtaMinm1p4EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myh0PFmetPt1p0EtaMin1p4EtaMax2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myh0PFmetPt1p0EtaMin2p1EtaMax2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myh0PFmetPt1p0EtaMin2p4EtaMax3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myh0PFmetPt1p0EtaMin3p0EtaMax5p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 0.5
process.myh0PFmetPt1p5EtaMinm5p0EtaMaxm3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myh0PFmetPt1p5EtaMinm3p0EtaMaxm2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myh0PFmetPt1p5EtaMinm2p4EtaMaxm2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myh0PFmetPt1p5EtaMinm2p1EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myh0PFmetPt1p5EtaMinm1p4EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myh0PFmetPt1p5EtaMin1p4EtaMax2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myh0PFmetPt1p5EtaMin2p1EtaMax2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myh0PFmetPt1p5EtaMin2p4EtaMax3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myh0PFmetPt1p5EtaMin3p0EtaMax5p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 2.0
process.myh0PFmetPt2p0EtaMinm5p0EtaMaxm3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myh0PFmetPt2p0EtaMinm3p0EtaMaxm2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myh0PFmetPt2p0EtaMinm2p4EtaMaxm2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myh0PFmetPt2p0EtaMinm2p1EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myh0PFmetPt2p0EtaMinm1p4EtaMaxm1p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myh0PFmetPt2p0EtaMin1p4EtaMax2p1  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myh0PFmetPt2p0EtaMin2p1EtaMax2p4  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myh0PFmetPt2p0EtaMin2p4EtaMax3p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myh0PFmetPt2p0EtaMin3p0EtaMax5p0  =  process.myh0PFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
#H EGAMMA HF
# PT > 0.0
process.myhegammaHFPFmetPt0p0EtaMinm5p0EtaMaxm3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myhegammaHFPFmetPt0p0EtaMinm3p0EtaMaxm2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myhegammaHFPFmetPt0p0EtaMinm2p4EtaMaxm2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myhegammaHFPFmetPt0p0EtaMinm2p1EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myhegammaHFPFmetPt0p0EtaMinm1p4EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myhegammaHFPFmetPt0p0EtaMin1p4EtaMax2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myhegammaHFPFmetPt0p0EtaMin2p1EtaMax2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myhegammaHFPFmetPt0p0EtaMin2p4EtaMax3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myhegammaHFPFmetPt0p0EtaMin3p0EtaMax5p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 0.5
process.myhegammaHFPFmetPt0p5EtaMinm5p0EtaMaxm3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myhegammaHFPFmetPt0p5EtaMinm3p0EtaMaxm2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myhegammaHFPFmetPt0p5EtaMinm2p4EtaMaxm2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myhegammaHFPFmetPt0p5EtaMinm2p1EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myhegammaHFPFmetPt0p5EtaMinm1p4EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myhegammaHFPFmetPt0p5EtaMin1p4EtaMax2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myhegammaHFPFmetPt0p5EtaMin2p1EtaMax2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myhegammaHFPFmetPt0p5EtaMin2p4EtaMax3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myhegammaHFPFmetPt0p5EtaMin3p0EtaMax5p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 1.0
process.myhegammaHFPFmetPt1p0EtaMinm5p0EtaMaxm3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myhegammaHFPFmetPt1p0EtaMinm3p0EtaMaxm2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myhegammaHFPFmetPt1p0EtaMinm2p4EtaMaxm2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myhegammaHFPFmetPt1p0EtaMinm2p1EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myhegammaHFPFmetPt1p0EtaMinm1p4EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myhegammaHFPFmetPt1p0EtaMin1p4EtaMax2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myhegammaHFPFmetPt1p0EtaMin2p1EtaMax2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myhegammaHFPFmetPt1p0EtaMin2p4EtaMax3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myhegammaHFPFmetPt1p0EtaMin3p0EtaMax5p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 0.5
process.myhegammaHFPFmetPt1p5EtaMinm5p0EtaMaxm3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myhegammaHFPFmetPt1p5EtaMinm3p0EtaMaxm2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myhegammaHFPFmetPt1p5EtaMinm2p4EtaMaxm2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myhegammaHFPFmetPt1p5EtaMinm2p1EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myhegammaHFPFmetPt1p5EtaMinm1p4EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myhegammaHFPFmetPt1p5EtaMin1p4EtaMax2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myhegammaHFPFmetPt1p5EtaMin2p1EtaMax2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myhegammaHFPFmetPt1p5EtaMin2p4EtaMax3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myhegammaHFPFmetPt1p5EtaMin3p0EtaMax5p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 2.0
process.myhegammaHFPFmetPt2p0EtaMinm5p0EtaMaxm3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.myhegammaHFPFmetPt2p0EtaMinm3p0EtaMaxm2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.myhegammaHFPFmetPt2p0EtaMinm2p4EtaMaxm2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.myhegammaHFPFmetPt2p0EtaMinm2p1EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.myhegammaHFPFmetPt2p0EtaMinm1p4EtaMaxm1p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.myhegammaHFPFmetPt2p0EtaMin1p4EtaMax2p1  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.myhegammaHFPFmetPt2p0EtaMin2p1EtaMax2p4  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.myhegammaHFPFmetPt2p0EtaMin2p4EtaMax3p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.myhegammaHFPFmetPt2p0EtaMin3p0EtaMax5p0  =  process.myhegammaHFPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
#TK
# PT > 0.0
process.mytkmethemuPFmetPt0p0EtaMinm5p0EtaMaxm3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mytkmethemuPFmetPt0p0EtaMinm3p0EtaMaxm2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mytkmethemuPFmetPt0p0EtaMinm2p4EtaMaxm2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mytkmethemuPFmetPt0p0EtaMinm2p1EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mytkmethemuPFmetPt0p0EtaMinm1p4EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mytkmethemuPFmetPt0p0EtaMin1p4EtaMax2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mytkmethemuPFmetPt0p0EtaMin2p1EtaMax2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mytkmethemuPFmetPt0p0EtaMin2p4EtaMax3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mytkmethemuPFmetPt0p0EtaMin3p0EtaMax5p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 0.5
process.mytkmethemuPFmetPt0p5EtaMinm5p0EtaMaxm3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mytkmethemuPFmetPt0p5EtaMinm3p0EtaMaxm2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mytkmethemuPFmetPt0p5EtaMinm2p4EtaMaxm2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mytkmethemuPFmetPt0p5EtaMinm2p1EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mytkmethemuPFmetPt0p5EtaMinm1p4EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mytkmethemuPFmetPt0p5EtaMin1p4EtaMax2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mytkmethemuPFmetPt0p5EtaMin2p1EtaMax2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mytkmethemuPFmetPt0p5EtaMin2p4EtaMax3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mytkmethemuPFmetPt0p5EtaMin3p0EtaMax5p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(0.5),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 1.0
process.mytkmethemuPFmetPt1p0EtaMinm5p0EtaMaxm3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mytkmethemuPFmetPt1p0EtaMinm3p0EtaMaxm2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mytkmethemuPFmetPt1p0EtaMinm2p4EtaMaxm2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mytkmethemuPFmetPt1p0EtaMinm2p1EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mytkmethemuPFmetPt1p0EtaMinm1p4EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mytkmethemuPFmetPt1p0EtaMin1p4EtaMax2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mytkmethemuPFmetPt1p0EtaMin2p1EtaMax2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mytkmethemuPFmetPt1p0EtaMin2p4EtaMax3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mytkmethemuPFmetPt1p0EtaMin3p0EtaMax5p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 0.5
process.mytkmethemuPFmetPt1p5EtaMinm5p0EtaMaxm3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mytkmethemuPFmetPt1p5EtaMinm3p0EtaMaxm2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mytkmethemuPFmetPt1p5EtaMinm2p4EtaMaxm2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mytkmethemuPFmetPt1p5EtaMinm2p1EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mytkmethemuPFmetPt1p5EtaMinm1p4EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mytkmethemuPFmetPt1p5EtaMin1p4EtaMax2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mytkmethemuPFmetPt1p5EtaMin2p1EtaMax2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mytkmethemuPFmetPt1p5EtaMin2p4EtaMax3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mytkmethemuPFmetPt1p5EtaMin3p0EtaMax5p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(1.5),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
# PT > 2.0
process.mytkmethemuPFmetPt2p0EtaMinm5p0EtaMaxm3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-5.0),
                                                  candEtaMax      = cms.double(-3.0),
                                                  )
process.mytkmethemuPFmetPt2p0EtaMinm3p0EtaMaxm2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-3.0),
                                                  candEtaMax      = cms.double(-2.4),
                                                  )
process.mytkmethemuPFmetPt2p0EtaMinm2p4EtaMaxm2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-2.4),
                                                  candEtaMax      = cms.double(-2.1),
                                                  )
process.mytkmethemuPFmetPt2p0EtaMinm2p1EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-2.1),
                                                  candEtaMax      = cms.double(-1.4),
                                                  )
process.mytkmethemuPFmetPt2p0EtaMinm1p4EtaMaxm1p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(-1.4),
                                                  candEtaMax      = cms.double(1.4),
                                                  )
process.mytkmethemuPFmetPt2p0EtaMin1p4EtaMax2p1  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(1.4),
                                                  candEtaMax      = cms.double(2.1),
                                                  )
process.mytkmethemuPFmetPt2p0EtaMin2p1EtaMax2p4  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(2.1),
                                                  candEtaMax      = cms.double(2.4),
                                                  )
process.mytkmethemuPFmetPt2p0EtaMin2p4EtaMax3p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(2.4),
                                                  candEtaMax      = cms.double(3.0),
                                                  )
process.mytkmethemuPFmetPt2p0EtaMin3p0EtaMax5p0  =  process.mytkmethemuPFmet.clone(
                                                  candPtMin       = cms.double(2.0),
                                                  candEtaMin      = cms.double(3.0),
                                                  candEtaMax      = cms.double(5.0),
                                                  )
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            
                                            

# process.myTKmet         =  process.myPFmet.clone(  
                                                  # MetFlavor = cms.int32(1),
                                                  # candFlavor      = cms.string(""), 
                                            # )


process.p = cms.Path( (process.cmgDiMuonSel +
                      process.cmgDiMuonCount )
                      +process.mygammaPFmet
                      +process.myh0PFmet
                      +process.myhegammaHFPFmet
                      +process.mytkmethemuPFmet
                      #GAMMA
                      # PT > 0.0
                      +process.mygammaPFmetPt0p0EtaMinm5p0EtaMaxm3p0
                      +process.mygammaPFmetPt0p0EtaMinm3p0EtaMaxm2p4
                      +process.mygammaPFmetPt0p0EtaMinm2p4EtaMaxm2p1
                      +process.mygammaPFmetPt0p0EtaMinm2p1EtaMaxm1p4
                      +process.mygammaPFmetPt0p0EtaMinm1p4EtaMaxm1p4
                      +process.mygammaPFmetPt0p0EtaMin1p4EtaMax2p1
                      +process.mygammaPFmetPt0p0EtaMin2p1EtaMax2p4
                      +process.mygammaPFmetPt0p0EtaMin2p4EtaMax3p0
                      +process.mygammaPFmetPt0p0EtaMin3p0EtaMax5p0
                      # PT > 0.5
                      +process.mygammaPFmetPt0p5EtaMinm5p0EtaMaxm3p0
                      +process.mygammaPFmetPt0p5EtaMinm3p0EtaMaxm2p4
                      +process.mygammaPFmetPt0p5EtaMinm2p4EtaMaxm2p1
                      +process.mygammaPFmetPt0p5EtaMinm2p1EtaMaxm1p4
                      +process.mygammaPFmetPt0p5EtaMinm1p4EtaMaxm1p4
                      +process.mygammaPFmetPt0p5EtaMin1p4EtaMax2p1
                      +process.mygammaPFmetPt0p5EtaMin2p1EtaMax2p4
                      +process.mygammaPFmetPt0p5EtaMin2p4EtaMax3p0
                      +process.mygammaPFmetPt0p5EtaMin3p0EtaMax5p0
                      # PT > 1.0
                      +process.mygammaPFmetPt1p0EtaMinm5p0EtaMaxm3p0
                      +process.mygammaPFmetPt1p0EtaMinm3p0EtaMaxm2p4
                      +process.mygammaPFmetPt1p0EtaMinm2p4EtaMaxm2p1
                      +process.mygammaPFmetPt1p0EtaMinm2p1EtaMaxm1p4
                      +process.mygammaPFmetPt1p0EtaMinm1p4EtaMaxm1p4
                      +process.mygammaPFmetPt1p0EtaMin1p4EtaMax2p1
                      +process.mygammaPFmetPt1p0EtaMin2p1EtaMax2p4
                      +process.mygammaPFmetPt1p0EtaMin2p4EtaMax3p0
                      +process.mygammaPFmetPt1p0EtaMin3p0EtaMax5p0
                      # PT > 1.5
                      +process.mygammaPFmetPt1p5EtaMinm5p0EtaMaxm3p0
                      +process.mygammaPFmetPt1p5EtaMinm3p0EtaMaxm2p4
                      +process.mygammaPFmetPt1p5EtaMinm2p4EtaMaxm2p1
                      +process.mygammaPFmetPt1p5EtaMinm2p1EtaMaxm1p4
                      +process.mygammaPFmetPt1p5EtaMinm1p4EtaMaxm1p4
                      +process.mygammaPFmetPt1p5EtaMin1p4EtaMax2p1
                      +process.mygammaPFmetPt1p5EtaMin2p1EtaMax2p4
                      +process.mygammaPFmetPt1p5EtaMin2p4EtaMax3p0
                      +process.mygammaPFmetPt1p5EtaMin3p0EtaMax5p0
                      # PT > 2.0
                      +process.mygammaPFmetPt2p0EtaMinm5p0EtaMaxm3p0
                      +process.mygammaPFmetPt2p0EtaMinm3p0EtaMaxm2p4
                      +process.mygammaPFmetPt2p0EtaMinm2p4EtaMaxm2p1
                      +process.mygammaPFmetPt2p0EtaMinm2p1EtaMaxm1p4
                      +process.mygammaPFmetPt2p0EtaMinm1p4EtaMaxm1p4
                      +process.mygammaPFmetPt2p0EtaMin1p4EtaMax2p1
                      +process.mygammaPFmetPt2p0EtaMin2p1EtaMax2p4
                      +process.mygammaPFmetPt2p0EtaMin2p4EtaMax3p0
                      +process.mygammaPFmetPt2p0EtaMin3p0EtaMax5p0
                      #h0
                      # PT > 0.0
                      +process.myh0PFmetPt0p0EtaMinm5p0EtaMaxm3p0
                      +process.myh0PFmetPt0p0EtaMinm3p0EtaMaxm2p4
                      +process.myh0PFmetPt0p0EtaMinm2p4EtaMaxm2p1
                      +process.myh0PFmetPt0p0EtaMinm2p1EtaMaxm1p4
                      +process.myh0PFmetPt0p0EtaMinm1p4EtaMaxm1p4
                      +process.myh0PFmetPt0p0EtaMin1p4EtaMax2p1
                      +process.myh0PFmetPt0p0EtaMin2p1EtaMax2p4
                      +process.myh0PFmetPt0p0EtaMin2p4EtaMax3p0
                      +process.myh0PFmetPt0p0EtaMin3p0EtaMax5p0
                      # PT > 0.5
                      +process.myh0PFmetPt0p5EtaMinm5p0EtaMaxm3p0
                      +process.myh0PFmetPt0p5EtaMinm3p0EtaMaxm2p4
                      +process.myh0PFmetPt0p5EtaMinm2p4EtaMaxm2p1
                      +process.myh0PFmetPt0p5EtaMinm2p1EtaMaxm1p4
                      +process.myh0PFmetPt0p5EtaMinm1p4EtaMaxm1p4
                      +process.myh0PFmetPt0p5EtaMin1p4EtaMax2p1
                      +process.myh0PFmetPt0p5EtaMin2p1EtaMax2p4
                      +process.myh0PFmetPt0p5EtaMin2p4EtaMax3p0
                      +process.myh0PFmetPt0p5EtaMin3p0EtaMax5p0
                      # PT > 1.0
                      +process.myh0PFmetPt1p0EtaMinm5p0EtaMaxm3p0
                      +process.myh0PFmetPt1p0EtaMinm3p0EtaMaxm2p4
                      +process.myh0PFmetPt1p0EtaMinm2p4EtaMaxm2p1
                      +process.myh0PFmetPt1p0EtaMinm2p1EtaMaxm1p4
                      +process.myh0PFmetPt1p0EtaMinm1p4EtaMaxm1p4
                      +process.myh0PFmetPt1p0EtaMin1p4EtaMax2p1
                      +process.myh0PFmetPt1p0EtaMin2p1EtaMax2p4
                      +process.myh0PFmetPt1p0EtaMin2p4EtaMax3p0
                      +process.myh0PFmetPt1p0EtaMin3p0EtaMax5p0
                      # PT > 1.5
                      +process.myh0PFmetPt1p5EtaMinm5p0EtaMaxm3p0
                      +process.myh0PFmetPt1p5EtaMinm3p0EtaMaxm2p4
                      +process.myh0PFmetPt1p5EtaMinm2p4EtaMaxm2p1
                      +process.myh0PFmetPt1p5EtaMinm2p1EtaMaxm1p4
                      +process.myh0PFmetPt1p5EtaMinm1p4EtaMaxm1p4
                      +process.myh0PFmetPt1p5EtaMin1p4EtaMax2p1
                      +process.myh0PFmetPt1p5EtaMin2p1EtaMax2p4
                      +process.myh0PFmetPt1p5EtaMin2p4EtaMax3p0
                      +process.myh0PFmetPt1p5EtaMin3p0EtaMax5p0
                      # PT > 2.0
                      +process.myh0PFmetPt2p0EtaMinm5p0EtaMaxm3p0
                      +process.myh0PFmetPt2p0EtaMinm3p0EtaMaxm2p4
                      +process.myh0PFmetPt2p0EtaMinm2p4EtaMaxm2p1
                      +process.myh0PFmetPt2p0EtaMinm2p1EtaMaxm1p4
                      +process.myh0PFmetPt2p0EtaMinm1p4EtaMaxm1p4
                      +process.myh0PFmetPt2p0EtaMin1p4EtaMax2p1
                      +process.myh0PFmetPt2p0EtaMin2p1EtaMax2p4
                      +process.myh0PFmetPt2p0EtaMin2p4EtaMax3p0
                      +process.myh0PFmetPt2p0EtaMin3p0EtaMax5p0
                      #hegammaHF
                      # PT > 0.0
                      +process.myhegammaHFPFmetPt0p0EtaMinm5p0EtaMaxm3p0
                      +process.myhegammaHFPFmetPt0p0EtaMinm3p0EtaMaxm2p4
                      +process.myhegammaHFPFmetPt0p0EtaMinm2p4EtaMaxm2p1
                      +process.myhegammaHFPFmetPt0p0EtaMinm2p1EtaMaxm1p4
                      +process.myhegammaHFPFmetPt0p0EtaMinm1p4EtaMaxm1p4
                      +process.myhegammaHFPFmetPt0p0EtaMin1p4EtaMax2p1
                      +process.myhegammaHFPFmetPt0p0EtaMin2p1EtaMax2p4
                      +process.myhegammaHFPFmetPt0p0EtaMin2p4EtaMax3p0
                      +process.myhegammaHFPFmetPt0p0EtaMin3p0EtaMax5p0
                      # PT > 0.5
                      +process.myhegammaHFPFmetPt0p5EtaMinm5p0EtaMaxm3p0
                      +process.myhegammaHFPFmetPt0p5EtaMinm3p0EtaMaxm2p4
                      +process.myhegammaHFPFmetPt0p5EtaMinm2p4EtaMaxm2p1
                      +process.myhegammaHFPFmetPt0p5EtaMinm2p1EtaMaxm1p4
                      +process.myhegammaHFPFmetPt0p5EtaMinm1p4EtaMaxm1p4
                      +process.myhegammaHFPFmetPt0p5EtaMin1p4EtaMax2p1
                      +process.myhegammaHFPFmetPt0p5EtaMin2p1EtaMax2p4
                      +process.myhegammaHFPFmetPt0p5EtaMin2p4EtaMax3p0
                      +process.myhegammaHFPFmetPt0p5EtaMin3p0EtaMax5p0
                      # PT > 1.0
                      +process.myhegammaHFPFmetPt1p0EtaMinm5p0EtaMaxm3p0
                      +process.myhegammaHFPFmetPt1p0EtaMinm3p0EtaMaxm2p4
                      +process.myhegammaHFPFmetPt1p0EtaMinm2p4EtaMaxm2p1
                      +process.myhegammaHFPFmetPt1p0EtaMinm2p1EtaMaxm1p4
                      +process.myhegammaHFPFmetPt1p0EtaMinm1p4EtaMaxm1p4
                      +process.myhegammaHFPFmetPt1p0EtaMin1p4EtaMax2p1
                      +process.myhegammaHFPFmetPt1p0EtaMin2p1EtaMax2p4
                      +process.myhegammaHFPFmetPt1p0EtaMin2p4EtaMax3p0
                      +process.myhegammaHFPFmetPt1p0EtaMin3p0EtaMax5p0
                      # PT > 1.5
                      +process.myhegammaHFPFmetPt1p5EtaMinm5p0EtaMaxm3p0
                      +process.myhegammaHFPFmetPt1p5EtaMinm3p0EtaMaxm2p4
                      +process.myhegammaHFPFmetPt1p5EtaMinm2p4EtaMaxm2p1
                      +process.myhegammaHFPFmetPt1p5EtaMinm2p1EtaMaxm1p4
                      +process.myhegammaHFPFmetPt1p5EtaMinm1p4EtaMaxm1p4
                      +process.myhegammaHFPFmetPt1p5EtaMin1p4EtaMax2p1
                      +process.myhegammaHFPFmetPt1p5EtaMin2p1EtaMax2p4
                      +process.myhegammaHFPFmetPt1p5EtaMin2p4EtaMax3p0
                      +process.myhegammaHFPFmetPt1p5EtaMin3p0EtaMax5p0
                      # PT > 2.0
                      +process.myhegammaHFPFmetPt2p0EtaMinm5p0EtaMaxm3p0
                      +process.myhegammaHFPFmetPt2p0EtaMinm3p0EtaMaxm2p4
                      +process.myhegammaHFPFmetPt2p0EtaMinm2p4EtaMaxm2p1
                      +process.myhegammaHFPFmetPt2p0EtaMinm2p1EtaMaxm1p4
                      +process.myhegammaHFPFmetPt2p0EtaMinm1p4EtaMaxm1p4
                      +process.myhegammaHFPFmetPt2p0EtaMin1p4EtaMax2p1
                      +process.myhegammaHFPFmetPt2p0EtaMin2p1EtaMax2p4
                      +process.myhegammaHFPFmetPt2p0EtaMin2p4EtaMax3p0
                      +process.myhegammaHFPFmetPt2p0EtaMin3p0EtaMax5p0
                      #tkmethemu
                      # PT > 0.0
                      +process.mytkmethemuPFmetPt0p0EtaMinm5p0EtaMaxm3p0
                      +process.mytkmethemuPFmetPt0p0EtaMinm3p0EtaMaxm2p4
                      +process.mytkmethemuPFmetPt0p0EtaMinm2p4EtaMaxm2p1
                      +process.mytkmethemuPFmetPt0p0EtaMinm2p1EtaMaxm1p4
                      +process.mytkmethemuPFmetPt0p0EtaMinm1p4EtaMaxm1p4
                      +process.mytkmethemuPFmetPt0p0EtaMin1p4EtaMax2p1
                      +process.mytkmethemuPFmetPt0p0EtaMin2p1EtaMax2p4
                      +process.mytkmethemuPFmetPt0p0EtaMin2p4EtaMax3p0
                      +process.mytkmethemuPFmetPt0p0EtaMin3p0EtaMax5p0
                      # PT > 0.5
                      +process.mytkmethemuPFmetPt0p5EtaMinm5p0EtaMaxm3p0
                      +process.mytkmethemuPFmetPt0p5EtaMinm3p0EtaMaxm2p4
                      +process.mytkmethemuPFmetPt0p5EtaMinm2p4EtaMaxm2p1
                      +process.mytkmethemuPFmetPt0p5EtaMinm2p1EtaMaxm1p4
                      +process.mytkmethemuPFmetPt0p5EtaMinm1p4EtaMaxm1p4
                      +process.mytkmethemuPFmetPt0p5EtaMin1p4EtaMax2p1
                      +process.mytkmethemuPFmetPt0p5EtaMin2p1EtaMax2p4
                      +process.mytkmethemuPFmetPt0p5EtaMin2p4EtaMax3p0
                      +process.mytkmethemuPFmetPt0p5EtaMin3p0EtaMax5p0
                      # PT > 1.0
                      +process.mytkmethemuPFmetPt1p0EtaMinm5p0EtaMaxm3p0
                      +process.mytkmethemuPFmetPt1p0EtaMinm3p0EtaMaxm2p4
                      +process.mytkmethemuPFmetPt1p0EtaMinm2p4EtaMaxm2p1
                      +process.mytkmethemuPFmetPt1p0EtaMinm2p1EtaMaxm1p4
                      +process.mytkmethemuPFmetPt1p0EtaMinm1p4EtaMaxm1p4
                      +process.mytkmethemuPFmetPt1p0EtaMin1p4EtaMax2p1
                      +process.mytkmethemuPFmetPt1p0EtaMin2p1EtaMax2p4
                      +process.mytkmethemuPFmetPt1p0EtaMin2p4EtaMax3p0
                      +process.mytkmethemuPFmetPt1p0EtaMin3p0EtaMax5p0
                      # PT > 1.5
                      +process.mytkmethemuPFmetPt1p5EtaMinm5p0EtaMaxm3p0
                      +process.mytkmethemuPFmetPt1p5EtaMinm3p0EtaMaxm2p4
                      +process.mytkmethemuPFmetPt1p5EtaMinm2p4EtaMaxm2p1
                      +process.mytkmethemuPFmetPt1p5EtaMinm2p1EtaMaxm1p4
                      +process.mytkmethemuPFmetPt1p5EtaMinm1p4EtaMaxm1p4
                      +process.mytkmethemuPFmetPt1p5EtaMin1p4EtaMax2p1
                      +process.mytkmethemuPFmetPt1p5EtaMin2p1EtaMax2p4
                      +process.mytkmethemuPFmetPt1p5EtaMin2p4EtaMax3p0
                      +process.mytkmethemuPFmetPt1p5EtaMin3p0EtaMax5p0
                      # PT > 2.0
                      +process.mytkmethemuPFmetPt2p0EtaMinm5p0EtaMaxm3p0
                      +process.mytkmethemuPFmetPt2p0EtaMinm3p0EtaMaxm2p4
                      +process.mytkmethemuPFmetPt2p0EtaMinm2p4EtaMaxm2p1
                      +process.mytkmethemuPFmetPt2p0EtaMinm2p1EtaMaxm1p4
                      +process.mytkmethemuPFmetPt2p0EtaMinm1p4EtaMaxm1p4
                      +process.mytkmethemuPFmetPt2p0EtaMin1p4EtaMax2p1
                      +process.mytkmethemuPFmetPt2p0EtaMin2p1EtaMax2p4
                      +process.mytkmethemuPFmetPt2p0EtaMin2p4EtaMax3p0
                      +process.mytkmethemuPFmetPt2p0EtaMin3p0EtaMax5p0
                      )

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = cms.untracked.vstring('keep *')
    )

process.endpath = cms.EndPath(process.out)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
