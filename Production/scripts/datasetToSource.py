#!/usr/bin/env python

from CMGTools.Production.datasetToSource import *

if __name__ == '__main__':
    import sys,pprint

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = "%prog <user> <dataset> <regexp pattern>"
    (opts, args) = parser.parse_args()
    if len(args)!=3:
        parser.print_help()
        sys.exit(1)

    source = datasetToSource( sys.argv[1], sys.argv[2], sys.argv[3])
    dump = source.dumpPython()
    dump = dump.replace("'/store","\n'/store")
    print dump
