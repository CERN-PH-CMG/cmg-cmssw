from PhysicsTools.PatAlgos.patTemplate_cfg import *

########## CONTROL CARDS


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 5

dataset_user = 'cmgtools'
dataset_name = None

eventSource = 'DYJets'

if eventSource == 'WJets':
    dataset_name = '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0'
elif eventSource == 'DYJets':
    dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0'
else:
    raise ValueError('eventSource should be WJets or DYJets, not ' + eventSource)

##########



# Input  & JSON             -------------------------------------------------


process.setName_('ANA')

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    'tree.*root') 


# restricting the number of files to process to a given number
if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]



# Sequence & path definition -------------------------------------------------

# a few printers which can be useful.just add them to the path if needed

process.tauMuPrint = cms.EDAnalyzer(
    "PhysicsObjectPrinter",
    inputCollection = cms.untracked.InputTag('cmgTauMuSel'),
    reportEvery = cms.untracked.uint32(1)
    )

process.recoilJetPrint = cms.EDAnalyzer(
    "PhysicsObjectPrinter",
    inputCollection = cms.untracked.InputTag('cmgPFJetForRecoil'),
    reportEvery = cms.untracked.uint32(1)
    )

process.jetPrint = process.recoilJetPrint.clone( inputCollection='cmgPFJetSel')

process.list = cms.EDAnalyzer(
    "ParticleListDrawer",
    maxEventsToPrint = cms.untracked.int32(10),
    printVertex = cms.untracked.bool(False),
    src = cms.InputTag("genWorZ")
    )

# preparing the input for the recoil correction sequence

process.load('CMGTools.Common.diTau_cff')
process.load('CMGTools.Common.skims.cmgTauMuCount_cfi')
# this mass cut is to remove tau-mu objects where the tau and the mu are made from the same PFCandidates
process.cmgTauMuSel.cut = 'mass()>10'
process.cmgTauMuCount.minNumber = 1 

# the recoil correction sequence

process.load('CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff')

process.recoilCorrectedMET.verbose = False

# creating an updated di-tau collection, with the new MET collection

process.cmgTauMuCor = cms.EDFilter(
    "TauMuUpdatePOProducer",
    cuts = cms.PSet(),
    cfg = cms.PSet( diObjectCollection = cms.InputTag('cmgTauMuSel'),
                    metCollection = cms.InputTag('recoilCorrectedMET'),
                    leg1Collection = cms.InputTag(''),
                    leg2Collection = cms.InputTag('')
    )
    )


if eventSource == 'DYJets':
    from CMGTools.Common.generator.metRecoilCorrection.recoilCorrectedMET_cfi import rootfile_dir
    print rootfile_dir
    process.recoilCorrectedMET.fileCorrectTo = rootfile_dir + 'recoilfit_zjets_ltau_njet.root'
    process.recoilCorrectedMET.leptonLeg = 0

process.p = cms.Path(
    process.diTauSequence +
    process.cmgTauMuCount +
    process.metRecoilCorrectionSequence +
    process.cmgTauMuCor
    # process.genWorZ +
    # process.list
    )

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )


# OUTPUT definition ----------------------------------------------------------


treeName = 'tree_testMetRecoilCorrections.root'
process.out.fileName = cms.untracked.string( treeName )
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = everything
process.out.outputCommands.extend([ 'keep *_cmgTauMuSel_*_*',
                                    'keep *_cmgPFJetForRecoil_*_*',
                                    'keep *_genWorZ_*_*',
                                    'keep *_recoilCorrectedMET_*_*',
                                    'keep *_cmgTauMuCor_*_*'])



# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )


sep_line = '-'*70

print sep_line
print 'INPUT:'
print sep_line
print process.source.fileNames
print
print 'OUTPUT'
print sep_line
print treeName
