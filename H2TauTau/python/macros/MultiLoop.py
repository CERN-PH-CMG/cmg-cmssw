import os
import shutil
import glob
import sys
import imp
from multiprocessing import Pool
from pprint import pprint

from CMGTools.H2TauTau.macros.Loop2 import Loop

def CallBack( result ):
    pass
    print 'production done:', str(result)

def RunLoopAsync(comp, config):
    loop = RunLoop( comp, config)
    return loop.name

def RunLoop( comp, config, iEvent=None):
    fullName = '/'.join( [outDir, comp.name ] )
    loop = Loop( fullName, comp, config )
    print loop
    if iEvent is None:
        loop.Loop( options.nevents )
        loop.Write()
        print loop
    else:
        # loop.InitOutput()
        iEvent = int(iEvent)
        loop.ToEvent( iEvent )
    return loop

def TestComponentList( complist ):
    badPattern = False
    for pattern in complist.values():
        nFiles = len( glob.glob(pattern) )
        if not nFiles:
            print 'no file match', pattern
            badPattern = True
    if badPattern:
        raise ValueError('Check your component list')


def CreateOutputDir(dir, components):
    '''Creates the output dir, dealing with the case where dir exists.'''
    answer = None 
    try:
        os.mkdir(dir)
        return True
    except OSError:
        print 'directory %s already exists' % dir
        print 'contents: '
        dirlist = [path for path in os.listdir(dir) if os.path.isdir( '/'.join([dir, path]) )]
        pprint( dirlist )
        print 'component list: '
        print [comp.name for comp in components]
        if options.force is True:
            print 'force mode, continue.'
            return True
        else:
            while answer not in ['Y','y','yes','N','n','no']:
                answer = raw_input('Continue? [y/n]')
            if answer.lower().startswith('n'):
                return False
            elif answer.lower().startswith('y'):
                return True
            else:
                raise ValueError( ' '.join(['answer can not have this value!',
                                            answer]) )

if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog <name> <analysis_cfg>
    For each component, start a Loop.
    'name' is whatever you want.
    """
    parser.add_option("-N", "--nevents", 
                      dest="nevents", 
                      help="number of events to process",
                      default=-1)
    parser.add_option("-i", "--iEvent", 
                      dest="iEvent", 
                      help="jump to a given event. ignored in multiprocessing.",
                      default=None)
    parser.add_option("-f", "--force", 
                      dest="force",
                      action='store_true',
                      help="don't ask questions in case output directory already exists.",
                      default=False)

    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        print 'ERROR: please provide the processing name and the component list'
        sys.exit(1)
    outDir = args[0]
    if os.path.exists(outDir) and not os.path.isdir( outDir ):
        parser.print_help()
        print 'ERROR: when it exists, first argument must be a directory.'
        sys.exit(2)
    cfgFileName = args[1]
    if not os.path.isfile( cfgFileName ):
        parser.print_help()
        print 'ERROR: second argument must be an existing file (your input cfg).'
        sys.exit(3)

    # anacfg = AnalysisConfig( cfgFileName )   
    # selComps = anacfg.SelectedComponents()
    # defaults = anacfg.defaults
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    selComps = cfg.config.components
    for comp in selComps:
        print comp
    if len(selComps)>12:
        raise ValueError('too many threads')
    if not CreateOutputDir(outDir, selComps):
        print 'exiting'
        sys.exit(0)
    if len(selComps)>1:
        shutil.copy( cfgFileName, outDir )
        pool = Pool(processes=len(selComps))
        for comp in selComps:
            print 'submitting', comp.name
            pool.apply_async( RunLoopAsync, [comp, cfg.config], callback=CallBack)     
        pool.close()
        pool.join()
    else:
        # when running only one loop, do not use multiprocessor module.
        # then, the exceptions are visible -> use only one sample for testing
        loop = RunLoop( comp, cfg.config, options.iEvent )
