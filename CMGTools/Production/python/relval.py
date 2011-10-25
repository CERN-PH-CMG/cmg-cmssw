
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles

import sys,os,imp


import CMGTools.Production.castortools as castortools
from CMGTools.Production.relvalDefinition import *
from CMGTools.Production.addToDatasets import *

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
        
    outDir = '.' + dataset
    castorOutDir = castorBaseDir + '/' + dataset

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

