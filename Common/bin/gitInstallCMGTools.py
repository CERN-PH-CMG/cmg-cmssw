#!/usr/bin/env python

import sys
import os
from optparse import OptionParser
import subprocess
import urllib


ENABLE=True 

def isInputDict(seq):
    '''Duck-typing a sequence to see if its a dictionary'''
    try:
        seq.keys()
        return True
    except:
        return False


def getCPUCount():
    
    result = 1
    try:
        import multiprocessing
        result = multiprocessing.cpu_count()
    except (ImportError, NotImplementedError):
        import platform
        if platform.node().lower().startswith('lxplus'):
            result = 4
        pass
    
    return result

class InstallCMGTools(object):
    """
    A class to make the installation of CMGTools easier
    @author Luke Heo - seong.gu.heo@cern.ch
        """

    def setReleaseInfo(self, version, recipe=None):

        try:
            repo_url = "https://raw.github.com/anantoni/CMGTools"
            file_name_base = version
            if recipe:
                file_name_base = "%s_%s" % (file_name_base, recipe)
            full_url = '%s/master/Common/python/release_info/%s.py' % \
                       (repo_url, file_name_base)
            release_info = urllib.urlopen(full_url)
            content = release_info.read()
            if content.find('404 Not Found') != -1:
                raise Exception("The file '%s' does not exist or could not be accessed" % full_url)
            print content
            if content is not None and content:
                self.dic_list = eval(content)
        except Exception, e:
            message = "Release info can not be found for version '%s' " \
                      "(recipe: %s) - the error was '%s'" % \
                      (version, recipe, str(e))
            raise Exception(message)

    def __init__(self):
        self.options, self.args = self.parseOptions()

        self.setReleaseInfo(self.options.cmssw_version, self.options.recipe)

        pwd = os.getcwd()
        try:
            self.init()
        finally:
            os.chdir(pwd)

        if self.options.build:
            try:
                src = os.path.join(self.options.installation_directory,self.options.cmssw_version,'src')
                os.chdir(src)
                subprocess.call(['scram', 'b', '-j', str(getCPUCount())])
            finally:
                os.chdir(pwd)

    def parseOptions(self):
        """ Accept arguments"""
        parser = OptionParser()
        parser.add_option("-d", "--dir", type="string", dest="installation_directory", default='CMGTools',
            help="write a installation directory, e.g. '--dir ~/scratch0/CMGTools'")
        parser.add_option("-r", "--release",type="string", dest="cmssw_version", default="${CMSSW_VERSION}",
            help="write a CMSSW version, e.g. '--release=${CMSSW_VERSION}'")
        parser.add_option("", "--recipe", type="string",
                          dest="recipe", default=None,
                          help="Recipe to follow (useful for 'experimental' releases)")
        parser.add_option("-b", "--build", action="store_true", dest="build", default=False,
            help="Build the packages after checking out")
        options, args = parser.parse_args()

        #expand environment variables etc
        options.cmssw_version = os.path.expandvars(options.cmssw_version)
        options.installation_directory = os.path.abspath(os.path.expanduser(options.installation_directory))

        return options, args

    def init(self):
        """Checkout CMGTools"""

        self.installCMSSW()
        
        pwd = os.getcwd()
        try:
            src = os.path.join(self.options.installation_directory,self.options.cmssw_version,'src')
            os.chdir(src)
            self.checkoutSoftwarePackages()
        finally:
            os.chdir(pwd)


    def installCMSSW(self):
        """Install CMSSW"""
        if not os.path.exists(self.options.installation_directory):
            os.mkdir(self.options.installation_directory)

        try:
            os.chdir("%s" % self.options.installation_directory)
        finally:
            subprocess.call(['scram', 'project', 'CMSSW', '%s' % self.options.cmssw_version])
   

    def cvsCheckout(self, pack, cvsmod, tag, doit = True):
        '''Check out one package.'''

        tagcmd = ' '.join( ['-r', tag] )
        if tag.lower() == 'head':
            tagcmd = ''

        dircmd = ''
        if pack != cvsmod:
            dircmd = ' '.join( ['-d', pack] )

        cmd = ' '.join( ['cvs co', tagcmd, dircmd, cvsmod] )
        print cmd
        if doit:
            os.system(cmd)


    def checkoutSoftwarePackages(self):
        """Checkout CMGTools"""
        print os.getcwd()
        os.system( 'eval `scramv1 runtime -sh`' )
        
        cmgtools_repo_cloned = False
        analysisdataformats_repo_cloned = False

        if not isInputDict( self.dic_list):
            for pack, cvsmod, tag in self.dic_list:    

                if ( cvsmod.startswith( 'UserCode/CMG/' ) ):        #if it's our UserCode in github
                    cmg_repo, cmg_package = pack.split( '/', 1 )    #split pack to get repo and cmg package
                    
                    if ( cmg_repo == "AnalysisDataFormats" ):               #AnalysisDataFormats repo
                        if ( analysisdataformats_repo_cloned == False ):    #git clone if not already cloned
                            subprocess.call(['git', 
                                             'clone', 
<<<<<<< HEAD
                                             'git@github.com:anantoni/AnalysisDataFormats.git'])
                            analysisdataformats_repo_clone = True
=======
                                             'https://github.com/anantoni/AnalysisDataFormats.git'])
                            analysisdataformats_repo_cloned = True
>>>>>>> 2b81db96055b351b21d986d4f700d283aaf0831f
                    
                    if ( cmg_repo == "CMGTools" ):                          #CMGTools repo
                        if ( cmgtools_repo_cloned == False ):               #git clone if not already cloned
                            subprocess.call(['git', 
                                             'clone', 
<<<<<<< HEAD
                                             'git@github.com:anantoni/CMGTools.git'])
                            cmgtools_repo_clone = True
=======
                                             'https://github.com/anantoni/CMGTools.git'])
                            cmgtools_repo_cloned = True
>>>>>>> 2b81db96055b351b21d986d4f700d283aaf0831f
                    
                    pwd = os.getcwd()
                    cmg_repo_path = os.path.join( pwd, cmg_repo )
                    os.chdir( cmg_repo )
                    gitcmd = ' '.join( ['git checkout', tag, '--', cmg_package ] )
                    print gitcmd
                    os.system( gitcmd )
                    os.chdir( pwd )

                elif ( cvsmod.startswith( 'UserCode/' ) ):
                    print "Usercode package \"" + pack + "\"  not under our control - checking out from cvs"
                    self.cvsCheckout( pack, cvsmod, tag, doit=ENABLE)

                else:
                    print "CMSSW package \"" + pack + "\" - checking out from cmssw git repo"
                    gitcmd = ' '.join( ['git checkout', tag, '--', pack] )
                    print gitcmd
                    p = subprocess.Popen(['git', 'checkout', tag, '--', pack], 
                                         stdout=subprocess.PIPE, 
                                         stderr=subprocess.STDOUT)

                    git_out, git_err = p.communicate()
                    print git_out
                    print git_err
                    if ( git_out.startswith( 'fatal:' ) ):
                        print git_out                        
                        print "ERROR - could not checkout package from git repo - checking out from cvs"
                        self.cvsCheckout( pack, cvsmod, tag, doit=ENABLE)
                
        else:
            # the following is kept for backward compatibility
            print "Woops"
            for key in self.dic_list.keys():
                cmd = 'git cms-addpkg '
                #add a tag if needed
                if not self.dic_list[key][0].lower() == 'head':
                    cmd += ' %s ' % self.dic_list[key][0]
                #specify a target directory if needed
                if self.dic_list[key][1] and key != self.dic_list[key][1]:
                    cmd += ' %s ' % self.dic_list[key][1]
                cmd = '%s %s' % (cmd,key)
                print cmd
                os.system(cmd)


######################################################################

if __name__ == '__main__':

    try:
        m = InstallCMGTools()
    except Exception, e:
        print "ERROR %s" % str(e)

######################################################################
