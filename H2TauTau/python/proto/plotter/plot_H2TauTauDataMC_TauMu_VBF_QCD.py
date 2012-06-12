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
from CMGTools.H2TauTau.proto.plotter.plot_H2TauTauDataMC_TauMu_Inclusive import makePlot as makePlotInclusive

from CMGTools.RootTools.Style import *
from ROOT import kPink, TH1, TPaveText, TPad

cp = copy.deepcopy
EWK = 'WJets'

    
NBINS = 100
XMIN  = 0
XMAX  = 200



def makePlot( var, weights, wJetScaleSS, wJetScaleOS,
              nbins=None, xmin=None, xmax=None,
              weight='weight', embed=False):
    
    if nbins is None: nbins = NBINS
    if xmin is None: xmin = XMIN
    if xmax is None: xmax = XMAX

    oscut = str(inc_sig & Cut('l1_charge*l2_charge<0 && mt<40') & cat_VBF)
    print '[OS]', oscut
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight,
                           embed=embed)
    osign.Hist(EWK).Scale( wJetScaleOS ) 
    osign = replaceWJets(osign, var,'l1_charge*l2_charge<0', weight, embed)
    # osign = replaceTTJets(osign, var,'l1_charge*l2_charge<0', weight, embed)
    osign = replaceZtt(osign, var,'l1_charge*l2_charge<0', weight, embed)
    
    sscut = ' && '.join( [cat_Inc_RlxMuIso, 'l1_charge*l2_charge>0', cat_VBF] )
    print '[SS]', sscut
    ssign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=sscut, weight=weight,
                           embed=embed)
    ssign.Hist(EWK).Scale( wJetScaleSS ) 
    ssign = replaceWJets(ssign, var, 'l1_charge*l2_charge>0 && mt<40' , weight, embed)
    # ssign = replaceTTJets(ssign, var, 'l1_charge*l2_charge>0 && mt<40' , weight, embed)
    ssign = replaceZtt(ssign, var, 'l1_charge*l2_charge>0 && mt<40' , weight, embed)

    ssQCD, osQCD = getQCD_VBF( ssign, osign, 'Data', var, weights, wJetScaleSS, wJetScaleOS,
                               nbins, xmin, xmax,
                               weight, embed )

    groupEWK( osQCD )
    return ssign, osign, ssQCD, osQCD


def factor_QCD_nonIsoToIso():
    # iso
    fssign, fosign, fssQCD, fosQCD = makePlotInclusive( options.hist, anaDir, selComps, weights, fwss, fwos, NBINS, XMIN, XMAX, cat_Inc + ' && mt<40 && ' + cat_VBF_Rel, weight=weight, embed=options.embed)

    # non iso
    fssignni, fosignni, fssQCDni, fosQCDni = makePlotInclusive( options.hist, anaDir, selComps, weights, fwss, fwos, NBINS, XMIN, XMAX, cat_Inc_RlxMuIso + ' && mt<40 && ' + cat_VBF_Rel, weight=weight, embed=options.embed)

    # import pdb; pdb.set_trace()

    factor = fssQCD.Hist('QCD').Integral() / fssQCDni.Hist('QCD').Integral()
    return factor



def getQCD_VBF( plotSS, plotOS, dataName, var, weights, wJetScaleSS, wJetScaleOS,
                nbins, xmin, xmax,
                weight, embed  ):
    # use SS data as a control region
    # to get the expected QCD shape and yield
    plotSSWithQCD = addQCD( plotSS, dataName )

    # extrapolate the expected QCD shape and yield to the
    # signal region

    plotOSWithQCD = copy.deepcopy( plotOS )

    qcdyield = plotSSWithQCD.Hist('QCD').Integral()
    qcdyield *= 1.11
    qcdyield *= factor_QCD_nonIsoToIso() 


    ssrelcut = ' && '.join( [cat_Inc_RlxMuIso, 'l1_charge*l2_charge>0', cat_VBF] )
    print '[SSREL]', ssrelcut
    ssignrel = H2TauTauDataMC(var, anaDir,
                              selComps, weights, nbins, xmin, xmax,
                              cut=ssrelcut, weight=weight,
                              embed=embed)
    ssignrel.Hist(EWK).Scale( wJetScaleSS ) 
    # ssignrel = replaceWJets(ssignrel, var, 'l1_charge*l2_charge>0 && mt<40' , weight, embed)
    # ssignrel = replaceZtt(ssignrel, var, 'l1_charge*l2_charge>0 && mt<40' , weight, embed)
    ssignrelQCD = addQCD( ssignrel, dataName )
    
    qcdOS = copy.deepcopy( ssignrelQCD.Hist('QCD') )
    qcdOS.RemoveNegativeValues()
    qcdOS.Normalize()
    qcdOS.Scale( qcdyield )

    import pdb; pdb.set_trace()

    # qcdOS.Scale( 1.11 )
    # qcdOS.Scale( factor_QCD_nonIsoToIso() )

    plotOSWithQCD.AddHistogram('QCD', qcdOS.weighted, 1030)
    plotOSWithQCD.Hist('QCD').layer=1.5

    return plotSSWithQCD, plotOSWithQCD

    

def replaceZtt(plot, var, chargecut, weight, embed):
    plotWithNewShape = cp( plot )
    oldh = plotWithNewShape.Hist('Ztt').weighted
    dyjets = Ztt_VBF(var, anaDir, chargecut, selComps,
                     zComps, weights,
                     oldh.GetNbinsX(),
                     oldh.GetXaxis().GetXmin(), oldh.GetXaxis().GetXmax(),
                     weight, embed)

    plotWithNewShape.Replace('Ztt', dyjets) 
    return plotWithNewShape
    

def replaceWJets(plot, var, chargecut, weight, embed):
    plotWithNewShape = cp( plot )
    oldh = plotWithNewShape.Hist('WJets').weighted
    wjets = WJets_VBF(var, anaDir, chargecut, selComps,
                      zComps, weights,
                      oldh.GetNbinsX(),
                      oldh.GetXaxis().GetXmin(), oldh.GetXaxis().GetXmax(),
                      weight, embed)

    plotWithNewShape.Replace('WJets', wjets) 
    return plotWithNewShape


def replaceQCD(plot, var, chargecut, weight, embed):
    plotWithNewShape = cp( plot )
    oldh = plotWithNewShape.Hist('QCD').weighted
    qcdyield = oldh.Hist('QCD').Integral()
    plotWithNewShape.Replace('QCD', wjets) 
    return plotWithNewShape


def eff_VBF(var, anaDir,
            comp, weights,
            nbins, xmin, xmax,
            cut, weight,
            embed, treeName=None ):
    relcut = cut.replace( cat_VBF, cat_VBF_Rel )
    relhisto = hist(var, anaDir,
                   comp, weights,
                   nbins, xmin, xmax,
                   relcut, weight,
                   embed, treeName )
    histo = hist(var, anaDir,
                 comp, weights,
                 nbins, xmin, xmax,
                 cut, weight,
                 embed, treeName )
##     relhisto.Draw()
##     h = histo.weighted
##     h.SetFillStyle(3001)
##     h.SetFillColor(5)
##     h.SetMarkerColor(4)
##     histo.Draw('same')
##     gPad.Update()
    eff = histo.Integral() / relhisto.Integral()
    print eff
    return histo, relhisto, eff



def WJets_shape_VBF(var, anaDir,
                    selComps, zComps, weights,
                    nbins, xmin, xmax, weight,
                    embed):
    # to get the shape.
    # we need to fully relax the tau iso, and
    # to relax VBF to -0.7.
    # if I relax more VBF, I'm not sure I can estimate the yield
    # with z data
    cutforshape = Cut( cutstr_rlxtauiso( str(inc_sig), -999) )
    cutforshape = cutforshape & Cut(cat_VBF_Rel)
    print cutforshape
    
    wjshape = shape(var, anaDir,
                    selComps['WJets'], weights,
                    nbins, xmin, xmax,
                    cutforshape, weight,
                    embed)
    return wjshape


def WJets_yield_VBF(var, anaDir, chargecut,
                    selComps, zComps, weights,
                    nbins, xmin, xmax, weight,
                    embed):
    # import pdb; pdb.set_trace()
    # to get the yield, we only relax the VBF cuts, to get y_rel
    # then, y_s = y_rel * eff_VBF,
    # where eff_VBF is obtained using a Z->mumu data sample
    wjcutforyield = inc_sig & 'mt<40' & Cut(cat_VBF_Rel) & Cut(chargecut)
    print 'cut for yield', wjcutforyield
    
    wjrelshape, y_rel = shape_and_yield(var, anaDir,
                                        selComps['WJets'], weights,
                                        nbins, xmin, xmax,
                                        wjcutforyield, weight,
                                        embed)
    print 'yield, relaxed VBF cuts', y_rel
    zvbfcut = 'visMass>50 && l1_relIso05<0.2 && l2_relIso05<0.2 && l1_charge*l2_charge<0 && l1_tightId>0.5 && l2_tightId>0.5 && l1_charge*l2_charge<0'+' && ' + cat_VBF 
    h, relh, eff = eff_VBF(var, anaDir,
                           zComps['zdata_Run2012A'], weights,
                           nbins, xmin, xmax,
                           zvbfcut, weight,
                           embed, 'H2TauTauTreeProducerMuMu')
    print 'VBF efficiency from Z data', eff
    return y_rel * eff


def WJets_VBF(var, anaDir, chargecut,
              selComps, zComps, weights,
              nbins, xmin, xmax, weight,
              embed):
    # import pdb; pdb.set_trace()
    wjshape = WJets_shape_VBF(var, anaDir,
                              selComps, zComps, weights,
                              nbins, xmin, xmax, weight,
                              embed)
    wjyield = WJets_yield_VBF(var, anaDir, chargecut, 
                              selComps, zComps, weights,
                              nbins, xmin, xmax, weight,
                              embed)
    wjshape.Scale(wjyield)
    return wjshape


def QCD_shape_VBF(var, anaDir,
                  selComps, zComps, weights,
                  nbins, xmin, xmax, weight,
                  embed):
    # to get the shape.
    # we need to fully relax the tau iso, and
    # to relax VBF to -0.7.
    # if I relax more VBF, I'm not sure I can estimate the yield
    # with z data
    cutforshape = Cut( cutstr_rlxtauiso( str(inc_sig), -999) )
    cutforshape = cutforshape & Cut(cat_VBF_Rel)
    print cutforshape
    
    wjshape = shape(var, anaDir,
                    selComps['QCD'], weights,
                    nbins, xmin, xmax,
                    cutforshape, weight,
                    embed)
    return wjshape



def QCD_VBF(var, anaDir, chargecut,
            selComps, zComps, weights,
            nbins, xmin, xmax, weight,
            embed):
    # import pdb; pdb.set_trace()
    wjshape = QCD_shape_VBF(var, anaDir,
                            selComps, zComps, weights,
                            nbins, xmin, xmax, weight,
                            embed)
    wjyield = QCD_yield_VBF(var, anaDir, chargecut, 
                            selComps, zComps, weights,
                            nbins, xmin, xmax, weight,
                            embed)
    wjshape.Scale(wjyield)
    return wjshape



def Ztt_shape_VBF(var, anaDir,
                  selComps, zComps, weights,
                  nbins, xmin, xmax, weight,
                  embed):
    # to get the shape.
    # we need to fully relax the tau iso, and
    # to relax VBF to -0.7.
    # if I relax more VBF, I'm not sure I can estimate the yield
    # with z data
    cutforshape = Cut( cutstr_rlxtauiso( str(inc_sig), -0.5) )
    cutforshape = cutforshape & Cut(cat_VBF_Rel)
    print cutforshape
    
    zjshape = shape(var, anaDir,
                    selComps['Ztt'], weights,
                    nbins, xmin, xmax,
                    cutforshape, weight,
                    embed)
    # import pdb; pdb.set_trace()
    return zjshape


def Ztt_yield_VBF(var, anaDir, chargecut,
                    selComps, zComps, weights,
                    nbins, xmin, xmax, weight,
                    embed):
    # to get the yield, we only relax the VBF cuts, to get y_rel
    # then, y_s = y_rel * eff_VBF,
    # where eff_VBF is obtained using a Z->mumu data sample
    wjcutforyield = inc_sig & 'mt<40' & Cut(cat_VBF_Rel) & Cut(chargecut)
    print 'cut for yield', wjcutforyield
    
    wjrelshape, y_rel = shape_and_yield(var, anaDir,
                                        selComps['Ztt'], weights,
                                        nbins, xmin, xmax,
                                        wjcutforyield, weight,
                                        embed)
    print 'yield, relaxed VBF cuts', y_rel
    zvbfcut = 'visMass>50 && l1_relIso05<0.2 && l2_relIso05<0.2 && l1_charge*l2_charge<0 && l1_tightId>0.5 && l2_tightId>0.5 && l1_charge*l2_charge<0'+' && ' + cat_VBF 
    h, relh, eff = eff_VBF(var, anaDir,
                           zComps['zdata_Run2012A'], weights,
                           nbins, xmin, xmax,
                           zvbfcut, weight,
                           embed, 'H2TauTauTreeProducerMuMu')
    print 'VBF efficiency from Z data', eff
    return y_rel * eff



def Ztt_VBF(var, anaDir, chargecut,
              selComps, zComps, weights,
              nbins, xmin, xmax, weight,
              embed):
    # import pdb; pdb.set_trace()
    zjshape = Ztt_shape_VBF(var, anaDir,
                            selComps, zComps, weights,
                            nbins, xmin, xmax, weight,
                            embed)
    zjyield = Ztt_yield_VBF(var, anaDir, chargecut, 
                            selComps, zComps, weights,
                            nbins, xmin, xmax, weight,
                            embed)
    zjshape.Scale(zjyield)
    return zjshape


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

    
    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight='weight'
    
    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    embed = options.embed
    selComps, weights, zComps = prepareComponents(anaDir, cfg.config)


    can, pad, padr = buildCanvas()

    fwss, fwos, ss, os = plot_W_inclusive( options.hist, anaDir, selComps, weights,
                                           30, 60, 300, 'isSignal',
                                           weight=weight, embed=options.embed)

    ssign, osign, ssQCD, osQCD = makePlot( options.hist, weights, fwss, fwos, NBINS, XMIN, XMAX, weight=weight, embed=options.embed); draw(osQCD)
    
    # from CMGTools.H2TauTau.proto.plotter.plot_H2TauTauDataMC_TauMu_Inclusive import makePlot as makePlotInclusive

    # ssign, osign, ssQCD, osQCD = makePlotInclusive( options.hist, anaDir, selComps, weights, fwss, fwos, NBINS, XMIN, XMAX, cat_Inc + ' && mt<40', weight=weight, embed=options.embed); draw(osQCD)

    # factor_QCD_nonIsoToIso()
