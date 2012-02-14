#!/usr/bin/env python

from CMGTools.Production.dataset import * 

if __name__ == '__main__':

    import sys
    from optparse import OptionParser
    import pprint
    
    parser = OptionParser()
    parser.usage = "%prog [options] <dataset>\nPrints information on a sample."
    parser.add_option("-p", "--pattern", dest="pattern", default='tree.*root',help='regexp pattern for root file printout')
    parser.add_option("-u", "--user", dest="user", default=os.environ['USER'],help='user owning the dataset.\nInstead of the username, give "LOCAL" to read datasets in a standard unix filesystem, and "CMS" to read official CMS datasets present at CERN.')
    parser.add_option("-b", "--basedir", dest="basedir", default=os.environ['CMGLOCALBASEDIR'],help='in case -u LOCAL is specified, this option allows to specify the local base directory containing the dataset. default is CMGLOCALBASEDIR')
    parser.add_option("-a", "--abspath", dest="abspath",
                      action = 'store_true',
                      default=False,
                      help='print absolute path')
    parser.add_option("-n", "--noinfo", dest="noinfo",
                      action = 'store_true',
                      default=False,
                      help='do not print additional info (file size and status)')
    parser.add_option("-r", "--report", dest="report",
                      action = 'store_true',
                      default=False,
                      help='Print edmIntegrityCheck report')

    (options,args) = parser.parse_args()

    if len(args)!=1:
        parser.print_help()
        sys.exit(1)

    user = options.user
    name = args[0]
    info = not options.noinfo
    
    if user == 'CMS':
        data = CMSDataset( name )
        info = False
    elif user == 'LOCAL':
        data = LocalDataset( name, options.basedir, options.pattern)
        info = False        
    else:
        data = Dataset( user, name, options.pattern)
    data.printInfo()
    data.printFiles(abspath = options.abspath,
                    info = info)
    if options.report:
        pprint.pprint( data.report )

