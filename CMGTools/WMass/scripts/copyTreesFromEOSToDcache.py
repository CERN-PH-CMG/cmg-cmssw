#!/usr/bin/env python

import os
import CMGTools.Production.eostools as eostools
from optparse import OptionParser

if __name__ == "__main__":

    usage="""%prog [options] remoteEOSdir remoteDCACHEdir [listfiles.txt]

# This script can be used to copy files from EOS to DCache in Rome T2.
#
# It needs a valid grid proxy, it will use lcg-cp

"""

    parser = OptionParser(usage=usage)
    parser.add_option("-s", "--select", dest="select", type='string', default=None, help='Filter on files with this pattern')
    parser.add_option("-r", "--remove", dest="remove", type='string', default=None, help='Do not copy files with this pattern')
    parser.add_option("-t", "--destt2", dest="destT2", type='string', default="T2_IT_Rome", help='destination T2')
    parser.add_option("--dryrun", dest="dryrun", action="store_true", default=False, help='only print the commands, do not run the copy')
    (options, args) = parser.parse_args()
    
    if len(args)<2: raise RuntimeError, 'Expecting at least two arguments'
    
    eosdir = args[0]
    remdir = args[1]
    
    listfiles=[]
    if len(args)==3: listfiles=open(args[2]).readlines()
    
    if not eostools.isEOS(eosdir): raise RuntimeError, 'Remote directory should be on EOS.'

    if len(listfiles)>0: files=listfiles
    else: files = eostools.ls(eosdir)
    if options.select: files = [f for f in files if options.select in f]
    if options.remove: files = [f for f in files if options.remove not in f]

    tocopy = []
    for f in files:
        basename = os.path.basename(f)
        tocopy.append( basename )
        print "Will copy file ",basename

    print '\nCopy the above files from %s -> %s. Do you agree? [y/N]\n' % (eosdir,remdir)
    if raw_input()!='y':
        print 'Aborting'
        exit()

    command = 'lcg-cp -b --vo cms -D srmv2 -U srmv2 -v'
    eosconn='srm://srm-eoscms.cern.ch:8443/srm/v2/server?SFN='
    remconn=''

    wgetcomm = "wget -O- --no-check-certificate \"http://cmsweb.cern.ch/phedex/datasvc/xml/prod/lfn2pfn?node=%s&protocol=srmv2&lfn=/XXX\" | sed -e \"s/.*pfn='\([^']*\\).*/\\1\\n/\"" % options.destT2
    os.system(wgetcomm+" > wget.txt")
    lines = open("wget.txt").readlines()
    remconn = (lines[-1]).rstrip()
    remconn = (remconn.split("=",1))[0]
    os.system("rm -f wget.txt")
    if len(remconn)==0:
        print "Not found a corresponding T2 in Phedex"
        exit()

    for f in tocopy:
        source = eosconn + eosdir + "/" + f
        dest = remconn + remdir + "/" + f
        fullcommand = command + " '%s' '%s'" % (source,dest)
        if options.dryrun: print "running ", fullcommand
        else: os.system(fullcommand)
