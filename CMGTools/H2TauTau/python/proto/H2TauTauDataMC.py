import os
from fnmatch import fnmatch
import copy

from ROOT import TFile, TH1F, TPaveText

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.RootTools.fwlite.Weight import Weight
from CMGTools.RootTools.fwlite.Weight import printWeights
from CMGTools.RootTools.Style import *

print 'WARNING, THIS FILE IS OBSOLETE, TAKE THE ONE IN plotter/ !'


class H2TauTauDataMC( AnalysisDataMC ):

    keeper = {}
    HINDEX = 0

    def __init__(self, varName, directory, selComps, weights,
                 nbins = 50, xmin = 0, xmax=200, cut = '',
                 weight='weight', embed = False):
        '''Data/MC plotter adapted to the H->tau tau analysis.
        The plotter takes a collection of trees in input. The trees are found according
        to the dictionary of selected components selComps.
        The weighting information for each component is read from the weights dictionary.
        The weight parameter is the name of an event weight variable that can be found in the tree.
        The default is "weight" (full event weight computed at python analysis stage).
        To do an unweighted plot, choose weight="1" (the string, not the number).
        
        To do:
        - need to revive embedded samples (when they are ready from Simone)
        '''
        self.selComps = selComps
        self.varName = varName
        self.cut = cut
        self.eventWeight = weight
        # import pdb; pdb.set_trace()
        self.nbins = nbins
        self.xmin = xmin
        self.xmax = xmax
        # self.keeper = []
        
        super(H2TauTauDataMC, self).__init__(varName, directory, weights)

        self.legendBorders = 0.711, 0.563, 0.895, 0.892

        self.dataComponents = [ key for key, value in selComps.iteritems() \
                                if value.isData is True ]
        groupDataName = 'Data'
        self.groupDataComponents( self.dataComponents, groupDataName)
        if embed: 
            self.setupEmbedding( self.dataComponents, embed )
        else:
            self.removeEmbeddedSamples()


    def _BuildHistogram(self, tree, comp, compName, varName, cut, layer ):
        '''Build one histogram, for a given component'''
        histName = '_'.join( [compName, self.varName] )
        hist = TH1F( histName, '', self.nbins, self.xmin, self.xmax )
        hist.Sumw2()
        tree.Project( histName, varName, '{weight}*({cut})'.format(cut=cut,
                                                                   weight=self.eventWeight) )
        hist.SetStats(0)
        # hist.Sumw2()

        componentName = compName
        legendLine = compName
        if legendLine=='Data':
            legendLine = 'Observed'
        self.AddHistogram( componentName, hist, layer, legendLine)
        if comp.isData:
            self.Hist(componentName).stack = False
        # self.Hist(componentName).tree = tree
        if not hasattr( comp, 'tree'):
            comp.tree = tree


    def _ReadHistograms(self, directory):
        '''Build histograms for all components.'''
        # self.__class__.keeper = {}
        for layer, (compName, comp) in enumerate( self.selComps.iteritems() ) : 
            fileName = '/'.join([ directory,
                                  compName,
                                  'H2TauTauTreeProducerTauMu',
                                  'H2TauTauTreeProducerTauMu_tree.root'])

            file = self.__class__.keeper[ fileName + str(self.__class__.HINDEX) ] = TFile(fileName) 
            self.__class__.HINDEX+=1

            # file = TFile(fileName)
            # self.keeper.append( file )
            tree = file.Get('H2TauTauTreeProducerTauMu')
            
            if compName == 'DYJets':
                self._BuildHistogram(tree, comp, compName, self.varName,
                                     self.cut + ' && isFake==0', layer)
                fakeCompName = 'DYJets_Fakes'
                self._BuildHistogram(tree, comp, fakeCompName, self.varName,
                                     self.cut + ' && isFake', layer)
                self.weights[fakeCompName] = self.weights[compName]
                # grouping fakes and WJets into EWK
            else:
                self._BuildHistogram(tree, comp, compName, self.varName,
                                     self.cut, layer )     

        self._ApplyWeights()
        self._ApplyPrefs()
        

    def removeEmbeddedSamples(self):
        for compname in self.selComps:
            if compname.startswith('embed_'):
                hist = self.Hist(compname)
                hist.stack = False
                hist.on = False
                

    def setupEmbedding(self, dataComponents, doEmbedding ):

        name = 'DYJets'
        dyHist = self.Hist(name)
        dyYield = dyHist.Yield()

        newName = 'DYJets (emb)'
        
        # get the embedded samples corresponding to the data components
        # merge them into a single embedded component
        embed = None
        for dataName in dataComponents:
            if dataName.find('data_')==-1:
                raise ValueError('the directory names for the data components should start by data...')
            embedHistName = dataName.replace('data_', 'embed_')
            embedHist = self.Hist( embedHistName )
            embedHist.stack = False
            embedHist.on = False
            if doEmbedding:
                if embed is None:
                    embed = copy.deepcopy( embedHist )
                    self.AddHistogram(newName, embed.weighted, 3.5)
                    self.Hist(newName).stack = True
                    continue
                self.Hist(newName).Add(embedHist)
       
        # dyYield = dyHist.Yield()
        # print '2', dyYield
        if doEmbedding:
            print 'embedding is used'
            embedYield = self.Hist(newName).Yield()
            self.Hist(newName).Scale( dyYield / embedYield ) 
            self._ApplyPrefs()
            self.Hist(name).on = False

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
        if self.intLumi>0:
            for component, weight in self.weights.iteritems():
                if component not in dataComponents:
                    self.weights[component].intLumi = self.intLumi
        self._ApplyWeights()
        self._ApplyPrefs()
        

    def _InitPrefs(self):
        '''Definine preferences for each component'''
        self.histPref = {}
        self.histPref['Data'] = {'style':sData, 'layer':-99}
        self.histPref['data_Run2012B_194480_195016'] = {'style':sBlack, 'layer':-99}
        self.histPref['data_Run2012B_start_194479'] = {'style':sBlack, 'layer':-99}
        self.histPref['data_Run2012A'] = {'style':sBlack, 'layer':-99}
        self.histPref['data_Run2011A_May10ReReco_v1'] = {'style':sViolet, 'layer':-1000}
        self.histPref['data_Run2011A_PromptReco_v4'] = {'style':sBlue, 'layer':-1000}
        self.histPref['data_Run2011A_PromptReco_v6'] = {'style':sRed, 'layer':-1100}
        self.histPref['data_Run2011A_03Oct2011_v1'] = {'style':sYellow, 'layer':-1105}
        self.histPref['data_Run2011A_05Aug2011_v1'] = {'style':sBlack, 'layer':-1150}
        self.histPref['data_Run2011B_PromptReco_v1'] = {'style':sViolet, 'layer':-1200}
        self.histPref['embed_Run2011A_May10ReReco_v1'] = {'style':sViolet, 'layer':-1000}
        self.histPref['embed_Run2011A_PromptReco_v4'] = {'style':sBlue, 'layer':-1000}
        self.histPref['embed_Run2011A_PromptReco_v6'] = {'style':sRed, 'layer':-1100}
        self.histPref['embed_Run2011A_03Oct2011_v1'] = {'style':sYellow, 'layer':-1105}
        self.histPref['embed_Run2011A_05Aug2011_v1'] = {'style':sBlack, 'layer':-1150}
        self.histPref['embed_Run2011B_PromptReco_v1'] = {'style':sViolet, 'layer':-1200}
        self.histPref['dMay10ReReco_v1'] = {'style':sGreen, 'layer':-1200}
        self.histPref['QCD'] = {'style':sHTT_QCD, 'layer':1.5}
        self.histPref['TTJets'] = {'style':sHTT_TTJets, 'layer':1} 
        self.histPref['WJets'] = {'style':sHTT_WJets, 'layer':2}  
        self.histPref['DYJets'] = {'style':sHTT_DYJets, 'layer':3}
        self.histPref['DYJets (emb)'] = {'style':sYellow, 'layer':3}
        self.histPref['DYJets_Fakes'] = {'style':sBlack, 'layer':2.5}
        self.histPref['HiggsVBF110'] = {'style':sBlack, 'layer':4}
        self.histPref['HiggsVBF115'] = {'style':sBlack, 'layer':5}
        self.histPref['HiggsVBF120'] = {'style':sBlack, 'layer':6}
        self.histPref['HiggsVBF125'] = {'style':sBlack, 'layer':7}
        self.histPref['HiggsVBF130'] = {'style':sBlack, 'layer':8}
        self.histPref['HiggsVBF135'] = {'style':sBlack, 'layer':9}


