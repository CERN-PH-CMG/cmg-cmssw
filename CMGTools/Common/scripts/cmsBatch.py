#!/usr/bin/env python
# Colin
# batch mode for cmsRun, March 2009


import os, sys,  imp, re, pprint, string
from optparse import OptionParser

# particle flow specific
from batchmanager import BatchManager
import castortools

# cms specific
import FWCore.ParameterSet.Config as cms
from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper




def batchScriptCCIN2P3():
   script = """!/usr/bin/env bash
#PBS -l platform=LINUX,u_sps_cmsf,M=2000MB,T=2000000
#PBS -q T
#PBS -eo
#PBS -me
#PBS -V

source $HOME/.bash_profile

echo '***********************'

ulimit -v 3000000

# coming back to submission dir do setup the env
cd $PBS_O_WORKDIR
eval `scramv1 ru -sh`


# back to the worker
cd -

# copy job dir here
cp -r $PBS_O_WORKDIR .

# go inside
jobdir=`ls`
echo $jobdir

cd $jobdir

cat > sysinfo.sh <<EOF
#! env bash
echo '************** ENVIRONMENT ****************'

env

echo
echo '************** WORKER *********************'
echo

free
cat /proc/cpuinfo 

echo
echo '************** START *********************'
echo
EOF

source sysinfo.sh > sysinfo.txt

cmsRun run_cfg.py

# copy job dir do disk
cd -
cp -r $jobdir $PBS_O_WORKDIR
"""
   return script


def batchScriptCERN( remoteDir, index ):
   
   
   script = """#!/bin/bash
#BSUB -q 8nm
echo 'environment:'
echo
env
ulimit -v 3000000
echo 'copying job dir to worker'
cd $LS_SUBCWD
eval `scramv1 ru -sh`
cd -
cp -rf $LS_SUBCWD .
ls
cd `find . -type d | grep /`
echo 'running'
%s run_cfg.py
echo
echo 'sending the job directory back'
""" % prog
   
#   castorCopy = ''


   if remoteDir != '':
      script += """
for file in *.root; do
newFileName=`echo $file | sed -r -e 's/\./_%s\./'`
cmsStage $file %s/$newFileName 
done
""" % (index, remoteDir)
          
   script += 'rm *.root\n'
   script += 'cp -rf * $LS_SUBCWD\n'
   
   return script


def batchScriptLocal(  remoteDir, index ):
   
   
   script = """#!/bin/bash
echo 'running'
%s run_cfg.py
echo
echo 'sending the job directory back'
""" % prog

   if remoteDir != '':
      script += """
for file in *.root; do
newFileName=`echo $file | sed -r -e 's/\./_%s\./'`
cmsStage $file %s/$newFileName 
done
""" % (index, remoteDir)

   return script


class MyBatchManager( BatchManager ):

    # prepare a job
    def PrepareJobUser(self, jobDir, value ):

       process.source = fullSource.clone()

       #prepare the batch script
       scriptFileName = jobDir+'/batchScript.sh'
       scriptFile = open(scriptFileName,'w')

       # are we at CERN or somewhere else? testing the afs path
       cwd = os.getcwd()
       
       patternLxplus = re.compile( '^lxplus' )
       onLxplus = patternLxplus.match( os.environ['HOSTNAME'] ) 
       patternBatch = re.compile( '\w*bsub')
       wantBatch = patternBatch.match( options.batch )

       local = False

       storeDir = self.remoteOutputDir_.replace('/castor/cern.ch/cms','')
       
       if onLxplus:
          print 'Running on LXPLUS'
          if wantBatch:
             print 'Wanna use batch system'
             scriptFile.write( batchScriptCERN( storeDir,
                                                value) )
          else:
             print 'Wanna use local ressources'
             local = True
       else:
          if wantBatch:
             print "your -b option contains the string bsub, and you're not on lxplus... aborting"
             sys.exit(1)
          else:
             print 'Not on lxplus, running locally'
             local = True

       if local:
          scriptFile.write( batchScriptLocal( storeDir,
                                              value) )          
  
          
       scriptFile.close()
       os.system('chmod +x %s' % scriptFileName)

       #prepare the cfg
       
       # replace the list of fileNames by a chunk of filenames:
       if generator:
          randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
          randSvc.populate()
       else:

          print "grouping : ", grouping
          print "value : ", value
          
          iFileMin = (value)*grouping 
          iFileMax = (value+1)*grouping 
          
          process.source.fileNames = fullSource.fileNames[iFileMin:iFileMax]
          print process.source
          
       cfgFile = open(jobDir+'/run_cfg.py','w')
       cfgFile.write(process.dumpPython())
       cfgFile.close()
       
    def SubmitJob( self, jobDir ):
       os.system( self.options_.batch )


batchManager = MyBatchManager()


file = open('cmsBatch.txt', 'w')
file.write(string.join(sys.argv) + "\n")
file.close()

batchManager.parser_.usage = """
%prog [options] <number of input files per job> <your_cfg.py>.

Submits a number of jobs taking your_cfg.py as a template. your_cfg.py can either read events from input files, or produce them with a generator. In the later case, the seeds are of course updated for each job.

A local output directory is created locally. This directory contains a job directory for each job, and a Logger/ directory containing information on the software you are using. 
By default:
- the name of the output directory is created automatically.
- the output root files end up in the job directories.

Each job directory contains:
- the full python configuration for this job. You can run it interactively by doing:
cmsRun run_cfg.py
- the batch script to run the job. You can submit it again by calling the batch command yourself, see the -b option.
- while running interactively: nohup.out, where the job stderr and stdout are redirected. To check the status of a job running interactively, do:
tail nohup.out
- after running:
  o the full nohup.out (your log) and your root files, in case you ran interactively
  o the LSF directory, in case you ran on LSF

Also see fwBatch.py, which is a layer on top of cmsBatch.py adapted to the organization of our samples on the CMST3. 

Examples:

First do:
cd $CMSSW_BASE/src/CMGTools/Common/test

to run on your local machine:
cmsBatch.py 1 testCMGTools_cfg.py -b 'nohup ./batchScript.sh&' 

to run on LSF (you must be logged on lxplus, not on your interactive machine, so that you have access to LSF)
cmsBatch.py 1 testCMGTools_cfg.py -b 'bsub -q 8nm < ./batchScript.sh' 
"""
batchManager.parser_.add_option("-p", "--program", dest="prog",
                                help="program to run on your cfg file",
                                default="cmsRun")
batchManager.parser_.add_option("-b", "--batch", dest="batch",
                                help="batch command. default is: 'bsub -q 8nh < batchScript.sh'. You can also use 'nohup < ./batchScript.sh &' to run locally.",
                                default="bsub -q 8nh < .batchScript.sh")

(options,args) = batchManager.parser_.parse_args()
batchManager.ParseOptions()

prog = options.prog

if len(args)!=2:
   batchManager.parser_.print_help()
   sys.exit(1)

grouping = int(args[0])
nJobs = grouping
cfgFileName = args[1]
# queue = options.queue

print 'Loading cfg'

# load cfg script
handle = open(cfgFileName, 'r')
cfo = imp.load_source("pycfg", cfgFileName, handle)
process = cfo.process
handle.close()

# keep track of the original source
fullSource = process.source.clone()



generator = False

try:
   process.source.fileNames
except:
   print 'No input file. This is a generator process.'
   generator = True
   listOfValues = range( 0, nJobs)
else:
   print "Number of files in the source:",len(process.source.fileNames), ":"
   pprint.pprint(process.source.fileNames)
   
   nFiles = len(process.source.fileNames)
   nJobs = nFiles / grouping
   if (nJobs!=0 and (nFiles % grouping) > 0) or nJobs==0:
      nJobs = nJobs + 1
      
   print "number of jobs to be created: ", nJobs

   listOfValues = range( 0, nJobs)


batchManager.PrepareJobs( listOfValues )

batchManager.SubmitJobs()

from logger import logger

oldPwd = os.getcwd()
os.chdir(batchManager.outputDir_)
logDir = 'Logger'
os.system( 'mkdir ' + logDir )
log = logger( logDir )
log.logCMSSW()
#COLIN not so elegant... but tar is behaving in a strange way.
log.addFile( oldPwd + '/' + cfgFileName )

if not batchManager.options_.negate:
   if batchManager.remoteOutputDir_ != "":
      # we don't want to crush an existing log file on castor
      #COLIN could protect the logger against that.
      log.stageOut( batchManager.remoteOutputDir_ )
      
os.chdir( oldPwd )

