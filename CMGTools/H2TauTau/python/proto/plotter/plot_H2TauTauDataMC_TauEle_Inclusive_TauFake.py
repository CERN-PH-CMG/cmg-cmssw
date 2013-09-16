from CMGTools.H2TauTau.proto.plotter.plot_H2TauTauDataMC_TauEle_Inclusive import *
from CMGTools.RootTools.Style import formatPad
from ROOT import TH1F

def fakePlots( opt, var, anaDir, selComps, weights, wJetScaleSS, wJetScaleOS,
               nbins=None, xmin=None, xmax=None,
               cut='', weight='weight', embed=False):

    cut_iso = cat_Inc + ' && mt<40 '
    dss, dos, dssQCD, plot_iso = makePlot( var, anaDir, selComps, weights,
                                           wJetScaleSS, wJetScaleOS,
                                           nbins, xmin, xmax, 
                                           cut_iso, weight, embed=False)
    plot_antiso = None
    if opt=='ele':
        cut_antisoele = cat_Inc_AntiEleIso + ' && mt<40 '
        amdss, amdos, amdssQCD, plot_antiso = makePlot( var, anaDir, selComps, weights,
                                                        wJetScaleSS, wJetScaleOS,
                                                        nbins, xmin, xmax, 
                                                        cut_antisoele, weight, embed=False)
    else:
        cut_antisotau = cat_Inc_AntiTauIso + ' && mt<40 '
        atdss, atdos, atdssQCD, plot_antiso = makePlot( var, anaDir, selComps, weights,
                                                        wJetScaleSS, wJetScaleOS,
                                                        nbins, xmin, xmax, 
                                                        cut_antisotau, weight, embed=False)
    
    
    return plot_iso, plot_antiso


def fakeRate(opt, plot_iso, plot_antiso):
    if opt != 'ele' and opt != 'tau':
        print 'opt must be ele or tau.'
        return 
    fakerate = copy.deepcopy( plot_iso.Hist('QCD') )
    qcd_antiso = plot_antiso.Hist('QCD')
    hf = fakerate.weighted
    hf.Divide(qcd_antiso.weighted)
    sData.formatHisto(hf)
    if opt == 'ele':
        hf.SetTitle(';#ele jet p_{T} (GeV)')
    else:
        hf.SetTitle(';#tau jet p_{T} (GeV)')
        
    return fakerate

#PG FIXME change the cuts according to the analysis!
jose_qcdTauIsoRatio = "((20<l1Jet_pt&&l1Jet_pt<=25)*2.85+(25<l1Jet_pt&&l1Jet_pt<=30)*1+(30<l1Jet_pt&&l1Jet_pt<=35)*0.411+(35<l1Jet_pt&&l1Jet_pt<=40)*0.196+(40<l1Jet_pt&&l1Jet_pt<=45)*0.109+(45<l1Jet_pt&&l1Jet_pt<=50)*0.0644+(50<l1Jet_pt&&l1Jet_pt<=55)*0.0458+(55<l1Jet_pt&&l1Jet_pt<=60)*0.0349+(60<l1Jet_pt&&l1Jet_pt<=65)*0.0338)";

jose_qcdEleIsoRatio  = "((17<l2Jet_pt&&l2Jet_pt<=22)*1.51+(22<l2Jet_pt&&l2Jet_pt<=27)*0.462+(27<l2Jet_pt&&l2Jet_pt<=32)*0.2+(32<l2Jet_pt&&l2Jet_pt<=37)*0.12+(37<l2Jet_pt&&l2Jet_pt<=42)*0.101+(42<l2Jet_pt&&l2Jet_pt<=47)*0.0964+(47<l2Jet_pt&&l2Jet_pt<=52)*0.14+(52<l2Jet_pt&&l2Jet_pt<=57)*0.116)";

colin_qcdTauIsoRatio = '((20.0<l1Jet_pt&&l1Jet_pt<=25.0)*4.89 + (25.0<l1Jet_pt&&l1Jet_pt<=30.0)*1.03 + (30.0<l1Jet_pt&&l1Jet_pt<=35.0)*0.45 + (35.0<l1Jet_pt&&l1Jet_pt<=40.0)*0.20 + (40.0<l1Jet_pt&&l1Jet_pt<=45.0)*0.10 + (45.0<l1Jet_pt&&l1Jet_pt<=50.0)*0.06 + (50.0<l1Jet_pt&&l1Jet_pt<=55.0)*0.04 + (55.0<l1Jet_pt&&l1Jet_pt<=60.0)*0.02 + (60.0<l1Jet_pt&&l1Jet_pt<=65.0)*0.00 + (65.0<l1Jet_pt&&l1Jet_pt<=70.0)*0.00)'

colin_qcdEleIsoRatio = '((17.0<l2Jet_pt&&l2Jet_pt<=22.0)*1.65 + (22.0<l2Jet_pt&&l2Jet_pt<=27.0)*0.79 + (27.0<l2Jet_pt&&l2Jet_pt<=32.0)*0.41 + (32.0<l2Jet_pt&&l2Jet_pt<=37.0)*0.30 + (37.0<l2Jet_pt&&l2Jet_pt<=42.0)*0.24 + (42.0<l2Jet_pt&&l2Jet_pt<=47.0)*0.08 + (47.0<l2Jet_pt&&l2Jet_pt<=52.0)*0.17 + (52.0<l2Jet_pt&&l2Jet_pt<=57.0)*0.17 + (57.0<l2Jet_pt&&l2Jet_pt<=62.0)*0.00 + (62.0<l2Jet_pt&&l2Jet_pt<=67.0)*0.44)'


def cutstrToHist(hname, cutstr):
    patinpar = re.compile('\((.*)\)')
    patbindesc = re.compile('\((\d+)<.*<=(\d+)\)')
    m = patinpar.match(cutstr)
    theStr = m.group(1)
    bins = theStr.split('+')
    histTuple = []
    binSize = None
    for bin in bins:
        bindesc, bininc = bin.split('*')
        bininc = float(bininc)
        m2 = patbindesc.match(bindesc)
        binlow = float(m2.group(1))
        binup = float(m2.group(2))
        print bindesc, bininc, binlow, binup
        histTuple.append( (binlow, binup, bininc) )
        bs = binup - binlow 
        if not binSize:
            binSize = bs
        elif bs != binSize:
            print 'bin size is not constant, failing.'
            return None
    xmin = histTuple[0][0]
    xmax = histTuple[-1][1]
    nbins = int((xmax - xmin) / binSize)
    h = TH1F(hname, hname, nbins, xmin, xmax)
    for index, (low, up, inc) in enumerate(histTuple):
        h.SetBinContent( index+1, inc )
    return h

def histToCutstr(var, hist):
    bins = []
    for bin in range(hist.GetNbinsX()):
        bin += 1 
        low = hist.GetBinLowEdge(bin)
        up = low + hist.GetBinWidth(bin)
        inc = hist.GetBinContent(bin)
        # print low, up, inc
        bins.append('({low}<{var}&&{var}<={up})*{inc:4.2f}'.format(var=var, low=low, up=up, inc=inc) )
    theStr = ' + '.join(bins)
    theStr = '({theStr})'.format(theStr=theStr)
    return theStr

h_jose_qcdTauIsoRatio = cutstrToHist('jose_tau', jose_qcdTauIsoRatio)
h_jose_qcdEleIsoRatio = cutstrToHist('jose_ele', jose_qcdEleIsoRatio)

    
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
##     parser.add_option("-H", "--hist", 
##                       dest="hist", 
##                       help="histogram list",
##                       default=None)
    parser.add_option("-C", "--cut", 
                      dest="cut", 
                      help="cut to apply in TTree::Draw",
                      default='1')
    parser.add_option("-E", "--embed", 
                      dest="embed", 
                      help="Use embedd samples.",
                      action="store_true",
                      default=False)
##     parser.add_option("-n", "--nbins", 
##                       dest="nbins", 
##                       help="Number of bins",
##                       default=50)
##     parser.add_option("-m", "--min", 
##                       dest="xmin", 
##                       help="xmin",
##                       default=0)
##     parser.add_option("-M", "--max", 
##                       dest="xmax", 
##                       help="xmax",
##                       default=200)

    
    
    (options,args) = parser.parse_args()
    if len(args) != 3:
        parser.print_help()
        sys.exit(1)


    NBINS = None
    XMIN = None
    XMAX = None

    options.cut = replaceCategories(options.cut) 
    
    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight='weight'
    
    anaDir = args[0]
    cfgFileName = args[1]
    opt = args[2]
    var = None
    if opt != 'ele' and opt != 'tau':
        print '3rd argument must be ele or tau.'
        sys.exit(1)
    elif opt == 'ele': #PG FIXME fix the cut
        var = 'l2Jet_pt'
        NBINS = 10
        XMIN = 17
        XMAX = 67
    elif opt == 'tau':
        var = 'l1Jet_pt'
        NBINS = 10
        XMIN = 20
        XMAX = 70
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    embed = options.embed
    selComps, weights, zComps = prepareComponents(anaDir, cfg.config)


    can, pad, padr = buildCanvas()

    fwss, fwos, ss, os = plot_W_inclusive( var, anaDir, selComps, weights,
                                           30, 60, 300, options.cut,
                                           weight=weight, embed=options.embed)

    iso, antiso = fakePlots( opt, var, anaDir, selComps, weights,
                             fwss, fwos,
                             NBINS, XMIN, XMAX, options.cut,
                             weight=weight, embed=options.embed)
    draw(iso)
    c2 = TCanvas()
    formatPad(c2)
    hfake = fakeRate(opt, iso, antiso)
    hfake.Draw()
    if opt == 'ele':
        h_jose_qcdEleIsoRatio.Draw('same')
    elif opt == 'tau':
        h_jose_qcdTauIsoRatio.Draw('same')
    gPad.SaveAs('fake_{opt}.png'.format(opt=opt))

    print histToCutstr(var, hfake.weighted)
    
    
    
