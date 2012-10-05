import imp
import math
import copy
import time
import re

#from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.H2TauTau.proto.plotter.prepareComponents import prepareComponents #, readPickles
from CMGTools.H2TauTau.proto.plotter.rootutils import buildCanvas, draw
from CMGTools.H2TauTau.proto.plotter.categories_TauEle import *
from CMGTools.H2TauTau.proto.plotter.binning import binning_svfitMass
from CMGTools.H2TauTau.proto.plotter.titles import xtitles
from CMGTools.H2TauTau.proto.plotter.blind import blind
from CMGTools.H2TauTau.proto.plotter.plotmod import *
from CMGTools.H2TauTau.proto.plotter.datacards import *
from CMGTools.H2TauTau.proto.plotter.embed import *
from CMGTools.H2TauTau.proto.plotter.plotinfo import *
from CMGTools.RootTools.statistics.Counter import Counters
from CMGTools.RootTools.Style import *
from ROOT import kGray, kPink, TH1, TPaveText, TPad, TCanvas

cp = copy.deepcopy
EWK = 'WJets'

    
NBINS = 100
XMIN  = 0
XMAX  = 200


def plotPurity (orig, num, den,plotname):
    local = copy.deepcopy(orig)
    h_num = local.Hist(num).weighted
    h_den = local.Hist(den).weighted
    h_num.Divide(h_den)
    can0 = TCanvas ('can_'+plotname,'',100,100,600,600)
    bkg = can0.DrawFrame(h_num.GetXaxis().GetXmin(),0,h_num.GetXaxis().GetXmax(),1.2)
    bkg.GetXaxis().SetTitle(orig.histName)
    h_num.SetFillColor(2)
    h_num.Draw('histsame')
    h_num.Draw('same')
    can0.Print(plotname+'.png','png')


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


def replaceShapeInclusive(plot, var, anaDir,
                          comp, weights, 
                          cut, weight,
                          embed, shift=None, 
                          treeName = 'H2TauTauTreeProducerTauEle'):
    '''Replace WJets with the shape obtained using a relaxed tau iso'''
    cut = cut.replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.5')
    print '[INCLUSIVE] estimate',comp.name,'with cut',cut
    plotWithNewShape = cp( plot )
    wjyield = plot.Hist(comp.name).Integral()
    nbins = plot.bins
    xmin = plot.xmin
    xmax = plot.xmax
    wjshape = shape(var, anaDir,
                    comp, weights, nbins, xmin, xmax,
                    cut, weight,
                    embed, shift=shift, treeName = treeName)
    # import pdb; pdb.set_trace()
    wjshape.Scale( wjyield )
    # import pdb; pdb.set_trace()
    plotWithNewShape.Replace(comp.name, wjshape) 
    # plotWithNewShape.Hist(comp.name).on = False 
    return plotWithNewShape


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


def makePlot( var, anaDir, selComps, weights, wJetScaleSS, wJetScaleOS,
              nbins=None, xmin=None, xmax=None,
              cut='', weight='weight', embed=False, shift=None, replaceW=False,
              useExclusiveVV=True):
    
    print 'making the plot:', var, 'cut', cut

    oscut = cut+' && diTau_charge==0'
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight,
                           embed=embed, shift=shift, treeName = 'H2TauTauTreeProducerTauEle')
    osign.Hist(EWK).Scale( wJetScaleOS )
    if replaceW:
        osign = replaceShapeInclusive(osign, var, anaDir,
                                      selComps['WJets'], weights, 
                                      oscut, weight,
                                      embed, shift=shift)    
    sscut = cut+' && diTau_charge!=0'
    ssign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=sscut, weight=weight,
                           embed=embed, shift=shift, treeName = 'H2TauTauTreeProducerTauEle')
    ssign.Hist(EWK).Scale( wJetScaleSS ) 
    # import pdb; pdb.set_trace()
    if replaceW:
        ssign = replaceShapeInclusive(ssign, var, anaDir,
                                      selComps['WJets'], weights, 
                                      sscut, weight,
                                      embed, shift=shift)

    ssQCD, osQCD = getQCD( ssign, osign, 'Data', 1.10 ) #PG scale value according to the note
        
    plotPurity (ssQCD,'QCD','Data','SS_QCD_purity')
    plotPurity (ssQCD,'WJets','Data','SS_WJets_purity')

#    if 0:
#        qcd_yield = osQCD.Hist('QCD').Integral()
#        
#        sscut_qcdshape = cut.replace('l2_relIso05<0.1','l2_relIso05>0.2').replace('l1_looseMvaIso>0.5', 'l1_rawMvaIso>-0.75') + ' && diTau_charge!=0' 
#        ssign_qcdshape = H2TauTauDataMC(var, anaDir,
#                                        selComps, weights, nbins, xmin, xmax,
#                                        cut=sscut_qcdshape, weight=weight,
#                                        embed=embed, shift=shift, treeName = 'H2TauTauTreeProducerTauEle')
#        qcd_shape = copy.deepcopy( ssign_qcdshape.Hist('Data') )    
#        qcd_shape.Normalize()
#        qcd_shape.Scale(qcd_yield)
#        # qcd_shape.Scale( qcd_yield )
#        osQCD.Replace('QCD', qcd_shape)

    if useExclusiveVV :
        osQCD.Group('VV', ['WW2l2v', 'WZ2l2q', 'WZ3lv', 'ZZ2l2q', 'ZZ2l2v', 'ZZ4l'])
        print 'grouping the exclusive samples into VV:'
        for VV in ['WW2l2v', 'WZ2l2q', 'WZ3lv', 'ZZ2l2q', 'ZZ2l2v', 'ZZ4l'] :
            print '    - ',VV,selComps[VV].nGenEvents, selComps[VV].xSection, selComps[VV].effCorrFactor, selComps[VV].intLumi 
    else :
        osQCD.Group('VV', ['WW','WZ','ZZ'])
        print 'grouping the inclusive samples into VV'
        for VV in ['WW','WZ','ZZ'] :
            print '    - ',VV,selComps[VV].nGenEvents, selComps[VV].xSection, selComps[VV].effCorrFactor, selComps[VV].intLumi 
    
    osQCD.Group('EWK', ['WJets', 'Ztt_ZL', 'Ztt_ZJ','VV'])
    osQCD.Group('Higgs 125', ['HiggsVBF125', 'HiggsGGH125', 'HiggsVH125'])
    return ssign, osign, ssQCD, osQCD


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


def drawAll(cut, plots, embed, selComps, weights, fwss, fwos,useExcusiveVV):
    '''See plotinfo for more information'''
    for plot in plots.values():
        print plot.var
        ss, os, ssQ, osQ = makePlot( plot.var, anaDir,
                                     selComps, weights, fwss, fwos,
                                     plot.nbins, plot.xmin, plot.xmax,
                                     cut, weight=weight, embed=embed,
                                     useExclusiveVV=useExclusiveVV)

        osQ.legendOn = False
        print 'drawing ', plot.var
        draw(osQ, False, 'TauEle')
        plot.ssign = cp(ss)
        plot.osign = cp(os)
        plot.ssQCD = cp(ssQ)
        plot.osQCD = cp(osQ)
        time.sleep(1)


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


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
    parser.add_option("-X", "--exclusiveVV", 
                      dest="useExcusiveVV", 
                      help="Use exclusive VV.",
                      action="store_true",
                      default=False)
    parser.add_option("-B", "--blind", 
                      dest="blind", 
                      help="Blind.",
                      action="store_true",
                      default=False)
    parser.add_option("-W", "--replaceW", 
                      dest="replaceW", 
                      help="replace W shape by relaxing isolation on the hadronic tau",
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
                      default=None)
    parser.add_option("-p", "--plots", 
                      dest="plots", 
                      help="plots: set it to true to make control plots",
                      default=False)
    
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
        
    cutstring = options.cut
    options.cut = replaceCategories(options.cut, categories) 

    print 'CUT APPLIED:', options.cut
    
    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight='weight'
    replaceW = options.replaceW
    useW11 = False
    
    anaDir = args[0].rstrip('/')
    shift = None
    if anaDir.endswith('_Down'):
        shift = 'Down'
    elif anaDir.endswith('_Up'):
        shift = 'Up'
        
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
#    embed = options.embed

    origComps = copy.deepcopy(cfg.config.components)
    
    # WJet normalization
    comps = []
    useExcusiveVV = True
    for comp in cfg.config.components:
        if comp.name == 'W3Jets': continue
        if comp.name == 'W2Jets': continue
        if comp.name == 'TTJets11': continue
        if useW11:
            if comp.name == 'WJets': continue
        else:
            if comp.name == 'WJets11': continue
        if options.useExcusiveVV :
            if comp.name == 'WW' : continue
            if comp.name == 'ZZ' : continue
            if comp.name == 'WZ' : continue
        else :
            if comp.name == 'WW2l2v' : continue
            if comp.name == 'WZ2l2q' : continue
            if comp.name == 'WZ3lv' : continue
            if comp.name == 'ZZ2l2q' : continue
            if comp.name == 'ZZ2l2v' : continue
            if comp.name == 'ZZ4l' : continue
        comps.append( comp )
    aliases = None
    if useW11:
        aliases = {'WJets11':'WJets'}

    cfg.config.components = comps

    selComps, weights, zComps = prepareComponents(anaDir, cfg.config, None, 
                                                  options.embed, 'TauEle', options.higgs)


#    pickles = readPickles(anaDir, cfg.config, options.embed, 'TauMu', options.higgs)
#    print 'TTJets',pickles['TTJets']['all events'][1]

#    for pick in pickles:
#        print 'READ',pick,pickles[pick]['all events'][1]
#        selComps[pick].totEvents = pickles[pick]['all events'][1]

    #import pdb ; pdb.set_trace()
    #print 'SELCOMPS:',selComps

    cutw = options.cut.replace('mt<40', '1')
    fwss, fwss_error, fwos, fwos_error, ss, os = plot_W(anaDir, selComps, weights,
                                                        12, 60, 120, cutw,
                                                        weight=weight, embed=options.embed,
                                                        treeName='H2TauTauTreeProducerTauEle')
    #PG fwss = W normalization factor for the same sign plots
    #PG fwos = W normalization factor for the opposite sign plots
    #PG ss   = mt plot with the scaled W, according to fwss
    #PG os   = mt plot with the scaled W, according to fwos

    can0 = TCanvas('can0','',100,100,600,600)

    #PG compare the MC-subtracted data to the WJets MC only for SS
    #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    W_ss = copy.deepcopy( ss )
    W_ss_WJets = W_ss.Hist('WJets').weighted
    W_ss_Data = W_ss.Hist('Data - DY - TT').weighted
    W_ss_WJets.SetFillColor (0)
    W_ss_WJets.GetXaxis().SetTitle ('mt')
    W_ss_Data.GetXaxis().SetTitle ('mt')
    W_ss_WJets.Draw ('hist')
    W_ss_Data.Draw ('same')
    can0.Print ('compare_W_ss.png','png')
    W_ss_Data.Divide (W_ss_WJets)
    W_ss_WJets.Divide (W_ss_WJets)
#    W_ss_WJets.Divide (W_ss_Data)
    can0.DrawFrame(W_ss_WJets.GetXaxis().GetXmin(), 0.5,W_ss_WJets.GetXaxis().GetXmax(), 2)
    W_ss_Data.Draw ('same')
    W_ss_WJets.SetFillStyle (4001)
    W_ss_WJets.SetFillColor (2)
    W_ss_WJets.SetMarkerStyle (9)
    W_ss_WJets.Draw ('sameE3')
    can0.Print ('compare_W_ss_ratio.png','png')


    #import pdb ; pdb.set_trace()

    #PG compare the MC-subtracted data to the WJets MC only for OS
    #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    W_os = copy.deepcopy( os )
    W_os_WJets = W_os.Hist('WJets').weighted
    W_os_Data = W_os.Hist('Data - DY - TT').weighted
    W_os_WJets.SetFillColor (0)
    W_os_WJets.GetXaxis().SetTitle ('mt')
    W_os_Data.GetXaxis().SetTitle ('mt')
    W_os_WJets.Draw ('hist')
    W_os_Data.Draw ('same')
    can0.Print ('compare_W_os.png','png')

    #PG save the sidebands to be able and get numbers for the systematics
#    currdir = copy(gDirectory)
    wsyst_file = TFile ('wsyst.root','recreate')
    wsyst_file.cd()
    W_os_WJets.Write('W_os_WJets')
    W_os_Data.Write('W_os_Data')    
    wsyst_file.Close()
#    currdir.cd()

    W_os_Data.Divide (W_os_WJets)
    W_os_WJets.Divide (W_os_WJets)
#    W_os_WJets.Divide (W_os_Data)
    can0.DrawFrame(W_os_WJets.GetXaxis().GetXmin(), 0.5,W_os_WJets.GetXaxis().GetXmax(), 2)
    W_os_Data.Draw ('same')
    W_os_WJets.SetFillStyle (4001)
    W_os_WJets.SetFillColor (2)
    W_os_WJets.SetMarkerStyle (9)
    W_os_WJets.Draw ('sameE3')
    can0.Print ('compare_W_os_ratio.png','png')

    can, pad, padr = buildCanvas()
    draw(ss, False, 'TauEle', plotprefix = 'W_ss')
    draw(os, False, 'TauEle', plotprefix = 'W_os')

    #PG final drawing
    #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    if (options.plots == 'True') :
        drawAll(options.cut, plots_TauEle, options.embed, selComps, weights, fwss, fwos,
                useExclusiveVV=options.useExcusiveVV)
        #PG this does not work yet, it does not get the DY right
    else :
    
        ssign, osign, ssQCD, osQCD = makePlot( options.hist, anaDir, selComps, weights, 
                                               fwss, fwos, NBINS, XMIN, XMAX, 
                                               options.cut, weight=weight, embed=options.embed, 
                                               replaceW=replaceW,useExclusiveVV=options.useExcusiveVV )
        # ssign = all cuts, same sign, before QCD estimate
        # osign = all cuts, opposite sign, before QCD estimate
        # ssQCD = all cuts, same sign, after QCD estimate, i.e. the QCD is in
        # osQCD = all cuts, opposite sign, after QCD estimate, i.e. the QCD is in
        draw(ssign, False, 'TauEle', 'QCD_ss')
        draw(osign, False, 'TauEle', 'QCD_os')

        #PG save the QCD sideband to be able and get numbers for the systematics
#        currdir = copy(gDirectory)
        qcdsyst_file = TFile ('qcdsyst.root','recreate')
        qcdsyst_file.cd()
        ssQCD.Hist('QCD').weighted.Write('ssQCD_QCD')
        ssQCD.Hist('Data').weighted.Write('ssQCD_Data')
        ssQCD.Hist('Ztt').weighted.Write('ssQCD_Ztt')
        ssQCD.Hist('TTJets').weighted.Write('ssQCD_TTJets')
        ssQCD.Hist('WJets').weighted.Write('ssQCD_WJets')
        try:
            ssQCD.Hist('Ztt_ZL').weighted.Write('QCD_ss_Ztt_ZL')
            ssQCD.Hist('Ztt_ZJ').weighted.Write('QCD_ss_Ztt_ZJ')
        except:
            print 'cannot find Ztt_Fakes in QCD SS plot saving'
            print plot
            pass    
        qcdsyst_file.Close()   
#        currdir.cd()

#        osQCD.legendOn = False
        draw(osQCD, options.blind, 'TauEle')
        datacards(osQCD, cutstring, shift, 'eTau')

        #PG errors for datacards
        #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
        
        #PG WJets
#        WJets_stats_component = 1. / math.sqrt(osQCD.Hist('WJets').obj.GetEntries())
#        WJets_tot_error = sqsum(WJets_stats_component, fwos_error/fwos)
#        print 'WJets total relative error', round(WJets_tot_error, 2)
#        #PG FIXME missing the error due to other bkg subtraction in the fwos_error calculation
#        #PG FIXME move getpickels in prepare components
#        #PG FIXME and assign to the compontents the number of intial events
#        #PG FIXME directly there, so that I am sure it can be used also to estimate
#        #PG FIXME the uncertainty on fos within plotmod.py
#        
#        #PG TTbar
#        TTJets_eff = osQCD.Hist('TTJets').obj.GetEntries() / selComps['TTJets'].totEvents
#        TTJet_eff_error = math.sqrt(TTJets_eff * (1 - TTJets_eff) / selComps['TTJets'].totEvents)
#        print 'TTjets relative eff error', TTJet_eff_error / TTJets_eff
#        #PG missing the error on the scale factor and the error on the total cross-section
#        #PG for 2011 and 2012 separately
#        
#        #PG QCD
#        QCD_norm_component = ssQCD.Hist('QCD').weighted.Integral()
#        print 'QCD norm component', round(QCD_norm_component,0)
#        QCD_extrap_scale = 1.10
#        QCD_extrap_error = 0.10 / QCD_extrap_scale
#        print 'QCD extrapolation factor (and relative error)',round(QCD_extrap_scale,2),'+-',round(QCD_extrap_error,2)
#        
#        #PG Z+jets
#        ZJets_total_stats = selComps['Ztt'].totEvents * \
#                            selComps['Ztt'].tree.GetEntries('isFake == 0') / selComps['Ztt'].tree.GetEntries()
#        if options.embed :
#            ZJets_total_stats = sum(comp.totEvents for name,comp in selComps.iteritems() if comp.isEmbed)
#        ZJets_eff = osQCD.Hist('Ztt').obj.GetEntries() / ZJets_total_stats
#        ZJets_eff_error = math.sqrt(ZJets_eff * (1 - ZJets_eff) / ZJets_total_stats)
#        print 'ZJets relative eff error:',round (ZJets_eff_error / ZJets_eff,2)
#        #PG FIXME normalization error is missing
#
#        #PG Z+jets, lepton tau fake
#        tauEle_fake_error = 0.2
#        ZJets_ZL_total_stats = selComps['Ztt'].totEvents * \
#                               selComps['Ztt'].tree.GetEntries('isFake == 1') / selComps['Ztt'].tree.GetEntries()
#        ZJets_ZL_eff = osQCD.Hist('Ztt_ZL').obj.GetEntries() / ZJets_ZL_total_stats
#        ZJets_ZL_eff_error = math.sqrt(ZJets_ZL_eff * (1 - ZJets_ZL_eff) / ZJets_ZL_total_stats)
#        ZJets_ZL_tot_error = sqsum(tauEle_fake_error, ZJets_ZL_eff_error/ZJets_ZL_eff)
#        print 'ZJets_ZL relative tot error:',round(ZJets_ZL_tot_error,2)
#
#        #PG Z+jets, jet tau fake
#        tauJet_fake_error = 0.2
#        ZJets_ZJ_total_stats = selComps['Ztt'].totEvents * \
#                               selComps['Ztt'].tree.GetEntries('isFake == 2') / selComps['Ztt'].tree.GetEntries()
#        ZJets_ZJ_eff = osQCD.Hist('Ztt_ZJ').obj.GetEntries() / ZJets_ZJ_total_stats
#        ZJets_ZJ_eff_error = math.sqrt(ZJets_ZJ_eff * (1 - ZJets_ZJ_eff) / ZJets_ZJ_total_stats)
#        ZJets_ZJ_tot_error = sqsum(tauJet_fake_error, ZJets_ZJ_eff_error/ZJets_ZJ_eff)
#        print 'ZJets_ZJ relative tot error:',round(ZJets_ZJ_tot_error,2)
