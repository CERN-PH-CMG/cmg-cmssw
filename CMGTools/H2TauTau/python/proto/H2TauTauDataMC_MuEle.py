import os
import imp
from fnmatch import fnmatch

from ROOT import kPink

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.RootTools.Style import formatPad
from CMGTools.RootTools.fwlite.Weight import Weight
from CMGTools.RootTools.fwlite.Weight import printWeights
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet


class H2TauTauDataMC( AnalysisDataMC ):

    def __init__(self, histName, directory, selComps,
                 filePattern, weights, embed = True ):
        '''Constructor

        histName  : histogram to be plotted
        directory : analysis directory containing subdirectories for each component
        selComps : selected components (among the component subdirectories).
                   this is a dictionary(component name:component object)
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
        if embed: 
            self.setupEmbedding( self.dataComponents, embed )
        else:
            self.removeEmbeddedSamples()

    def removeEmbeddedSamples(self):
        for compname in self.selComps:
            if compname.startswith('embed_'):
                try:
                    hist = self.Hist(compname)
                    hist.stack = False
                    hist.on = False
                except:
                    pass
                

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
        self.histPref['TTJets'] = {'style':sBlue, 'layer':1} 
        self.histPref['WJets'] = {'style':sRed, 'layer':2}  
        self.histPref['DYJets'] = {'style':sYellow, 'layer':3}
        self.histPref['DYJets (emb)'] = {'style':sYellow, 'layer':3}
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
                compName = root.split('/')[1]
                fileNames.append( (compName, '/'.join([root, matchingFiles[0]])))
        # print fileNames
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
    
    
def wJetScale( plot, dataName, min, max ):
    
    # WJets_data = data - DY - TTbar 
    wjet = copy.deepcopy(plot.Hist(dataName))
    wjet.Add(plot.Hist('DYJets'), -1)
    try:
        dyJetsFakes = plot.Hist('DYJets_Fakes')
        wjet.Add(plot.Hist('DYJets_Fakes'), -1)
    except:
        pass
    wjet.Add(plot.Hist('TTJets'), -1)

    # adding the WJets_data estimation to the stack
    plot.AddHistogram( 'Data - DY - TT', wjet.weighted, 1010)
    plot.Hist('Data - DY - TT').stack = False
    # with a nice pink color
    pink = kPink+7
    sPinkHollow = Style( lineColor=pink, markerColor=pink, markerStyle=4)
    plot.Hist('Data - DY - TT').SetStyle( sPinkHollow )

    # determine scaling factor for the WJet MC
    # min, max = 60, 200
    # scale = WJets_data / WJets 
    scale_WJets = plot.Hist('Data - DY - TT').Integral(True, min, max) \
                  / plot.Hist('WJets').Integral(True, min, max)
    # apply this additional scaling factor to the WJet component 
    # plot.Hist('WJets').Scale(scale_WJets)

    # hide the WJets_data component from the plot. can be set to True interactively
    plot.Hist('Data - DY - TT').on = True

    return scale_WJets



def getQCD( plotSS, plotOS, dataName ):

    # use SS data as a control region
    # to get the expected QCD shape and yield

    plotSSWithQCD = copy.deepcopy( plotSS )

    # QCD_data = data - DY - TTbar - W
    qcd = copy.deepcopy(plotSSWithQCD.Hist(dataName))
    qcd.Add(plotSSWithQCD.Hist('DYJets'), -1)
    try:
        dyJetsFakes = plotSS.Hist('DYJets_Fakes')
        qcd.Add(dyJetsFakes, -1)
    except:
        print 'cannot find DYJets_Fakes'
        pass

    qcd.Add(plotSSWithQCD.Hist('TTJets'), -1)
    qcd.Add(plotSSWithQCD.Hist('WJets'), -1)

    # adding the QCD data-driven estimation to the SS plot
    plotSSWithQCD.AddHistogram( 'QCD', qcd.weighted, 1020)
    plotSSWithQCD.Hist('QCD').stack = True
    plotSSWithQCD.Hist('QCD').SetStyle( sGreen )


    # extrapolate the expected QCD shape and yield to the
    # signal region

    plotOSWithQCD = copy.deepcopy( plotOS )

    qcdOS = copy.deepcopy( plotSSWithQCD.Hist('QCD') )
    qcdOS.Scale( 1.11 )

    plotOSWithQCD.AddHistogram('QCD', qcdOS.weighted, 1030)
    plotOSWithQCD.Hist('QCD').layer=1.5
    # plotOSWithQCD.DrawStack('HIST')

    return plotSSWithQCD, plotOSWithQCD
    

def makePlot( hist, weights, wJetScaleSS, wJetScaleOS,
              box, mtregion, embed ):

    osign = H2TauTauDataMC(hist, anaDir,
                           selComps,
                           '%s_HighPZeta_OS_%s.root' % (mtregion, box), weights,
                           embed )
    osign.Hist('WJets').Scale( wJetScaleOS ) 

    boxss = box.replace('OS','SS')
    ssign = H2TauTauDataMC(hist, anaDir,
                           selComps,
                           '%s_HighPZeta_SS_%s.root' % (mtregion, box), weights,
                           embed)
    ssign.Hist('WJets').Scale( wJetScaleSS ) 
    
    return ssign, osign


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

    parser = OptionParser()
    parser.usage = '''
    %prog <anaDir> <cfgFile>

    cfgFile: analysis configuration file, see CMGTools.H2TauTau.macros.MultiLoop
    anaDir: analysis directory containing all components, see CMGTools.H2TauTau.macros.MultiLoop.
    hist: histogram you want to plot
    '''
    parser.add_option("-B", "--box", 
                      dest="box", 
                      help="box. Default is Inclusive",
                      default='Inclusive')
    parser.add_option("-M", "--mtregion", 
                      dest="mtregion", 
                      help="mT region. Default is LowMT",
                      default='LowMT')
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
    parser.add_option("-E", "--embed", 
                      dest="embed", 
                      help="Use embedding samples.",
                      action="store_true",
                      default=False)
    
    
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
    
    
    # get WJet scaling factor for same sign
    mtSS = H2TauTauDataMC('diTau/diTau_h_pzeta', anaDir,
                          selComps,
                          'AllMT_LowPZeta_SS_%s.root' % options.box, weights,
                          options.embed)
    wJetScaleSS = wJetScale( mtSS, dataName, -200, -40)
    
    # get WJet scaling factor for opposite sign
    mtOS = H2TauTauDataMC('diTau/diTau_h_pzeta', anaDir,
                          selComps, 
                          'AllMT_LowPZeta_OS_%s.root' % options.box, weights,
                          options.embed)
    wJetScaleOS = wJetScale( mtOS, dataName, -200, -40)
    
    SSD = {}
    OSD = {}
    OSDR = {}
    canvases = []

    xmin = None
    xmax = None


    for hist in sorted(hists):
        print 'Processing: ',hist,dataName, anaDir
        ssign,osign = makePlot( hist, weights, wJetScaleSS, wJetScaleOS,
                                options.box, options.mtregion, options.embed)
        ssQCD, osQCD = getQCD( ssign, osign, dataName )
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
        SSD[hist] = ssQCD
        OSD[hist] = osQCD
        histName = os.path.basename( hist )
        canvas.cd()
        osQCD.DrawStack('HIST', xmin=xmin, xmax=xmax )
        savePlot( histName + '_lin.png') 
        gPad.SetLogy()
        savePlot( histName + '_log.png') 
        gPad.SetLogy(False)
        gPad.Update()
        canvasRatio.cd()
        osQCDRatio = copy.deepcopy(osQCD)
        OSDR[hist] = osQCDRatio
        osQCDRatio.DrawRatioStack('HIST', xmin=xmin, xmax=xmax, ymin=0.001, ymax=2)
        savePlot( histName + '_ratio.png') 


        
    
