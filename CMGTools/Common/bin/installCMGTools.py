#!/usr/bin/env python

import sys
import os
from optparse import OptionParser
import subprocess

class InstallCMGTools(object):
    """
    A class to make the installation of CMGTools easier
    @author Luke Heo - seong.gu.heo@cern.ch
        """
    
    def setReleaseInfo(self, version):
        
        import urllib
        try:
            url = 'http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/Common/python/release_info/%s.py?view=co' % version
            release_info = urllib.urlopen(url)
            content = release_info.read()
            if content.find('404 Not Found') != -1:
                raise Exception("The file '%s' does not exist or could not be accessed" % url)
            if content is not None and content:
                self.dic_list = eval(content)
        except Exception, e:
            message = "Release info can not be found for version '%s' - the error was '%s'" % (version,str(e))
            raise Exception(message)
    
    def __init__(self):
        self.options, self.args = self.parseOptions()

        self.setReleaseInfo(self.options.cmssw_version)
        
        pwd = os.getcwd()
        try:
            self.init()
        finally:
            os.chdir(pwd)

        if self.options.build:
            try:
                src = os.path.join(self.options.installation_directory,self.options.cmssw_version,'src')
                os.chdir(src)
                subprocess.call(['scram', 'b', '-j', '4'])
            finally:
                os.chdir(pwd)
        
    def parseOptions(self):
        """ Accept arguments"""
        parser = OptionParser()
        parser.add_option("-d", "--dir", type="string", dest="installation_directory", default='~/scratch0/CMGTools',
            help="write a installation directory, e.g. '--dir ~/scratch0/CMGTools'")
        parser.add_option("-r", "--release",type="string", dest="cmssw_version", default="${CMSSW_VERSION}",
            help="write a CMSSW version, e.g. '--release=${CMSSW_VERSION}'")
        parser.add_option("-c", "--cvs", type="string", dest="cvs_command", default="checkout",
            help="write a cvs command. e.g. '--cvs checkout' or '--cvs update'")
        parser.add_option("-b", "--build", action="store_true", dest="build", default=False,
            help="Build the packages after checking out")
        options, args = parser.parse_args()

        #expand environment variables etc
        options.cmssw_version = os.path.expandvars(options.cmssw_version)
        options.installation_directory = os.path.expanduser(options.installation_directory)

        return options, args

    def init(self):
        """Checkout and/or update CMGTools"""

        if self.options.cvs_command == 'checkout' or self.options.cvs_command == 'co':
            
            self.installCMSSW()

            pwd = os.getcwd()
            try:
                src = os.path.join(self.options.installation_directory,self.options.cmssw_version,'src')
                os.chdir(src)
                self.checkoutCMGTools()
            finally:
                os.chdir(pwd)

        if self.options.cvs_command == 'update' or self.options.cvs_command == 'up':

            pwd = os.getcwd()
            try:
                src = os.path.join(self.options.installation_directory,self.options.cmssw_version,'src')
                if not os.path.exists(src):
                    print("**************************************************")
                    print("* Run checkout first. e.g. > ... --cvs=checkout' *")
                    print("**************************************************")
                else:
                    os.chdir(src)
                    self.updateCMGTools()
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

    def checkoutCMGTools(self):
        """Checkout CMGTools"""
        for key in self.dic_list.keys():
            cmd = 'cvs checkout '
            #add a tag if needed
            if not self.dic_list[key][0].lower() == 'head':
                cmd += '-r %s ' % self.dic_list[key][0]
            #specify a target directory if needed    
            if self.dic_list[key][1] and key != self.dic_list[key][1]:
                cmd += '-d %s ' % self.dic_list[key][1]
            cmd = '%s %s' % (cmd,key)    
            print cmd
            os.system(cmd)

    def updateCMGTools(self):
        """Update CMGTools"""
        for key in self.dic_list.keys():
            if not self.dic_list[key][0].lower() == 'head':
                os.system('cvs update -r %s -d %s' % (self.dic_list[key][0], self.dic_list[key][1]))
            else:
                os.system('cvs update -A -d %s' % (self.dic_list[key][1]))

if __name__ == '__main__':

    m = InstallCMGTools()
