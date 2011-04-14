#!/usr/bin/python

from datetime import datetime
from optparse import OptionParser

import sys, string, os, re, pprint, shutil

import castortools

class BatchManager:
    """
    This class manages batch jobs
    Used in batch scripts
    Colin Bernet 2008
    """

    # constructor
    # self is this
    # parse batch manager options 
    def __init__(self):    
        self.DefineOptions()


    def DefineOptions(self):
        # define options and arguments ====================================
        # how to add more doc to the help?
        self.parser_ = OptionParser()
        self.parser_.add_option("-o", "--output-dir", dest="outputDir",
                          help="local output directory for your jobs",
                          default=None)
        self.parser_.add_option("-r", "--remote-copy", dest="remoteCopy",
                          help="remote output directory for your jobs, and file to be copied. Example: /castor/cern.ch/user/c/cbern/truc.root: for job 1, the file truc.root will be copied to /castor/cern.ch/user/c/cbern/truc_1.root",
                          default=None)
        # this opt can be removed
        self.parser_.add_option("-n", "--negate", action="store_true",
                                dest="negate", default=False,
                                help="create jobs, but do nothing")

        
    def ParseOptions(self):       
        (self.options_,args) = self.parser_.parse_args()
        if self.options_.remoteCopy == None:
            self.remoteOutputDir_ = ""
        else:
            self.remoteOutputDir_ = self.options_.remoteCopy.rstrip('/') 
            nsls = 'nsls %s > /dev/null' % self.remoteOutputDir_
            dirExist = os.system( nsls )
            if dirExist != 0:
                print 'creating ', self.remoteOutputDir_
                os.system('nsmkdir -p ' + self.remoteOutputDir_ )
                # print 'check that the castor output directory specified with the -r option exists.'
                # sys.exit(1)
                #        self.remoteOutputFile_ = os.path.basename( self.options_.remoteCopy )
            else:
                # directory exists.
                if self.options_.negate == False:
                    if not castortools.protectedRemove( self.remoteOutputDir_, '.*root'):
                        # the user does not want to delete the root files
                        sys.exit(1)
                
        self.remoteOutputFile_ = ""

        self.ManageOutputDir()

        
    def PrepareJobs(self, listOfValues ):
        print 'PREPARING JOBS ======== '
        
        # self.ParseOptions()
 
#        self.ManageOutputDir()
        #self.CheckBatchScript( self.options_.batchScript )

        self.listOfJobs_ = []

        print listOfValues

        # prepare jobs
        for value in listOfValues:
            self.PrepareJob( value )
        
        print "list of jobs:"
        pp = pprint.PrettyPrinter(indent=4)
        pp.pprint( self.listOfJobs_)



    # create output dir, if necessary
    def ManageOutputDir( self ):
        
        #if the output dir is not specified, generate a name
        #else 
        #test if the directory exists 
        #if yes, returns

        outputDir = self.options_.outputDir

        if outputDir==None:
            today = datetime.today()
            outputDir = 'OutCmsBatch_%s' % today.strftime("%d%h%y_%H%M%S")
            print 'output directory not specified, using %s' % outputDir            
            
        self.outputDir_ = os.path.abspath(outputDir)

        if( os.path.isdir(self.outputDir_) == True ):
            input = ''
            while input != 'y' and input != 'n':
                input = raw_input( 'The directory ' + self.outputDir_ + ' exists. Are you sure you want to continue? its contents will be overwritten [y/n]' )
            if input == 'n':
                sys.exit(1)
            else:
                os.system( 'rm -rf ' + self.outputDir_)
                
        self.mkdir( self.outputDir_ )
 

    # prepare job for a given value
    def PrepareJob( self, value):
        print 'PrepareJob : %s' % value 
        jobDir = '%s/Job_%s' % (self.outputDir_, value)
        print '\t',jobDir 
        self.mkdir( jobDir )
        self.listOfJobs_.append( jobDir )

        # if self.options_.batchScript != '':
        #    shutil.copy( self.options_.batchScript, jobDir)

        self.PrepareJobUser( jobDir, value )
        
    def PrepareJobUser(self, value ):
        print '\to be customized'

   
    def SubmitJobs( self ):

        if(self.options_.negate):
            print '*NOT* SUBMITTING JOBS - exit '
            return

           
        print 'SUBMITTING JOBS ======== '

        for jobDir  in self.listOfJobs_:
            root = os.getcwd()

            # run it
            print 'processing ', jobDir
            os.chdir( jobDir )
            self.SubmitJob( jobDir )

            # and come back
            os.chdir(root)

    # generate outputFile 
    def SubmitJob( self, jobDir ):
        print 'submitting (to be customized): ', jobDir  


    def CheckBatchScript( self, batchScript ):

        if batchScript == '':
            return
        
        if( os.path.isfile(batchScript)== False ):
            print 'file ',batchScript,' does not exist'
            sys.exit(3)

        try:
            ifile = open(batchScript)
        except:
            print 'cannot open input %s' % batchScript
            sys.exit(3)
        else:
            for line in ifile:
                p = re.compile("\s*cp.*\$jobdir\s+(\S+)$");
                m=p.match(line)
                if m:
                    if os.path.isdir( os.path.expandvars(m.group(1)) ):
                        print 'output directory ',  m.group(1), 'already exists!'
                        print 'exiting'
                        sys.exit(2)
                    else:
                        if self.options_.negate==False:
                            os.mkdir( os.path.expandvars(m.group(1)) )
                        else:
                            print 'not making dir', self.options_.negate

    # create a directory
    def mkdir( self, dirname ):
        # there is probably a command for this in python
        mkdir = 'mkdir -p %s' % dirname
        ret = os.system( mkdir )
        if( ret != 0 ):
            print 'please remove or rename directory: ', dirname
            sys.exit(4)
       
