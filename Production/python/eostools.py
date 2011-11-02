#!/usr/bin/env python

import sys,os,re
import subprocess

def setCAFPath():
    """Hack to get the CAF scripts on the PYTHOPATH"""
    caf = '/afs/cern.ch/cms/caf/python'
    if caf not in sys.path:
        sys.path.append(caf)
setCAFPath()
import cmsIO

def runXRDCommand(path, cmd, *args):
    """Run an xrd command"""
    
    lfn = eosToLFN(path)
    pfn = lfnToPFN(lfn)
    tokens = cmsIO.splitPFN(pfn)
    
    command = ['xrd',tokens[1],cmd, tokens[2]]
    command.extend(args)
    runner = cmsIO.cmsFileManip()
    return runner.runCommand(command)

def runEOSCommand(path, cmd, *args):
    """Run an eos command"""
    
    lfn = eosToLFN(path)
    pfn = lfnToPFN(lfn)
    tokens = cmsIO.splitPFN(pfn)
    
    #obviously, this is not nice
    command = ['/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select',cmd]
    command.extend(args)
    command.append(tokens[2])
    runner = cmsIO.cmsFileManip()
    return runner.runCommand(command)

def isLFN( path ):
    """Tests whether this path is a CMS LFN"""
    return re.match('^/store.*', path ) is not None

def eosToLFN( path ):
    """Converts a EOS PFN to an LFN"""
    if not isLFN(path):
        return path.replace('/eos/cms','')
    else:
        return path
#also define an alias for backwards compatibility
castorToLFN = eosToLFN

def lfnToPFN( path, tfcProt = 'rfio'):
    """Converts an LFN to a PFN"""
    entity = cmsIO.cmsFile( path, tfcProt )
    return entity.pfn

def lfnToEOS( path ):
    if isLFN(path):
        pfn = '/eos/cms/' + path
        return pfn.replace('//','/') 
    else:
        return path
#also define an alias for backwards compatibility
lfnToCastor = lfnToEOS

def isEOSDir( path ):
    """Checks whether this file or directory is stored on EOS."""
    pfn = lfnToPFN(eosToLFN(path))
    tokens = cmsIO.splitPFN(pfn)
    return tokens and tokens[1].lower().startswith('eos')
#also define an alias for backwards compatibility
isCastorDir = isEOSDir

def isEOSFile( path, tfcProt = 'rfio'):
    """Checks whether this file exists"""
    lfn = eosToLFN(path)
    entity = cmsIO.cmsFile( os.path.dirname(lfn), tfcProt )
    return lfn in entity.ls(lfn)
#also define an alias for backwards compatibility
isCastorFile = isEOSFile

def fileExists( path ):
    """Check whether a file exists either on EOS or locally"""
    
    lfn = eosToLFN(path)
    eos = isEOSDir(path)
    
    result = False
    if eos:
        result = isEOSFile(path)
    else:
        #check locally
        result = os.path.exists(path)
    return result

def createEOSDir( path ):
    """Makes a directory in EOS"""
    lfn = eosToLFN(path)
    if not isEOSFile(lfn):
        entity = cmsIO.cmsFile( lfn,"stageout")
        entity.mkdir([])
    return path
#also define an alias for backwards compatibility
createCastorDir = createEOSDir

def isDirectory(path):
    """Checks whether a lfn is a file or a directory"""

    out,err,ret = runXRDCommand(path,'existdir')
    return 'The directory exists' in out

def isFile(path):
    """Checks whether a lfn is a file or a directory"""

    out,err,ret = runXRDCommand(path,'existfile')
    return 'The file exists' in out

def chmod(path, mode):
    """Does chmod on a file or directory"""
    #
    return runEOSCommand(path,'chmod','-r',str(mode))


def listFiles(path, rec = False):
    """Provides a list of the specified directory"""

    cmd = 'dirlist'
    if rec:
        cmd = 'dirlistrec'

    files, err, ret = runXRDCommand(path,cmd)
    result = []
    for line in files.split('\n'):
        tokens = [t for t in line.split() if t]
        if tokens:
            #convert to an LFN
            result.append(tuple(tokens))
    return result

def which(cmd):
    command = ['which',cmd]
    runner = cmsIO.cmsFileManip()
    out, err, ret = runner.runCommand(command)
    
    lines = [line for line in out.split('\n') if line]
    if len(lines) == 1:
        return lines[0]
    elif len(lines) == 2:
        return lines[1]
    else:
        return lines

def ls(path, rec = False):
    """Provides a simple list of the specified directory"""
    list = listFiles(path, rec)
    return [eosToLFN(t[-1]) for t in list]

def rm(path, tfcProt = 'rfio'):
    """Delete a file from EOS"""
    
    lfn = eosToLFN(path)

    entity = cmsIO.cmsFile(lfn, tfcProt )
    if isDirectory(lfn):
        entity.rmdir([])
    elif isFile(lfn):
        entity.rm([])

def remove( files, rec = False):
    """Remove a list of files and directories, possibly recursively"""
    
    for path in files:
        lfn = eosToLFN(path)
        if not rec:
            rm(lfn)
        else:
            #this should be used with care
            file_list = ls(path, rec = True)
            file_list.append(lfn)
            
            #order the files in depth order - i.e. remove the deepest files first
            files_rec = sorted([(len([ff for ff in f.split('/') if ff]),f) for f in file_list if f and f.startswith(lfn)], reverse = True)
            
            for f in files_rec:
                rm(f[1])
                
def matchingFiles( path, regexp):
    """Return a list of files matching a regexp"""

    pattern = re.compile( regexp )
    files = ls(path)
    return [f for f in files if pattern.match(f) is not None]

def xrdcp( absDestDir, files ):
    cp = 'cp'
    destIsCastorDir = isCastorDir(absDestDir)
    if destIsCastorDir: 
        cp = 'xrdcp'
        createCastorDir( absDestDir )
        
    for file in files:

        cpfile = '%s %s %s' % (cp, file,absDestDir)
        
        if destIsCastorDir == False:
            if isCastorDir( os.path.abspath(file) ):
                cp = 'xrdcp'
                cpfile = '%s "root://castorcms/%s?svcClass=cmst3&stageHost=castorcms" %s' % (cp, file,absDestDir)

        print cpfile
        os.system(cpfile)

    
if __name__ == '__main__':
    
    import castortools
    import unittest
    
    class TestEosTools(unittest.TestCase):
        
        def setUp(self):
            
            self.castorfile = '/castor/cern.ch/cms/store/cmst3/user/wreece/EOS_TEST/test_file.txt'
            self.eosfile = '/eos/cms/store/cmst3/user/wreece/EOS_TEST/test_file.txt'
        
        def testWhich(self):
            
            self.assertEqual(which('bash'),'/usr/local/bin/bash')
            self.assertEqual(which('ls'),'/bin/ls')
        
        def testIsLFN(self):
            
            self.assertFalse(isLFN(self.castorfile))
            self.assertFalse(isLFN(self.eosfile))
            
            lfn = eosToLFN(self.eosfile)
            self.assertTrue(isLFN(lfn))
        
        def testLFNToCastor(self):
            self.assertNotEqual(lfnToCastor(self.eosfile),castortools.lfnToCastor(self.castorfile))
            
        def testCastorToLFN(self):
            self.assertEqual(castorToLFN(self.eosfile),castortools.castorToLFN(self.castorfile))
        
        def testIsEOSDir(self):
            self.assertTrue(isEOSDir(eosToLFN(self.eosfile + 'FOO'))) #should still work if it doesn't exist
            self.assertTrue(isEOSDir(eosToLFN(self.eosfile)))
            self.assertTrue(isEOSDir(self.eosfile))
            self.assertFalse(isEOSDir(self.castorfile))
            
        def testIsEOSFile(self):
            self.assertTrue(isEOSFile(self.eosfile))
            self.assertFalse(isEOSFile(self.eosfile + 'FOO'))
            
        def testFileExists(self):
            self.assertTrue(fileExists(self.eosfile))
            self.assertFalse(fileExists(self.eosfile + 'FOO'))
            
            local = '/dev/null'
            self.assertTrue(fileExists(local))
            self.assertFalse(fileExists(local + 'FOO'))
            
            self.assertEqual(fileExists(local),castortools.fileExists(local))
            self.assertEqual(fileExists(local + 'FOO'),castortools.fileExists(local + 'FOO'))
            
        def testIsDirectory(self):
            
            dir = os.path.dirname(self.eosfile)
            self.assertTrue(isDirectory(dir))
            self.assertFalse(isDirectory(self.eosfile))
            
        def testIsFile(self):
            
            dir = os.path.dirname(self.eosfile)
            self.assertFalse(isFile(dir))
            self.assertTrue(isFile(self.eosfile))
            
        def testCreateEOSDir(self):
            
            dir = os.path.join(os.path.dirname(self.eosfile),'TEST_DIR','BAR','FOO')
            path = createEOSDir(dir)
            self.assertTrue(isEOSDir(path))
            self.assertTrue(fileExists(path))
            
            self.assertTrue(isDirectory(path))
            
            remove([os.path.join(os.path.dirname(self.eosfile),'TEST_DIR')], rec = True)
            self.assertFalse(fileExists(path))
            
        def testChmod(self):
            
            out, err, ret = chmod(os.path.dirname(self.eosfile),'775')
            self.assertEquals(ret,0)
            
        def testMatchingFiles(self):
            
            dir = os.path.dirname(self.eosfile)
            matches = matchingFiles(dir, '.*test_file\\.txt$')
            self.assertEqual(len(matches),1)
            self.assertEqual(matches[0],eosToLFN(self.eosfile))
            
            
    suite = unittest.TestLoader().loadTestsFromTestCase(TestEosTools)
    unittest.TextTestRunner(verbosity=2).run(suite)
            
            