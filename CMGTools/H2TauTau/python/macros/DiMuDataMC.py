import os
import imp
from fnmatch import fnmatch

from ROOT import kPink

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.RootTools.Style import formatPad
from CMGTools.H2TauTau.macros.Weight import Weight
from CMGTools.H2TauTau.macros.Weight import printWeights
from CMGTools.H2TauTau.macros.HistogramSet import histogramSet


class H2TauTauDataMC( AnalysisDataMC ):

    def __init__(self, histName, directory, selComps,
                 filePattern, weights, embed = True ):
        '''Constructor

        histName  : histogram to be plotted
        directory : analysis directory containing subdirectories for each component
        selComps : selected components (among the component subdirectories)
        filePattern : pattern to find a single root file in each component subdirectory
                      from which the histogram will be obtained.
        weights   : weight dictionary
        groupDataName : all data components will be grouped into a single component
                        with this name.
        '''
        self.filePattern = filePattern
        self.selComps = selComps
        super(H2TauTauDataMC, self).__init__(histName, directory, weights)
        # if histName.endswith('mT') or histName.find('mass')>-1:
        offsetx = 0.55
        offsety = 0.1
        self.legendBorders = 0.13+offsetx,0.66+offsety,0.44+offsetx,0.89+offsety

        self.dataComponents = [ key for key, value in selComps.iteritems() \
                                if value.isData is True ]
        groupDataName = 'Data'
        self.groupDataComponents( self.dataComponents, groupDataName)
        self.setupEmbedding( self.dataComponents, embed )

    def setupEmbedding(self, dataComponents, doEmbedding ):

        name = 'DYJets'
        dyHist = self.Hist(name)
        dyYield = dyHist.Yield()
        
        # get the embedded samples corresponding to the data components
        # merge them into a single embedded component
        embed = None
        for dataName in dataComponents:
            if dataName.find('data_')==-1:
                raise ValueError('the directory names for the data components should start by data...')
            embedHistName = dataName.replace('data_', 'embed_')
            try:
                embedHist = self.Hist( embedHistName )
                embedHist.stack = False
                embedHist.on = False
                if doEmbedding:
                    if embed is None:
                        embed = copy.deepcopy( embedHist )
                        self.AddHistogram(name, embed.weighted, 3.5)
                        self.Hist(name).stack = True
                        continue
                    self.Hist(name).Add(embedHist)
            except:
                pass
        # dyYield = dyHist.Yield()
        # print '2', dyYield
        if doEmbedding:
            embedYield = self.Hist(name).Yield()
            self.Hist(name).Scale( dyYield / embedYield ) 
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
        for component, weight in self.weights.iteritems():
            if component not in dataComponents:
                self.weights[component].intLumi = self.intLumi
        self._ApplyWeights()
        self._ApplyPrefs()
        

    def _InitPrefs(self):
        '''Definine preferences for each component'''
        self.histPref = {}
        self.histPref['Data'] = {'style':sBlack, 'layer':-99}
        self.histPref['data_May10ReReco_v1'] = {'style':sViolet, 'layer':-1000}
        self.histPref['data_PromptReco_v4'] = {'style':sBlue, 'layer':-1000}
        self.histPref['data_PromptReco_v6'] = {'style':sRed, 'layer':-1100}
        self.histPref['data_03Oct2011'] = {'style':sYellow, 'layer':-1105}
        self.histPref['data_05Aug2011_v1'] = {'style':sBlack, 'layer':-1150}
        self.histPref['embed_May10ReReco_v1'] = {'style':sViolet, 'layer':-1000}
        self.histPref['embed_PromptReco_v4'] = {'style':sBlue, 'layer':-1000}
        self.histPref['embed_PromptReco_v6'] = {'style':sRed, 'layer':-1100}
        self.histPref['embed_03Oct2011'] = {'style':sYellow, 'layer':-1105}
        self.histPref['embed_05Aug2011_v1'] = {'style':sBlack, 'layer':-1150}
        self.histPref['dMay10ReReco_v1'] = {'style':sGreen, 'layer':-1200}
        self.histPref['TTJets'] = {'style':sBlue, 'layer':1} 
        self.histPref['WJets'] = {'style':sRed, 'layer':2}  
        self.histPref['DYJets'] = {'style':sYellow, 'layer':3}
        self.histPref['DYJets_Fakes'] = {'style':sBlack, 'layer':2.5}


    def _GetFileNames(self, directory):
        '''Overloading a function from the base classes.
        Tells this class how to find the root files in the analysis directory'''
        fileNames = []
        for root,dirs,files in os.walk(directory, followlinks=True):
            if root is directory:
                continue
            if os.path.basename(root) not in self.selComps:
                print root,'is not selected'
                continue
            matchingFiles = [file for file in files if fnmatch(file, self.filePattern)]
            if len(matchingFiles)!=1:
                raise ValueError('files matching %s in %s: %s. Need to match only 1 file.'
                                 % (self.filePattern,
                                    root,
                                    matchingFiles))
            else:
                fileNames.append( '/'.join([root, matchingFiles[0]])) 
        return fileNames

    def _ComponentName(self, name):
        # print name 
        return name.split('/')[-2]

    def __str__(self):
        id = 'H2TauTauDataMC : histo = %s, dir = %s, file = %s' % ( self.histName,
                                                                    self.directory,
                                                                    self.filePattern )
        return '\n'.join( [id, super(H2TauTauDataMC, self).__str__()] )
        


def prepareComponents(dir, config):

    # list of components from configuration file
    selComps = dict( [ (comp.name, comp) for comp in config.components ])

    dySplit = False
    if 'DYJets_Fakes' in os.listdir( dir ):
        dySplit = True
    if dySplit:
        print 'adding DYJets_Fakes'
        dyJetsFakeComp = copy.copy( selComps['DYJets'] )
        dyJetsFakeComp.name = 'DYJets_Fakes'
        selComps[dyJetsFakeComp.name] = dyJetsFakeComp
        
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in selComps.values() ] )

    return selComps, weights
    

def savePlot(name):
    if gPad is None:
        print 'no active canvas'
        return
    fileName = '%s/%s' % (anaDir, name)
    print 'pad', gPad.GetName(), 'saved to', fileName    
    gPad.SaveAs( fileName )   




if __name__ == '__main__':

    import copy
    from optparse import OptionParser
    from CMGTools.RootTools.RootInit import *
    from CMGTools.H2TauTau.macros.AnalysisConfig import AnalysisConfig

    parser = OptionParser()
    parser.usage = '''
    %prog <anaDir> <cfgFile>
    '''
    parser.add_option("-H", "--histlist", 
                      dest="histlist", 
                      help="histogram list",
                      default=None)
    parser.add_option("-G", "--histgroup", 
                      dest="histgroup", 
                      help="histogram group",
                      default=None)
    parser.add_option("-R", "--rebin", 
                      dest="rebin", 
                      help="rebinning factor",
                      default=None)
    
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)

    dataName = 'Data'

    anaDir = args[0]
    hists = histogramSet( options )
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights = prepareComponents(anaDir, cfg.config)
    
    plots = {}
    plotsR = {}
    canvases = []

    xmin = None
    xmax = None


    for hist in sorted(hists):
        print 'Processing: ',hist,dataName, anaDir

        plot =  H2TauTauDataMC(hist, anaDir,
                               selComps,
                               'hists.root', weights,
                               embed=False)
        if options.rebin is not None:
            rebin = int( options.rebin )
            osQCD.Rebin( rebin )
            ssQCD.Rebin( rebin )
        canvas = TCanvas( hist, hist, 750, 700)
        canvasRatio = TCanvas( hist + '_ratio', hist + '_ratio', 750, 700)
        canvases.append( canvas )  
        canvases.append( canvasRatio )  
        formatPad( canvas )
        formatPad( canvasRatio )
        plots[hist] = plot
        histName = os.path.basename( hist )
        canvas.cd()
        #         if histName.find('_iso')>-1:
        #            osQCD.Rebin(2)
        #            ssQCD.Rebin(2)
        #            xmin = 0
        #            xmax = 0.2
        plot.DrawStack('HIST', xmin=xmin, xmax=xmax )
        savePlot( histName + '_lin.png') 
        gPad.SetLogy()
        savePlot( histName + '_log.png') 
        gPad.SetLogy(False)
        gPad.Update()
        canvasRatio.cd()
        plotRatio = copy.deepcopy(plot)
        plotsR[hist] = plotRatio
        plotRatio.DrawRatioStack('HIST', xmin=xmin, xmax=xmax, ymin=0.001, ymax=2)
        savePlot( histName + '_ratio.png') 


        
    
