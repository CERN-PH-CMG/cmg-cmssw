import imp
import math
import copy
import time
import re

from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.H2TauTau.proto.plotter.prepareComponents import prepareComponents
from CMGTools.H2TauTau.proto.plotter.rootutils import * 
from CMGTools.H2TauTau.proto.plotter.categories_TauMu import *
from CMGTools.H2TauTau.proto.plotter.binning import binning_svfitMass
from CMGTools.H2TauTau.proto.plotter.titles import xtitles
from CMGTools.H2TauTau.proto.plotter.blind import blind
from CMGTools.H2TauTau.proto.plotter.plotmod import *
from CMGTools.H2TauTau.proto.plotter.datacards import *
from CMGTools.H2TauTau.proto.plotter.embed import *
from CMGTools.H2TauTau.proto.plotter.plotinfo import plots_All, PlotInfo
from CMGTools.RootTools.Style import *
from ROOT import kPink, TH1, TPaveText, TPad

cp = copy.deepcopy
EWK = 'WJets'

    
NBINS = 100
XMIN  = 0
XMAX  = 200


def replaceShapeInclusive(plot, var, anaDir,
                          comp, weights, 
                          cut, weight,
                          embed):

    mvaTauIsoCut = 0
    cut = cut.replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>0.')
    print '[INCLUSIVE] estimate',comp.name,'with cut',cut
    plotWithNewShape = cp( plot )
    wjyield = plot.Hist(comp.name).Integral()
    nbins = plot.bins
    xmin = plot.xmin
    xmax = plot.xmax
    wjshape = shape(var, anaDir,
                    comp, weights, nbins, xmin, xmax,
                    cut, weight,
                    embed)
    # import pdb; pdb.set_trace()
    wjshape.Scale( wjyield )
    # import pdb; pdb.set_trace()
    plotWithNewShape.Replace(comp.name, wjshape) 
    # plotWithNewShape.Hist(comp.name).on = False 
    return plotWithNewShape

    

def makePlot( var, anaDir, selComps, weights, wJetYieldSS, wJetYieldOS,
              nbins=None, xmin=None, xmax=None,
              cut='', weight='weight', embed=False):
    
    print 'making the plot:', var, 'cut', cut
    # if nbins is None: nbins = NBINS
    # if xmin is None: xmin = XMIN
    # if xmax is None: xmax = XMAX


    oscut = cut+' && diTau_charge==0'
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight,
                           embed=embed)
    # osign.Hist(EWK).Scale( wJetScaleOS )
    osign.Hist(EWK).Normalize()
    osign.Hist(EWK).Scale(wJetYieldOS)    
    replaceWJets = True
    if replaceWJets:
        osign = replaceShapeInclusive(osign, var, anaDir,
                                      selComps['WJets'], weights, 
                                      oscut, weight,
                                      embed)
    
    sscut = cut+' && diTau_charge!=0'
    ssign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=sscut, weight=weight,
                           embed=embed)
    # ssign.Hist(EWK).Scale( wJetScaleSS ) 
    # import pdb; pdb.set_trace()
    ssign.Hist(EWK).Normalize()
    ssign.Hist(EWK).Scale(wJetYieldSS)    
    if replaceWJets:
        ssign = replaceShapeInclusive(ssign, var, anaDir,
                                      selComps['WJets'], weights, 
                                      sscut, weight,
                                      embed)

    ssQCD, osQCD = getQCD( ssign, osign, 'Data' )

    
    qcd_yield = osQCD.Hist('QCD').Integral()

    # replace the QCD shape by the one obtained from the data in the isolation sideband
    sscut_qcdshape = cut.replace('l2_relIso05<0.1','l2_relIso05>0.2').replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.75') + ' && diTau_charge!=0' 
    ssign_qcdshape = H2TauTauDataMC(var, anaDir,
                                    selComps, weights, nbins, xmin, xmax,
                                    cut=sscut_qcdshape, weight=weight,
                                    embed=embed)
    # import pdb; pdb.set_trace()
    # ssign.Hist(EWK).Scale( wJetScaleSS ) 
    qcd_shape = copy.deepcopy( ssign_qcdshape.Hist('Data') )    
    qcd_shape.Normalize()
    qcd_shape.Scale(qcd_yield)
    # qcd_shape.Scale( qcd_yield )
    osQCD.Replace('QCD', qcd_shape)
    # import pdb; pdb.set_trace()
    if var!='mt':
        osQCD.Hist('QCD').Scale( 1618. / 1860.)
    osQCD.Group('VV', ['WW','WZ','ZZ'])
    osQCD.Group('EWK', ['WJets', 'Ztt_ZL', 'Ztt_ZJ','VV'])
    osQCD.Group('Higgs 125', ['HiggsVBF125', 'HiggsGGH125', 'HiggsVH125'])
    
    return ssign, osign, ssQCD, osQCD


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
    parser.add_option("-B", "--blind", 
                      dest="blind", 
                      help="Blind.",
                      action="store_false",
                      default=True)
    parser.add_option("-n", "--nbins", 
                      dest="nbins", 
                      help="Number of bins",
                      default=None)
    parser.add_option("-m", "--min", 
                      dest="xmin", 
                      help="xmin",
                      default=None)
    parser.add_option("-M", "--max", 
                      dest="xmax", 
                      help="xmax",
                      default=None)

    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)

    if options.nbins is None:
        NBINS = binning_svfitMass
        XMIN = None
        XMAX = None
    else:
        NBINS = int(options.nbins)
        XMIN = float(options.xmin)
        XMAX = float(options.xmax)
        

    options.cut = replaceCategories(options.cut, categories) 
    
    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight='weight'
    
    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    embed = options.embed

    origComps = copy.deepcopy(cfg.config.components)

    can, pad, padr = buildCanvas()

    # WJet normalization, done with the 2012 sample
    comps = [comp for comp in cfg.config.components if comp.name!='W3Jets' and comp.name!='TTJets11' and comp.name!='WJets11']
    cfg.config.components = comps
    selComps, weights, zComps = prepareComponents(anaDir, cfg.config, None, options.embed, 'TauMu', '125')

    cutw = options.cut.replace('mt<40', '1')
    fwss, fwos, ss, os = plot_W( anaDir, selComps, weights,
                                 12, 70, 310, cutw,
                                 weight=weight, embed=options.embed)

    cutwss = options.cut + ' && diTau_charge!=0'
    shapess, wjyieldss = shape_and_yield('mt', anaDir, selComps['WJets'], weights,
                                         50, 0, 9999, cutwss, weight=weight,
                                         embed=options.embed)
    cutwos = options.cut + ' && diTau_charge==0'
    shapeos, wjyieldos = shape_and_yield('mt', anaDir, selComps['WJets'], weights,
                                         50, 0, 9999, cutwos, weight=weight,
                                         embed=options.embed)
        
    wjyieldss *= fwss
    wjyieldos *= fwos 
    
    # remove WJets from components, and aliase WJets11 -> WJets
    comps = [comp for comp in origComps if comp.name!='W3Jets' and comp.name!='TTJets11' and comp.name!='WJets']
    cfg.config.components = comps


    aliases = {'WJets11':'WJets'}
    selComps, weights, zComps = prepareComponents(anaDir,
                                                  cfg.config, aliases, options.embed,
                                                  'TauMu', '125')
    # import pdb; pdb.set_trace()

    ssign, osign, ssQCD, osQCD = makePlot( options.hist, anaDir, selComps, weights, wjyieldss, wjyieldos, NBINS, XMIN, XMAX, options.cut, weight=weight, embed=options.embed)
    draw(osQCD, False)
    
