#!/usr/bin/env python

from CMGTools.Production.castorBaseDir import *
from CMGTools.Production.castortools import *

class Dataset():
    def __init__(self, user, name, pattern='.*root'):
        self.name = name
        self.user = user
        self.lfnDir = castorBaseDir(user) + name
        self.castorDir = castortools.lfnToCastor( self.lfnDir )
        self.pollFiles( pattern )

    def pollFiles(self, pattern='.*root'):
        self.files = castortools.matchingFiles( self.castorDir, pattern )

    def listOfFiles(self):
        return self.files
        
    def dump(self):
        print 'sample      :  ' + self.name
        print 'LFN         :  ' + self.lfnDir
        print 'Castor path :  ' + self.castorDir

    def printFiles(self, pattern='.*root'):
        if self.files == None:
            self.pollFiles(pattern)
        for file in self.files:
            print file

if __name__ == '__main__':

    import sys
    from optparse import OptionParser
    
    parser = OptionParser()
    parser.usage = "%prog <user> <dataset>\nPrints information on a sample."
    parser.add_option("-p", "--pattern", dest="pattern", default='.*root',help='regexp pattern for root file printout')

    (options,args) = parser.parse_args()

    if len(args)!=2:
        parser.print_help()
        sys.exit(1)

    user = args[0]
    name = args[1]

    data = Dataset( user, name )
    data.dump()
    data.printFiles(options.pattern)
