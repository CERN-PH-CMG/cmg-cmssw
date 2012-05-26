import os
from fnmatch import fnmatch
import copy

from ROOT import TFile, TH1F

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.RootTools.fwlite.Weight import Weight
from CMGTools.RootTools.fwlite.Weight import printWeights
from CMGTools.RootTools.Style import *


class HToZZDataMC( AnalysisDataMC ):

    def __init__(self, varName, directory, selComps, weights,
                 nbins = 50, xmin = 0, xmax=200, cut = '',
                 weight='weight'):
        '''Data/MC plotter adapted to the H->tau tau analysis.
        The plotter takes a collection of trees in input. The trees are found according
        to the dictionary of selected components selComps.
        The weighting information for each component is read from the weights dictionary.
        The weight parameter is the name of an event weight variable that can be found in the tree.
        The default is "weight" (full event weight computed at python analysis stage).
        To do an unweighted plot, choose weight="1" (the string, not the number).
        
        To do:
        '''
        self.selComps = selComps
        self.varName = varName
        self.cut = cut
        self.eventWeight = weight
        # import pdb; pdb.set_trace()
        self.nbins = nbins
        self.xmin = xmin
        self.xmax = xmax
        self.keeper = []
        
        super(HToZZDataMC, self).__init__(varName, directory, weights)
        offsetx = 0.55
        offsety = 0.1
        self.legendBorders = 0.13+offsetx,0.66+offsety,0.44+offsetx,0.89+offsety

        self.dataComponents = [ key for key, value in selComps.iteritems() \
                                if value.isData is True ]
        groupDataName = 'Data'

        self.groupDataComponents( self.dataComponents, groupDataName)

    def _BuildHistogram(self, tree, comp, compName, varName, cut, layer ):
        '''Build one histogram, for a given component'''
        histName = '_'.join( [compName, self.varName] )
        hist = TH1F( histName, histName, self.nbins, self.xmin, self.xmax )
        tree.Project( histName, varName, '{weight}*({cut})'.format(cut=cut,
                                                                   weight=self.eventWeight) )
        hist.SetStats(0)
        hist.Sumw2()
        componentName = compName
        legendLine = compName
        self.AddHistogram( componentName, hist, layer, legendLine)
        if comp.isData:
            self.Hist(componentName).stack = False
        self.Hist(componentName).tree = tree

    def _ReadHistograms(self, directory):
        '''Build histograms for all components.'''
        for layer, (compName, comp) in enumerate( self.selComps.iteritems() ) : 
            fileName = '/'.join([ directory,
                                  compName,
                                  'FourLeptonTreeProducer_MuMuFourLeptonAnalyzer',
                                  'FourLeptonTreeProducer_MuMuFourLeptonAnalyzer_tree.root'])
            file = TFile(fileName)
            self.keeper.append( file )
            tree = file.Get('FourLeptonTreeProducer_MuMuFourLeptonAnalyzer')
            
##             if compName == 'DYJets':
##                 self._BuildHistogram(tree, comp, compName, self.varName,
##                                      self.cut + ' && isFake==0', layer)
##                 fakeCompName = 'DYJets_Fakes'
##                 self._BuildHistogram(tree, comp, fakeCompName, self.varName,
##                                      self.cut + ' && isFake', layer)
##                 self.weights[fakeCompName] = self.weights[compName]
##             else:
            self._BuildHistogram(tree, comp, compName, self.varName,
                                 self.cut, layer )     

        self._ApplyWeights()
        self._ApplyPrefs()
   
    def groupDataComponents( self, dataComponents, name ):
        '''Groups all data components into a single component with name <name>.

        The resulting histogram is the sum of all data histograms.
        The resulting integrated luminosity is used to scale all the
        MC components.
        '''
        
        self.intLumi = 0
        # self.dataComponents = dataComponents
        data = None
        for component in dataComponents:
            # print component
            hist = self.Hist(component)
            hist.stack = False
            hist.on = False
            self.intLumi += self.weights[component].intLumi
            if data is None:
                # keep first histogram
                data = copy.deepcopy( hist )
                self.AddHistogram(name, data.weighted, -10000)
                self.Hist(name).stack = False
                continue
            # other data histograms added to the first one...
            # ... and removed from the stack
            self.Hist(name).Add( hist )
            # compute integrated luminosity for all data samples
        # print intLumi
        # set lumi for all MC samples:
        if self.intLumi==0:
            self.intLumi = 4700
        for component, weight in self.weights.iteritems():
            if component not in dataComponents:
                self.weights[component].intLumi = self.intLumi
        self._ApplyWeights()
        self._ApplyPrefs()
        

    def _InitPrefs(self):
        '''Definine preferences for each component'''
        self.histPref = {}
        self.histPref['Hig120GluGlu'] = {'style':sBlack, 'layer':0.1}
        self.histPref['Data'] = {'style':sBlack, 'layer':-99}
        self.histPref['DYJets'] = {'style':sYellow, 'layer':99
                                  }
        layerZZ = 5
        self.histPref['ZZ'] = {'style':sRed, 'layer':layerZZ} 
        self.histPref['ZZTo4tau'] = {'style':sGreen, 'layer':layerZZ}
        self.histPref['ZZTo4mu'] = {'style':sRed, 'layer':layerZZ}
        self.histPref['ZZTo2e2tau'] = {'style':sRed, 'layer':layerZZ}
        self.histPref['ZZTo2e2mu'] = {'style':sYellow, 'layer':layerZZ}
        self.histPref['ZZTo4e'] = {'style':sBlue, 'layer':layerZZ}
        self.histPref['ZZTo2mu2tau'] = {'style':sViolet, 'layer':layerZZ}
