import os
from fnmatch import fnmatch
import copy

from ROOT import TFile, TH1F, TPaveText

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.RootTools.fwlite.Weight import Weight
from CMGTools.RootTools.fwlite.Weight import printWeights
from CMGTools.RootTools.Style import *

class H2TauTauDataMC( AnalysisDataMC ):

    keeper = {}
    HINDEX = 0

    def __init__(self, varName, directory, selComps, weights,
                 bins = None, xmin = None, xmax=None, cut = '',
                 weight='weight', embed = False, shift=None, treeName=None):
        '''Data/MC plotter adapted to the H->tau tau analysis.
        The plotter takes a collection of trees in input. The trees are found according
        to the dictionary of selected components selComps.
        The global weighting information for each component is read from the weights dictionary.
        The weight parameter is the name of an event weight variable that can be found in the tree.
        The default is "weight" (full event weight computed at python analysis stage),
        but you can build up the weight string you want before calling this constructor.
        To do an unweighted plot, choose weight="1" (the string, not the number).        
        '''
        if treeName is None:
            treeName = 'H2TauTauTreeProducerTauMu'
        self.treeName = treeName
        self.selComps = selComps
        self.varName = varName
        self.shift = shift
        self.cut = cut
        self.eventWeight = weight
        # import pdb; pdb.set_trace()
        self.bins = bins
        self.xmin = xmin
        self.xmax = xmax
        # self.keeper = []
        
        super(H2TauTauDataMC, self).__init__(varName, directory, weights)

        self.legendBorders = 0.651, 0.463, 0.895, 0.892

        self.dataComponents = [ key for key, value in selComps.iteritems() \
                                if value.isData is True ]
        groupDataName = 'Data'

        self.groupDataComponents( self.dataComponents, groupDataName)
        
        if embed: 
            self.setupEmbedding( embed )
        else:
            self.removeEmbeddedSamples()


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
        weight = self.eventWeight
## to do the following, modify the eventWeight before giving it to self
##         if not comp.isData:
##             weight = ' * '.join( [self.eventWeight, recEffId.weight(), recEffIso.weight()])
        if tree == None:
            raise ValueError('tree does not exist for component '+compName)
        var = varName
        if not comp.isData and self.shift:
            if self.shift == 'Up':
                if varName == 'visMass' or varName == 'svfitMass':
                    print 'Shifting visMass and svfitMass by sqrt(1.03) for', comp.name
                    var = varName + '* sqrt(1.03)'
            elif self.shift == 'Down':
                if varName == 'visMass' or varName == 'svfitMass':
                    print 'Shifting visMass and svfitMass by sqrt(0.97) for', comp.name
                    var = varName + '* sqrt(0.97)'
            else:
                raise ValueError( self.shift + ' is not recognized. Use None, "Up" or "Down".')
            
        tree.Project( histName, var, '{weight}*({cut})'.format(cut=cut,
                                                               weight=weight) )
        hist.SetStats(0)
        componentName = compName
        legendLine = self._GetHistPref(compName)['legend']
        if legendLine is None:
            legendLine = compName
        self.AddHistogram( componentName, hist, layer, legendLine)
        self.Hist(componentName).realName = comp.realName
        if comp.isData:
            self.Hist(componentName).stack = False


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
            
            if compName == 'Ztt':
                self._BuildHistogram(tree, comp, compName, self.varName,
                                     self.cut + ' && isFake==0', layer)
                fakeCompName = 'Ztt_ZL'
                self._BuildHistogram(tree, comp, fakeCompName, self.varName,
                                     self.cut + ' && isFake==1', layer)
                self.Hist(fakeCompName).realName =  comp.realName + '_ZL'
                self.weights[fakeCompName] = self.weights[compName]
                fakeCompName = 'Ztt_ZJ'
                self._BuildHistogram(tree, comp, fakeCompName, self.varName,
                                     self.cut + ' && isFake==2', layer)
                self.Hist(fakeCompName).realName =  comp.realName + '_ZJ'
                self.weights[fakeCompName] = self.weights[compName]

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
                

    def setupEmbedding(self, doEmbedding ):
        name = 'Ztt'
        try:
            dyHist = self.Hist(name)
        except KeyError:
            return
        if len(self.selComps)== 1:
            return
        newName = name
        embed = None
        embedFactor = None
        for comp in self.selComps.values():
            if not comp.isEmbed:
                continue
            embedHistName = comp.name
            if embedFactor is None:
                # import pdb; pdb.set_trace()
                embedFactor = comp.embedFactor
            elif embedFactor != comp.embedFactor:
                raise ValueError('All embedded samples should have the same scale factor')
            embedHist = self.Hist( embedHistName )
            embedHist.stack = False
            embedHist.on = False
            if doEmbedding:
                if embed is None:
                    embed = copy.deepcopy( embedHist )
                    embed.name = 'Ztt'
                    embed.legendLine = 'Ztt'
                    embed.on = True
                    # self.AddHistogram(newName, embed.weighted, 3.5)
                    self.Replace('Ztt', embed)
                    self.Hist(newName).stack = True
                else:
                    self.Hist(newName).Add(embedHist)
        if doEmbedding:
            print 'EMBEDDING: scale factor = ', embedFactor
            self.Hist(newName).Scale( embedFactor * self.weights['Ztt'].GetWeight() ) 
            self._ApplyPrefs()


    def groupDataComponents( self, dataComponents, name ):
        '''Groups all data components into a single component with name <name>.

        The resulting histogram is the sum of all data histograms.
        The resulting integrated luminosity is used to scale all the
        MC components.
        '''        
        self.intLumi = 0
        data = None
        for component in dataComponents:
            hist = self.Hist(component)
            hist.stack = False
            hist.on = False
            self.intLumi += self.weights[component].intLumi
            if data is None:
                # keep first histogram
                data = copy.deepcopy( hist )
                self.AddHistogram(name, data.weighted, 10000, 'Observed')
                self.Hist(name).stack = False
                continue
            # other data histograms added to the first one...
            # ... and removed from the stack
            self.Hist(name).Add( hist )
        self._ApplyWeights()
        self._ApplyPrefs()
        

    def _InitPrefs(self):
        '''Definine preferences for each component'''
        self.histPref = {}
        self.histPref['Data'] = {'style':sData, 'layer':2999, 'legend':'Observed'}
        self.histPref['data_*'] = {'style':sBlack, 'layer':2002, 'legend':None}
        self.histPref['Ztt'] = {'style':sHTT_DYJets, 'layer':4, 'legend':'Z#rightarrow#tau#tau'}
        self.histPref['embed_*'] = {'style':sViolet, 'layer':4.1, 'legend':None}
        self.histPref['TTJets*'] = {'style':sHTT_TTJets, 'layer':1, 'legend':'t#bar{t}'} 
        self.histPref['T*tW*'] = {'style':sHTT_TTJets, 'layer':1, 'legend':'t#bar{t}'} 
        self.histPref['WW'] = {'style':sBlue, 'layer':0.9, 'legend':None} 
        self.histPref['WW*'] = {'style':sBlue, 'layer':0.9, 'legend':None} 
        self.histPref['WZ'] = {'style':sRed, 'layer':0.8, 'legend':None} 
        self.histPref['WZ*'] = {'style':sRed, 'layer':0.8, 'legend':None} 
        self.histPref['ZZ'] = {'style':sGreen, 'layer':0.7, 'legend':None} 
        self.histPref['ZZ*'] = {'style':sGreen, 'layer':0.7, 'legend':None} 
        self.histPref['QCD'] = {'style':sHTT_QCD, 'layer':2, 'legend':None}
        self.histPref['WJets*'] = {'style':sHTT_WJets, 'layer':3, 'legend':None}  
        self.histPref['Ztt_ZJ'] = {'style':sGreen, 'layer':3.1, 'legend':None}
        self.histPref['Ztt_ZL'] = {'style':sBlue, 'layer':3.2, 'legend':None}
        self.histPref['Higgs*'] = {'style':sHTT_Higgs, 'layer':1001, 'legend':None}


