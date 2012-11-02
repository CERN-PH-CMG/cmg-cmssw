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
    
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
    

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
    
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
    

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
    
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
    

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
    
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
    

def addQCD( plot, dataName ):
    # import pdb; pdb.set_trace()
    plotWithQCD = copy.deepcopy( plot )
    qcd = copy.deepcopy(plotWithQCD.Hist(dataName))
    qcd.Add(plotWithQCD.Hist('Ztt'), -1)
    qcd.Add(plotWithQCD.Hist('Ztt_ZL'), -1)
    qcd.Add(plotWithQCD.Hist('Ztt_ZJ'), -1)  
    qcd.Add(plotWithQCD.Hist('TTJets'), -1)
    qcd.Add(plotWithQCD.Hist('WJets'), -1)
    qcd.Add(plotWithQCD.Hist('VV'), -1)

    # adding the QCD data-driven estimation to the  plot
    plotWithQCD.AddHistogram( 'QCD', qcd.weighted, 888)
    plotWithQCD.Hist('QCD').stack = True
    plotWithQCD.Hist('QCD').SetStyle( sHTT_QCD )
    return plotWithQCD
    
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
    

def getQCD( plotSS, plotOS, dataName, scale=1.06 ):

    # use SS data as a control region
    # to get the expected QCD shape and yield
    plotSSWithQCD = addQCD( plotSS, dataName )

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
    
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    

def fW(mtplot, dataName, xmin, xmax, channel = 'TauMu'):

    wjet = copy.deepcopy(mtplot.Hist(dataName))
    wjet.Add(mtplot.Hist('Ztt'), -1)
    wjet.Add(mtplot.Hist('Ztt_ZL'), -1)
    wjet.Add(mtplot.Hist('Ztt_ZJ'), -1)
    wjet.Add(mtplot.Hist('TTJets'), -1)
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

## def fW(mtplot, dataName, xmin, xmax, channel = 'TauMu'):
    
##     # WJets_data = data - DY - TTbar
##     wjet = copy.deepcopy(mtplot.Hist(dataName))
##     #SYNC with JOSH: remove the following line
##     wjet.Add(mtplot.Hist('Ztt'), -1)
##     removingFakes = False
##     #COLIN why a try block?
##     try:
##         #SYNC with JOSH: replace the following 4 lines by pass
##         f1 = mtplot.Hist('Ztt_ZL')
##         f2 = mtplot.Hist('Ztt_ZJ')
##         wjet.Add(f1, -1)
##         wjet.Add(f2, -1)
##         removingFakes = True
##     except:
##         print 'cannot find Ztt_Fakes in W+jets estimate'
##         pass
##     # FIXME
##     wjet.Add(mtplot.Hist('TTJets'), -1)
    
##     if mtplot.histosDict.get('VV', None) != None :
##         wjet.Add(mtplot.Hist('VV'), -1)
##     else:
##         print 'fW: VV group not found, VV not subtracted'

##     # adding the WJets_data estimation to the stack
##     mtplot.AddHistogram( 'Data - DY - TT', wjet.weighted, 1010)
##     mtplot.Hist('Data - DY - TT').stack = False
##     # with a nice pink color
##     pink = kPink+7
##     sPinkHollow = Style( lineColor=pink, markerColor=pink, markerStyle=4)
##     mtplot.Hist('Data - DY - TT').SetStyle( sPinkHollow )

##     # determine scaling factor for the WJet MC
##     mtmin, mtmax = xmin, xmax
##     # import pdb; pdb.set_trace()

##     data_error = Double(0.)
##     data_integral = mtplot.Hist('Data - DY - TT').weighted.IntegralAndError(
##             0 if xmin == None else mtplot.Hist('Data - DY - TT').weighted.FindFixBin (xmin), 
##             mtplot.Hist('Data - DY - TT').weighted.GetNbinsX() if xmax == None else mtplot.Hist('Data - DY - TT').weighted.FindFixBin(xmax) - 1, 
##             data_error)

##     data_integral = mtplot.Hist('Data - DY - TT').weighted.IntegralAndError(
##             0 if xmin == None else mtplot.Hist('Data - DY - TT').weighted.FindFixBin (xmin), 
##             mtplot.Hist('Data - DY - TT').weighted.GetNbinsX() if xmax == None else mtplot.Hist('Data - DY - TT').weighted.FindFixBin(xmax) - 1, 
##             data_error)

##     wjets_error = Double(0.)
##     wjets_integral = mtplot.Hist('WJets').weighted.IntegralAndError(
##             0 if xmin == None else mtplot.Hist('WJets').weighted.FindFixBin (xmin), 
##             mtplot.Hist('WJets').weighted.GetNbinsX() if xmax == None else mtplot.Hist('WJets').weighted.FindFixBin(xmax) - 1, 
##             wjets_error)

##     scale_WJets = data_integral / wjets_integral
                  
##     # apply this additional scaling factor to the WJet component
##     mtplot.Hist('WJets').Scale(scale_WJets)

##     # hide the WJets_data component from the mtplot. can be set to True interactively
##     mtplot.Hist('Data - DY - TT').on = True

##     mtplot.Hist('WJets').layer = -999999

##     #PG calculating error
##     #PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    
##     #PG statistics
##     scale_WJets_error = sqsum (data_error / data_integral, wjets_error / wjets_integral)

##     #PG systematics on the subtracted fakes
##     lepTau_fakerate_error = 0.3 
##     if channel == 'TauEle' : lepTau_fake_rate = 0.2
##     jetTau_fakerate_error = 0.2
##     if removingFakes == True:
##         rel_ZL_error = lepTau_fakerate_error * mtplot.Hist('Ztt_ZL').Integral() / mtplot.Hist('Data').Integral() 
##         scale_WJets_error = sqsum (scale_WJets_error, rel_ZL_error)
##         rel_ZJ_error = jetTau_fakerate_error * mtplot.Hist('Ztt_ZJ').Integral() / mtplot.Hist('Data').Integral() 
##         scale_WJets_error = sqsum (scale_WJets_error, rel_ZJ_error)

##     #PG systematics on the suctracted DY
##     #PG FIXME normalization error is missing, as not considered in the analysis by now

##     #PG systematics on the suctracted TT
##     TTbar_error = 0.08 #PG FIXME this has to be determined and assigned somewhere
##                        #PG FIXME comes from the error on the cross-section
##                        #PG FIXME and the one on the scale factor used
##                        #PG FIXME which could be added to the sample?
##                        #PG FIXME remember that the scale factor used depends on the jet binning in 2012:
##                        #PG FIXME need to find a way to cope with that
##                        #PG FIXME (Valentina says that the error on the cross-section covers everything)
##     scale_WJets_error = sqsum (scale_WJets_error, TTbar_error * mtplot.Hist('TTJets').Integral() / mtplot.Hist('Data').Integral())

##     #PG transform the error into absolute one for the function output
##     scale_WJets_error = scale_WJets * scale_WJets_error

##     return scale_WJets, scale_WJets_error
    
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
    

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
    
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
    

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

    # import pdb; pdb.set_trace()
    data_OS, mc_OS = fW( mtOS, 'Data', xmin, xmax)
    fW_OS = data_OS / mc_OS


    print 'extracting WJets data/MC factor in high mt region, SS'
    print sscut 
    mtSS = H2TauTauDataMC(var, anaDir, comps, weights,
                          nbins, xmin, xmax,
                          cut = sscut, weight=weight,
                          embed=embed, treeName=treeName)
    if VVgroup != None :
        mtSS.Group ('VV',VVgroup)
        
    data_SS, mc_SS = fW( mtSS, 'Data', xmin, xmax)
    fW_SS = data_SS / mc_SS

    
    print 'fW_SS=',fW_SS,'fW_OS=',fW_OS
 
    return fW_SS, fW_OS, mtSS, mtOS

