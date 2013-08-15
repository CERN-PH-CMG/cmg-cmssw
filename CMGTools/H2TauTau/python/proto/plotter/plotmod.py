import copy
import math
from CMGTools.H2TauTau.proto.plotter.H2TauTauDataMC import H2TauTauDataMC
from CMGTools.RootTools.Style import *
from ROOT import kPink, Double, gPad

def sqsum (numa, numb) :
    return math.sqrt(numa * numa + numb * numb)


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
           embed, shift=None, treeName=None):
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
    
    
    

def addQCD( plot, dataName, VVgroup):
    # import pdb; pdb.set_trace()
    plotWithQCD = copy.deepcopy( plot )
    qcd = copy.deepcopy(plotWithQCD.Hist(dataName))
    qcd.Add(plotWithQCD.Hist('Ztt'), -1)
    qcd.Add(plotWithQCD.Hist('Ztt_ZL'), -1)
    qcd.Add(plotWithQCD.Hist('Ztt_ZJ'), -1)  
    qcd.Add(plotWithQCD.Hist('TTJets'), -1)
    qcd.Add(plotWithQCD.Hist('WJets'), -1)
    if VVgroup:
        qcd.Add(plotWithQCD.Hist('VV'), -1)

    # adding the QCD data-driven estimation to the  plot
    plotWithQCD.AddHistogram( 'QCD', qcd.weighted, 888)
    plotWithQCD.Hist('QCD').stack = True
    plotWithQCD.Hist('QCD').SetStyle( sHTT_QCD )
    return plotWithQCD
    
    
    

def getQCD( plotSS, plotOS, dataName, VVgroup=None, scale=1.06 ):

    # use SS data as a control region
    # to get the expected QCD shape and yield
    plotSSWithQCD = addQCD( plotSS, dataName, VVgroup)

    # extrapolate the expected QCD shape and yield to the
    # signal region
    qcd_yield = plotSSWithQCD.Hist('QCD').Integral()
    qcd_yield *= scale
    
    plotOSWithQCD = copy.deepcopy( plotOS )

    qcdOS = copy.deepcopy( plotSSWithQCD.Hist('QCD') )
    # qcdOS.RemoveNegativeValues()
    qcdOS.Scale( qcd_yield / qcdOS.Integral() )

    plotOSWithQCD.AddHistogram('QCD', qcdOS.weighted, 1030)
    plotOSWithQCD.Hist('QCD').layer=1.5
    plotOSWithQCD.Hist('QCD').SetStyle(sHTT_QCD)

    return plotSSWithQCD, plotOSWithQCD
    
    

def fW(mtplot, dataName, xmin, xmax, VVgroup=None, channel = 'TauMu'):

    wjet = copy.deepcopy(mtplot.Hist(dataName))
    wjet.Add(mtplot.Hist('Ztt'), -1)
    wjet.Add(mtplot.Hist('Ztt_ZL'), -1)
    wjet.Add(mtplot.Hist('Ztt_ZJ'), -1)
    wjet.Add(mtplot.Hist('TTJets'), -1)
    if VVgroup:
        wjet.Add(mtplot.Hist('VV'), -1)

    mtplot.AddHistogram( 'Data-DY-TT-VV', wjet.weighted, 1010)
    mtplot.Hist('Data-DY-TT-VV').stack = False
    # with a nice pink color
    pink = kPink+7
    sPinkHollow = Style( lineColor=pink, markerColor=pink, markerStyle=4)
    mtplot.Hist('Data-DY-TT-VV').SetStyle( sPinkHollow )

    data_integral = mtplot.Hist('Data-DY-TT-VV').Integral(True, xmin, xmax)
    mc_integral = mtplot.Hist('WJets').Integral(True, xmin, xmax)
    return data_integral, mc_integral


    

def w_lowHighMTRatio( var, anaDir,
                      comp, weights, 
                      cut, weight, lowMTMax, highMTMin, highMTMax, chargeRequirement, treeName = None):
    cutWithChargeReq = ' && '.join([cut, chargeRequirement]) 
    max = 1000
    mt = shape(var, anaDir,
               comp, weights, max, 0, max,
               cutWithChargeReq, weight,
               None, None, treeName = treeName)
    mt_low = mt.Integral(True, 0, lowMTMax)
    mt_high = mt.Integral(True, highMTMin, highMTMax)
    mt_ratio = mt_low / mt_high    
    return mt_ratio
    
    
    

def plot_W(anaDir, comps, weights, 
           nbins, xmin, xmax,
           cut, weight,
           embed, VVgroup = None, treeName=None):

    # get WJet scaling factor for same sign
    var = 'mt'
    sscut = '{cut} && mt>{min} && mt<{max} && diTau_charge!=0'.format(
        cut = cut, 
        min=xmin,
        max=xmax
        )
    oscut = sscut.replace('diTau_charge!=0', 'diTau_charge==0')


    # get WJet scaling factor for opposite sign
    print 'extracting WJets data/MC factor in high mt region, OS'
    print oscut
    mtOS = H2TauTauDataMC( var, anaDir, comps, weights,
                           nbins, xmin, xmax, 
                           cut = oscut, weight=weight,
                           embed=embed, treeName=treeName)
    if VVgroup != None :
        mtOS.Group ('VV',VVgroup)

    data_OS, mc_OS = fW( mtOS, 'Data', xmin, xmax, VVgroup)
    fW_OS = data_OS / mc_OS


    print 'extracting WJets data/MC factor in high mt region, SS'
    print sscut 
    mtSS = H2TauTauDataMC(var, anaDir, comps, weights,
                          nbins, xmin, xmax,
                          cut = sscut, weight=weight,
                          embed=embed, treeName=treeName)
    if VVgroup != None :
        mtSS.Group ('VV',VVgroup)
        
    data_SS, mc_SS = fW( mtSS, 'Data', xmin, xmax, VVgroup)
    fW_SS = data_SS / mc_SS

    
    print 'fW_SS=',fW_SS,'fW_OS=',fW_OS
 
    return fW_SS, fW_OS, mtSS, mtOS

