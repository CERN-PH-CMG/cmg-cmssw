#!/usr/bin/env python

from CMGTools.Production.dataset import * 

if __name__ == '__main__':

    import sys
    from optparse import OptionParser
    
    parser = OptionParser()
    parser.usage = "%prog <user> <dataset>\nPrints information on a sample."
    parser.add_option("-p", "--pattern", dest="pattern", default='.*tree.*root',help='regexp pattern for root file printout')

    (options,args) = parser.parse_args()

    if len(args)!=2:
        parser.print_help()
        sys.exit(1)

    user = args[0]
    name = args[1]

    data = Dataset( user, name, options.pattern)
    data.printInfo()
    data.printFiles()

    # print 'good files: '

    # import pprint
    # pprint.pprint( data.listOfGoodFiles() )
