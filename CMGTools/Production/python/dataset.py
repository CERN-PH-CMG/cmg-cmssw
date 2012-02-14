#!/usr/bin/env python

import os
import pprint
import re

from CMGTools.Production.castorBaseDir import castorBaseDir
import CMGTools.Production.eostools as castortools


class BaseDataset( object ):
    
    def __init__(self, name, user, pattern='.*root'):
        self.name = name
        self.user = user
        self.pattern = pattern
        self.buildListOfFiles( self.pattern )
        self.extractFileSizes()
        self.buildListOfBadFiles()

    def buildListOfFiles( self, pattern ):
        self.files = []

    def extractFileSizes(self):
        '''Get the file size for each file, from the eos ls -l command.'''
        self.filesAndSizes = {}

    def buildListOfBadFiles(self):
        self.good_files = []
        self.bad_files = {}

    def printInfo(self):
        print 'sample      :  ' + self.name
        print 'user        :  ' + self.user

    def printFiles(self, abspath=True, info=True):
        # import pdb; pdb.set_trace()
        if self.files == None:
            self.buildListOfFiles(self.pattern)
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
                size = self.filesAndSizes.get(file, 'UNKNOWN').rjust(10)
                # if size is not None:
                #     size = size.rjust(10)
                print status.ljust(10), size, \
                      '\t', fileNameToPrint
            else:
                print fileNameToPrint
                
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
    


class CMSDataset( BaseDataset ):

    def __init__(self, name):
        super(CMSDataset, self).__init__( name, 'CMS')
        
    def buildListOfFiles(self, pattern='.*root'):
        sampleName = self.name.rstrip('/')
        dbs = 'dbs search --query="find file where dataset like %s"' % sampleName
        dbsOut = os.popen(dbs)
        self.files = []
        for line in dbsOut:
            if line.find('/store')==-1:
                continue
            line = line.rstrip()
            # print 'line',line
            self.files.append(line)



class LocalDataset( BaseDataset ):

    def __init__(self, name, basedir, pattern):
        self.basedir = basedir 
        super(LocalDataset, self).__init__( name, 'LOCAL', pattern)
        
    def buildListOfFiles(self, pattern='.*root'):
        pat = re.compile( pattern )
        sampleName = self.name.rstrip('/')
        sampleDir = ''.join( [os.path.abspath(self.basedir), sampleName ] )
        self.files = []
        for file in sorted(os.listdir( sampleDir )):
            if pat.match( file ) is not None:
                self.files.append( '/'.join([sampleDir, file]) )
                # print file
##         dbs = 'dbs search --query="find file where dataset like %s"' % sampleName
##         dbsOut = os.popen(dbs)
##         self.files = []
##         for line in dbsOut:
##             if line.find('/store')==-1:
##                 continue
##             line = line.rstrip()
##             # print 'line',line
##             self.files.append(line)

        

class Dataset( BaseDataset ):
    
    def __init__(self, user, name, pattern='.*root'):
        self.lfnDir = castorBaseDir(user) + name
        self.castorDir = castortools.lfnToCastor( self.lfnDir )
        self.maskExists = False
        self.report = None
        # import pdb; pdb.set_trace()
        super(Dataset, self).__init__(user, name, pattern)
#        self.buildListOfFiles( pattern )
#        self.extractFileSizes()
#        self.maskExists = False
#        self.report = None
#        self.buildListOfBadFiles()
        
    def buildListOfFiles(self, pattern='.*root'):
        '''fills list of files, taking all root files matching the pattern in the castor dir'''
        self.files = castortools.matchingFiles( self.castorDir, pattern )
        if len(self.files)==0:
            raise ValueError(' '.join( ['No file matching',
                                        pattern, 'in', self.castorDir] ))
                             
    def buildListOfBadFiles(self):
        '''fills the list of bad files from the IntegrityCheck log.

        When the integrity check file is not available,
        files are considered as good.'''
        mask = "IntegrityCheck"
        file_mask = castortools.matchingFiles(self.castorDir, '^%s_.*\.txt$' % mask)
        self.bad_files = {}
        self.good_files = []
        # import pdb; pdb.set_trace()
        if file_mask:
            from CMGTools.Production.edmIntegrityCheck import PublishToFileSystem
            p = PublishToFileSystem(mask)
            report = p.get(self.castorDir)
            if report is not None and report:
                self.maskExists = True
                self.report = report
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
         
    def printInfo(self):
        print 'sample      :  ' + self.name
        print 'LFN         :  ' + self.lfnDir
        print 'Castor path :  ' + self.castorDir


