from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import sys

### rename this process
process.setName_('CMG')

isData=False

### SOURCES
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import getLocalSourceFor
theLocalSrc='GluGluToH200ToZZTo2L2Nu'
if(len(sys.argv)>2 ): theLocalSrc = sys.argv[2]
ffile=0
if(len(sys.argv)>3 ): ffile = int(sys.argv[3])
fstep=-1
if(len(sys.argv)>4 ): fstep = int(sys.argv[4])
fnames = getLocalSourceFor( theLocalSrc, ffile, fstep ) 
#fnames = cms.untracked.vstring('/store/cmst3/user/wreece/VJets/3_8_7/020211/Zmumu/PF2PAT_MuonFilter_99_1_8aQ.root')
process.source.fileNames=fnames
print ' ***** Local source is defined for ' + theLocalSrc + ' with ' + str(len(process.source.fileNames)) + ' files'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

### common selection
process.load('CMGTools.HtoZZ2l2nu.StandardSelection_cff')

### configure the process
if(not isData):
    print '*** MC based analysis'
    process.e = cms.EndPath(process.saveHistosInRunInfo*process.out)
    process.schedule = cms.Schedule(process.genPath,process.leptonPath,process.eePath,process.mumuPath,process.emuPath,process.jetmetPath,process.e)
else :
    print '*** Data based analysis'
    process.e = cms.EndPath(process.out)
    process.schedule = cms.Schedule(process.leptonPath,process.eePath,process.mumuPath,process.emuPath,process.jetmetPath,process.e)

print '*** Scheduling the following sequence: '
print process.schedule

### output file for histograms etc
import getpass
outdir='/tmp/'+getpass.getuser()+'/'+theLocalSrc+'_'+str(ffile)
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(outdir+'_histograms.root'))


process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.out.outputCommands = process.outputConfig.outputCommandsHtoZZ
process.out.fileName = cms.untracked.string(outdir+'_cmgTuple.root')
process.out.SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('eePath','mumuPath','emuPath') )


process.options.SkipEvent = cms.untracked.vstring('ProductNotFound')

