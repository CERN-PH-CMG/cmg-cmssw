import imp
import math

from CMGTools.H2TauTau.proto.HistogramSet import histogramSet
from CMGTools.H2TauTau.proto.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.RootTools.Style import *
from ROOT import kPink

def prepareComponents(dir, config):
    # list of components from configuration file
    selComps = dict( [ (comp.name, comp) for comp in config.components ])
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in selComps.values() ] )
    return selComps, weights
    
    
def wJetScale( mtplot, dataName ):
    
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
    mtmin, mtmax = 60, 200
    # scale = WJets_data / WJets 
    scale_WJets = mtplot.Hist('Data - DY - TT').Integral(True, mtmin, mtmax) \
                  / mtplot.Hist('WJets').Integral(True, mtmin, mtmax)
    # apply this additional scaling factor to the WJet component 
    # mtplot.Hist('WJets').Scale(scale_WJets)

    # hide the WJets_data component from the mtplot. can be set to True interactively
    mtplot.Hist('Data - DY - TT').on = True

    return scale_WJets



def getQCD( plotSS, plotOS, dataName ):

    # use SS data as a control region
    # to get the expected QCD shape and yield

    plotSSWithQCD = copy.deepcopy( plotSS )

    # QCD_data = data - DY - TTbar - W
    qcd = copy.deepcopy(plotSSWithQCD.Hist(dataName))
    qcd.Add(plotSSWithQCD.Hist('DYJets'), -1)
    try:
        dyJetsFakes = plotSS.Hist('DYJets_Fakes')
        qcd.Add(dyJetsFakes, -1)
    except:
        print 'cannot find DYJets_Fakes'
        print plotSS
        pass

    #FIXME
    qcd.Add(plotSSWithQCD.Hist('TTJets'), -1)
    qcd.Add(plotSSWithQCD.Hist('WJets'), -1)

    # adding the QCD data-driven estimation to the SS plot
    plotSSWithQCD.AddHistogram( 'QCD', qcd.weighted, 1020)
    plotSSWithQCD.Hist('QCD').stack = True
    plotSSWithQCD.Hist('QCD').SetStyle( sGreen )


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
XMAX  = 200

def makePlot( hist, weights, wJetScaleSS, wJetScaleOS,
              nbins=None, xmin=None, xmax=None,
              cut='', weight='weight', embed=False):

    if nbins is None: nbins = NBINS
    if xmin is None: xmin = XMIN
    if xmax is None: xmax = XMAX

    osign = H2TauTauDataMC(hist, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=cut+' && diTau_charge==0', weight=weight,
                           embed=embed)
    osign.Hist('WJets').Scale( wJetScaleOS ) 

    # boxss = box.replace('OS','SS')
    ssign = H2TauTauDataMC(hist, anaDir,
                           selComps, weights, nbins, xmin, xmax,
                           cut=cut+' && diTau_charge!=0', weight=weight,
                           embed=embed)
    ssign.Hist('WJets').Scale( wJetScaleSS ) 
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

    dataName = 'Data'
    weight='weight'

    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights = prepareComponents(anaDir, cfg.config)
    
    # get WJet scaling factor for same sign
    mtSS = H2TauTauDataMC('mt', anaDir, selComps, weights,
                          NBINS, XMIN, XMAX,
                          cut = 'mt>60 && diTau_charge!=0', weight=weight,
                          embed=options.embed)
    wJetScaleSS = wJetScale( mtSS, dataName)

    
    # get WJet scaling factor for opposite sign
    mtOS = H2TauTauDataMC('mt', anaDir, selComps, weights,
                          NBINS, XMIN, XMAX, 
                          cut = 'mt>60 && diTau_charge==0', weight=weight,
                          embed=options.embed)

    
    wJetScaleOS = wJetScale( mtOS, dataName)

    ssign, osign, ssQCD, osQCD = makePlot( options.hist, weights, wJetScaleSS, wJetScaleOS, NBINS, XMIN, XMAX, options.cut, weight=weight, embed=options.embed)
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

    
