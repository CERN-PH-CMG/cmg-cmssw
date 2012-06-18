import imp
import math
import copy
import time
import re
from numpy import array

from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.H2TauTau.proto.plotter.prepareComponents import prepareComponents
from CMGTools.H2TauTau.proto.plotter.rootutils import buildCanvas, draw
from CMGTools.H2TauTau.proto.plotter.categories import *
from CMGTools.H2TauTau.proto.plotter.titles import xtitles
from CMGTools.H2TauTau.proto.plotter.blind import blind
from CMGTools.H2TauTau.proto.plotter.plotmod import *
from CMGTools.H2TauTau.proto.plotter.plotinfo import plots_All, plots_J1
from CMGTools.H2TauTau.proto.plotter.plot_H2TauTauDataMC_TauMu_Inclusive import makePlot as makePlotInclusive
from CMGTools.H2TauTau.proto.plotter.plot_H2TauTauDataMC_TauMu_Inclusive_TauFake import jose_qcdTauIsoRatio, jose_qcdMuIsoRatio, colin_qcdTauIsoRatio, colin_qcdMuIsoRatio

from CMGTools.RootTools.Style import *
from ROOT import kPink, TH1, TPaveText, TPad

cp = copy.deepcopy
EWK = 'WJets'

    
NBINS = 100
XMIN  = 0
XMAX  = 200

cutw = ' && '.join([cat_Inc, cat_J2]) 

def makePlot( var, weights, wJetScaleSS, wJetScaleOS, vbf_qcd_yield,
              nbins=None, xmin=None, xmax=None,
              weight='weight', embed=False):
    
    if nbins is None: nbins = NBINS
    # if xmin is None: xmin = XMIN
    # if xmax is None: xmax = XMAX
    
    oscut = str(inc_sig & Cut('l1_charge*l2_charge<0 && mt<40') & cat_VBF)
    print '[OS]', oscut
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight,
                           embed=embed)
    osign.Hist(EWK).Scale( wJetScaleOS ) 
    
    wjshape = WJets_shape_VBF(var, anaDir, cutw,
                              selComps, zComps, weights,
                              nbins, xmin, xmax, weight,
                              embed)
    wjshape.Scale( osign.Hist('WJets').Integral() )
    osign.Replace('WJets', wjshape )
    groupEWK( osign )

    sscut = ' && '.join( [ cat_Inc_AntiMuTauIso_B,
                           'l1_charge*l2_charge>0 && mt<40',
                           cat_VBF ] ) 
    print '[OS]', sscut
    ssign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=sscut, weight=weight,
                           embed=embed)
    
    ssQCD = addQCD(ssign, 'Data')

    qcd_shape = ssQCD.Hist('QCD')
    qcd_shape.Scale( vbf_qcd_yield/qcd_shape.Integral() )
    
    
    osQCD = copy.deepcopy( osign )
    osQCD.AddHistogram('QCD', qcd_shape.weighted, 1.5)   
    
    return osign, osQCD 




## def replaceWJets(plot, var, cut, weight, embed):
##     plotWithNewShape = cp( plot )
##     oldh = plotWithNewShape.Hist('WJets').weighted
##     wjets = WJets_VBF(var, anaDir, cut, selComps,
##                       zComps, weights,
##                       oldh.GetNbinsX(),
##                       oldh.GetXaxis().GetXmin(), oldh.GetXaxis().GetXmax(),
##                       weight, embed)

##     plotWithNewShape.Replace('WJets', wjets) 
##     return plotWithNewShape


def WJets_shape_VBF(var, anaDir, cut, 
                    selComps, zComps, weights,
                    nbins, xmin, xmax, weight,
                    embed):
    # to get the shape.
    # we need to fully relax the tau iso, and
    # to relax VBF to -0.7.
    # if I relax more VBF, I'm not sure I can estimate the yield
    # with z data
    # cutforshape = Cut( cutstr_rlxtauiso( str(inc_sig), -999) )
    # cutforshape = cutforshape & Cut(cat_VBF_Rel)
    # print cutforshape
    cutforshape = cut.replace(cat_VBF, cat_VBF_Rel).replace('l1_looseMvaIso>0.5',
                                                            'l1_rawMvaIso>0')
    cutforshape = cutforshape.replace('l1_charge*l2_charge>0','1')
    cutforshape = cutforshape.replace('l1_charge*l2_charge<0','1')
    
    # import pdb; pdb.set_trace()
    wjshape = shape(var, anaDir,
                    selComps['WJets'], weights,
                    nbins, xmin, xmax,
                    cutforshape, weight,
                    embed)
    return wjshape




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
    parser.add_option("-E", "--embed", 
                      dest="embed", 
                      help="Use embedd samples.",
                      action="store_true",
                      default=False)
##     parser.add_option("-n", "--nbins", 
##                       dest="nbins", 
##                       help="Number of bins",
##                       default=14)
##     parser.add_option("-m", "--min", 
##                       dest="xmin", 
##                       help="xmin",
##                       default=0)
##     parser.add_option("-M", "--max", 
##                       dest="xmax", 
##                       help="xmax",
##                       default=350)

    
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)


    NBINS = array([0., 20., 40., 60., 80., 100., 120., 140., 160., 180.,
                   200., 250., 300., 350.])
    XMIN = None
    XMAX = None

    can, pad, padr = buildCanvas()
    
    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight='weight'
    
    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    embed = options.embed

    origComps = copy.deepcopy(cfg.config.components)

    comps = [comp for comp in cfg.config.components if comp.name!='W3Jets' and  comp.name!='TTJets11']
    cfg.config.components = comps
    
    selComps, weights, zComps = prepareComponents(anaDir, cfg.config)

    
    # inclusive, iso sideband, SS
    inc_w_cut = cat_Inc
    inc_fwss, inc_fwos, inc_w_ss, inc_w_os = plot_W( options.hist, anaDir, selComps, weights,
                                                     12, 70, 310, inc_w_cut,
                                                     weight=weight, embed=options.embed)

    # inclusive QCD yield in signal region
    # import pdb; pdb.set_trace()
    inc_ssign, inc_osign, inc_ssQCD, inc_osQCD = makePlotInclusive(
        options.hist, anaDir,
        selComps, weights,
        inc_fwss, inc_fwos,
        NBINS, XMIN, XMAX, cat_Inc + ' && mt<40',
        weight=weight, embed=options.embed
        )

    incsig_qcd_yield = inc_osQCD.Hist('QCD').Integral()

    # import pdb; pdb.set_trace()

    # computing VBF efficiency

    inc_qcd_cut = cat_Inc_AntiMuTauIsoJosh + ' && mt<40'
    inc_qcd_ssign, inc_qcd_osign, inc_qcd_ssQCD, inc_qcd_osQCD = makePlotInclusive(
        options.hist, anaDir,
        selComps, weights,
        inc_fwss, inc_fwos,
        NBINS, XMIN, XMAX, inc_qcd_cut,
        weight=weight, embed=options.embed
        )

    # neglecting contribution of other backgrounds
    inc_qcd_yield = inc_qcd_ssQCD.Hist('Data').Integral()


    # remove WJets and TTJets from components, and alias W3Jets -> WJets; TTJets11 -> TTJets
    comps = [comp for comp in origComps if comp.name!='WJets' and  comp.name!='TTJets']
    cfg.config.components = comps

    aliases = {'DYJets':'Ztt',
               'W3Jets':'WJets',
               'TTJets11':'TTJets' }
    
    selComps, weights, zComps = prepareComponents(anaDir, cfg.config, aliases)
    
    # import pdb; pdb.set_trace()


    # WJets normalization in VBF region
    fwss, fwos, ss, os = plot_W( options.hist, anaDir, selComps, weights,
                                 15, 60, 120, cutw,
                                 weight=weight, embed=options.embed)
    

    # QCD in SS VBF region, for QCD efficiency
    vbf_qcd_cut = inc_qcd_cut + ' && ' + cat_VBF + ' && diTau_charge!=0'
    vbf_qcd_ssQCD = H2TauTauDataMC(options.hist, anaDir,
                                   selComps, weights, NBINS, XMIN, XMAX,
                                   cut=vbf_qcd_cut, weight=weight,
                                   embed=embed)
    # vbf_qcd_ssQCD.Hist(EWK).Scale( fwss )

    vbf_qcd_yield = vbf_qcd_ssQCD.Hist('Data').Integral()
    
    vbf_eff = vbf_qcd_yield / inc_qcd_yield

    # vbf_qcd = copy.deepcopy( inc_osQCD.Hist('QCD') )
    # vbf_qcd.Scale( vbf_eff )

    osign, osQCD  = makePlot( options.hist, weights, fwss, fwos,
                              vbf_eff * incsig_qcd_yield,
                              NBINS, XMIN, XMAX, weight=weight,
                              embed=options.embed);

    draw(osQCD)


