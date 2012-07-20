import imp
import math
import copy
import time
import re
from numpy import array

from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.H2TauTau.proto.plotter.prepareComponents import prepareComponents
from CMGTools.H2TauTau.proto.plotter.rootutils import *
from CMGTools.H2TauTau.proto.plotter.categories_TauEle import *
from CMGTools.H2TauTau.proto.plotter.binning import binning_svfitMass
from CMGTools.H2TauTau.proto.plotter.titles import xtitles
from CMGTools.H2TauTau.proto.plotter.blind import blind
from CMGTools.H2TauTau.proto.plotter.plotmod import *
from CMGTools.H2TauTau.proto.plotter.datacards import *
from CMGTools.H2TauTau.proto.plotter.plotinfo import plots_All, plots_J1
from CMGTools.H2TauTau.proto.plotter.plot_H2TauTauDataMC_TauEle_Inclusive import makePlot as makePlotInclusive
from CMGTools.H2TauTau.proto.plotter.plot_H2TauTauDataMC_TauEle_Inclusive import plotPurity
#PG fixme to be reproduced!
#PG fixme what is it meant for?
#from CMGTools.H2TauTau.proto.plotter.plot_H2TauTauDataMC_TauEle_Inclusive_TauFake import jose_qcdTauIsoRatio, jose_qcdMuIsoRatio, colin_qcdTauIsoRatio, colin_qcdMuIsoRatio

from CMGTools.RootTools.Style import *
from ROOT import kPink, TH1, TPaveText, TPad, Double, TCanvas

cp = copy.deepcopy
EWK = 'WJets'

    
NBINS = 100
XMIN  = 0
XMAX  = 200

cutwJ2 = ' && '.join([cat_Inc, cat_J2]) 


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


def makePlot( var, weights, wJetScaleSS, wJetScaleOS, vbf_qcd_yield,
              nbins, xmin, xmax, cut,
              weight='weight', embed=False, shift=None):
    
    if nbins is None: nbins = NBINS
    # if xmin is None: xmin = XMIN
    # if xmax is None: xmax = XMAX

    #PG prepare the final plot, that will be modified in the subsequent lines
    #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    oscut = '&&'.join( [cat_Inc, cat_VBF, 'diTau_charge==0', cut])
    # oscut = str(inc_sig & Cut('l1_charge*l2_charge<0 && mt<40') & cat_VBF)
    print '[OS]', oscut
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight, shift=shift,
                           embed=embed, treeName = 'H2TauTauTreeProducerTauEle')
    osign.Hist(EWK).Scale( wJetScaleOS ) 

    #PG get the W+jets shape with loose VBF and loose isolation
    #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    
    wjshape = WJets_shape_VBF(var, anaDir, cutwJ2,
                              selComps, zComps, weights,
                              nbins, xmin, xmax, weight,
                              embed, shift)
    wjshape.Scale( osign.Hist('WJets').Integral() )
    osign.Replace('WJets', wjshape )

    #PG get the QCD shape from SS with relaxed cuts
    #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    
    sscut = ' && '.join( [ cat_Inc_AntiEleTauIsoCERNShape,
                           'diTau_charge!=0',
                           cut,
                           cat_VBF ] ) 
    print '[SS]', sscut
    ssign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=sscut, weight=weight, shift=shift,
                           embed=embed, treeName = 'H2TauTauTreeProducerTauEle')
    
    ssQCD = addQCD(ssign, 'Data')

    qcd_shape = ssQCD.Hist('QCD')
    qcd_shape_before = copy.deepcopy(qcd_shape)
    qcd_shape.Scale( vbf_qcd_yield/qcd_shape.Integral() )
    
    #PG prepare the final plot
    #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    
    osQCD = copy.deepcopy( osign )
    osQCD.AddHistogram('QCD', qcd_shape.weighted, 1.5)  

    osQCD.Group('VV', ['WW','WZ','ZZ'])
    osQCD.Group('EWK', ['WJets', 'Ztt_ZL', 'Ztt_ZJ'])
    osQCD.Group('Higgs 125', ['HiggsVBF125', 'HiggsGGH125', 'HiggsVH125'])    

    return qcd_shape_before, osign, osQCD 


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


def WJets_shape_VBF(var, anaDir, cut, 
                    selComps, zComps, weights,
                    nbins, xmin, xmax, weight,
                    embed, shift):
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
                    embed, shift, treeName = 'H2TauTauTreeProducerTauEle')
    return wjshape


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


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
                      help="cut",
                      default='1')
    parser.add_option("-E", "--embed", 
                      dest="embed", 
                      help="Use embedd samples.",
                      action="store_true",
                      default=False)
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
    parser.add_option("-g", "--higgs", 
                      dest="higgs", 
                      help="Higgs mass: 125, 130,... or dummy",
                      default=125)

    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)

    options.higgs = '125' #PG FIXME I have to fix the grouping at line 93

    if options.nbins is None:
        NBINS = binning_svfitMass
        XMIN = None
        XMAX = None
    else:
        NBINS = int(options.nbins)
        XMIN = float(options.xmin)
        XMAX = float(options.xmax)
        
    can, pad, padr = buildCanvas()
    
    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight='weight'
    # vbf_eff = 0.0025 # for 2012
    # vbf_eff = 0.001908 # for 2011
    vbf_eff = None
    vbf_eff_err = None
    useTT11 = False
    replaceW=False
    
    anaDir = args[0].rstrip('/')
    shift = None
    if anaDir.endswith('_Down'):
        shift = 'Down'
    elif anaDir.endswith('_Up'):
        shift = 'Up'

    
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    embed = options.embed

    origComps = copy.deepcopy(cfg.config.components)

    comps = [comp for comp in cfg.config.components if comp.name!='W3Jets' and comp.name!='W2Jets' and  comp.name!='TTJets11']
    cfg.config.components = comps
    
    selComps, weights, zComps = prepareComponents(anaDir, cfg.config, None, options.embed, 'TauEle', options.higgs)


    # normalizing WJets (not WJets11 here) 
    # inclusive, iso sideband, SS
    inc_w_cut = cat_Inc
    inc_fwss, inc_fwos, inc_w_ss, inc_w_os = plot_W( anaDir, selComps, weights,
                                                     12, 70, 310, inc_w_cut,
                                                     weight=weight, embed=options.embed,
                                                     treeName='H2TauTauTreeProducerTauEle')

    # inclusive QCD yield in signal region
    # this yield will be multiplied by the VBF efficiency
    # import pdb; pdb.set_trace()
    insig_qcd_cut = '&&'.join([cat_Inc, options.cut])
    inc_ssign, inc_osign, inc_ssQCD, inc_osQCD = makePlotInclusive(
        options.hist, anaDir,
        selComps, weights,
        inc_fwss, inc_fwos,
        NBINS, XMIN, XMAX, insig_qcd_cut,
        weight=weight, embed=options.embed, 
        replaceW=replaceW)

    incsig_qcd_yield = inc_osQCD.Hist('QCD').Integral()
    plotPurity (inc_osQCD,'QCD','Data','OS_inc_QCD_purity')
    plotPurity (inc_osQCD,'WJets','Data','OS_inc_WJets_purity')
    
    if vbf_eff is None:
        # computing VBF efficiency, in anti-isolated region ==================

        # QCD, Inclusive, SS, anti-isolation, for QCD efficiency
        # inc_qcd_cut = '&&'.join([cat_Inc_AntiEleTauIsoCERNEff, options.cut])
        inc_qcd_cut = '&&'.join([cat_Inc_AntiEleTauIsoCERNEff, 'diTau_charge!=0'])
        print 'VBF Efficiency denominator',inc_qcd_cut
        inc_qcd_ssign, inc_qcd_osign, inc_qcd_ssQCD, inc_qcd_osQCD = makePlotInclusive(
            options.hist, anaDir,
            selComps, weights,
            inc_fwss, inc_fwos,
            NBINS, XMIN, XMAX, inc_qcd_cut,
            weight=weight, embed=options.embed
            )

        # neglecting contribution of other backgrounds
        inc_qcd_yield_old = inc_qcd_ssQCD.Hist('Data').Integral()
        inc_qcd_yield_error = Double(0.)
        inc_qcd_yield = inc_qcd_ssQCD.Hist('Data').weighted.IntegralAndError(
            0, 
            inc_qcd_ssQCD.Hist('Data').weighted.GetNbinsX(), 
            inc_qcd_yield_error)
        print 'TEST',inc_qcd_yield_old - inc_qcd_yield


    # remove WJets and TTJets from components, and alias W3Jets -> WJets; TTJets11 -> TTJets
    comps = []
    for comp in origComps:
        if comp.name == 'WJets': continue
        if comp.name == 'W2Jets': continue
        if useTT11:
            if comp.name == 'TTJets': continue
        else:
            if comp.name == 'TTJets11': continue
        comps.append( comp )


    # comps = [comp for comp in origComps if comp.name!='WJets' and comp.name!='WJets11' and  comp.name!='TTJets' ]
    cfg.config.components = comps

    aliases = {'DYJets':'Ztt',
               'W3Jets':'WJets'} 
    if useTT11:
        aliases = {'DYJets':'Ztt',
                   'W3Jets':'WJets',
                   'TTJets11':'TTJets'}


    selComps, weights, zComps = prepareComponents(anaDir, cfg.config, aliases,
                                                  options.embed, 'TauEle', options.higgs)

    # WJets normalization using 2 jet category
    fwss, fwos, ss, os = plot_W( anaDir, selComps, weights,
                                 15, 60, 120, cutwJ2,
                                 weight=weight, embed=options.embed,
                                 treeName='H2TauTauTreeProducerTauEle')


    if vbf_eff is None:
        # QCD VBF, SS, anti-isolation, for QCD efficiency
        vbf_qcd_cut = '&&'.join( [inc_qcd_cut, cat_VBF] ) #PG NB inc_qcd_cut is the denominator
        print 'VBF Efficiency numerator',inc_qcd_cut
        vbf_qcd_ssQCD = H2TauTauDataMC(options.hist, anaDir,
                                       selComps, weights, NBINS, XMIN, XMAX,
                                       cut=vbf_qcd_cut, weight=weight,
                                       embed=embed, treeName = 'H2TauTauTreeProducerTauEle')

#        vbf_qcd_yield = vbf_qcd_ssQCD.Hist('Data').Integral()
        vbf_qcd_yield_error = Double(0.)
        vbf_qcd_yield = vbf_qcd_ssQCD.Hist('Data').weighted.IntegralAndError(
            0, 
            vbf_qcd_ssQCD.Hist('Data').weighted.GetNbinsX(), 
            vbf_qcd_yield_error)

        vbf_eff = vbf_qcd_yield / inc_qcd_yield
        vbf_eff_err = vbf_eff * math.sqrt(vbf_qcd_yield_error * vbf_qcd_yield_error / (vbf_qcd_yield * vbf_qcd_yield) +
                           inc_qcd_yield_error * inc_qcd_yield_error / (inc_qcd_yield * inc_qcd_yield))
                      
        

    print 'VBF Efficiency = ', vbf_eff,' +- ',vbf_eff_err

    qcd_shape_before, osign, osQCD  = makePlot( options.hist, weights, fwss, fwos,
                                                vbf_eff * incsig_qcd_yield,
                                                NBINS, XMIN, XMAX, options.cut, weight=weight,
                                                embed=options.embed, shift=shift )


    #PG save the sidebands to be able and get numbers for the systematics
#    currdir = copy(gDirectory)
    qcdsyst_file = TFile ('qcdsyst.root','recreate')
    qcdsyst_file.cd()
    osQCD.Hist('QCD').weighted.Write('QCD_os_QCD')
    qcd_shape_before.weighted.Write('QCD_ss_QCD')
    qcdsyst_file.Close()   
#    currdir.cd()
    #PG FIXME in this way I am forgetting the systematics due to other samples subtraction
    #PG FIXME i.e. I should check the purities at least

    draw(osQCD, False, 'TauEle', 'VBF')
    datacards(osQCD, 'Xcat_VBFX', shift, 'eTau')


