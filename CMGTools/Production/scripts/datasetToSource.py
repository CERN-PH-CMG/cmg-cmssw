#!/usr/bin/env python

from CMGTools.Production.datasetToSource import *

if __name__ == '__main__':
    import sys,pprint

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = "%prog [options] <dataset>\nPrints information on a sample."
    parser.add_option("-p", "--pattern", dest="pattern", default='.*tree.*root',help='regexp pattern for root file printout')
    parser.add_option("-u", "--user", dest="user", default=os.environ['USER'],help='user owning the dataset')

    (options,args) = parser.parse_args()

    if len(args)!=1:
        parser.print_help()
        sys.exit(1)

    user = options.user
    dataset = args[0]
    pattern = options.pattern
    
    source = datasetToSource( user, dataset, pattern )
    dump = 'source = '
    dump += source.dumpPython()
    dump = dump.replace("'/store","\n'/store")
    print 'import FWCore.ParameterSet.Config as cms'
    print dump
