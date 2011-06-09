#!/usr/bin/env python
# colin
# additional layer, on top of cmsBatch.py

import os, sys,  imp, re, pprint, string
from optparse import OptionParser

import castortools

from addToDatasets import *

parser = OptionParser()
parser.usage = "fwBatch.py <cfg> <sampleName>\n\nExample:\n"

# parser.add_option("-n", "--negate", action="store_true",
#                  dest="negate",
#                  help="do not proceed",
#                  default=False)



parser.add_option("-t", "--tier", 
                  dest="tier",
                  help="Tier: extension you can give to specify you are doing a new production",
                  default="")
parser.add_option("-o", "--other", action="store_true",
                  dest="other", 
                  help="reading other people's files",
                  default=False)
parser.add_option("-N", "--numberOfInputFiles", 
                  dest="nInput",
                  help="number of input files per job",
                  default="5")
parser.add_option("-q", "--queue", 
                  dest="queue",
                  help="batch queue",
                  default="8nh")


(options,args) = parser.parse_args()

if len(args)!=2:
    parser.print_help()
    sys.exit(1)

cfg = args[0]
sampleName = args[1]

import castorBaseDir
destBaseDir = castorBaseDir.myCastorBaseDir()

#if options.castorBaseDir.find('/castor/cern.ch/user/c/cbern') == -1:
#    destBaseDir = castorBaseDir.defaultCastorBaseDir


print 'starting prod for sample:', sampleName

# preparing castor dir -----------------

# cdir = options.castorBaseDir 

if options.tier != "":
    sampleName += "/" + options.tier

print "sampleName ",sampleName
outFile = destBaseDir
outFile += sampleName

# prepare local output dir:
localOutputDir = './' + sampleName 
mkdir = 'mkdir -p ' + localOutputDir
print mkdir
os.system(mkdir)

# the output castor directory will be prepared by cmsBatch

cmsBatch = 'cmsBatch.py %s %s -r %s -b "%s" -o %s' % (options.nInput, cfg, outFile, options.queue, localOutputDir)

addToDatasets( sampleName )


print cmsBatch
