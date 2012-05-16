#!/bin/env python

import sys
import re
import os
import pprint

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-b","--batch", dest="batch",
                  default=None,
                  help="batch command for resubmission")

(options,args) = parser.parse_args()

if len(args)==0:
    print 'provide at least one directory in argument'

dirs = sys.argv[1:]

badDirs = []

for dir in dirs:
    if not os.path.isdir(dir):
        continue
    if dir.find('_Chunk') == -1:
        continue
    logName  = '/'.join([dir, 'log.txt'])
    if not os.path.isfile( logName ):
        print dir, ': log.txt does not exist'
        badDirs.append(dir)
        continue
    logFile = open(logName)
    nEvents = -1
    for line in logFile:
        try:
            nEvents = line.split('processed:')[1]
        except:
            pass
    if nEvents == -1:
        print dir, 'cannot find number of processed events'
    elif nEvents == 0:
        print dir, '0 events'
    else:
        continue
    badDirs.append(dir)

print 'list of bad directories:'
pprint.pprint(badDirs)

if options.batch is not None:
    for dir in badDirs:
        oldPwd = os.getcwd()
        os.chdir( dir )
        cmd =  [options.batch, '-J', dir, ' < batchScript.sh' ]
        print 'resubmitting in', os.getcwd()
        cmds = ' '.join( cmd )
        print cmds
        os.system( cmds )
        os.chdir( oldPwd )
        
