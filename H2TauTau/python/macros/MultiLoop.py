import os
import shutil
import glob
import sys
from multiprocessing import Pool
from pprint import pprint

#COLIN: I think I don't need PersistentDict
from CMGTools.RootTools.PersistentDict import PersistentDict
from CMGTools.H2TauTau.macros.Loop import Loop
from CMGTools.H2TauTau.macros.AnalysisConfig import AnalysisConfig

def CallBack( result ):
    print 'production done:\n', str(result)

def RunLoop( comp ):
    #COLIN need to be able to catch exceptions! 
    fullName = '/'.join( [outDir, comp.name ] )
    files = glob.glob(comp.files)
    loop = Loop( fullName, files,
                 triggers = comp.triggers,
                 vertexWeight = comp.vertexWeight )
    print loop
    loop.Loop()
    loop.Write()
    print loop
    return fullName

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
        pprint( os.listdir(dir) )
        print 'component list: '
        pprint( components )
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
    'analysis_cfg' may look like that:

[DEFAULT]
basedir = 2011/CMGTmp
trigger_MC = HLT_IsoMu12_v1
components = TTJets,PromptReco-v4
mc_eff_corrfactor = 0.968 * 0.92

[WJets]
files = %(basedir)s/WJets/*baseline*.root
trigger = %(trigger_MC)s
eff_corrfactor = %(mc_eff_corrfactor)s
xsection = 31314
# 53655290 = PFAOD integrity, 81352581 = "DBS"
n_gen_events  =  53227112 
mc = 1 
recoil = 1

[TTJets]
files = %(basedir)s/TTJets/*baseline*.root
trigger = %(trigger_MC)s
eff_corrfactor = %(mc_eff_corrfactor)s
xsection = 157.5
n_gen_events = 3542770
mc = 1 
recoil = 0

[DYJets]
files = %(basedir)s/DYJets/*baseline*.root
trigger = %(trigger_MC)s
eff_corrfactor = %(mc_eff_corrfactor)s
xsection = 3048
n_gen_events = 34915945
mc = 1 
recoil = 1 

[PromptReco-v4]
files = %(basedir)s/PromptReco-v4/*baseline*.root
trigger = HLT_IsoMu15_LooseIsoPFTau15_v[2,4,5,6]	
lumi = 929.748

[PromptReco-v6]
files = %(basedir)s/PromptReco-v6/*baseline*.root
trigger = HLT_IsoMu15_LooseIsoPFTau15_v[8,9]
lumi = 658.886
mc = 0
 
    """
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        print 'ERROR: please provide the processing name and the component list'
        sys.exit(1)

    outDir = args[0]
    cfgFile = args[1]

    # jobArgs = PersistentDict( 'jobArgs', componentList)

    anacfg = AnalysisConfig( cfgFile )
    selComps = anacfg.SelectedComponents()
    for comp in selComps.values():
        print comp
    if len(selComps)>10:
        raise ValueError('too many threads')
    if not CreateOutputDir(outDir,
                           selComps.keys()):
        print 'exiting'
        sys.exit(0)

    if len(selComps)>1:
        shutil.copy( cfgFile, outDir )
        pool = Pool(processes=len(selComps))
        for name,comp in selComps.iteritems():
            print 'submitting', name
            pool.apply_async( RunLoop, [comp], callback=CallBack)     
        pool.close()
        pool.join()
    else:
        # when running only one loop, do not use multiprocessor module.
        # then, the exceptions are visible -> use only one sample for testing
        RunLoop( comp )
