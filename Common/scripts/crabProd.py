#!/usr/bin/env python
# Colin
# interface to CRAB, a bit like multicrab

import os, sys,  imp, re, pprint, string
from optparse import OptionParser

import castortools
from addToDatasets import *

parser = OptionParser()
parser.usage = "%prog <dataset name>\nPrepare a local directory where to run crab, for a given dataset. You need to have a valid crab.cfg in the current directory. This script will deal with the preparation of your destination directory on castor, and will add the dataset to your local database (~/DataSets.txt)."

parser.add_option("-t", "--tier", 
                  dest="tier",
                  help="Tier: extension you can give to specify you are doing a new production. The resulting dataset will be called dataset/tier.",
                  default="")
parser.add_option("-f", "--force", action="store_true",
                  dest="force", 
                  help="Force creation of the destination castor directory. To be used with care, first run without this option",
                  default=False)


(options,args) = parser.parse_args()

if len(args)!=1:
    parser.print_help()
    sys.exit(1)

sampleName = args[0]


sampleNameDir = sampleName
if options.tier != "":
    sampleNameDir += "/" + options.tier
    

# preparing castor dir -----------------

import castorBaseDir
cdir = castortools.lfnToCastor( castorBaseDir.myCastorBaseDir() )
cdir += sampleNameDir

if castortools.isCastorFile( cdir ) and not options.force:
    print 'The destination castor directory already exists:'
    print cdir
    print 'Please check. If everything is fine, run again with the -f option.'
    sys.exit(1)

rfmkdir = 'rfmkdir -p ' + cdir
print rfmkdir
os.system( rfmkdir )
rfchmod = 'rfchmod 775 ' + cdir
print rfchmod
os.system( rfchmod )

# making local crab directory ---------
ldir = '.' + sampleNameDir

mkdir = 'mkdir -p ' + ldir
print mkdir
os.system( mkdir )

#cpcrab = 'cp crab.cfg %s/crab.cfg' % ldir
#print cpcrab
#os.system( cpcrab )

#prepare the crab file
newCrabPath = '%s/crab.cfg' % ldir
print newCrabPath

newCrab = open(newCrabPath,'w')
try:
    oldCrab = open('crab.cfg','r')
except:
    sys.exit(1)
newPSet = ""
newJson = ""

patternDataSet = re.compile("\s*datasetpath")
patternRemoteDir = re.compile('\s*user_remote_dir')
patternPSet = re.compile('pset\s*=\s*(.*py)\s*')
patternLumiMask = re.compile('lumi_mask\s*=\s*(\S+)\s*')

for line in oldCrab.readlines():
    if patternDataSet.match( line ):
        # removing dataset lines
        continue
    if patternRemoteDir.match( line ):
        # removing remote dir lines 
        continue
    # find and copy parameter set cfg
    match = patternPSet.match( line )
    if match != None:
        pset  = match.group(1)
        newPSet = ldir + "/" + match.group(1)
        os.system('cp %s %s' % (pset, newPSet) )
    # find and copy json file
    match = patternLumiMask.match( line )
    if match != None:
        json  = match.group(1)
        newJson = ldir + "/" + match.group(1)
        os.system('cp %s %s' % (json, newJson) )
   
    newCrab.write( line )

newCrab.write('[CMSSW]\n')
newCrab.write('datasetpath = '+sampleName+'\n')

outDir = cdir.replace('/castor/cern.ch','')
newCrab.write('[USER]\n')
newCrab.write('user_remote_dir = %s\n' % outDir  )

addToDatasets( sampleNameDir ) 


print ''
print 'SUMMARY'
print cdir
print ldir
print newCrabPath
print newPSet
print newJson


