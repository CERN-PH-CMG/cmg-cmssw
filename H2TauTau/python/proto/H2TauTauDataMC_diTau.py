import os
from fnmatch import fnmatch
import copy

from ROOT import TFile, TH1F

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.RootTools.fwlite.Weight import Weight
from CMGTools.RootTools.fwlite.Weight import printWeights
from CMGTools.RootTools.Style import *
from ROOT import kPink, kOrange, kViolet

sOrange = Style(lineColor=1, markerColor=kOrange, fillColor=kOrange)
sViolet = Style(lineColor=1, markerColor=kViolet, fillColor=kViolet)

sRedLine = Style(lineColor=2, markerColor=2, fillStyle=0)
sBlueLine = Style(lineColor=4, markerColor=4, fillStyle=0)

class H2TauTauDataMC( AnalysisDataMC ):

    def __init__(self, varName, directory, selComps, weights,
                 nbins = 50, xmin = 0, xmax=200, cut = '',
                 weight='weight', embed = False, photon = False, electron = True):
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
	self.photon = photon
	self.electron = electron
        self.keeper = []
        
        super(H2TauTauDataMC, self).__init__(varName, directory, weights)
        offsetx = 0.55
        offsety = 0.1
        self.legendBorders = 0.13+offsetx,0.56+offsety,0.44+offsetx,0.89+offsety

        self.dataComponents = [ key for key, value in selComps.iteritems() \
                                if value.isData is True ]
        if len(self.dataComponents)==0:
	    self.intLumi=1000
	    return

        groupDataName = 'Data'

        self.groupDataComponents( self.dataComponents, groupDataName)
        
##         if embed: 
##             self.setupEmbedding( self.dataComponents, embed )
##         else:
##             self.removeEmbeddedSamples()

    def _BuildHistogram(self, tree, comp, compName, varName, cut, layer ):
        '''Build one histogram, for a given component'''
        histName = '_'.join( [compName, self.varName] )
        hist = TH1F( histName, histName, self.nbins, self.xmin, self.xmax )
        tree.Project( histName, varName, '{weight}*({cut})'.format(cut=cut,
                                                                   weight=self.eventWeight) )
        hist.GetXaxis().SetTitle(varName)
        hist.SetTitle("")
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
                                  'H2TauTauTreeProducerTauTau',
                                  'H2TauTauTreeProducerTauTau_tree.root'])
            file = TFile(fileName)
            self.keeper.append( file )
            tree = file.Get('H2TauTauTreeProducerTauTau')
	    #print fileName, tree
            
            if compName == 'DYJets':
	        if self.photon:
		    phot="&& isPhoton==0"
	        elif self.electron:
		    phot="&& isElectron==0"
                else:
		    phot=""
                self._BuildHistogram(tree, comp, compName, self.varName,
                                     self.cut + ' && isFake==0'+phot, layer)
                fakeCompName = 'DYJets_Fakes'
                self._BuildHistogram(tree, comp, fakeCompName, self.varName,
                                     self.cut + ' && isFake', layer)
                self.weights[fakeCompName] = self.weights[compName]
		if self.photon:
                    photonCompName = 'DYJets_Photon'
                    self._BuildHistogram(tree, comp, photonCompName, self.varName,
                                         self.cut + ' && isPhoton', layer)
                    self.weights[photonCompName] = self.weights[compName]
		if self.electron:
                    electronCompName = 'DYJets_Electron'
                    self._BuildHistogram(tree, comp, electronCompName, self.varName,
                                         self.cut + ' && isElectron', layer)
                    self.weights[electronCompName] = self.weights[compName]
            elif compName == 'WJets':
                self._BuildHistogram(tree, comp, compName, self.varName,
                                     self.cut + ' && isFake==0', layer)
                fakeCompName = 'WJets_Fakes'
                self._BuildHistogram(tree, comp, fakeCompName, self.varName,
                                     self.cut + ' && isFake', layer)
                self.weights[fakeCompName] = self.weights[compName]
            else:
                self._BuildHistogram(tree, comp, compName, self.varName,
                                     self.cut, layer )     

        self._ApplyWeights()
        self._ApplyPrefs()
        

##     def removeEmbeddedSamples(self):
##         for compname in self.selComps:
##             if compname.startswith('embed_'):
##                 hist = self.Hist(compname)
##                 hist.stack = False
##                 hist.on = False
                

##     def setupEmbedding(self, dataComponents, doEmbedding ):

##         name = 'DYJets'
##         dyHist = self.Hist(name)
##         dyYield = dyHist.Yield()

##         newName = 'DYJets (emb)'
        
##         # get the embedded samples corresponding to the data components
##         # merge them into a single embedded component
##         embed = None
##         for dataName in dataComponents:
##             if dataName.find('data_')==-1:
##                 raise ValueError('the directory names for the data components should start by data...')
##             embedHistName = dataName.replace('data_', 'embed_')
##             embedHist = self.Hist( embedHistName )
##             embedHist.stack = False
##             embedHist.on = False
##             if doEmbedding:
##                 if embed is None:
##                     embed = copy.deepcopy( embedHist )
##                     self.AddHistogram(newName, embed.weighted, 3.5)
##                     self.Hist(newName).stack = True
##                     continue
##                 self.Hist(newName).Add(embedHist)
       
##         # dyYield = dyHist.Yield()
##         # print '2', dyYield
##         if doEmbedding:
##             print 'embedding is used'
##             embedYield = self.Hist(newName).Yield()
##             self.Hist(newName).Scale( dyYield / embedYield ) 
##             self._ApplyPrefs()
##             self.Hist(name).on = False

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
        for component, weight in self.weights.iteritems():
            if component not in dataComponents:
                self.weights[component].intLumi = self.intLumi
        self._ApplyWeights()
        self._ApplyPrefs()
        

    def _InitPrefs(self):
        '''Definine preferences for each component'''
        self.histPref = {}
        self.histPref['Data']                          = {'style':sBlack,    'layer':-99}
        self.histPref['data_Run2012A_PromptReco_v1']   = {'style':sBlue,     'layer':-1000}
        self.histPref['data_Run2012B_PromptReco_v1']   = {'style':sRed,      'layer':-1100}
        self.histPref['data_Run2011A_May10ReReco_v1']  = {'style':sViolet,   'layer':-1000}
        self.histPref['data_Run2011A_PromptReco_v4']   = {'style':sBlue,     'layer':-1000}
        self.histPref['data_Run2011A_PromptReco_v6']   = {'style':sRed,      'layer':-1100}
        self.histPref['data_Run2011A_03Oct2011_v1']    = {'style':sYellow,   'layer':-1105}
        self.histPref['data_Run2011A_05Aug2011_v1']    = {'style':sBlack,    'layer':-1150}
        self.histPref['data_Run2011B_PromptReco_v1']   = {'style':sViolet,   'layer':-1200}
        self.histPref['embed_Run2011A_May10ReReco_v1'] = {'style':sViolet,   'layer':-1000}
        self.histPref['embed_Run2011A_PromptReco_v4']  = {'style':sBlue,     'layer':-1000}
        self.histPref['embed_Run2011A_PromptReco_v6']  = {'style':sRed,      'layer':-1100}
        self.histPref['embed_Run2011A_03Oct2011_v1']   = {'style':sYellow,   'layer':-1105}
        self.histPref['embed_Run2011A_05Aug2011_v1']   = {'style':sBlack,    'layer':-1150}
        self.histPref['embed_Run2011B_PromptReco_v1']  = {'style':sViolet,   'layer':-1200}
        self.histPref['TTJets']                        = {'style':sRed,      'layer':1} 
        self.histPref['WJets']                         = {'style':sBlue,     'layer':2}  
        self.histPref['WJets_Fakes']                   = {'style':sViolet,   'layer':1.5}  
        self.histPref['WW']                            = {'style':sBlue,     'layer':0.93}  
        self.histPref['WZ']                            = {'style':sBlue,     'layer':0.92}  
        self.histPref['ZZ']                            = {'style':sBlue,     'layer':0.91}  
        self.histPref['DYJets']                        = {'style':sYellow,   'layer':3}
        self.histPref['DYJets (emb)']                  = {'style':sYellow,   'layer':3}
        self.histPref['DYJets_Photon']                 = {'style':sOrange,   'layer':2.7}
        self.histPref['DYJets_Electron']               = {'style':sRed,      'layer':2.6}
        self.histPref['DYJets_Fakes']                  = {'style':sGreen,    'layer':2.5}
        self.histPref['QCD15']                         = {'style':sBlack,    'layer':5.4}
        self.histPref['QCD30']                         = {'style':sBlack,    'layer':5.3}
        self.histPref['QCD50']                         = {'style':sBlack,    'layer':5.2}
        self.histPref['QCD80']                         = {'style':sBlack,    'layer':5.1}
        self.histPref['Higgsgg110']                    = {'style':sRedLine,  'layer':6.1}
        self.histPref['Higgsgg115']                    = {'style':sRedLine,  'layer':6.1}
        self.histPref['Higgsgg120']                    = {'style':sRedLine,  'layer':6.1}
        self.histPref['Higgsgg125']                    = {'style':sRedLine,  'layer':6.1}
        self.histPref['Higgsgg130']                    = {'style':sRedLine,  'layer':6.1}
        self.histPref['Higgsgg135']                    = {'style':sRedLine,  'layer':6.1}
        self.histPref['Higgsgg140']                    = {'style':sRedLine,  'layer':6.1}
        self.histPref['Higgsgg145']                    = {'style':sRedLine,  'layer':6.1}
        self.histPref['HiggsVBF110']                   = {'style':sBlueLine, 'layer':6.2}
        self.histPref['HiggsVBF115']                   = {'style':sBlueLine, 'layer':6.2}
        self.histPref['HiggsVBF120']                   = {'style':sBlueLine, 'layer':6.2}
        self.histPref['HiggsVBF125']                   = {'style':sBlueLine, 'layer':6.2}
        self.histPref['HiggsVBF130']                   = {'style':sBlueLine, 'layer':6.2}
        self.histPref['HiggsVBF135']                   = {'style':sBlueLine, 'layer':6.2}
        self.histPref['HiggsVBF140']                   = {'style':sBlueLine, 'layer':6.2}
        self.histPref['HiggsVBF145']                   = {'style':sBlueLine, 'layer':6.2}
        