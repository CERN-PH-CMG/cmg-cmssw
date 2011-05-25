from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

sep_line = "-" * 50
print
print sep_line
print "CMGTools main test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

process.source.fileNames = cms.untracked.vstring(
    # 'file:patTuple_HT.root'
    'file:/afs/cern.ch/user/c/cbern/scratch0/patTuple_PF2PAT.root'
    )

# process.load("CMGTools.Common.sources.relval.RelValQCD_FlatPt_15_3000.CMSSW_3_11_2.MC_311_V2.source_cff")
# process.load("CMGTools.Common.sources.relval.RelValTTbar.CMSSW_3_11_2.MC_311_V2.source_cff")
# process.source.fileNames = cms.untracked.vstring(
#    'file:input.root'
#    )
# process.load("CMGTools.Common.sources.relval.RelValTTbar.CMSSW_3_11_2.MC_311_V2.source_cff")

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_testCMGTools.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( everything ) 
process.out.dropMetaData = cms.untracked.string('PRIOR')
# not much change
# ttbar 13.4 QCD170to300 8.6 metaData 4.0 HT metaData 3.7
# process.out.outputCommands.append( 'drop pat*_*_*_*' )
# ttbar 11.2 QCD170to300 7.4
# process.out.outputCommands.append( 'drop cmgTriggerObjects*_*_*_*' )
# ttbar 9.2  QCD170to300 6.0
# process.out.outputCommands.append( 'drop cmgBaseJets_*_*_*' )
# ttbar 8.7  QCD170to300 5.8
# process.out.outputCommands.append( 'drop cmgPFJets_*_*_*' )
# ttbar 5.4  QCD170to300 4.9
# process.out.outputCommands.append( 'drop recoGenParticles_*_*_*' )
# ttbar 5.1  QCD170to300 4.8, metaData 1.4
# process.out.outputCommands.append( 'drop *' )


#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_testCMGTools.root"))

# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')

# now, we're going to tune the default analysis sequence to our needs
# by modifying the parameters of the modules present in this sequence. 

# Select events with 2 jet ...  
# process.cmgPFJetCount.minNumber = 2
# with pT > 50.
# process.cmgPFJetSel.cut = "pt()>50"
# and MET larger than 50
# process.cmgPFMETSel.cut = "pt()>50"

# note: we're reading ttbar events

runOnMC = False

if runOnMC:
    process.load("CMGTools.Common.runInfoAccounting_cfi")
    process.outpath += process.runInfoAccounting

process.p = cms.Path(
    process.analysisSequence
)
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
