import copy
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.RootTools.Style import *
from ROOT import kPink

def buildPlot( var, anaDir,
               comps, weights, nbins, xmin, xmax,
               cut, weight,
               embed, shift=None, treeName=None ):
    pl = H2TauTauDataMC(var, anaDir,
                        comps, weights, nbins, xmin, xmax,
                        str(cut), weight,
                        embed, shift, treeName )
    return pl


def hist( var, anaDir,
          comp, weights, nbins, xmin, xmax,
          cut, weight,
          embed, shift=None, treeName=None ):
    pl = buildPlot( var, anaDir,
                    {comp.name:comp}, weights, nbins, xmin, xmax,
                    cut, weight,
                    embed, shift, treeName )
    histo = copy.deepcopy( pl.Hist(comp.name) )    
    return histo


def shape( var, anaDir,
           comp, weights, nbins, xmin, xmax,
           cut, weight,
           embed, shift, treeName=None):
    shape = hist( var, anaDir,
                  comp, weights, nbins, xmin, xmax,
                  cut, weight,
                  embed, shift, treeName )
    shape.Normalize()
    return shape


def shape_and_yield( var, anaDir,
                     comp, weights, nbins, xmin, xmax,
                     cut, weight,
                     embed, treeName=None ):
    shape = hist( var, anaDir,
                  comp, weights, nbins, xmin, xmax,
                  cut, weight,
                  embed, treeName )
    yi = shape.Integral()
    shape.Normalize()
    return shape, yi

    
def addQCD( plot, dataName ):
    # import pdb; pdb.set_trace()
    plotWithQCD = copy.deepcopy( plot )
    # QCD_data = data - DY - TTbar - W
    qcd = copy.deepcopy(plotWithQCD.Hist(dataName))
    qcd.Add(plotWithQCD.Hist('Ztt'), -1)
    try:
        f1 = plot.Hist('Ztt_ZL')
        f2 = plot.Hist('Ztt_ZJ')
        qcd.Add(f1, -1)
        qcd.Add(f2, -1)
    except:
        print 'cannot find Ztt_Fakes'
        print plot
        pass    
    qcd.Add(plotWithQCD.Hist('TTJets'), -1)
    qcd.Add(plotWithQCD.Hist('WJets'), -1)
    # adding the QCD data-driven estimation to the  plot
    plotWithQCD.AddHistogram( 'QCD', qcd.weighted, 888)
    plotWithQCD.Hist('QCD').stack = True
    plotWithQCD.Hist('QCD').SetStyle( sHTT_QCD )
    return plotWithQCD


def getQCD( plotSS, plotOS, dataName ):

    # use SS data as a control region
    # to get the expected QCD shape and yield
    plotSSWithQCD = addQCD( plotSS, dataName )

    # extrapolate the expected QCD shape and yield to the
    # signal region

    plotOSWithQCD = copy.deepcopy( plotOS )

    qcdOS = copy.deepcopy( plotSSWithQCD.Hist('QCD') )
    qcdOS.RemoveNegativeValues()
    qcdOS.Scale( 1.11 )

    plotOSWithQCD.AddHistogram('QCD', qcdOS.weighted, 1030)
    plotOSWithQCD.Hist('QCD').layer=1.5
    plotOSWithQCD.Hist('QCD').SetStyle(sHTT_QCD)

    return plotSSWithQCD, plotOSWithQCD


def fW(mtplot, dataName, xmin, xmax):
    
    # WJets_data = data - DY - TTbar
    wjet = copy.deepcopy(mtplot.Hist(dataName))
    wjet.Add(mtplot.Hist('Ztt'), -1)
    try:
        dyJetsFakes = mtplot.Hist('Ztt_Fakes')
        wjet.Add(mtplot.Hist('Ztt_Fakes'), -1)
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
    mtmin, mtmax = xmin, xmax
    # scale = WJets_data / WJets
    # import pdb; pdb.set_trace()
    scale_WJets = mtplot.Hist('Data - DY - TT').Integral(True, xmin, xmax) \
                  / mtplot.Hist('WJets').Integral(True, xmin, xmax)
    # apply this additional scaling factor to the WJet component
    mtplot.Hist('WJets').Scale(scale_WJets)

    # hide the WJets_data component from the mtplot. can be set to True interactively
    mtplot.Hist('Data - DY - TT').on = True

    mtplot.Hist('WJets').layer = -999999
    return scale_WJets



def plot_W(anaDir,
           comps, weights, nbins, xmin, xmax,
           cut, weight,
           embed, treeName=None):

    # get WJet scaling factor for same sign
    var = 'mt'
    sscut = '{cut} && mt>{mtcut} && diTau_charge!=0'.format(
        cut = cut, 
        mtcut=xmin
        )
    oscut = '{cut} && mt>{mtcut} && diTau_charge==0'.format(
        cut = cut,
        mtcut=xmin
        )

    # import pdb; pdb.set_trace()
    
    print 'extracting WJets data/MC factor in high mt region, SS'
    print sscut 
    mtSS = H2TauTauDataMC(var, anaDir, comps, weights,
                          nbins, xmin, xmax,
                          cut = sscut, weight=weight,
                          embed=embed, treeName=treeName)
    # replaceWJetShape( mtSS, var, sscut)
    # import pdb; pdb.set_trace()
    fW_SS = fW( mtSS, 'Data', xmin, xmax)
    # get WJet scaling factor for opposite sign
    print 'extracting WJets data/MC factor in high mt region, OS'
    print oscut
    mtOS = H2TauTauDataMC(var, anaDir, comps, weights,
                          nbins, xmin, xmax, 
                          cut = oscut, weight=weight,
                          embed=embed, treeName=treeName)
    # replaceWJetShape( mtOS, var, oscut)
    fW_OS = fW( mtOS, 'Data', xmin, xmax)
    print 'fW_SS=',fW_SS,'fW_OS=',fW_OS
    return fW_SS, fW_OS, mtSS, mtOS

