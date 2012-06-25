import imp
import math
import copy
import time
import re

#from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.H2TauTau.proto.plotter.prepareComponents import prepareComponents
from CMGTools.H2TauTau.proto.plotter.rootutils import buildCanvas, draw
from CMGTools.H2TauTau.proto.plotter.categories_TauEle import *
from CMGTools.H2TauTau.proto.plotter.titles import xtitles
from CMGTools.H2TauTau.proto.plotter.blind import blind
from CMGTools.H2TauTau.proto.plotter.plotmod import *
from CMGTools.H2TauTau.proto.plotter.embed import *
from CMGTools.H2TauTau.proto.plotter.plotinfo import plots_All, PlotInfo
from CMGTools.RootTools.Style import *
from ROOT import kPink, TH1, TPaveText, TPad

cp = copy.deepcopy
EWK = 'WJets'

NBINS = 40
XMIN  = 0
XMAX  = 160


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    

def replaceShapeInclusive(plot, var, anaDir,
                          comp, weights, 
                          cut, weight,
                          embed):

    mvaTauIsoCut = -0.5
    # import pdb; pdb.set_trace()
    # if cut.find('isSignal')!=-1:
    # cut = cut.replace( str(inc_sig), cutstr_rlxtauiso(str(inc_sig), mvaTauIsoCut))
    cut = cut.replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.5')
    print '[INCLUSIVE] estimate',comp.name,'with cut',cut
    plotWithNewShape = cp( plot )
    wjyield = plot.Hist(comp.name).Integral()
##     nbins = plot.Hist(comp.name).obj.GetNbinsX()
##     xmin = plot.Hist(comp.name).obj.GetXaxis().GetXmin()
##     xmax = plot.Hist(comp.name).obj.GetXaxis().GetXmax()
    nbins = plot.bins
    xmin = plot.xmin
    xmax = plot.xmax
    wjshape = shape(var, anaDir,
                    comp, weights, nbins, xmin, xmax,
                    cut, weight,
                    embed, treeName = 'H2TauTauTreeProducerTauEle')
    # import pdb; pdb.set_trace()
    wjshape.Scale( wjyield )
    # import pdb; pdb.set_trace()
    plotWithNewShape.Replace(comp.name, wjshape) 
    # plotWithNewShape.Hist(comp.name).on = False 
    return plotWithNewShape


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    

def makePlotNOQCD( var, anaDir, selComps, weights, wJetScaleSS, wJetScaleOS,
                   nbins=None, xmin=None, xmax=None,
                   cut='', weight='weight', embed=False, replaceWJets=False):
    
    print 'making the plot:', var, 'cut', cut
    # if nbins is None: nbins = NBINS
    # if xmin is None: xmin = XMIN
    # if xmax is None: xmax = XMAX


    oscut = cut+' && diTau_charge==0'
    # import pdb; pdb.set_trace()
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight,
                           embed=embed, treeName = 'H2TauTauTreeProducerTauEle')
    osign.Hist(EWK).Scale( wJetScaleOS )
    if replaceWJets:
        osign = replaceShapeInclusive(osign, var, anaDir,
                                      selComps['WJets'], weights, 
                                      oscut, weight,
                                      embed)
    return osign


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    

def makePlot( var, anaDir, selComps, weights, wJetScaleSS, wJetScaleOS,
              nbins=None, xmin=None, xmax=None,
              cut='', weight='weight', embed=False, replaceWJets=True):
    
    print 'making the plot:', var, 'cut', cut
    # if nbins is None: nbins = NBINS
    # if xmin is None: xmin = XMIN
    # if xmax is None: xmax = XMAX


    oscut = cut+' && diTau_charge==0'
    # import pdb; pdb.set_trace()
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight,
                           embed=embed, treeName = 'H2TauTauTreeProducerTauEle')
    osign.Hist(EWK).Scale( wJetScaleOS )
    if replaceWJets:
        osign = replaceShapeInclusive(osign, var, anaDir,
                                      selComps['WJets'], weights, 
                                      oscut, weight,
                                      embed)
    
    # boxss = box.replace('OS','SS')
    sscut = cut+' && diTau_charge!=0'
    ssign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=sscut, weight=weight,
                           embed=embed, treeName = 'H2TauTauTreeProducerTauEle')
    ssign.Hist(EWK).Scale( wJetScaleSS ) 
    # import pdb; pdb.set_trace()
    if replaceWJets:
        ssign = replaceShapeInclusive(ssign, var, anaDir,
                                      selComps['WJets'], weights, 
                                      sscut, weight,
                                      embed)
    # import pdb; pdb.set_trace()

    ssQCD, osQCD = getQCD( ssign, osign, 'Data' )

#    groupEWK( osQCD )
    osQCD.Group('EWK', ['WJets', 'Ztt_Fakes'])
#    osQCD.Group('Higgs 125', ['HiggsVBF125', 'HiggsGGH125', 'HiggsVH125'])
    return ssign, osign, ssQCD, osQCD



    return ssign, osign, ssQCD, osQCD


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    

def drawAll(cut, plots, embed):
    for plot in plots.values():
        print plot.var
        ss, os, ssQ, osQ = makePlot( plot.var, anaDir,
                                     selComps, weights, fwss, fwos,
                                     plot.nbins, plot.xmin, plot.xmax,
                                     cut, weight=weight, embed=embed)
        draw(osQ, False)
        plot.ssign = cp(ss)
        plot.osign = cp(os)
        plot.ssQCD = cp(ssQ)
        plot.osQCD = cp(osQ)
        time.sleep(1)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    

## def replaceCategories(cutstr):
##     for catname, cat in categories.iteritems():
##         cutstr = cutstr.replace( catname, cat )
##     return cutstr


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
                      default=40)
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

    # the categories dictionary is in categories_TauEle.py
    # when putting -C key the key is substituted for the actual cut, for example:
    # -C Xcut_IncX (the X's are necessary for some reasons)
    options.cut = replaceCategories(options.cut, categories_Te) 
    
    print 'used cut:',options.cut

    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight = 'weight'

    #import pdb ; pdb.set_trace ()
    
    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    embed = options.embed

    comps = [comp for comp in cfg.config.components if comp.name!='W3Jets' and comp.name!='TTJets11']
    cfg.config.components = comps

    # selComps = dictionary name,component
    # weights = dictionary of (component_name, weight) to be given to events of each component
    # zComps = set of names of 'zdata' components
    selComps, weights, zComps = prepareComponents(anaDir, cfg.config, 
                                                  embed=embed, channel='TauEle')

    #import pdb ; pdb.set_trace ()

    can, pad, padr = buildCanvas()

    #PG first plot before any DD estimate of bkg
    #PG -----------------------------------------

#    weights['WJet']
#(Pdb) print weights['WJets']
# genN = 81345381, xsec = 31314.00000 pb, genEff = 1.00, intLumi = 4917.49, addWeight = 1.00 -> weight = 1.89299

    firstPlot = H2TauTauDataMC(options.hist, anaDir,
                               selComps, weights, NBINS, XMIN, XMAX,
                               cut=options.cut, weight=weight,
                               embed=False, treeName = 'H2TauTauTreeProducerTauEle')

# {'data_Run2011A_03Oct2011_v1': <CMGTools.RootTools.fwlite.Config.DataComponent object at 0x11d34850>, 'data_Run2011A_05Aug2011_v1': <CMGTools.RootTools.fwlite.Config.DataComponent object at 0x11d31750>, 'data_Run2011B_PromptReco_v1': <CMGTools.RootTools.fwlite.Config.DataComponent object at 0x11d34910>, 'WJets': <CMGTools.RootTools.fwlite.Config.MCComponent object at 0xbd40410>, 'TTJets': <CMGTools.RootTools.fwlite.Config.MCComponent object at 0xbd40450>, 'data_Run2011A_May10ReReco_v1': <CMGTools.RootTools.fwlite.Config.DataComponent object at 0x11d31510>, 'Ztt': <CMGTools.RootTools.fwlite.Config.MCComponent object at 0xbd40310>, 'data_Run2011A_PromptReco_v4': <CMGTools.RootTools.fwlite.Config.DataComponent object at 0x11d31690>}

    
#    draw(firstPlot, True, channel='TauEle')

    #PG plot of data and DY only
    #PG -----------------------------------------

    DY_data_comps = {}
    for comp in selComps:
        if 'data' in comp: 
            DY_data_comps[comp] = selComps[comp]
    DY_data_comps['Ztt'] = selComps['Ztt']
    
    DY_data_plot = H2TauTauDataMC(options.hist, anaDir,
                                  DY_data_comps, weights, NBINS, XMIN, XMAX,
                                  cut=options.cut, weight=weight,
                                  embed=False, treeName = 'H2TauTauTreeProducerTauEle')

#    draw(DY_data_plot, False, channel='TauEle')

    #PG plot of data and WJets only
    #PG -----------------------------------------
    
    WJ_data_comps = {}
    for comp in selComps:
        if 'data' in comp: 
            WJ_data_comps[comp] = selComps[comp]
    WJ_data_comps['WJets'] = selComps['WJets']
    
    WJ_data_plot = H2TauTauDataMC(options.hist, anaDir,
                                  WJ_data_comps, weights, NBINS, XMIN, XMAX,
                                  cut=options.cut, weight=weight,
                                  embed=False, treeName = 'H2TauTauTreeProducerTauEle')

#    draw(WJ_data_plot, False, channel='TauEle')

#    import pdb ; pdb.set_trace ()

    cutw = options.cut.replace('mt<40', '1') #PG questo a che serve? nel caso ci fosse mt<40
    fwss = 0.1
    fwos = 0.1
    fwss, fwos, ss, os = plot_W( anaDir, selComps, weights,
                                 12, 70, 310, cutw,
                                 weight=weight, embed=options.embed, 
                                 treeName='H2TauTauTreeProducerTauEle')

#def plot_W(anaDir, comps, weights, 
#           nbins, xmin, xmax, cut, 
#           weight, embed, 
#           treeName):


#    draw(ss, True, channel='TauEle')
#    draw(os, True, channel='TauEle')

    osNOQCD = makePlotNOQCD( options.hist, anaDir, selComps, 
                             weights, fwss, fwos,
                             NBINS, XMIN, XMAX, options.cut, 
                             weight=weight, embed=options.embed,
                             replaceWJets=False)

#    osNOQCD.legendOn = False
#    draw(osNOQCD, False, channel='TauEle')

    print 'TEST',options.cut
    ssign, osign, ssQCD, osQCD = makePlot( options.hist, anaDir, selComps, 
                                           weights, fwss, fwos,
                                           NBINS, XMIN, XMAX, options.cut, 
                                           weight=weight, embed=options.embed,
                                           replaceWJets=False)
    osQCD.legendOn = True
    draw(osQCD, False, channel='TauEle')
#    ssQCD.legendOn = False
#    draw(ssQCD, False, channel='TauEle')
    
