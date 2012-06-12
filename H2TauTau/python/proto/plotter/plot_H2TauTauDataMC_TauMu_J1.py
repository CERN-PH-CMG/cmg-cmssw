import imp
import math
import copy
import time
import re

from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.H2TauTau.proto.plotter.prepareComponents import prepareComponents
from CMGTools.H2TauTau.proto.plotter.rootutils import buildCanvas, draw
from CMGTools.H2TauTau.proto.plotter.categories import *
from CMGTools.H2TauTau.proto.plotter.titles import xtitles
from CMGTools.H2TauTau.proto.plotter.blind import blind
from CMGTools.H2TauTau.proto.plotter.plotmod import *
from CMGTools.H2TauTau.proto.plotter.plotinfo import plots_All, plots_J1, drawAll
from CMGTools.RootTools.Style import *
from ROOT import kPink, TH1, TPaveText, TPad

cp = copy.deepcopy
EWK = 'WJets'

    
NBINS = 100
XMIN  = 0
XMAX  = 200


def replaceShapeInclusive(plot, var, cut, comp):
    mvaTauIsoCut = -0.5
    # import pdb; pdb.set_trace()
    if cut.find('isSignal')!=-1:
        cut = cut.replace('isSignal', cutstr_rlxtauiso(str(inc_sig), mvaTauIsoCut))
    print '[INCLUSIVE] estimate',comp,'with cut',cut
    plotWithNewShape = cp( plot )
    wjyield = plot.Hist(comp).Integral()
    nbins = plot.Hist(comp).obj.GetNbinsX()
    xmin = plot.Hist(comp).obj.GetXaxis().GetXmin()
    xmax = plot.Hist(comp).obj.GetXaxis().GetXmax()
    wjshape = shape(var, anaDir,
                    selComps[comp], weights, nbins, xmin, xmax,
                    cut, weight,
                    embed)
    # import pdb; pdb.set_trace()
    wjshape.Scale( wjyield )
    # import pdb; pdb.set_trace()
    plotWithNewShape.Replace(comp, wjshape) 
    # plotWithNewShape.Hist(comp).on = False 
    return plotWithNewShape

    
def replaceShapeJets(plot, var, cut, comp):
    cut = cut.replace(cat_J1,cat_J1_Rel)
##     if cut.find('isSignal')!=-1:
##         cut = cut.replace('isSignal', isSignalRelaxedTauIso(mvaTauIsoCut))
    print '[JET] estimate',comp,'with cut',cut
    plotWithNewShape = cp( plot )
    # wjyield = plot.Hist(comp).Integral()
    nbins = plot.Hist(comp).obj.GetNbinsX()
    xmin = plot.Hist(comp).obj.GetXaxis().GetXmin()
    xmax = plot.Hist(comp).obj.GetXaxis().GetXmax()
    wjyirel, dummy = yield_and_shape(var, cut, selComps[comp],
                                     nbins, xmin, xmax)
    wjshape = shape(var, cat_VBF_Rel, selComps[comp],
                    nbins, xmin, xmax)
    wjyield = wjyirel * 0.05987 
    wjshape.Scale( wjyield )
    plotWithNewShape.Replace(comp, wjshape) 
    # plotWithNewShape.Hist(comp).on = False 
    return plotWithNewShape
    


def makePlot( var, weights, wJetScaleSS, wJetScaleOS,
              nbins=None, xmin=None, xmax=None,
              cut='', weight='weight', embed=False):
    
    print 'making the plot:', var, 'cut', cut
    if nbins is None: nbins = NBINS
    if xmin is None: xmin = XMIN
    if xmax is None: xmax = XMAX

    oscut = cut+' && diTau_charge==0'
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight,
                           embed=embed)
    osign.Hist(EWK).Scale( wJetScaleOS ) 
    osign = replaceShape(osign, var, oscut, 'WJets')
    
    # boxss = box.replace('OS','SS')
    sscut = cut+' && diTau_charge!=0'
    ssign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=sscut, weight=weight,
                           embed=embed)
    ssign.Hist(EWK).Scale( wJetScaleSS ) 
    # import pdb; pdb.set_trace()
    ssign = replaceShape(ssign, var, sscut, 'WJets')

    ssQCD, osQCD = getQCD( ssign, osign, 'Data' )

    groupEWK( osQCD )
    return ssign, osign, ssQCD, osQCD



def replaceCategories(cutstr):
    for catname, cat in categories.iteritems():
        cutstr = cutstr.replace( catname, cat )
    return cutstr


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
    parser.add_option("-H", "--hist", 
                      dest="hist", 
                      help="histogram list",
                      default=None)
    parser.add_option("-C", "--cut", 
                      dest="cut", 
                      help="cut to apply in TTree::Draw",
                      default='1')
    parser.add_option("-E", "--embed", 
                      dest="embed", 
                      help="Use embedd samples.",
                      action="store_true",
                      default=False)
    parser.add_option("-n", "--nbins", 
                      dest="nbins", 
                      help="Number of bins",
                      default=50)
    parser.add_option("-m", "--min", 
                      dest="xmin", 
                      help="xmin",
                      default=0)
    parser.add_option("-M", "--max", 
                      dest="xmax", 
                      help="xmax",
                      default=200)

    
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)


    NBINS = int(options.nbins)
    XMIN = float(options.xmin)
    XMAX = float(options.xmax)

    options.cut = replaceCategories(options.cut) 
    
    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight='weight'
    
    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    embed = options.embed
    selComps, weights = prepareComponents(anaDir, cfg.config)


    can, pad, padr = buildCanvas()

    fwss, fwos, ss, os = plot_W_inclusive( options.hist, anaDir, selComps, weights,
                                           30, 60, 300, options.cut,
                                           weight=weight, embed=options.embed)

    ssign, osign, ssQCD, osQCD = makePlot( options.hist, weights, fwss, fwos, NBINS, XMIN, XMAX, options.cut, weight=weight, embed=options.embed)
    draw(osQCD)
    
