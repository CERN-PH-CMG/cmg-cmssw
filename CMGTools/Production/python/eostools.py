#!/usr/bin/env python
"""
A module to manipulate files on EOS or on the local file system. Intended to have the same interface as castortools.py.
"""

import sys
import os
import re
import pprint
import shutil

def setCAFPath():
    """Hack to get the CAF scripts on the PYTHOPATH"""
    caf = '/afs/cern.ch/cms/caf/python'
    if caf not in sys.path:
        sys.path.append(caf)
setCAFPath()
import cmsIO


def runXRDCommand(path, cmd, *args):
    """Run an xrd command."""
    
    lfn = eosToLFN(path)
    tokens = cmsIO.splitPFN(lfnToPFN(lfn))
    
    command = ['xrd', tokens[1], cmd, tokens[2]]
    command.extend(args)
    runner = cmsIO.cmsFileManip()
    # print ' '.join(command)
    return runner.runCommand(command)


def runEOSCommand(path, cmd, *args):
    """Run an eos command"""
    
    lfn = eosToLFN(path)
    pfn = lfnToPFN(lfn)
    tokens = cmsIO.splitPFN(pfn)
    
    #obviously, this is not nice
    command = ['/afs/cern.ch/project/eos/installation/0.1.0-22d/bin/eos.select', cmd]
    command.extend(args)
    command.append(tokens[2])
    runner = cmsIO.cmsFileManip()
    return runner.runCommand(command)


def isLFN( path ):
    """Tests whether this path is a CMS LFN (/store...)"""
    # return re.match('^/store.*', path ) is not None
    return path.startswith('/store')


def isEOS( path ):
    """Tests whether this path is a CMS EOS (/eos...)"""
    return path.startswith('/eos')


def eosToLFN( path ):
    """Converts a EOS PFN to an LFN"""
    return path.replace('/eos/cms','')
    # if not isLFN(path):
    #    return path.replace('/eos/cms','')
    #else:
    #    return path
    
#also define an alias for backwards compatibility
castorToLFN = eosToLFN


def lfnToPFN( path, tfcProt = 'rfio'):
    """Converts an LFN to a PFN"""
    entity = cmsIO.cmsFile( path, tfcProt )
#    tokens = cmsIO.splitPFN(entity.pfn)
    pfn = '%s://%s//%s/' % (entity.protocol,entity.host,entity.path)
    
    pfn = entity.pfn
    if tfcProt == 'rfio' and \
        entity.path.startswith("/eos/cms/") and \
                str(entity.stat()).startswith("Error 3011: Unable to stat"):

            self.host = 'castorcms'
            pfn.replace("/eos/cms","/castor/cern.ch/cms")
            pfn.replace("eoscms","castorcms")
    return pfn


def lfnToEOS( path ):
    """Converts LFN to EOS"""
    if isLFN(path):
        pfn = '/eos/cms/' + path
        return pfn.replace('//','/') 
    else:
        return path

#also define an alias for backwards compatibility
lfnToCastor = lfnToEOS

def isEOSDir( path ):
    """Checks whether this file or directory is stored on EOS."""
    if os.path.exists( path ):
        return False
    if not path.startswith('/eos') and not path.startswith('/store'):
        return False
    pfn = lfnToPFN(eosToLFN(path))
    # print 'PFN',pfn
    # print 'done'
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

    eos = isEOSDir(path)
    result = False
    if eos:
        # print 'eos', path
        result = isEOSFile(path)
    else:
        # print 'not eos', path
        #check locally
        result = os.path.exists(path)
    # print result
    return result

def createEOSDir( path ):
    """Makes a directory in EOS"""
    lfn = eosToLFN(path)
    # print 'creating', path
    if not isEOSFile(lfn):
        runEOSCommand(lfn,'mkdir','-p')
#        entity = cmsIO.cmsFile( lfn,"stageout")
#        entity.mkdir([])
#        # print 'created ', path
    return path
#also define an alias for backwards compatibility
createCastorDir = createEOSDir


def mkdir(path):
    """Create a directory, either on EOS or locally"""
    # print 'mkdir', path
    if isEOS( path ) or isLFN(path):
        createEOSDir(path)
    else:
        # recursive directory creation (like mkdir -p)
        os.makedirs(path)
    return path


def isDirectory(path):
    """Checks whether a lfn is a file or a directory"""

    out, _, _ = runXRDCommand(path,'existdir')
    return 'The directory exists' in out


def isFile(path):
    """Checks whether a lfn is a file or a directory"""

    if not path.startswith('/eos') and not path.startswith('/store'):
        if( os.path.isfile(path) ):
            return True
        else:
            return False
    else: 
        out, _, _ = runXRDCommand(path,'existfile')
        return 'The file exists' in out

def chmod(path, mode):
    """Does chmod on a file or directory"""
    #
    return runEOSCommand(path, 'chmod', '-r', str(mode))


def listFiles(path, rec = False):
    """Provides a list of the specified directory

    COLIN : does not work for local directories...
    """
    # -- listing on the local filesystem --
    if os.path.isdir( path ):
        if not rec:
            # not recursive
            return [ '/'.join([path,file]) for file in os.listdir( path )]
        else:
            # recursive, directories are put in the list first,
            # followed by the list of all files in the directory tree
            result = []
            allFiles = []
            for root,dirs,files in os.walk(path):
                result.extend( [ '/'.join([root,dir]) for dir in dirs] )
                allFiles.extend( [ '/'.join([root,file]) for file in files] )
            result.extend(allFiles)
            return result
    # -- listing on EOS --
    cmd = 'dirlist'
    if rec:
        cmd = 'dirlistrec'
    files, _, _ = runXRDCommand(path, cmd)
    result = []
    for line in files.split('\n'):
        tokens = [t for t in line.split() if t]
        if tokens:
            #convert to an LFN
            # result.append(tuple(tokens))
            #COLIN need same interface for eos and local fs
            result.append( tokens[4])
    # print result
    return result

def which(cmd):
    command = ['which', cmd]
    runner = cmsIO.cmsFileManip()
    out, _, _ = runner.runCommand(command)
    
    lines = [line for line in out.split('\n') if line]
    if len(lines) == 1:
        return lines[0]
    elif len(lines) == 2:
        return lines[1]
    else:
        return lines


def ls(path, rec = False):
    """Provides a simple list of the specified directory, works on EOS and locally"""
    return [eosToLFN(t) for t in listFiles(path, rec)]


def rm(path, rec=False):
    """rm, works on EOS and locally.

    Colin: should implement a -f mode and a confirmation when deleting dirs recursively."""
    # print 'rm ', path
    path = lfnToEOS(path)
    if isEOS(path):
        if rec:
            runEOSCommand(path, 'rm', '-r')
        else: 
            runEOSCommand(path,'rm')
    elif os.path.exists(path):
        if not rec:
            os.remove( path )
        else:
            shutil.rmtree(path)
    else:
        raise ValueError(path + ' is not EOS and not local... should not happen!')

def remove( files, rec = False):
    """Remove a list of files and directories, possibly recursively

    Colin: Is that obsolete? why not use rm?"""
    
    for path in files:
        lfn = eosToLFN(path)
        if not rec:
            rm(path)
        else:
            #this should be used with care
            file_list = ls(path, rec = True)
            file_list.append(lfn)
            
            #order the files in depth order - i.e. remove the deepest files first
            files_rec = sorted([(len([ff for ff in f.split('/') if ff]), f) for f in file_list if f and f.startswith(lfn)], reverse = True)
            
            for f in files_rec:
                rm(f[1])
                
def cat(path):
    """cat, works on EOS and locally"""
    path = lfnToEOS(path)
    if isEOS(path):
        out, err, _ = runXRDCommand(path,'cat') 
        lines = []
        if out:
            pattern = re.compile('cat returned [0-9]+')
            for line in out.split('\n'):
                match = pattern.search(line)
                if line and match is not None:
                    lines.append(line.replace(match.group(0),''))
                    break
                else:
                    lines.append(line)
        if err:
            print >> sys.stderr, out
            print >> sys.stderr, err
        allLines = '\n'.join(lines)
        if allLines and not allLines.endswith('\n'):
            allLines += '\n'
        return allLines
    else:
        content = file(path).read()
        if content and not content.endswith('\n'):
            content += '\n'
        return content
    
def xrdcp(src, dest):
    """Does a copy of files using xrd.

    Colin: implement a generic cp interface as done for rm, ls, etc?"""
    
    recursive = False
    
    #first the src file
    pfn_src = src
    if os.path.exists(src):
        #local
        pfn_src = src
        if os.path.isdir(src):
            recursive = True
    elif fileExists(src):
        src = eosToLFN(src)
        pfn_src = lfnToPFN(src)
        if isDirectory(src):
            recursive = True
    else:
        raise ValueError(src + ' does not exist.')
            
    #now the dest
    pfn_dest = dest
    if isEOSDir(dest):
        dest = eosToLFN(dest)
        pfn_dest = lfnToPFN(dest)
        if isDirectory(dest):
            tokens = cmsIO.splitPFN(pfn_dest)
            pfn_dest = '%s://%s//%s/' % (tokens[0],tokens[1],tokens[2])
    elif os.path.exists(dest):
        pfn_dest = dest

    command = ['xrdcp','-force']
    if recursive:
        # print 'recursive'
        topDir = src.rstrip('/').split('/')[-1]
        if topDir != '.':
            dest = '/'.join([dest, topDir])
            # print 'mkdir ' + dest
            mkdir( dest )
        files = listFiles(src, rec=True)
        # pprint.pprint( [file[4] for file in files] )
        for srcFile in files:
            # srcFile = file[4]
            pfnSrcFile = srcFile
            if isEOSDir(srcFile):
                srcFile = eosToLFN(srcFile)
                pfnSrcFile = lfnToPFN(srcFile)
            destFile = srcFile.replace( src, '' )
            destFile = '/'.join([dest,destFile])
            pfnDestFile = destFile
            if isEOSDir(destFile):
                lfnDestFile = eosToLFN(destFile)
                pfnDestFile = lfnToPFN(lfnDestFile)
            # print 'srcFile', pfnSrcFile
            # print 'destFile', pfnDestFile
            if isFile(srcFile):
                _xrdcpSingleFile(  pfnSrcFile, pfnDestFile )
            else:
                mkdir(destFile)
    else:
        _xrdcpSingleFile( pfn_src, pfn_dest )


def _xrdcpSingleFile( pfn_src, pfn_dest):
    """Copies a single file using xrd."""
    
    command = ['xrdcp','-force']
    command.append(pfn_src)
    command.append(pfn_dest)
    # print ' '.join(command)
    run = True
    if run: 
        runner = cmsIO.cmsFileManip()
        out, err, ret = runner.runCommand(command)
        if err:
            print >> sys.stderr, out
            print >> sys.stderr, err
        return ret

def move(src, dest):
    """Move filename1 to filename2 locally to the same server"""
    
    src = eosToLFN(src)
    dest = eosToLFN(dest)

    runXRDCommand(src,'mv', lfnToEOS(dest))
                
def matchingFiles( path, regexp):
    """Return a list of files matching a regexp"""

    # print path, regexp
    pattern = re.compile( regexp )
    files = ls(path)
    # print files
    return [f for f in files if pattern.match(os.path.basename(f)) is not None]
    
def cmsStage( absDestDir, files, force):
    """Runs cmsStage with LFNs if possible"""

    destIsEOSDir = isEOSDir(absDestDir)
    if destIsEOSDir: 
        createEOSDir( absDestDir )

    for fname in files:
        command = ['cmsStage']
        if force:
            command.append('-f')
        command.append(eosToLFN(fname))
        command.append(eosToLFN(absDestDir))
        print ' '.join(command)
        runner = cmsIO.cmsFileManip()
        runner.runCommand(command)
