#!/usr/bin/env python

import ROOT as rt
import sys, copy
from DataFormats.FWLite import Events, Handle, Lumis, Runs
from CMGTools.RootTools import RootFile, RootTools

def getFiles(datasets, user, pattern):
    
    from CMGTools.Production.datasetToSource import datasetToSource

    files = []
    for d in datasets:
        ds = datasetToSource(
                             user,
                             d,
                             pattern
                             )
        files.extend(ds.fileNames)

    return ['root://eoscms//eos/cms%s' % f for f in files]

if __name__ == '__main__':
    
    # https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#VarParsing_Example
    from FWCore.ParameterSet.VarParsing import VarParsing
    options = VarParsing ('python')
    options.outputFile = 'mcWeights.root'
    options.register ('datasetName',
                  None,
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,          # string, int, or float
                  "A directory to read root files from")
    options.register ('maxFiles',
                  -1, # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.int,          # string, int, or float
                  "The maximum number of files to read")
    
    options.parseArguments()
    files = getFiles(
                      [options.datasetName],
                      'wreece',
                      'susy_tree_CMG_[0-9]+.root'

                     )
    if options.maxFiles > 0:
        options.inputFiles = files[0:options.maxFiles]
    else:
        options.inputFiles = files    

    lumis = Lumis(options)
    
    totalSimInput = 0
    totalSimOutput = 0
    totalPatInput = 0
    totalPatOutput = 0

    simFilterInfoH = Handle('GenFilterInfo')
    patFilterInfoH = Handle('GenFilterInfo')
    
    weights = []
    for lumi in lumis:
        lumi.getByLabel(('genFilterEfficiencyProducer','','SIM'),simFilterInfoH)
        lumi.getByLabel(('preSelFilterEfficiencyProducer','','PAT'),patFilterInfoH)

        totalSimInput += simFilterInfoH.product().numEventsTried()
        totalSimOutput += simFilterInfoH.product().numEventsPassed()
        
        totalPatInput += patFilterInfoH.product().numEventsTried()
        totalPatOutput += patFilterInfoH.product().numEventsPassed()
        
        fe = simFilterInfoH.product().filterEfficiency()
        sc = patFilterInfoH.product().numEventsTried()/(1.*simFilterInfoH.product().numEventsPassed())
        
        w = fe/(sc*simFilterInfoH.product().numEventsTried())
        weights.append(w)
        print w
    print 'Mean',sum(weights)/(1.*len(weights))
    
    genFilterEfficiency = totalSimInput/(1.*totalSimOutput)
    scalingFraction = totalPatInput/(1.*totalSimOutput)
    print options.datasetName
    print totalSimInput,totalSimOutput,totalPatInput,totalPatOutput,genFilterEfficiency,scalingFraction

