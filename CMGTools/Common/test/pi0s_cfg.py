from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools pi0 reconstruction"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cbern',
    '/QCD_Pt-30to50_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM/V2',
    'PFAOD.*root'
    ) 


# reading the first 10 files:
nFiles = 10
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
# process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_pi0s.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = everything
process.out.outputCommands.extend( ['keep *_particleFlow__*',
                                    'keep *_diPhotonSel_*_*'] ) 

process.out.dropMetaData = cms.untracked.string('PRIOR')

# select photons 

process.pfPhotonSel = cms.EDFilter(
    "GenericPFCandidateSelector",
    src = cms.InputTag("particleFlow"),
    cut = cms.string("energy()>2 && particleId()==4 && abs( eta() )<1.4")
)

# make cmg PFCandidates:

from CMGTools.Common.factories.cmgPFCandidate_cfi import *
process.photonSel = cmgPFCandidate.clone()
process.photonSel.cfg.inputCollection = 'pfPhotonSel'

# make di-photons

from CMGTools.Common.factories.cmgDiPFCandidate_cfi import *

process.diPhoton = cmgDiPFCandidate.clone()
process.diPhoton.cfg.leg1Collection = 'photonSel'
process.diPhoton.cfg.leg2Collection = 'photonSel'
process.diPhoton.cfg.metCollection = ''

from CMGTools.Common.skims.cmgDiPFCandidateSel_cfi import *
process.diPhotonSel = cmgDiPFCandidateSel.clone(
    src = 'diPhoton',
    cut = 'mass()<1'
    )

process.p = cms.Path(
    # select PF photons
    process.pfPhotonSel
    # make cmg PFCandidates
    + process.photonSel 
    # make di photons
    + process.diPhoton 
    # select di photons
    + process.diPhotonSel
  
)

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
