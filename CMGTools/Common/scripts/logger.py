#!/usr/bin/env python


import sys,os, re, subprocess, datetime

#scriptDir = os.getenv('CMSSW_BASE') + '/src/CMGTools/Common/scripts'
#sys.path.append( scriptDir ) 

import castortools

#sys.path.append( '.' )


class logger:

    def __init__(self, dirLocalOrTgzDirOnCastor):


        self.dirLocal = None
        self.tgzDirOnCastor = None
        
        dirLocalOrTgzDirOnCastor = dirLocalOrTgzDirOnCastor.rstrip('/')

        

        if self.isDirLocal( dirLocalOrTgzDirOnCastor ):
            self.dirLocal = dirLocalOrTgzDirOnCastor
        elif self.isTgzDirOnCastor( dirLocalOrTgzDirOnCastor ):
            self.tgzDirOnCastor = dirLocalOrTgzDirOnCastor
        else:
            raise ValueError( dirLocalOrTgzDirOnCastor + ' is neither a tgz directory on castor nor a local directory')
            

    def isDirLocal(self, file ):
        if os.path.isdir( file ):
            return True
        else:
            return False

    def isTgzDirOnCastor(self, file ):
        if castortools.isCastorFile( file ):
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
        output = subprocess.Popen( 'showtags', stdout=subprocess.PIPE).communicate()[0]

        d = datetime.datetime.today()
        tag = 'logger_' + os.getenv('USER') + '_' + d.strftime("%d%h%y-%Hh%Mm%Ss")

        tagPattern = re.compile('^\s*(\S+)\s+(\S+)\s*$')
        for line in output.split('\n'):
            m = tagPattern.match(line)
            # print 'line ', line
            if m!=None:
                package = m.group(2)
                curtag = m.group(1)
                self.logPackage( package, curtag, tag)

        showtagsLog = 'logger_showtags.txt'
        diffLog = 'logger_diff.txt'
        os.system('showtags > ' + showtagsLog)
        self.cvsdiff(diffLog)
        self.addFile(showtagsLog)
        self.addFile(diffLog)
        

    def logPackage(self, package, curtag, tag ):
        print 'logging package', package
        if curtag == 'NoTag':
            print 'package has not been tagged'
            self.cvstag( tag, package )
            self.cvsupdate( tag, package )
                
                # print os.getcwd()
                # self.cvstag(tag, package)

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
                

    def stageIn(self):
        if self.tgzDirOnCastor != None:
            castortools.xrdcp( '.', [self.tgzDirOnCastor] )
            tgzDir = os.path.basename( self.tgzDirOnCastor )
            print tgzDir 
            os.system('tar -zxvf ' + tgzDir)
            os.system('rm ' + tgzDir )
            (root, ext) = os.path.splitext(tgzDir)
            self.dirLocal = root
        else:
            print 'cannot stage in, the log had not been staged out'

    def stageOut(self, castorDir):
        if self.dirLocal != None:
            tgzDir = self.dirLocal + '.tgz'
            tgzCmd = 'tar -zcvf ' + tgzDir + ' ' + self.dirLocal
            print tgzCmd
            os.system( tgzCmd)
            rfcpCmd = 'rfcp %s %s' % (tgzDir, castorDir )
            os.system( rfcpCmd )
            os.system('rm ' + tgzDir )
            self.tgzDirOnCastor =  castorDir + '/' + tgzDir
        else:
            print 'cannot stage out, the log is not staged in'
            


if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    
    parser.usage = "logger.py ..."

    (options,args) = parser.parse_args()

    if len(args)!=1:
        parser.print_help()
        sys.exit(1)

    dirOrFile = args[0]

    try:
        log = logger(dirOrFile)
        log.logCMSSW()
        # log.stageIn()
        # log.addFile('patTuple_PATandPF2PAT_RecoJets_cfg.py')
        # log.stageOut('/castor/cern.ch/cms/store/cmst3/user/cbern/Test')
        # log.stageOut()
        # log.dump()
    except ValueError as err:
        print err, '. Exit!'
        sys.exit(1)



