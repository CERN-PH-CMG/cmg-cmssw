#!/usr/bin/env python

import os
import pprint

from CMGTools.Production.castorBaseDir import castorBaseDir
import CMGTools.Production.eostools as castortools

class Dataset():
    def __init__(self, user, name, pattern='.*root'):
        self.name = name
        self.user = user
        self.pattern = pattern
        self.lfnDir = castorBaseDir(user) + name
        self.castorDir = castortools.lfnToCastor( self.lfnDir )
        self.pollFiles( pattern )
        self.extractFileSizes()
        self.pollBadFiles()
        
    def pollFiles(self, pattern='.*root'):
        '''fills list of files, taking all root files matching the pattern in the castor dir'''
        self.files = castortools.matchingFiles( self.castorDir, pattern )
        if len(self.files)==0:
            raise ValueError(' '.join( ['No file matching',
                                        pattern, 'in', self.castorDir] ))
                             
    def pollBadFiles(self):
        '''fills the list of bad files from the IntegrityCheck log.

        When the integrity check file is not available,
        files are considered as good.'''
        mask = "IntegrityCheck"
        file_mask = castortools.matchingFiles(self.castorDir, '^%s_.*\.txt$' % mask)
        self.bad_files = {}
        self.good_files = []
        if file_mask:
            from CMGTools.Production.edmIntegrityCheck import PublishToFileSystem
            p = PublishToFileSystem(mask)
            report = p.get(self.castorDir)
            if report is not None and report:
                dup = report.get('ValidDuplicates',{})
                for name, status in report['Files'].iteritems():
                    # print name, status
                    if not status[0]:
                        self.bad_files[name] = 'MarkedBad'
                    elif dup.has_key(name):
                        self.bad_files[name] = 'ValidDup'
                    else:
                        self.good_files.append( name )

    def extractFileSizes(self):
        '''Get the file size for each file, from the eos ls -l command.'''
        lsout = castortools.runEOSCommand(self.castorDir, 'ls','-l')[0]
        lsout = lsout.split('\n')
        self.filesAndSizes = {}
        for entry in lsout:
            values = entry.split()
            if( len(values) != 9):
                continue
            # using full abs path as a key.
            file = '/'.join([self.lfnDir, values[8]])
            size = values[4]
            self.filesAndSizes[file] = size 

    def listOfFiles(self):
        '''Returns all files, even the bad ones.'''
        return self.files

    def listOfGoodFiles(self):
        '''Returns all files flagged as good in the integrity check text output,
        or not present in this file, are considered as good.'''
        self.good_files = []
        for file in self.files:            
            if not self.bad_files.has_key(file):
                self.good_files.append( file )
        return self.good_files
                
    def printInfo(self):
        print 'sample      :  ' + self.name
        print 'LFN         :  ' + self.lfnDir
        print 'Castor path :  ' + self.castorDir

    def printFiles(self, abspath=True, info=True):
        if self.files == None:
            self.pollFiles(self.pattern)
        for file in self.files:
            status = 'OK'
            if self.bad_files.has_key(file):
                status = self.bad_files[file]
            elif file not in self.good_files:
                status = 'UNKNOWN'
            fileNameToPrint = file
            if abspath == False:
                fileNameToPrint = os.path.basename(file)
            if info:
                print status.ljust(10), self.filesAndSizes[file].rjust(10), \
                      '\t', fileNameToPrint
            else:
                print fileNameToPrint
