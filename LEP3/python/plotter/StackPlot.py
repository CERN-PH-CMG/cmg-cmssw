import os
from fnmatch import fnmatch
import copy

from ROOT import TFile, TH1F, TPaveText

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.RootTools.DataMC.Stack import Stack
from CMGTools.RootTools.fwlite.Weight import Weight
from CMGTools.RootTools.fwlite.Weight import printWeights
from CMGTools.LEP3.plotter.prepareComponents import prepareComponents
from CMGTools.LEP3.plotter.Style import *


Stack.STAT_ERRORS = False

class StackPlot( AnalysisDataMC ):

    keeper = {}
    HINDEX = 0

    def __init__(self, varName, directory, selComps, weights,
                 bins = None, xmin = None, xmax=None, cut = ''):
        '''Data/MC plotter adapted to the LEP3 analysis.
        The plotter takes a collection of trees in input. The trees are found according
        to the dictionary of selected components selComps.
        The global weighting information for each component is read from the weights dictionary.
        '''
        self.treeName = 'SixJetTreeProducer'
        self.selComps = selComps
        self.varName = varName
        self.cut = cut
        self.bins = bins
        self.xmin = xmin
        self.xmax = xmax
        # self.keeper = []
        
        super(StackPlot, self).__init__(varName, directory, weights)

        self.legendBorders = 0.651, 0.463, 0.895, 0.892



    def _BuildHistogram(self, tree, comp, compName, varName, cut, layer ):
        '''Build one histogram, for a given component'''

        if not hasattr( comp, 'tree'):
            comp.tree = tree
                    
        histName = '_'.join( [compName, self.varName] )

        hist = None
        if self.xmin is not None and self.xmax is not None:
            hist = TH1F( histName, '', self.bins, self.xmin, self.xmax )
        else:
            hist = TH1F( histName, '', len(self.bins)-1, self.bins )
        hist.Sumw2()
        if tree == None:
            raise ValueError('tree does not exist for component '+compName)
        var = varName
            
        tree.Project( histName, var, '{cut}'.format(cut=cut) )
        hist.SetStats(0)
        componentName = compName
        legendLine = compName
        self.AddHistogram( componentName, hist, layer, legendLine)
        self.Hist(componentName).realName = comp.realName


    def _ReadHistograms(self, directory):
        '''Build histograms for all components.'''
        for layer, (compName, comp) in enumerate( sorted(self.selComps.iteritems()) ) : 
            fileName = '/'.join([ directory,
                                  comp.dir,
                                  self.treeName,
                                  '{treeName}_tree.root'.format(treeName=self.treeName)] )
            file = self.__class__.keeper[ fileName + str(self.__class__.HINDEX) ] = TFile(fileName) 
            self.__class__.HINDEX+=1
            tree = file.Get( self.treeName )
            self._BuildHistogram(tree, comp, compName, self.varName,
                                 self.cut, layer )     
        self._ApplyWeights()
        self._ApplyPrefs()
        


    def _InitPrefs(self):
        '''Definine preferences for each component'''
        self.histPref = {}
        self.histPref['WW'] = {'style':sWW, 'layer':3} 
        self.histPref['ZZ'] = {'style':sZZ, 'layer':2} 
        self.histPref['QQBAR'] = {'style':sQQ, 'layer':1}
        self.histPref['HZVIS'] = {'style':sHiggs, 'layer':1001}



if __name__ == '__main__':
    import imp
    import sys
    from CMGTools.RootTools.PyRoot import * 
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = '%prog <anaDir> <cfgFile>'
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)
    
    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights = prepareComponents(anaDir, cfg.config)

    stack = StackPlot( 'H_m', anaDir, selComps, weights, 50, 0, 200, '1')
    stack.DrawStack('HIST')
