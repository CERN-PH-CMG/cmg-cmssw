import CMGTools.RootTools.TestTools as TestTools
import CMGTools.RootTools.cmgTuple as cmgTuple
import ROOT as rt

import os,shutil,subprocess,tempfile,unittest

class TestBuild(TestTools.CFGTest):
    
    def __init__(self,methodName):
        TestTools.CFGTest.__init__(self,methodName)
        
    def getReleaseInfo(self, version):
        """Use the same file as in the installed to get the tags"""
        
        result = {}
        
        import urllib
        try:
            url = 'http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/Common/python/release_info/%s.py?view=co' % version
            release_info = urllib.urlopen(url)
            content = release_info.read()
            if content.find('404 Not Found') != -1:
                raise Exception("The file '%s' does not exist or could not be accessed" % url)
            if content is not None and content:
                result = eval(content)
        except Exception, e:
            message = "Release info can not be found for version '%s' - the error was '%s'" % (version,str(e))
            raise Exception(message)
        
        return result

    def checkOutAndBuild(self, keys = None):
        """Checks out and builds some code from cvs"""
        
        version = os.environ['CMSSW_VERSION']
        release_info = self.getReleaseInfo(version)
        
        stdout = None
        stderr = None
        
        pwd = os.getcwd()
        try:
            tmpdir = tempfile.mkdtemp()
            os.chdir(tmpdir)
            
            subprocess.call(['scram', 'project', 'CMSSW', version])
            src = os.path.join(tmpdir,version,'src')
            os.chdir(src)
            for key,val in release_info.iteritems():
                #we can specify to use only certain packages
                if keys is not None and key not in keys: continue
                os.system('cvs checkout -r %s -d %s %s' % (val[0], val[1], key))
            
            stdout, stderr = subprocess.Popen(['scram', 'b', '-j', '4'], stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
            
            os.chdir(pwd)
            shutil.rmtree(tmpdir, ignore_errors = True)
            
        finally:
            os.chdir(pwd)
            
        return (stdout,stderr)
    
    def lookForFail(self,output):
        """scan output for files that look like a build failure"""
        fail = False 
        for line in output.split('\n'):
            #look for an error msg from make
            print 'line:',line
            if 'gmake:' in line and 'Error' in line:
                fail = True
            if fail: break
        return fail

    def downloadAndBuildRecipe(self):

        version = os.environ['CMSSW_VERSION']
        release_info = self.getReleaseInfo(version)
        
        stdout = None
        stderr = None
        
        pwd = os.getcwd()
        try:
            tmpdir = tempfile.mkdtemp()
            os.chdir(tmpdir)

            subprocess.call(['wget','-O','installCMGTools.py',"http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/Common/bin/installCMGTools.py?view=co"])
            stdout, stderr = subprocess.Popen(['python','installCMGTools.py','-r', version, '-b'], stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
            
            os.chdir(pwd)
            shutil.rmtree(tmpdir, ignore_errors = True)
            
        finally:
            os.chdir(pwd)
            
        return (stdout,stderr)

        
    
    def testBuildFormatsStandAlone(self):
        """Check out and build the version of the format package from cvs with the recipe tags. Format package must build standalone."""
        
        stdout, stderr = self.checkOutAndBuild(keys = ['UserCode/CMG/AnalysisDataFormats/CMGTools'])
        self.failIf(stdout is None, 'The stdout must be complete')
        self.failIf(stderr is None, 'The stderr must be complete')
        
        self.assertFalse(self.lookForFail(stderr),'The build did not complete')
        
    def testBuildRecipe(self):
        """Check out and build the current recipe"""
        
        print "Testing build for recipe version '%s'" % os.environ['CMSSW_VERSION']
        
        stdout, stderr = self.downloadAndBuildRecipe()
        self.failIf(stdout is None, 'The stdout must be complete')
        self.failIf(stderr is None, 'The stderr must be complete')
        
        self.assertFalse(self.lookForFail(stderr),'The build did not complete')

        
if __name__ == '__main__':
    
    unittest.main()    
        
