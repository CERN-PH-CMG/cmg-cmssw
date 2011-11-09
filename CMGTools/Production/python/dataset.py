#!/usr/bin/env python

import os

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
