#!/usr/bin/env python
import tarfile
import os
import subprocess
import fnmatch

tarsuffix = 'out.tar.gz'

##_______________________________________________________________||
def _UnTar(tars,dry_run):

    try:
        for i in tars:
            cwd = os.getcwd()
            directory = os.path.dirname(i)
            os.chdir(directory)
            tar = tarfile.open(tarsuffix)
            if dry_run:
                for tarinfo in tar:
                    print "Extracting" , tarinfo.name, "from", tarsuffix
                    sys.exit(1)
            tar.extractall()
            tar.close()
            os.chdir(cwd)
            print 'Extracted in %s directory'  %directory
    except TypeError:
        print 'Unable to extract tar'

##_______________________________________________________________||
def _ListTars(direc,dry_run):
    
    try:
        tarfiles = [os.path.join(dirpath, f)
            for dirpath, dirnames, files in os.walk(direc)
                for f in fnmatch.filter(files,tarsuffix)]        
        if len(tarfiles) == 0:
            print 'No list formed'
            sys.exit(1)
    except ValueError:
        print 'Could not form list'

    _UnTar(tarfiles,dry_run)

##_______________________________________________________________||
if __name__ == '__main__':

    import os
    import sys
    from optparse import OptionParser
    
    parser = OptionParser()
    parser.add_option("--dry_run",action="store_true", default=False, help="Do not run any commands; only print them")

    (options,args) = parser.parse_args()

    if len(args)>1:
        print 'Please only provide 1 argument'
        sys.exit(1)
    elif len(args) == 0:
        print 'Please provide the path'
        sys.exit(1)
    else:
        iDir = args[0]
        if iDir == '.':
            iDir = os.getcwd()
        _ListTars(iDir,options.dry_run)
