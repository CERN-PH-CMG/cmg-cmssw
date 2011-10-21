#!/usr/bin/env python

from CMGTools.Production.castorBaseDir import *
from CMGTools.Production.castortools import *

class Dataset():
    def __init__(self, user, name, pattern='.*root'):
        self.name = name
        self.user = user
        self.lfnDir = castorBaseDir(user) + name
        self.castorDir = castortools.lfnToCastor( self.lfnDir )
        self.pollFiles( pattern )
        self.pollBadFiles()
        
    def pollFiles(self, pattern='.*root'):
        '''fills list of files, taking all root files matching the pattern in the castor dir'''
        self.files = castortools.matchingFiles( self.castorDir, pattern )

    def pollBadFiles(self):
        '''fills the list of bad files from the IntegrityCheck log'''
        mask = "IntegrityCheck"
        file_mask = castortools.matchingFiles(self.castorDir, '^%s_.*\.txt$' % mask)
        self.bad_files = {}    
        if file_mask:
            from CMGTools.Production.edmIntegrityCheck import PublishToFileSystem
            p = PublishToFileSystem(mask)
            report = p.get(self.castorDir)
            if report is not None and report:
                dup = report.get('ValidDuplicates',{})
                for name, status in report['Files'].iteritems():
                    if not status[0]:
                        self.bad_files[name] = 'MarkedBad'
                    elif dup.has_key(name):
                        self.bad_files[name] = 'ValidDup'

    def listOfFiles(self):
        return self.files

    def listOfGoodFiles(self):
        self.good_files = []
        for file in self.files:
            if not self.bad_files.has_key(file):
                self.good_files.append( file )
        return self.good_files
                
    def printInfo(self):
        print 'sample      :  ' + self.name
        print 'LFN         :  ' + self.lfnDir
        print 'Castor path :  ' + self.castorDir

    def printFiles(self):
        if self.files == None:
            self.pollFiles(self.pattern)
        for file in self.files:
            status = 'OK'
            if self.bad_files.has_key(file):
                status = self.bad_files[file]
            print file, status

if __name__ == '__main__':

    import sys
    from optparse import OptionParser
    
    parser = OptionParser()
    parser.usage = "%prog <user> <dataset>\nPrints information on a sample."
    parser.add_option("-p", "--pattern", dest="pattern", default='.*tree.*root',help='regexp pattern for root file printout')

    (options,args) = parser.parse_args()

    if len(args)!=2:
        parser.print_help()
        sys.exit(1)

    user = args[0]
    name = args[1]

    data = Dataset( user, name, options.pattern)
    data.printInfo()
    data.printFiles()

    # print 'good files: '

    # import pprint
    # pprint.pprint( data.listOfGoodFiles() )
