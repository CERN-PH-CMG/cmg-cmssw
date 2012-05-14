#!/bin/env python

from CMGTools.Production.hadd import haddChunks


if __name__ == '__main__':

    import os
    import sys
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog <dir>
    Find chunks in dir, and run recursive hadd to group all chunks.
    For example: 
    DYJets_Chunk0/, DYJets_Chunk1/ ... -> hadd -> DYJets/
    WJets_Chunk0/, WJets_Chunk1/ ... -> hadd -> WJets/
    """
    parser.add_option("-r","--remove", dest="remove",
                      default=False,action="store_true",
                      help="remove existing destination directories.")

    (options,args) = parser.parse_args()

    if len(args)!=1:
        print 'provide exactly one directory in argument.'
        sys.exit(1)

    dir = args[0]

    haddChunks(dir, options.remove)

##     # below, a hack for Colin's analysis. 
##     os.chdir( dir )
##     dy = 'DYJets'
##     if dy in os.listdir('.') and os.environ['USER']=='cbern':
##         fakes = '/'.join([dy,'Fakes'])
##         if os.path.exists( fakes ):
##             os.rename( fakes, '_'.join([dy,'Fakes']))
##         else:
##             print 'warning: DYJets/Fakes not found. Maybe you ran hadd already?'
