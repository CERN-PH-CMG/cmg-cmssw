from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# Unit test for the jet analysis sequence.

sep_line = "-" * 50
print
print sep_line
print "Jet sequence test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )
process.maxLuminosityBlocks = cms.untracked.PSet(
        input = cms.untracked.int32(1)
        )
process.source.fileNames = ['/store/cmst3/user/cmgtools/CMG/RelValQCD_Pt_80_120/CMSSW_4_2_3-START42_V12-v2/GEN-SIM-RECO/patTuple_PF2PAT_0.root']

extension = 'jet'

# Output module for EDM event (ntuple).
process.out.fileName = cms.untracked.string('tree_test_%s.root' % extension)
from CMGTools.Common.eventContent.everything_cff import everything
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands.extend(everything)
process.out.outputCommands.append('keep patJets_*_*_*')
process.out.outputCommands.append('keep recoGenJets_*_*_*')

# Output file for histograms etc.
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_test_%s.root" % extension))

# Default analysis sequence.
process.load('CMGTools.Common.analysis_cff')

process.load("CMGTools.Common.jet_cff")
process.load("CMGTools.Common.factories.cmgPFJetScaler_cfi")
process.load('CMGTools.Common.skims.patJetLead_cfi')

##########

# Specific to the testing of the JEC up/down variations.
process.pfJetSequence.replace(process.cmgPFJet,
                              process.cmgPFJet + process.cmgPFJetUp + process.cmgPFJetDown)

process.jetTestHistogramsUp = cms.EDAnalyzer(
    "CmgBaseJetHistograms",
    inputCollection = cms.InputTag("cmgPFJet"),
    inputCollectionY = cms.untracked.InputTag("cmgPFJetUp"),
    histograms = cms.untracked.PSet(
        pt = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('pt()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                title = cms.untracked.string("p_{T}")
                )
            ),
        pt_vs_pt = cms.untracked.VPSet(
            cms.untracked.PSet(
                var = cms.untracked.string('pt()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                title = cms.untracked.string("p_{T}")
                ),
            cms.untracked.PSet(
                var = cms.untracked.string('pt()'),
                nbins = cms.untracked.int32(100),
                low = cms.untracked.double(0),
                high = cms.untracked.double(200),
                title = cms.untracked.string("p_{T}")
                )
            ),
        )
    )
process.jetTestHistogramsMid = process.jetTestHistogramsUp.clone(inputCollectionY = cms.untracked.InputTag("cmgPFJet"))
process.jetTestHistogramsDown = process.jetTestHistogramsUp.clone(inputCollectionY = cms.untracked.InputTag("cmgPFJetDown"))
process.jetTestHistoSequence = cms.Sequence(
    process.jetTestHistogramsUp +
    process.jetTestHistogramsMid +
    process.jetTestHistogramsDown
    )

process.out.outputCommands.append('keep cmgPFJets_*_*_*')

##########

process.p = cms.Path(
    process.jetSequence +
    process.patJetLead +
    process.jetTestHistoSequence
)

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

print 'input  : ', process.source.fileNames
print 'output :'
print '  tree :', process.out.fileName
print '  hist :', process.TFileService.fileName
