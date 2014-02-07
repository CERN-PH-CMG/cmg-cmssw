import os
import imp
from fnmatch import fnmatch

from ROOT import kPink

from CMGTools.RootTools.DataMC.AnalysisDataMCPlot import AnalysisDataMC
from CMGTools.RootTools.Style import formatPad
from CMGTools.RootTools.fwlite.Weight import Weight
from CMGTools.RootTools.fwlite.Weight import printWeights
from CMGTools.H2TauTau.proto.HistogramSet import histogramSet


class H2TauTauMC( AnalysisDataMC ):

    def __init__(self, histName, directory, selComps,
                 filePattern, weights ):
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
        super(H2TauTauMC, self).__init__(histName, directory, weights)
        # if histName.endswith('mT') or histName.find('mass')>-1:
        offsetx = 0.55
        offsety = 0.1
        self.legendBorders = 0.13+offsetx,0.66+offsety,0.44+offsetx,0.89+offsety


    def _InitPrefs(self):
        '''Definine preferences for each component'''
        self.histPref = {}
        self.histPref['Data'] = {'style':sBlack, 'layer':-99}
        self.histPref['data_Run2011A_May10ReReco_v1'] = {'style':sViolet, 'layer':-1000}
        self.histPref['data_Run2011A_PromptReco_v4'] = {'style':sBlue, 'layer':-1000}
        self.histPref['data_Run2011A_PromptReco_v6'] = {'style':sRed, 'layer':-1100}
        self.histPref['data_Run2011A_03Oct2011_v1'] = {'style':sYellow, 'layer':-1105}
        self.histPref['data_Run2011A_05Aug2011_v1'] = {'style':sBlack, 'layer':-1150}
        self.histPref['embed_Run2011A_May10ReReco_v1'] = {'style':sViolet, 'layer':-1000}
        self.histPref['embed_Run2011A_PromptReco_v4'] = {'style':sBlue, 'layer':-1000}
        self.histPref['embed_Run2011A_PromptReco_v6'] = {'style':sRed, 'layer':-1100}
        self.histPref['embed_Run2011A_03Oct2011_v1'] = {'style':sYellow, 'layer':-1105}
        self.histPref['embed_Run2011A_05Aug2011_v1'] = {'style':sBlack, 'layer':-1150}
        self.histPref['dMay10ReReco_v1'] = {'style':sGreen, 'layer':-1200}
        self.histPref['TTJets'] = {'style':sBlue, 'layer':1} 
        self.histPref['WJets'] = {'style':sRed, 'layer':2}  
        self.histPref['DYJets'] = {'style':sYellow, 'layer':3}
        self.histPref['DYJets (emb)'] = {'style':sYellow, 'layer':3}
        self.histPref['DYJets_Fakes'] = {'style':sBlack, 'layer':2.5}


##     def _GetFileNames(self, directory):
##         '''Overloading a function from the base classes.
##         Tells this class how to find the root files in the analysis directory'''
##         fileNames = []
##         for root,dirs,files in os.walk(directory, followlinks=True):
##             if root is directory:
##                 continue
##             if os.path.basename(root) not in self.selComps:
##                 print root,'is not selected'
##                 continue
##             matchingFiles = [file for file in files if fnmatch(file, self.filePattern)]
##             if len(matchingFiles)!=1:
##                 raise ValueError('files matching %s in %s: %s. Need to match only 1 file.'
##                                  % (self.filePattern,
##                                     root,
##                                     matchingFiles))
##             else:
##                 compName = root.split('/')[1]
##                 fileNames.append( (compName, '/'.join([root, matchingFiles[0]])))
##         # print fileNames
##         return fileNames
    def _GetFileNames(self, directory):
        fileNames = []
        filePattern = self.filePattern
        for root,dirs,files in os.walk(directory, followlinks=True):
            if not root.endswith('H2TauTauEventSorter'):
                # print 'not event sorter'
                continue
            sp = root.split('/')
            if len(sp)>1:
                motherDir = root.split('/')[-2]
                if root.find('_Chunk')!=-1:
                    continue
            matchingFiles = [file for file in files if fnmatch(file, filePattern)]
            if len(matchingFiles)!=1:
                raise ValueError('files matching %s in %s: %s. Need to match exactly 1 file.'
                                 % (filePattern,
                                    root,
                                    matchingFiles))
            else:
                compName = root.split('/')[1]
                fileNames.append( (compName, '/'.join([root, matchingFiles[0]])))
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


def makePlot( hist, weights, box, mtregion):

    plot = H2TauTauMC(hist, anaDir,
                      selComps,
                      '%s_OS_%s.root' % (mtregion, box), weights)
    return plot

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
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)


    anaDir = args[0]
    hists = histogramSet( options )
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights = prepareComponents(anaDir, cfg.config)
    print selComps

    canvases = []
    plots = {}
    
    xmin = None
    xmax = None


    for hist in sorted(hists):
        print 'Processing: ',hist, anaDir
        plot = makePlot( hist, weights, options.box, options.mtregion) 
        if options.rebin is not None:
            rebin = int( options.rebin )
            plot.Rebin( rebin )
        canvas = TCanvas( hist, hist, 750, 700)
        canvases.append( canvas )  
        formatPad( canvas )
        plots[hist] = plot
        histName = os.path.basename( hist )
        canvas.cd()
        plot.DrawStack('HIST', xmin=xmin, xmax=xmax )
        savePlot( histName + '_lin.png') 
        gPad.SetLogy()
        savePlot( histName + '_log.png') 
        gPad.SetLogy(False)
        gPad.Update()


        
    
