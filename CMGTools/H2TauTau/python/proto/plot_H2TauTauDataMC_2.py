import imp
import math
import copy

from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.RootTools.Style import *
from ROOT import kPink, TH1

cp = copy.deepcopy


def blind(plot, minx, maxx):
    data = plot.Hist('Data').weighted
    minbin = data.FindBin(minx)
    maxbin = min( data.FindBin(maxx), data.GetNbinsX())
    for bin in range(minbin, maxbin):
        data.SetBinContent(bin,0)
        

def prepareComponents(dir, config):
    # list of components from configuration file
    selComps = dict( [ (comp.name, comp) for comp in config.components ])

    totIntLumi = 0
    for comp in selComps.values():
        if comp.isData is True:
            totIntLumi += comp.intLumi
            
    for comp in selComps.values():
        if comp.isMC is True:
            comp.intLumi = totIntLumi
    
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in selComps.values() ] )
    
    return selComps, weights
    
    
def fW_inclusive( mtplot, dataName ):
    
    # WJets_data = data - DY - TTbar
    wjet = copy.deepcopy(mtplot.Hist(dataName))
    wjet.Add(mtplot.Hist('DYJets'), -1)
    try:
        dyJetsFakes = mtplot.Hist('DYJets_Fakes')
        wjet.Add(mtplot.Hist('DYJets_Fakes'), -1)
    except:
        pass
    # FIXME
    wjet.Add(mtplot.Hist('TTJets'), -1)

    # adding the WJets_data estimation to the stack
    mtplot.AddHistogram( 'Data - DY - TT', wjet.weighted, 1010)
    mtplot.Hist('Data - DY - TT').stack = False
    # with a nice pink color
    pink = kPink+7
    sPinkHollow = Style( lineColor=pink, markerColor=pink, markerStyle=4)
    mtplot.Hist('Data - DY - TT').SetStyle( sPinkHollow )

    # determine scaling factor for the WJet MC
    mtmin, mtmax = 60, 300
    # scale = WJets_data / WJets 
    scale_WJets = mtplot.Hist('Data - DY - TT').Integral(True, mtmin, mtmax) \
                  / mtplot.Hist('WJets').Integral(True, mtmin, mtmax)
    # apply this additional scaling factor to the WJet component 
    mtplot.Hist('WJets').Scale(scale_WJets)

    # hide the WJets_data component from the mtplot. can be set to True interactively
    mtplot.Hist('Data - DY - TT').on = True

    mtplot.Hist('WJets').layer = -999999
    return scale_WJets


def plot_W_inclusive():
    # get WJet scaling factor for same sign
    print 'extracting SS WJets inclusive data/MC factor'
    var = 'mt'
    sscut = 'isSignal && mt>60 && diTau_charge!=0'
    oscut = 'isSignal && mt>60 && diTau_charge==0'
    mtSS = H2TauTauDataMC(var, anaDir, selComps, weights,
                          30, 60, 300,
                          cut = sscut, weight=weight,
                          embed=options.embed)
    # replaceWJetShape( mtSS, var, sscut)
    # import pdb; pdb.set_trace()
    fW_inclusive_SS = fW_inclusive( mtSS, dataName)
    # get WJet scaling factor for opposite sign
    print 'extracting OS WJets inclusive data/MC factor'
    mtOS = H2TauTauDataMC(var, anaDir, selComps, weights,
                          30, 60, 300, 
                          cut = oscut, weight=weight,
                          embed=options.embed)
    # replaceWJetShape( mtOS, var, oscut)
    fW_inclusive_OS = fW_inclusive( mtOS, dataName)
    print 'fW_inclusive_SS=',fW_inclusive_SS,'fW_inclusive_OS=',fW_inclusive_OS
    return fW_inclusive_SS, fW_inclusive_OS, mtSS, mtOS



def buildPlot( var, cut, comps, nbins, xmin, xmax):
    pl = H2TauTauDataMC(var, anaDir,
                        comps, weights, nbins, xmin, xmax,
                        cut=cut, weight=weight,
                        embed=embed)
    return pl



def shape(var, cut, comp, nbins, xmin, xmax):
    pl = buildPlot( var, cut, {comp.name:comp}, nbins, xmin, xmax )
    shape = copy.deepcopy( pl.Hist(comp.name) )
    shape.Normalize()
    return shape


def yield_and_shape(var, cut, comp, lumi):
    comp.intLumi = lumi
    pl = buildPlot( var, cut, {comp.name:comp} )
    histo = copy.deepcopy( pl.Hist(comp.name) )
    return histo.Integral(), histo


def dmcf_vbf():
    '''Data/MC correction factor for VBF efficiency
    obtained with the embedded samples.
    '''
    pass


def isSignal():
    '''Signal cuts. Should be equivalent to the variable isSignal!
    Note: opposite charge requirement is not done here. 
    FIXME To be validated'''
    cuts = [
        'l2_relIso05<0.1',
        'l2_tightId>0.5',
        # 'l2_dxy<0.2',
        # 'l2_dz<0.045'
        'l1_looseIso>0.5',
        '(l1_EOverp>0.2 || l1_decayMode!=0)',
        'l1_againstMuonTight>0.5',
        'l1_againstElectronLoose>0.5',
        # 'l1_dxy<0.2',
        # 'l1_dz<0.045'
        ]
    return ' && '.join(cuts)


def isSignalRelaxedIso():
    cuts = [
        'l2_relIso05<0.1',
        'l2_tightId>0.5',
        # 'l2_dxy<0.2',
        # 'l2_dz<0.045'
        # 'l1_veryLooseIso>0.5',
        'l1_rawMvaIso>-0.5',
        '(l1_EOverp>0.2 || l1_decayMode!=0)',
        'l1_againstMuonTight>0.5',
        'l1_againstElectronLoose>0.5',
        # 'l1_dxy<0.2',
        # 'l1_dz<0.045'
        ]
    return ' && '.join(cuts)
    

def isVBF():
    cuts = [
        'nJets>=2',
        'abs(VBF_deta)>4.0',
        'VBF_mjj>400'
        ]
    return ' && '.join(cuts)
    

def isVBFFullSel():
    cuts = [
        isVBF(),
        'isSignal',
        'diTau_charge==0'
        ]
    return ' && '.join(cuts)
    


def hist_WJets_VBF( var, cut, comp, lumi):
    # need to replace by the real one
    # FIXME for the shape, no need to apply any charge requirement
    vbfCut = isVBF()
    # make sure the looseLepCut is just relaxing these cuts, nothing else
    looseLepCut = isSignalRelaxedIso()

    fullSelCut = ' && '.join( [cut, vbfCut, 'isSignal', 'diTau_charge==0'] )
    
    # the shape is taken from a loose lepton cut 
    wjshape = shape( var, ' && '.join( [cut, vbfCut, looseLepCut]), comp)

    # import pdb; pdb.set_trace()
    wjshapeallcuts = shape( var, fullSelCut, comp)
    
    # the yield with the full cut:
    wjyield = yield_and_shape( var, fullSelCut,
                               comp, lumi )[0]
    
    hist = copy.deepcopy( wjshape )
    hist.Scale( wjyield )
    return hist, wjshape, wjyield, wjshapeallcuts
    

def addQCD( plot ):
    # import pdb; pdb.set_trace()
    plotWithQCD = copy.deepcopy( plot )
    # QCD_data = data - DY - TTbar - W
    qcd = copy.deepcopy(plotWithQCD.Hist(dataName))
    qcd.Add(plotWithQCD.Hist('DYJets'), -1)
    try:
        dyJetsFakes = plot.Hist('DYJets_Fakes')
        qcd.Add(dyJetsFakes, -1)
    except:
        print 'cannot find DYJets_Fakes'
        print plot
        pass    
    qcd.Add(plotWithQCD.Hist('TTJets'), -1)
    qcd.Add(plotWithQCD.Hist('WJets'), -1)
    # adding the QCD data-driven estimation to the  plot
    plotWithQCD.AddHistogram( 'QCD', qcd.weighted, 1020)
    plotWithQCD.Hist('QCD').stack = True
    plotWithQCD.Hist('QCD').SetStyle( sGreen )
    return plotWithQCD



def getQCD( plotSS, plotOS, dataName ):

    # use SS data as a control region
    # to get the expected QCD shape and yield
    plotSSWithQCD = addQCD( plotSS )

    # extrapolate the expected QCD shape and yield to the
    # signal region

    plotOSWithQCD = copy.deepcopy( plotOS )

    qcdOS = copy.deepcopy( plotSSWithQCD.Hist('QCD') )
    qcdOS.Scale( 1.11 )

    plotOSWithQCD.AddHistogram('QCD', qcdOS.weighted, 1030)
    plotOSWithQCD.Hist('QCD').layer=1.5
    # plotOSWithQCD.DrawStack('HIST')

    return plotSSWithQCD, plotOSWithQCD
    

NBINS = 100
XMIN  = 0
XMAX  = 300


def replaceWJetShape(plot, var, cut):
    if cut.find('isSignal')!=-1:
        cut = cut.replace('isSignal', isSignalRelaxedIso())
    print 'estimate WJet shape with cut',cut
    plotWithNewShape = cp( plot )
    wjyield = plot.Hist('WJets').Integral()
    nbins = plot.Hist('WJets').obj.GetNbinsX()
    xmin = plot.Hist('WJets').obj.GetXaxis().GetXmin()
    xmax = plot.Hist('WJets').obj.GetXaxis().GetXmax()
    wjshape = shape(var, cut, selComps['WJets'], nbins, xmin, xmax)
    wjshape.Scale( wjyield )
    plotWithNewShape.Replace('WJets', wjshape) 
    # plotWithNewShape.Hist('WJets').on = False 
    return plotWithNewShape


def makePlot( var, weights, wJetScaleSS, wJetScaleOS,
              nbins=None, xmin=None, xmax=None,
              cut='', weight='weight', embed=False):
    
    print 'making the plot'
    if nbins is None: nbins = NBINS
    if xmin is None: xmin = XMIN
    if xmax is None: xmax = XMAX

    oscut = cut+' && diTau_charge==0'
    osign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=oscut, weight=weight,
                           embed=embed)
    osign.Hist('WJets').Scale( wJetScaleOS ) 
    osign = replaceWJetShape(osign, var, oscut)
    # import pdb; pdb.set_trace()
    
    # boxss = box.replace('OS','SS')
    sscut = cut+' && diTau_charge!=0'
    ssign = H2TauTauDataMC(var, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=sscut, weight=weight,
                           embed=embed)
    ssign.Hist('WJets').Scale( wJetScaleSS ) 
    # import pdb; pdb.set_trace()
    ssign = replaceWJetShape(ssign, var, sscut)

    ssQCD, osQCD = getQCD( ssign, osign, 'Data' )    
    return ssign, osign, ssQCD, osQCD


def savePlot(name):
    if gPad is None:
        print 'no active canvas'
        return
    fileName = '%s/%s' % (anaDir, name)
    print 'pad', gPad.GetName(), 'saved to', fileName    
    gPad.SaveAs( fileName )   


def simpleSignificance(plot, sigName, bgdName, min, max):
    nSig = plot.Hist(sigName).Integral(True, min, max)
    nBgd = plot.Hist(bgdName).Integral(True, min, max)
    return nSig / math.sqrt(nBgd), nSig, nBgd


def examplePlot():
    return H2TauTauDataMC('mt', anaDir, selComps, weights, NBINS, XMIN, XMAX, cut = '1', weight=weight, embed=options.embed)
    

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
    
    
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)


    # TH1.AddDirectory(False)
    dataName = 'Data'
    weight='weight'

    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)
    embed = options.embed
    selComps, weights = prepareComponents(anaDir, cfg.config)

    # plot = examplePlot()
    
##     # get WJet scaling factor for same sign
##     mtSS = H2TauTauDataMC('mt', anaDir, selComps, weights,
##                           NBINS, XMIN, XMAX,
##                           cut = 'isSignal && mt>60 && diTau_charge!=0', weight=weight,
##                           embed=options.embed)
##     fW_inclusive_SS = fW_inclusive( mtSS, dataName)

    
##     # get WJet scaling factor for opposite sign
##     mtOS = H2TauTauDataMC('mt', anaDir, selComps, weights,
##                           NBINS, XMIN, XMAX, 
##                           cut = 'isSignal && mt>60 && diTau_charge==0', weight=weight,
##                           embed=options.embed)

    
##     fW_inclusive_OS = fW_inclusive( mtOS, dataName)

    fwss, fwos, ss, os = plot_W_inclusive()

    ssign, osign, ssQCD, osQCD = makePlot( options.hist, weights, fwss, fwos, NBINS, XMIN, XMAX, options.cut, weight=weight, embed=options.embed)
    osQCD.DrawStack('HIST')

    signalMu = 'l2_relIso05<0.1 && l2_tightId>0.5'
    signalMt = 'mt<40'

    muIsoMVA_template = '((l2_pt < 20 && abs(l2_eta)<1.479 && l2_mvaIso > {mva1}) || (l2_pt < 20 && abs(l2_eta)>1.479 && l2_mvaIso > {mva2}) || (l2_pt > 20 && abs(l2_eta)<1.479 && l2_mvaIso > {mva3}) || (l2_pt > 20 && abs(l2_eta)>1.479 && l2_mvaIso > {mva4}))'
    muIsoMVA_WP1 =  muIsoMVA_template.format( mva1 = 0.922,
                                              mva2 = 0.929,
                                              mva3 = 0.921,
                                              mva4 = 0.9)
    muIsoMVA_WP2 =  muIsoMVA_template.format( mva1 = 0.91,
                                              mva2 = 0.91,
                                              mva3 = 0.897,
                                              mva4 = 0.864)
    muIsoMVA_WP3 =  muIsoMVA_template.format( mva1 = 0.957,
                                              mva2 = 0.96,
                                              mva3 = 0.981,
                                              mva4 = 0.971)

    
