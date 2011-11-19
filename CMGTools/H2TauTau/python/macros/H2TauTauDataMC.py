import os
from fnmatch import fnmatch

from ROOT import kPink

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.H2TauTau.macros.Weight import Weight
from CMGTools.H2TauTau.macros.Weight import printWeights

#COLIN this macro does not work when a component is symlinked! is it what I want?
#COLIN need to be able to merge 2 components together:
# - WW, WZ, ZZ
# - all data periods, etc. [DONE]

class H2TauTauDataMC( AnalysisDataMC ):

    def __init__(self, histName, directory, selComponents,
                 filePattern, weights,
                 groupDataName = 'Data'):
        '''Constructor

        histName  : histogram to be plotted
        directory : analysis directory containing subdirectories for each component
        selComponents : selected components (among the component subdirectories)
        filePattern : pattern to find a single root file in each component subdirectory
                      from which the histogram will be obtained.
        weights   : weight dictionary
        groupDataName : all data components will be grouped into a single component
                        with this name.
        '''
        self.filePattern = filePattern
        self.selComponents = selComponents
        super(H2TauTauDataMC, self).__init__(histName, directory, weights)
        # if histName.endswith('mT') or histName.find('mass')>-1:
        offset = 0.45
        self.legendBorders = 0.13+offset,0.66,0.44+offset,0.89

        self.dataComponents = [ key for key, value in selComponents.iteritems() \
                                if value.isMC is False ]
        self.groupDataComponents( self.dataComponents, groupDataName)

        # print self
        

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
        self.histPref['PromptReco-v4'] = {'style':sBlue, 'layer':-1000}
        self.histPref['PromptReco-v6'] = {'style':sRed, 'layer':-1100}
        self.histPref['May10ReReco-v1'] = {'style':sGreen, 'layer':-1200}
        self.histPref['TTJets'] = {'style':sBlue, 'layer':1} 
        self.histPref['WJets'] = {'style':sRed, 'layer':2}  
        self.histPref['DYJets'] = {'style':sYellow, 'layer':3}


    def _GetFileNames(self, directory):
        '''Overloading a function from the base classes.
        Tells this class how to find the root files in the analysis directory'''
        fileNames = []
        for root,dirs,files in os.walk(directory):
            if root is directory:
                continue
            if os.path.basename(root) not in self.selComponents:
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
        


    
    
def wJetScale( plot, dataName ):
    
    # WJets_data = data - DY - TTbar 
    wjet = copy.deepcopy(plot.Hist(dataName))
    wjet.Add(plot.Hist('DYJets'), -1)
    wjet.Add(plot.Hist('TTJets'), -1)

    # adding the WJets_data estimation to the stack
    plot.AddHistogram( 'Data - DY - TT', wjet.weighted, 1010)
    plot.Hist('Data - DY - TT').stack = False
    # with a nice pink color
    pink = kPink+7
    sPinkHollow = Style( lineColor=pink, markerColor=pink, markerStyle=4)
    plot.Hist('Data - DY - TT').SetStyle( sPinkHollow )

    # determine scaling factor for the WJet MC
    mtmin, mtmax = 50, 200
    # scale = WJets_data / WJets 
    scale_WJets = plot.Hist('Data - DY - TT').Integral(True, mtmin, mtmax) \
                  / plot.Hist('WJets').Integral(True, mtmin, mtmax)
    # apply this additional scaling factor to the WJet component 
    # plot.Hist('WJets').Scale(scale_WJets)

    # hide the WJets_data component from the plot. can be set to True interactively
    plot.Hist('Data - DY - TT').on = False

    return scale_WJets



def getQCD( plotSS, plotOS, dataName ):

    # use SS data as a control region
    # to get the expected QCD shape and yield

    plotSSWithQCD = copy.deepcopy( plotSS )

    # QCD_data = data - DY - TTbar - W
    qcd = copy.deepcopy(plotSSWithQCD.Hist(dataName))
    qcd.Add(plotSSWithQCD.Hist('DYJets'), -1)
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
    qcdOS.Scale( 1.06 )

    plotOSWithQCD.AddHistogram('QCD', qcdOS.weighted, 1030)
    plotOSWithQCD.Hist('QCD').layer=2.5
    # plotOSWithQCD.DrawStack('HIST')

    return plotSSWithQCD, plotOSWithQCD
    

def plot( hist, weights, wJetScaleSS, wJetScaleOS ):

    ssign = H2TauTauDataMC(hist, anaDir,
                        selComponents,
                        'SSLowMT.root', weights)
    ssign.Hist('WJets').Scale( wJetScaleSS ) 
    osign = H2TauTauDataMC(hist, anaDir,
                        selComponents,
                        'OSLowMT.root', weights)
    osign.Hist('WJets').Scale( wJetScaleOS ) 
    
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
    from CMGTools.H2TauTau.macros.AnalysisConfig import AnalysisConfig

    parser = OptionParser()
    parser.usage = '''
    %prog <cfgFile> <anaDir> <hist>

    cfgFile: analysis configuration file, see CMGTools.H2TauTau.macros.MultiLoop
    anaDir: analysis directory containing all components, see CMGTools.H2TauTau.macros.MultiLoop.
    hist: histogram you want to plot
    '''
    
    (options,args) = parser.parse_args()
    if len(args) < 2:
        parser.print_help()
        sys.exit(1)

    cfgFile = args[0]
    anaDir = args[1]
    hists = [ hist for hist in args[2].split(',') if hist is not '' ]

    dataName = 'Data'

    anacfg = AnalysisConfig( cfgFile )
    selComponents = anacfg.SelectedComponents()
    
    weights = dict( [ (key,comp.GetWeight()) \
                      for key, comp in selComponents.iteritems()] )

    # get WJet scaling factor for same sign
    mtSS = H2TauTauDataMC('tauMu/tauMu_h_mT', anaDir,
                          selComponents,
                          'SS.root', weights)
    wJetScaleSS = wJetScale( mtSS, dataName)

    # get WJet scaling factor for opposite sign
    mtOS = H2TauTauDataMC('tauMu/tauMu_h_mT', anaDir,
                          selComponents, 
                          'OS.root', weights)
    wJetScaleOS = wJetScale( mtOS, dataName)

    SSD = {}
    OSD = {}
    canvases = []

    xmin = None
    xmax = None
    
    for hist in hists:
        print 'Processing: ',hist,dataName, anaDir
        ssign,osign = plot( hist, weights, wJetScaleSS, wJetScaleOS )
        ssQCD, osQCD = getQCD( ssign, osign, dataName )
        canvases.append(TCanvas( hist, hist, 800, 600))
        SSD[hist] = ssQCD
        OSD[hist] = osQCD
        histName = os.path.basename( hist )
        osQCD.DrawStack('HIST', xmin=xmin, xmax=xmax )
        savePlot( histName + '_lin.png') 
        gPad.SetLogy()
        savePlot( histName + '_log.png') 
        gPad.SetLogy(False)
        osQCD.DrawRatioStack('HIST', xmin=xmin, xmax=xmax, ymin=0.001, ymax=2)
        savePlot( histName + '_ratio.png') 

    # OSD[ hists[0] ].DrawStack('HIST')

        
    
