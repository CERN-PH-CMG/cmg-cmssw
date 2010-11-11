#!/usr/bin/env python
# Colin
# creates new source file for a dataset on castor
# compiles the python module
# prints the line to be added to the cfg. 

import os, sys,  imp, re, pprint, string
from optparse import OptionParser

import castortools
import pdb


def allSampleInfo( sampleName, listFiles ):
    print
    print
    print sampleName
    print '------------------------------------------------------------------------------------------------------------'
    print 'castor:'
    print castorDir
    if listFiles: 
        os.system('rfdir ' + castorDir )
    print 'local:'
    print localDir
    if os.path.isdir( localDir ):
        if listFiles:
            os.system('ls -l ' + localDir )
            print localDir + '*.root'
    else:
        print 'TO BE IMPORTED'
       


parser = OptionParser()
parser.usage = "%prog <sampleName>\nImport a sample locally."
parser.add_option("-n", "--negate", action="store_true",
                  dest="negate",
                  help="do not proceed",
                  default=False)
parser.add_option("-c", "--castorBaseDir", 
                  dest="castorBaseDir",
                  help="Base castor directory.",
                  default="/castor/cern.ch/user/c/cbern/cmst3/RA2SusyJetMET")
parser.add_option("-d", "--localBaseDir", 
                  dest="localBaseDir",
                  help="Local directory.",
                  default="/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET")
parser.add_option("-l", "--listFiles", 
                  dest="listFiles", action="store_true",
                  help="list files",
                  default=False)
## parser.add_option("-p", "--pattern", 
##                   dest="pattern",
##                   help="pattern for root files in castor dir",
##                   default=".*root")

(options,args) = parser.parse_args()

if len(args)!=1:
    parser.print_help()
    sys.exit(1)


castorDir = ""
localDir = ""

dataSets = os.environ['HOME'] + "/DataSets.txt"


ifile=open(dataSets,'r')

pattern = re.compile( args[0] )

for line in ifile.readlines():
    line = line.rstrip()

    
    if pattern.search( line ):
        # preparing castor dir -----------------

        sampleName = line
        
        castorDir = options.castorBaseDir
        castorDir += sampleName
    
        # making local source directory ---------
        
        localDir = options.localBaseDir
        localDir += sampleName

        allSampleInfo( sampleName, options.listFiles)


    

# allSampleInfo( sampleName, options.listFiles)
