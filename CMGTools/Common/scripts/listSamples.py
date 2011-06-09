#!/usr/bin/env python
# Colin
# creates new source file for a dataset on castor
# compiles the python module
# prints the line to be added to the cfg. 

import os, sys,  imp, re, pprint, string
from optparse import OptionParser

import castortools


def allSampleInfo( sampleName, listLevel ):

    if listLevel == 3:
        os.system("rfdir %s | awk '{print \"%s/\"$9}'" % (castorDir,castorDir) )
        return

    print sampleName

    if listLevel>0:
        print '------------------------------------------------------------------------------------------------------------'
        print 'castor:'
        print castorDir
        print castortools.castorToLFN(castorDir)
    if listLevel>1: 
        os.system('rfdir ' + castorDir )
    if listLevel>0:
        print 'local:'
        print localDir
    if os.path.isdir( localDir ):
        if listLevel>1:
            os.system('ls -l ' + localDir )
            print localDir + '*.root'
    else:
        if listLevel>0:
            print 'TO BE IMPORTED'
    if listLevel>0:
        print
        print


parser = OptionParser()
parser.usage = "%prog <sampleName>\nImport a sample locally."
parser.add_option("-n", "--negate", action="store_true",
                  dest="negate",
                  help="do not proceed",
                  default=False)

import castorBaseDir

parser.add_option("-u", "--user", 
                  dest="user",
                  help="user who is the owner of the castor base directory",
                  default=os.environ['USER'] )
parser.add_option("-d", "--localBaseDir", 
                  dest="localBaseDir",
                  help="Local directory.",
                  default="/afs/cern.ch/user/c/cbern/localscratch/Data/Analysis/SusyJetMET")
parser.add_option("-l", "--listLevel", 
                  dest="listLevel", 
                  help="list level",
                  default=False)

(options,args) = parser.parse_args()

if len(args)!=1:
    parser.print_help()
    sys.exit(1)


castorDir = ""
localDir = ""

dataSets = '/afs/cern.ch/user/'
dataSets += options.user[0]
dataSets += '/'
dataSets += options.user + "/public/DataSets.txt"


ifile=open(dataSets,'r')

pattern = re.compile( args[0] )

castorBaseDir = castortools.lfnToCastor(castorBaseDir.castorBaseDir( options.user ))

for line in ifile.readlines():
    line = line.rstrip()

    if line[0]!='/': continue 

    if pattern.search( line ):
        # preparing castor dir -----------------

        sampleName = line

        try:
            castorDir = castorBaseDir + sampleName
        except:
            sys.exit(1)

        # making local source directory ---------
        
        localDir = options.localBaseDir
        localDir += sampleName

        allSampleInfo( sampleName, int(options.listLevel) )


    

# allSampleInfo( sampleName, options.listLevel)
