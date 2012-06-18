from optparse import OptionParser
import sys,os, re, subprocess, datetime

import CMGTools.Production.eostools as castortools

class logger:

    def __init__(self, dirLocalOrTgzDirOnCastor):

        self.dirLocal = None
        self.tgzDirOnCastor = None
        self.tagPackage = True
        dirLocalOrTgzDirOnCastor = dirLocalOrTgzDirOnCastor.rstrip('/')

        if self.isDirLocal( dirLocalOrTgzDirOnCastor ):
            self.dirLocal = dirLocalOrTgzDirOnCastor
        elif self.isTgzDirOnCastor( dirLocalOrTgzDirOnCastor ):
            self.tgzDirOnCastor = dirLocalOrTgzDirOnCastor
        else:
            raise ValueError( dirLocalOrTgzDirOnCastor + ' is neither a tgz directory on castor (provide a LFN!) nor a local directory')
            
        
    def isDirLocal(self, file ):
        if os.path.isdir( file ):
            return True
        else:
            return False

    def isTgzDirOnCastor(self, file ):

        if not castortools.isCastorDir( file ):
            file = castortools.castorToLFN(file)
            
        if castortools.isLFN( file ):
            tgzPattern = re.compile('.*\.tgz$')
            m = tgzPattern.match( file )
            if m:
                return True
            else:
                return False
        else:
            return False

    def dump(self):
        print 'local dir      :', self.dirLocal
        print 'castor archive :',self.tgzDirOnCastor

    def addFile(self, file):
        #        if self.dirLocal == None:
        #            self.stageIn()
        #            os.system( 'cp %s %s' % (file, self.dirLocal) )
        #            self.stageOut( self.tgzDirOnCastor )
        if self.dirLocal != None:
            os.system( 'cp %s %s' % (file, self.dirLocal) )

    def logCMSSW(self): 
        self.logPackages()
        showtagsLog = 'logger_showtags.txt'
        diffLog = 'logger_diff.txt'
        # os.system('showtags > ' + showtagsLog)
        self.showtags(showtagsLog)
        self.cvsdiff(diffLog)
        self.addFile(showtagsLog)
        self.addFile(diffLog) 

    def logJobs(self, n):
        nJobs = 'logger_jobs.txt'
        out = file(nJobs,'w')
        out.write('NJobs: %i\n' % n)
        out.close()
        self.addFile(nJobs)

    def logPackages(self):
        # Records the working directory of the samples
        oldPwd = os.getcwd()
        # Changes the directory to the CMSSW base directory
        os.chdir( os.getenv('CMSSW_BASE') + '/src/' )
        # Opens file for recording 'showtags' and reads contents into $output
        output = subprocess.Popen( 'showtags', stdout=subprocess.PIPE).communicate()[0]
        # Gets current datetime
        d = datetime.datetime.today()
        # Creates tag name??? here i'm lost
        tag = 'logger_' + os.getenv('USER') + '_' + d.strftime("%d%h%y-%Hh%Mm%Ss")
        # Creates regexp to test incoming lines from 'showtags'
        tagPattern = re.compile('^\s*(\S+)\s+(\S+)\s*$')
        # For every line in showtags...
        for line in output.split('\n'):
            # Check if it is a tag
            m = tagPattern.match(line)
            # if line is a tag...
            if m!=None:
                # get package name
                package = m.group(2)
                # get tag name
                curtag = m.group(1)
                # updates package on CVS
                self.logPackage( package, curtag, tag)
        os.chdir( oldPwd )
        
    def logPackage(self, package, curtag, tag ):
        print 'logging package', package
        if curtag == 'NoTag':
            print 'package has not been tagged'
            if self.tagPackage==True:
                self.cvstag( tag, package )
                self.cvsupdate( tag, package )
            else:
                print 'No tagging requested'
    def cvstag(self, tag, package):
        oldPwd = os.getcwd()
        os.chdir( os.getenv('CMSSW_BASE') + '/src/' )
        # os.system( 'cvs tag ' + package )
        # print os.getcwd()
        tagCmd = 'cvs tag ' + tag + ' ' + package
        print tagCmd
        tag = subprocess.Popen( tagCmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE ).communicate()
        os.chdir( oldPwd )

    def cvsupdate(self, tag, package):
        oldPwd = os.getcwd()
        os.chdir( os.getenv('CMSSW_BASE') + '/src/' )
        upCmd = 'cvs up -r ' + tag + ' ' + package
        print upCmd
        update = subprocess.Popen( upCmd.split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE ).communicate()
        os.chdir( oldPwd )
       
    def cvsdiff(self, log):
        oldPwd = os.getcwd()
        os.chdir( os.getenv('CMSSW_BASE') + '/src/' )
        diffCmd = 'cvs diff > %s/%s 2> /dev/null' % (oldPwd, log)
        print diffCmd
        os.system( diffCmd )
        os.chdir( oldPwd )

    def showtags(self, log):
        oldPwd = os.getcwd()
        os.chdir( os.getenv('CMSSW_BASE') + '/src/' )
        cmd = 'showtags > %s/%s 2> /dev/null' % (oldPwd, log)
        print cmd
        os.system( cmd )
        os.chdir( oldPwd )        

    def stageIn(self):
        if self.tgzDirOnCastor != None:
            # castortools.xrdcp( '.', [self.tgzDirOnCastor] )
            cmsStage = 'cmsStage -f ' + self.tgzDirOnCastor + ' .'
            print cmsStage
            os.system( cmsStage ) 
            tgzDir = os.path.basename( self.tgzDirOnCastor )
            print tgzDir 
            os.system('tar -zxvf ' + tgzDir)
            os.system('rm ' + tgzDir )
            (root, ext) = os.path.splitext(tgzDir)
            self.dirLocal = root
        else:
            print 'cannot stage in, the log had not been staged out'

    def stageOut(self, castorDir):

        castorDir = castortools.eosToLFN( castorDir )
        if not castortools.isLFN( castorDir ):
            print 'cannot stage out, you need to provide an LFN as a destination directory, beginning with /store .'
            return False
        
        if self.dirLocal != None:
            tgzDir = self.dirLocal + '.tgz'
            tgzCmd = 'tar -zcvf ' + tgzDir + ' ' + self.dirLocal
            print tgzCmd
            os.system( tgzCmd)
            cmsStage = 'cmsStage -f %s %s' % (tgzDir, castorDir )
            print cmsStage
            os.system( cmsStage )
            os.system('rm ' + tgzDir )
            self.tgzDirOnCastor =  castorDir + '/' + tgzDir
        else:
            print 'cannot stage out, the log is not staged in'
            
