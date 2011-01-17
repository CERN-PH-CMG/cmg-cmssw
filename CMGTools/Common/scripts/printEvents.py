#!/usr/bin/env python

import FWCore.ParameterSet.Config as cms

import os,sys,re
from optparse import OptionParser

parser = OptionParser()
parser.usage = "%prog <file> : prints all events in an EDM file"

(options,args) = parser.parse_args()

process = cms.Process("COPY")

if len(args)!=1:
    parser.print_help()
    sys.exit(1)
    
file = args[0]

process.source = cms.Source(
        "PoolSource",
        noEventSort = cms.untracked.bool(True),
        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
        fileNames = cms.untracked.vstring()
)
process.source.fileNames.extend([
                'file:%s' % file,
])


# print 'Printing events in file:'
fileName = process.source.fileNames[0]
fileName = fileName.replace('file:','')
print fileName 

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

outFile = open("tmpConfig.py","w")
outFile.write("import FWCore.ParameterSet.Config as cms\n")
outFile.write(process.dumpPython())
outFile.close()

from subprocess import Popen, PIPE, STDOUT

cmd = 'cmsRun tmpConfig.py'
p = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
# output = p.stdout.read()

# print 'toto', output

pattern = re.compile('Run\s+(\d+), Event\s+(\d+), LumiSection\s+(\d+)')

while 1:
    line = p.stdout.readline()
    if line:
        line.rstrip()
        m = pattern.search( line )
        if m:
            print 'Run: ',m.group(1).rjust(7), '\tEvent: ', m.group(2).rjust(12), '\tLumi: ', m.group(3).rjust(10)
    else:
        break

print
