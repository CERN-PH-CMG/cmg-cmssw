#!/usr/bin/env python

import os
import pprint
import re
import pickle
import sys

from CMGTools.Production.castorBaseDir import castorBaseDir
import CMGTools.Production.eostools as castortools
import fnmatch

class IntegrityCheckError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

class BaseDataset( object ):
    
    def __init__(self, name, user, pattern='.*root', run_range = None):
        self.name = name
        self.user = user
        self.pattern = pattern
        self.run_range = run_range
        self.primaryDatasetEntries = -1
        self.report = None
        self.buildListOfFiles( self.pattern )
        self.extractFileSizes()
        self.buildListOfBadFiles()
        self.primaryDatasetEntries = self.getPrimaryDatasetEntries()
     
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

    def getPrimaryDatasetEntries(self):
        return self.primaryDatasetEntries

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
        print 'PrimaryDatasetEntries: %d' % self.primaryDatasetEntries
                
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

    def listOfGoodFilesWithPrescale(self, prescale):
        """Takes the list of good files and selects a random sample from them according to the prescale factor. E.g. a prescale of 10 will select 1 in 10 files."""

        good_files = self.listOfGoodFiles()
        if prescale < 2:
            return self.good_files
        
        #the number of files to select from the dataset
        num_files = int( (len(good_files)/(1.0*prescale)) + 0.5)
        if num_files < 1:
            num_files = 1
        if num_files > len(good_files):
            num_files = len(good_files)
        
        #pick unique good files randomly
        import random
        subset = set()
        while len(subset) < num_files:
            #pick a random file from the list
            choice = random.choice(good_files)
            slen = len(subset)
            #add to the set
            subset.add(choice)
            #if this was a unique file remove so we don't get very slow corner cases where prescale is small
            if len(subset) > slen:
                good_files.remove(choice)
        assert len(subset) == num_files, 'The number of files should matcht the expected'

        return [f for f in subset]

class CMSDataset( BaseDataset ):

    def __init__(self, name, run_range = None):
        super(CMSDataset, self).__init__( name, 'CMS', run_range=run_range)

    def buildListOfFilesDBS(self, pattern, begin=-1, end=-1):
        print 'buildListOfFilesDBS',begin,end
        sampleName = self.name.rstrip('/')
        query = sampleName
        if self.run_range is not None:
            if self.run_range[0] > 0:
                query = "%s and run >= %i" % (query,self.run_range[0])
            if self.run_range[1] > 0:
                query = "%s and run <= %i" % (query,self.run_range[1])
        dbs = 'dbs search --query="find file where dataset like %s"' % query
        if begin >= 0:
            dbs += ' --begin %d' % begin
        if end >= 0:
            dbs += ' --end %d' % end
        print 'dbs\t: %s' % dbs
        dbsOut = os.popen(dbs)
        files = []
        for line in dbsOut:
            if line.find('/store')==-1:
                continue
            line = line.rstrip()
            # print 'line',line
            files.append(line)
        return files

    def buildListOfFiles(self, pattern='.*root'):
        runs = (-1,-1)
        if self.run_range is not None:
            runs = self.run_range
        num_files = self.findPrimaryDatasetNumFiles(self.name.rstrip('/'),runs[0],runs[1])
        limit = 10000
        if num_files > limit:
            num_steps = int(num_files/limit)+1
            self.files = []
            for i in xrange(num_steps):
                self.files.extend(self.buildListOfFilesDBS(pattern,i*limit,((i+1)*limit)-1))
        else:
            self.files = self.buildListOfFilesDBS(pattern)
            
    @staticmethod
    def findPrimaryDatasetEntries(dataset, runmin, runmax):

        query = dataset
        if runmin > 0:
            query = "%s and run >= %i" % (query,runmin)
        if runmax > 0:
            query = "%s and run <= %i" % (query,runmax)
        dbs = 'dbs search --query="find sum(file.numevents) where dataset like %s"' % query
        dbsOut = os.popen(dbs).readlines()

        entries = []
        for line in dbsOut:
            line = line.replace('\n','')
            if line:
                try:
                    entries.append(int(line))
                except ValueError:
                    pass
        if entries:
            return sum(entries)
        return -1

    @staticmethod
    def findPrimaryDatasetNumFiles(dataset, runmin, runmax):

        query = dataset
        if runmin > 0:
            query = "%s and run >= %i" % (query,runmin)
        if runmax > 0:
            query = "%s and run <= %i" % (query,runmax)
        dbs = 'dbs search --query="find sum(block.numfiles) where dataset like %s"' % query
        dbsOut = os.popen(dbs).readlines()

        entries = []
        for line in dbsOut:
            line = line.replace('\n','')
            if line:
                try:
                    entries.append(int(line))
                except ValueError:
                    pass
        if entries:
            return sum(entries)
        return -1

    def getPrimaryDatasetEntries(self):
        runmin = -1
        runmax = -1
        if self.run_range is not None:
            runmin = self.run_range[0]
            runmax = self.run_range[1]
        return self.findPrimaryDatasetEntries(self.name, runmin, runmax)

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
    
    def __init__(self, name, user, pattern='.*root'):
        self.lfnDir = castorBaseDir(user) + name
        self.castorDir = castortools.lfnToCastor( self.lfnDir )
        self.maskExists = False
        self.report = None
        super(Dataset, self).__init__(name, user, pattern)
        #        self.buildListOfFiles( pattern )
        #        self.extractFileSizes()
        #        self.maskExists = False
        #        self.report = None
        #        self.buildListOfBadFiles()
        
    def buildListOfFiles(self, pattern='.*root'):
        '''fills list of files, taking all root files matching the pattern in the castor dir'''
        self.files = castortools.matchingFiles( self.castorDir, pattern )
                             
    def buildListOfBadFiles(self):
        '''fills the list of bad files from the IntegrityCheck log.

        When the integrity check file is not available,
        files are considered as good.'''
        mask = "IntegrityCheck"
        #file_mask = castortools.matchingFiles(self.castorDir, '^%s_.*\.txt$' % mask)
        #if not file_mask:
           #os.system("/afs/cern.ch/user/a/anantoni/CMSSW_5_3_3_patch3/src/CMGTools/Production/scripts/edmIntegrityCheck.py " + self.name + " -u " + self.user )
           
        self.bad_files = {}
        self.good_files = []

        file_mask = castortools.matchingFiles(self.castorDir, '^%s_.*\.txt$' % mask)
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
        else:
            raise IntegrityCheckError( "ERROR: IntegrityCheck log file IntegrityCheck_XXXXXXXXXX.txt not found" )

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

    def getPrimaryDatasetEntries(self):
        if self.report is not None and self.report:
            return int(self.report.get('PrimaryDatasetEntries',-1))
        return -1

def createDataset( user, dataset, pattern,  readcache=False, basedir=None, run_range = None):
    
    cachedir =  '/'.join( [os.environ['HOME'],'.cmgdataset'])
    
    def cacheFileName(data, user, pattern):
        cf =  data.replace('/','_')
        name = '{dir}/{user}%{name}%{pattern}.pck'.format(
            dir = cachedir,
            user = user,
            name = cf,
            pattern = pattern)
        return name

    def writeCache(dataset):
        if not os.path.exists(cachedir):
            os.mkdir(cachedir)
        cachename = cacheFileName(dataset.name,
                                  dataset.user,
                                  dataset.pattern)
        pckfile = open( cachename, 'w')
        pickle.dump(dataset, pckfile)

    def readCache(data, user, pattern):
        cachename = cacheFileName(data, user, pattern)
        pckfile = open( cachename)
        dataset = pickle.load(pckfile)      
        print 'reading cache'
        return dataset

    if readcache:
        try:
            data = readCache(dataset, user, pattern)
        except IOError:
            readcache = False
    if not readcache:
        if user == 'CMS':
            data = CMSDataset( dataset , run_range = run_range)
            info = False
        elif user == 'LOCAL':
            data = LocalDataset( dataset, basedir, pattern)
            info = False        
        else:
            data = Dataset( dataset, user, pattern)
        writeCache(data)
##     if user == 'CMS':
##         data = CMSDataset( dataset )
##     elif user == 'LOCAL':
##         if basedir is None:
##             basedir = os.environ['CMGLOCALBASEDIR']
##         data = LocalDataset( dataset, basedir, pattern )
##     else:
##         data = Dataset( user, dataset, pattern )
    return data
