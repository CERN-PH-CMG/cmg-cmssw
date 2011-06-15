#!/usr/bin/env python


from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles

from addToDatasets import *

import sys,os,imp

#scriptDir = os.getenv('CMSSW_BASE') + '/src/CMGTools/Common/scripts'
#print scriptDir
#sys.path.append( scriptDir ) 

import castortools

#sys.path.append( '.' )

from relvalDefinition import *



# main parameters
    
def processRelVal( relval, cfgFileName, process, negate, tier=None):
    
    relvalID = relval.id()

    files = pickRelValInputFiles(
        cmsswVersion  =  relval.cmssw
        , relVal = relval.relval
        , globalTag = relval.tag
        , numberOfFiles = 999
        )


    # changing the source to the chosen relval files
    process.source.fileNames = files
    
    print process.source.fileNames
    
    # building cfg

    outFile = open("tmpConfig.py","w")
    outFile.write("import FWCore.ParameterSet.Config as cms\n")
    outFile.write(process.dumpPython())
    outFile.close()

    # building cmsBatch command
    
    print relvalID

    dataset = relval.dataset
    if tier!=None:
        dataset += '/' + tier
        
    outDir = '.' + relval.dataset
    castorOutDir = castorBaseDir + '/' + relval.dataset

    #if tier!=None:
    #    castorOutDir += '/' + tier
    #    outDir += '/' + tier

    print 'local  output: ', outDir
    print 'castor output:', castorOutDir
    
    # output directory creation will be handled by cmsBatch
    # os.system( 'mkdir -p ' + outDir )
    
    cmsBatch = "cmsBatch.py 1 tmpConfig.py -r %s -o %s -b '%s' " % (castorOutDir,outDir,options.batch)
    if negate:
        cmsBatch += ' -n'
    print cmsBatch
    os.system( cmsBatch )
    addToDatasets( dataset ) 

    return (outDir, castorOutDir)


if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    
    parser.usage = "relval.py <cfg.py> <relvalList.py>\nRuns a cfg on the batch, for a given set of RelVal datasets"
    parser.add_option("-n", "--negate", action="store_true",
                      dest="negate", default=False,
                      help="create jobs, but do nothing")
    parser.add_option("-t", "--tier", 
                      dest="tier",
                      help="Tier: extension you can give to specify you are doing a new production",
                      default=None)
    parser.add_option("-b", "--batch", 
                      dest="batch",
                      help="Batch command. Same as in cmsBatch.py",
                      default="bsub -q 1nh < batchScript.sh")
    
    import castorBaseDir
    
#    parser.add_option("-c", "--castorBaseDir", 
#                      dest="castorBaseDir",
#                      help="Base castor directory. Subdirectories will be created automatically for each prod",
#                      default=castorBaseDir.defaultCastorBaseDir)
    
    (options,args) = parser.parse_args()

    if len(args)!=2:
        parser.print_help()
        sys.exit(1)

    cfgFileName = args[0]
    relvalListFileName = args[1]
    castorBaseDir = castorBaseDir.myCastorBaseDir()
    
    if not os.path.isfile( cfgFileName ):
        print 'cfg file does not exist: ', cfgFileName
        sys.exit(1)
    if not os.path.isfile( relvalListFileName ):
        print 'relval list file does not exist: ', relvalListFileName
        sys.exit(1)
    
    handle = open(relvalListFileName, 'r')
    cfo = imp.load_source("pycfg", relvalListFileName, handle)
    relvals = cfo.relvals
    handle.close()

    # from myRelvalList import relvals
    # loading cfg in the current directory.
    # sys.path.append('.')
    # from patTuple_PATandPF2PAT_RecoJets_cfg import process

    handle = open( cfgFileName, 'r')
    cfo = imp.load_source("pycfg", cfgFileName, handle)
    process = cfo.process
    handle.close()

    locals = []
    remotes = []
    myRelvals = []
    for relval in relvals.list:
        (local,remote) = processRelVal(relval, cfgFileName, process, options.negate, options.tier)
        locals.append( local )
        remotes.append( remote ) 
        myRelvals.append( relval )

    print 
    print 'SUMMARY'
    print '-------'
    i = 0
    for relval in myRelvals:
        print ''
        print 'output of relval: ', relval, ' will appear in:'
        print '----------------------------------------------'
        print 'local  : '
        print locals[i]+'/*'
        print 'remote : '
        print remotes[i]
        i = i+1


