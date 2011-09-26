#!/usr/bin/python

from optparse import OptionParser
import sys,os, pprint, re
import castortools



def sourceFileList( files ):
    print '''
import FWCore.ParameterSet.Config as cms

source = cms.Source(
"PoolSource",
'''
    print 'noEventSort = cms.untracked.bool(True),'
    print 'duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),'

    print "fileNames = cms.untracked.vstring("
    for file in files:
        fileLine = "'file:%s'," % os.path.abspath(file)
        print fileLine
        print ")"
        print ")"    


parser = OptionParser()
parser.usage = "%prog <dir> <regexp> : format a set of root files matching a regexp in a directory, as an input to the PoolSource. \n\nExample (just try!):\nsourceFileList.py /castor/cern.ch/user/c/cbern/CMSSW312/SinglePions '.*\.root'"
parser.add_option("-c", "--check", dest="check", default=False, action='store_true',help='Check filemask if available')

(options,args) = parser.parse_args()


if len(args) != 2:
    parser.print_help()
    sys.exit(1)

dir = args[0]
regexp = args[1]


exists = castortools.fileExists( dir )
if not exists:
    print 'sourceFileList: directory does not exist. Exiting'
    sys.exit(1)


files = castortools.matchingFiles( dir, regexp, addProtocol = True)

mask = "IntegrityCheck"
file_mask = []  
if options.check:
    file_mask = castortools.matchingFiles(dir, '^%s_.*\.txt$' % mask)
bad_files = {}    
if options.check and file_mask:
    from edmIntegrityCheck import PublishToFileSystem
    p = PublishToFileSystem(mask)
    report = p.get(dir)
    if report is not None and report:
        for name, status in report['Files'].iteritems():
            if not status[0]:
                bad_files[name] = status[1]

print '''
import FWCore.ParameterSet.Config as cms

source = cms.Source(
\t"PoolSource",
'''
print '\tnoEventSort = cms.untracked.bool(True),'
print '\tduplicateCheckMode = cms.untracked.string("noDuplicateCheck"),'
print "\tfileNames = cms.untracked.vstring()"
print ")"

print 'source.fileNames.extend(['
for file in files:
    file = file.replace('//','/')
#     file = file.replace( protocol+'/castor/cern.ch/cms/store', '/store')  
    if not bad_files.has_key(file):
        fileLine = "\t\t'%s'," % file
    else:
        fileLine = "###MarkedBad\t'%s'," % file
    print fileLine
print "])"

