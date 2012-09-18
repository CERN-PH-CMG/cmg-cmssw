#!/usr/bin/env python

#this script runs cmsStage multiple times in the case where it failes for some reason

if __name__ == '__main__':

    import CMGTools.Production.eostools as eostools
    eostools.setCAFPath()

    from cmsIO import *
    from cmsStage import *

    import sys, time

    #this taken from the main of cmsStage
    argv = sys.argv[1:]
    (args, debug, force ) = parseOpts( argv )

    source = cmsFile( args[0], "rfio" )
    destination = cmsFile( args[1], "stageout" )
    checkArgs( source, destination, force )

    #find the destination LFN
    dest = args[1]
    if eostools.isDirectory(dest):
        dest = os.path.join(dest,args[0])
        
    for i in xrange(5):
        try:
            #run cmsStage
            print 'cmsStage %s [%d/5]' % (' '.join(argv) , i+1)
            main(argv)

        except SystemExit, e:
            print "cmsStage exited with code '%s'. Retrying... [%d/5]" % ( str(e), i+1 )
        
        #sleep for a while before checking
        time.sleep(1+i)

        if eostools.fileExists(dest) and eostools.isFile(dest):
            if source.size() == destination.size():
                break
