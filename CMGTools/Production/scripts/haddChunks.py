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
    parser.add_option("-c","--clean", dest="clean",
                      default=False,action="store_true",
                      help="move chunks to Chunks/ after processing.")
    parser.add_option("-o","--outputdir", dest="outputdir",
                      default=False,action="store_true",
                      help="define output folder (default is inside same directory).")

    (options,args) = parser.parse_args()

    if len(args)>2:
        print 'provide at most 2 directory as arguments: first the source, then the destination (optional)'
        sys.exit(1)

    dir = args[0]
    if(len(args)>1):
      odir = args[1]
    else:
      odir='./'

    haddChunks(dir, options.remove, options.clean, options.outputdir, odir)

