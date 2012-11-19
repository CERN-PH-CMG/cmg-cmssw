#!/usr/bin/env python

##
## This script is used to perform the pseudo-esperiments on the workspace obtained
## from "FitSecVtxDistributions.cxx"
##
## * pass a workspace to this script
## * will perform the pseudo experiments
## * store the calibration histogram to a root file
## * fit the root file with a linear function
## * correct the fit function with the calibration obtained
## * fit the data to extract the top mass
##
##
##

import os
import optparse
import sys
from array import array
import ROOT
from ROOT import *

def openTFile(path, option='r'):
    f =  ROOT.TFile.Open(path,option)
    if not f.__nonzero__() or not f.IsOpen():
        raise NameError('File '+path+' not open')
    return f

def getWorkspace(file,name):
    print ' * getting workspace '+name
    ## open the TFile
    ifile = openTFile(file)
    ifile.ls()
    ## read the workspace
    w = ifile.Get(name)
    #w.Print()
    return w


def getAllKeys(f,dir = ""):
    f.cd(dir)
    return [key.GetName() for key in gDirectory.GetListOfKeys()]


def getAllChannels(f):
    keys = getAllKeys(f,'')
    return set([k.split('_')[2] for k in keys])
    
def getAllMasses(f):
    keys = getAllKeys(f,'')
    return set([k.split('_')[3] for k in keys])
    


def getHist(file, hist):
    h = file.Get(hist)
    if not h.__nonzero__():
        raise NameError('Histogram '+hist+' doesn\'t exist in '+str(file))
    return h        

massbindict = {
    '1615':1,
    '1635':2,
    '1665':3,
    '1695':4,
    '1725':5,
    '1755':6,
    '1785':7,
    '1815':8,
    '1845':9,
    }


## def getCalibration(allmassfits):
##     ## split in different channels
##     for chan in channels:
##         c_bias_summary = TCanvas('c_bias_summary_'+chan,'c_bias_summary_'+chan,400,400)
##         ## one histogram per channel
##         ## get all mass points for the channel
##         allfits = set([f for f in allmassfits if chan in f])
##         ## FIXME: get the binning from the masses automatically => now hardcoded
##         binning = [160.5,162.5,165,168,171,174,177,180,183,186]
##         hist = TH1F('bias_summary','bias_summary',len(binning)-1,array('d',binning))
        
##         for tag in allfits:
##             print tag
##             mass = tag.split('_')[-1]
##             m = float(mass)/10.
##             hist.SetBinContent(massbindict[mass],allmassfits[chan+'_'+mass].histograms[chan+'_'+mass+'_bias'].GetMean())
##             hist.SetBinError(massbindict[mass],allmassfits[chan+'_'+mass].histograms[chan+'_'+mass+'_bias'].GetMeanError())

##         ## perform a linear fit to the histogram
##         lfunc = TF1('linfunc','[0]+[1]*x',160,185)
##         lfunc.SetLineWidth(3)
##         lfunc.SetLineColor(2)
##         lfunc.SetParameters(0.,0.)
##         hist.Fit('linfunc')
##         a = lfunc.GetParameter(0)
##         b = lfunc.GetParameter(1)
##         hist.Draw('e1')
##         lfunc.Draw('lsame')

##         c_bias_summary.Print('c_bias_summary_'+chan+'.pdf')
##         c_bias_summary.Print('c_bias_summary_'+chan+'.png')
        
##         return [a,b]


    #def makeDataFit():









class toyResult():
    def __init__(self):
        self.variable = 'lxy'
        self.channel = ''
        self.mass = ''
        self.histograms = {}
        self.biasHistos = []
        self.massHistos = []

        ## self.nSamples = 10
        ## self.nToys = 10000


    def doCalibrationStep(self,workspace,nSamples,nToys,fittype,floatSignal):
        ## get the model for the fit, the signal and the background
        mod_fit  = workspace.pdf(self.channel+'model')
        mod_bkg  = workspace.pdf(self.channel+self.variable+'_bkg')
        mod_sig  = workspace.pdf(self.channel+'f'+self.variable+'_'+self.channel+self.mass)
        mod_25q  = workspace.function(self.channel+'25qInv')
        mod_50q  = workspace.function(self.channel+'50qInv')
        mod_mean = workspace.function(self.channel+'meanInv')
        
        ## the top mass and the relative signal fractions are not constant
        ## define the range of mtop
        workspace.var('mtop').setConstant(kFALSE)
        workspace.var('mtop').setRange(140,200) 
        ## define the range of the signal fraction
        ## workspace.var(self.channel+'sigfrac').setConstant(kTRUE)
        ## workspace.var(self.channel+'sigfrac').setRange(0.,1.)
        ## make sure lxy is not constant
        var = w.var(self.variable)
        var.setConstant(kFALSE)
        var.setRange(0.,5.) ## FIXME: range can be variable

        ## sum the signal and background pdfs to use for the generation step
        sigfrac = workspace.var(self.channel+'sigfrac')
        if(not floatSignal) : sigfrac.setConstant(kTRUE) 
        mod_gen = RooAddPdf('s+b_model_'+self.channel+'_'+self.mass,'s+b_model_'+self.channel+'_'+self.mass,RooArgList(mod_sig,mod_bkg),RooArgList(sigfrac))

        getattr(w,'import')(mod_gen)
        ## FIXME is this needed?
        
        ## set some of the variables constant and define the correct ranges
        print ' * initializing variables'
        workspace.var(self.channel+'alpha1').setConstant(kTRUE)
        workspace.var(self.channel+'alpha2').setConstant(kTRUE)
        workspace.var(self.channel+'alpha3').setConstant(kTRUE)
        workspace.var(self.channel+'alpha4').setConstant(kTRUE)
        workspace.var(self.channel+'beta1').setConstant(kTRUE)
        workspace.var(self.channel+'beta2').setConstant(kTRUE)
        workspace.var(self.channel+'thr').setConstant(kTRUE)
        workspace.var(self.channel+'wid').setConstant(kTRUE)

        workspace.var(self.channel+'wid_bkg').setConstant(kTRUE)
        
        ## change here for the closure test...
        #rooargs = RooArgSet(workspace.var(self.variable))
        #mc = RooMCStudy(mod_gen, rooargs,
        ## mc = RooMCStudy(mod_sig, rooargs,
        #            RooCmdArg(RooFit.FitModel(mod_fit)),
        #            RooCmdArg(RooFit.Binned(kTRUE)),
        #            RooCmdArg(RooFit.Silence()),
        #            #RooCmdArg(RooFit.Extended()),
        #    RooCmdArg(RooFit.FitOptions(RooFit.Save(kTRUE), RooFit.PrintEvalErrors(0) ) )
        #) 
        #print ' * starting the generation and fitting procedure'
        #mc.generateAndFit(nSamples,nToys)
        #self.makeControlPlots(mc,workspace)
        ## keep the histogram for the summary plot
        #hist = self.makeBiasPlot(mc,nSamples)
        #self.biasHistos.append(hist)

        #save the default parameters
        nullParams = workspace.allVars().snapshot()
        workspace.saveSnapshot("default",nullParams,kTRUE)
        
        mtop=workspace.var('mtop')
        observable=workspace.var(self.variable)
        modelConfig=None
        if(fittype.find('plr')>=0) :
            modelConfig=RooStats.ModelConfig("mc",workspace)
            modelConfig.SetPdf(mod_fit)
            modelConfig.SetParametersOfInterest(RooArgSet(mtop))
            modelConfig.SetObservables(RooArgSet(observable))
            modelConfig.SetNuisanceParameters(RooArgSet(sigfrac))

        #run the pseudo-experiments
        resultsSummary={}
        resultsSummary["fit"]=[]
        resultsSummary["mean"]=[]
        resultsSummary["q25"]=[]
        resultsSummary["q50"]=[]
        import array
        xq=array.array('d',[0.25,0.5,0.75,1])
        yq=array.array('d',[0,   0,  0,   0])
        for toy in xrange(0,nSamples):

            #load the defaults
            workspace.loadSnapshot("default")

            #generate (binned or not)
            toyData=None
            if(fittype=='plr' or fittype=='ll'):
                toyData=mod_gen.generateBinned(RooArgSet(observable),nToys)
            else:
                toyData=mod_gen.generate(RooArgSet(observable),nToys)

            #momenta of the distribution
            binnedData=toyData.createHistogram("tmp",observable)

            observable.setVal(binnedData.GetMean())
            res=mod_mean.getVal()
            observable.setVal(binnedData.GetMean()+binnedData.GetMeanError())
            eHigh=mod_mean.getVal()-res
            observable.setVal(binnedData.GetMean()-binnedData.GetMeanError())
            eLow=res-mod_mean.getVal()
            eStat=0.5*(fabs(eLow)+fabs(eHigh))
            pull  = (res-float(self.mass)/10)/eStat
            resultsSummary["mean"].append([res,eStat,pull,eHigh,eLow])
            
            binnedData.GetQuantiles(4,yq,xq)
            observable.setVal(yq[0])
            resultsSummary["q25"].append([mod_25q.getVal(),0,0,0,0])
            observable.setVal(yq[1])
            resultsSummary["q50"].append([mod_50q.getVal(),0,0,0,0])

            #fit
            if(fittype.find('plr')>=0):
                pl=RooStats.ProfileLikelihoodCalculator(toyData,modelConfig)
                pl.SetConfidenceLevel(0.68)
                interval = pl.GetInterval();
                res   = modelConfig.GetParametersOfInterest().first().getVal()
                eLow  = res-interval.LowerLimit(mtop)
                eHigh = interval.UpperLimit(mtop)-res
            else:
                mod_fit.fitTo(toyData)
                res   = mtop.getVal()
                eLow  = mtop.getErrorLo()
                eHigh = mtop.getErrorHi()
            eStat = 0.5*(fabs(eLow)+fabs(eHigh))
            pull  = (res-float(self.mass)/10)/eStat
            resultsSummary["fit"].append([res,eStat,pull,eHigh,eLow])

        #display the results
        for method in resultsSummary.items() :
            biasH,pullH=self.makeControlPlots(method[0],method[1])
            if(method[0]=='fit') : self.biasHistos.append(biasH)


    def makeControlPlots(self,method,resultsSummary):

        #define and fill the histograms
        fitResH  = ROOT.TH1F(method+'res_'+self.channel+'_'+self.mass,        ';Fitted m_{top} [GeV];Toys / (1 GeV)',                    50,  150,   200)
        biasH    = ROOT.TH1F(method+'bias_histo_'+self.channel+'_'+self.mass, ';Fitted m_{top}-Generated m_{top} [GeV];Toys / 0.5 GeV',  50,  -12.25, 12.75)
        asymErrH = ROOT.TH1F(method+'asymerr_'+self.channel+'_'+self.mass,    ';#sigma_{m_{top}} [GeV];Toys / (0.25 GeV)',               100,  -12.375, 12.625)
        errH     = ROOT.TH1F(method+'err_'+self.channel+'_'+self.mass,        ';#sigma_{m_{top}} [GeV];Toys / (0.5 GeV)',                25,  0,     12.5)
        pullH    = ROOT.TH1F(method+'pull_'+self.channel+'_'+self.mass,       ';Pull;Toys / (0.05)',                                     50,  -2.45, 2.55)

        for r in resultsSummary:
            fitResH.Fill(r[0])
            biasH.Fill(r[0]-float(self.mass)/10)
            errH.Fill(r[1])
            pullH.Fill(r[2])
            asymErrH.Fill(+fabs(r[3]))
            asymErrH.Fill(-fabs(r[4]))

        #show the results
        c_mtop = TCanvas('c_mtop','c_mtop',600,600)
        c_mtop.Divide(2,2)
        c_mtop.cd(1)
        fitResH.Draw()  
        c_mtop.cd(2)
        asymErrH.Draw()
        c_mtop.cd(3)
        errH.Draw()  
        c_mtop.cd(4)
        pullH.Draw()
        c_mtop.Update()
        c_mtop.Print('c_mtop_'+method+'_'+self.channel+'_'+self.mass+'.pdf')
        c_mtop.Print('c_mtop_'+method+'_'+self.channel+'_'+self.mass+'.png')

        c_bias = TCanvas('c_bias_'+self.channel+'_'+self.mass,'c_bias_'+self.channel+'_'+self.mass,400,400)
        biasH.Draw()
        c_bias.Print('c_bias_'+method+'_'+self.channel+'_'+self.mass+'.pdf')
        c_bias.Print('c_bias_'+method+'_'+self.channel+'_'+self.mass+'.png')

        return biasH,pullH
 
#    def makeBiasPlot(self,mc,nSamples):
#        print 'making bias histogram'
#        ## loop over the results from the toy experiments
#        ## and fill a histogram with the difference of the fit result to
#        ## the generated top mass
#        hist = ROOT.TH1F('bias_histo_'+self.channel+'_'+self.mass,'bias_histo_'+self.channel+'_'+self.mass,60,-15,15)
#
#        ## some cosmetics
#        hist.GetXaxis().SetTitle('m_{fit}-m_{gen} [GeV]')
#        hist.GetYaxis().SetTitle('1 / 0.5 GeV')
#
#        gen_mass = float(self.mass)/10.
#        for n in xrange(nSamples):
#            res_val = self.getResult(mc,'mtop',n)
#            #res_val = 
#            hist.Fill(res_val-gen_mass)
#        ## and save it as a picture
#        c_bias_hist = TCanvas('c_bias_hist_'+self.channel+'_'+self.mass,'c_bias_hist_'+self.channel+'_'+self.mass,400,400)
#        hist.Draw()
#        c_bias_hist.Print('c_bias_hist_'+self.channel+'_'+self.mass+'.pdf')
#        c_bias_hist.Print('c_bias_hist_'+self.channel+'_'+self.mass+'.png')
#        return hist
#
#    def getResult(self,mc,var,n):
#        try:
#            val = mc.fitResult(n).floatParsFinal().find(var).getValV()
#        except (RuntimeError,ValueError):
#            print 'Oops!'
#            val = -1
#        return val



#    def makeControlPlots(self,mc,workspace):
#        print ' * making control hitograms'
#        ## first mtop...
#        c_mtop = TCanvas('c_mtop','c_mtop',600,300)
#        c_mtop.Divide(2,1)
#        ## get the results of the MC study
#        mtop_frame = mc.plotParam(workspace.var('mtop'),RooFit.Bins(40))
#        c_mtop.cd(1)
#        mtop_frame.Draw()
#        mtop_error_frame = mc.plotError(workspace.var('mtop'),RooFit.Bins(40))
#        c_mtop.cd(2)
#        mtop_error_frame.Draw() 
#        ## FIXME: pull doesn't work....
#        ## mtop_pull_frame = mc.plotPull(mtop,-10,10,RooFit.Bins(50),RooFit.FitGauss(kTRUE))
#        ## mtop_pull_frame = mc.plotPull(mtop)
#        ## c_mtop.cd(3)
#        ## mtop_pull_frame.Draw()
#        c_mtop.Update()
#        c_mtop.Print('c_mtop_'+self.channel+'_'+self.mass+'.pdf')
#        c_mtop.Print('c_mtop_'+self.channel+'_'+self.mass+'.png')
# 
#        ## plot signal fraction information
#        c_sigfrac = TCanvas('c_sigfrac','c_sigfrac',900,300)
#        c_sigfrac.Divide(3,1)
#        ## get the results of the MC study
#        sigfrac_frame = mc.plotParam(workspace.var(self.channel+'sigfrac'),RooFit.Bins(40))
#        c_sigfrac.cd(1)
#        sigfrac_frame.Draw()
#        sigfrac_error_frame = mc.plotError(workspace.var(self.channel+'sigfrac'),RooFit.Bins(40))
#        c_sigfrac.cd(2)
#        sigfrac_error_frame.Draw()        
#        sigfrac_pull_frame = mc.plotPull(workspace.var(self.channel+'sigfrac'),RooFit.Bins(40),RooFit.FitGauss(kTRUE))
#        c_sigfrac.cd(3)
#        sigfrac_pull_frame.Draw()
#        c_sigfrac.Update()
#        c_sigfrac.Print('c_sigfrac_'+self.channel+'_'+self.mass+'.pdf')
#        c_sigfrac.Print('c_sigfrac_'+self.channel+'_'+self.mass+'.png')
#
## FIXME: read masses from command line
masses = [ '1615','1635','1665','1695','1725','1755','1785','1815','1845' ]
#masses = ['1695','1725'] 
## FIXME: read channels from command line
channels = ['mu']


def main():

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-i', '--inputfile' ,    dest='inputfile'      , help='Name of the local input file.'          , default=None)
    parser.add_option('-r', '--rootfile'  ,    dest='rootfile'       , help='Name of the root file.'                 , default=None)
    parser.add_option('-c', '--channel'   ,    dest='channel'        , help='If only one channel is processed.'      , default=None)
    parser.add_option('-f', '--fit'       ,    dest='fittype'        , help='Choose fit type ll,ull,plr,uplr'        , default='ll')
    parser.add_option('-S', '--floatsignal',   dest='floatsignal'    , help='Signal is left to float in the fit'     , default=False,  action="store_true")
    parser.add_option('-m', '--mass'      ,    dest='mass'           , help='If only one mass is processed.'         , default=None)
    parser.add_option('-n', '--ntoys'     ,    dest='ntoys'          , help='Number of toy experiments.'             , default=25      ,type = int)
    parser.add_option('-s', '--samplesize',    dest='samplesize'     , help='Number of events per toy experiment.'   , default=50000   ,type = int)

    (opt, args) = parser.parse_args()

    print '''
                          __ _ _   _            
                         / _(_) | | |           
 _ __ ___   __ _ ___ ___| |_ _| |_| |_ ___ _ __ 
| '_ ` _ \ / _` / __/ __|  _| | __| __/ _ \ '__|
| | | | | | (_| \__ \__ \ | | | |_| ||  __/ |   
|_| |_| |_|\__,_|___/___/_| |_|\__|\__\___|_|   
                                                '''
    print 'Chosen operaion: ',sys.argv[1]
    if sys.argv[1] not in ['all', 'toys', 'calib', 'fit']:
        print 'Need to specify a valid operation! Possibilities are: \'all\' \'toys\', \'calib\', \'fit\' '
        sys.exit(2)


    if opt.inputfile is None:
        parser.error('No input file defined! (This has to be a workspace)')
    if opt.channel is not None:
        channels = [opt.channel]
    if opt.mass is not None:
        masses = [opt.mass]
    else:
        masses = [ '1615','1635','1665','1695','1725','1755','1785','1815','1845' ]

    if sys.argv[1] == 'all' and len(masses) == 1:
        print 'It\'s not possible to run \'all\' for only one mass point!'
        sys.exit(2)


    ## FIXME: add this flag from the command line 
    ## run ROOT in batch mode
    ## sys.argv.append( '-b' )
    ## ROOT.gROOT.SetBatch()

    inputfile = opt.inputfile

    ## run the toy experiments
    
    if sys.argv[1] == 'all' or sys.argv[1] == 'toys':
        ## get the workspace form the root file
        workspace = getWorkspace(inputfile,'w')

        ## prepare a container for the toyResults
        ## a dict sorted according to channel and mass
        allFitResults = {}

        ## loop over all channels and mass points
        for chan in channels:
            for mass in masses:
                thisResult = toyResult()
                thisResult.variable = 'lxy'
                thisResult.channel = chan
                thisResult.mass = mass
                thisResult.doCalibrationStep(workspace,opt.ntoys,opt.samplesize,opt.fittype,opt.floatsignal)
                print thisResult.biasHistos
                tag = thisResult.channel+'_'+thisResult.mass
                allFitResults[tag] = thisResult

        ## save the bias hitograms in a root file
        if len(channels) == 1 and len(masses) == 1:
            biasoutfileName = 'bias_hists_'+channels[0]+'_'+masses[0]+'.root'
        elif len(channels) == 1:
            biasoutfileName = 'bias_hists_'+channels[0]+'.root'
        elif len(masses) == 1:
            biasoutfileName = 'bias_hists_'+masses[0]+'.root'
        else:
            biasoutfileName = 'bias_hists.root'

        biasoutfile = ROOT.TFile().Open(biasoutfileName,'RECREATE')
        for tag in allFitResults:
            print tag
            allFitResults[tag].biasHistos[0].Write()
        biasoutfile.Close()



    ## second step
    if sys.argv[1] == 'all' or sys.argv[1] == 'calib':
        if sys.argv[1] == 'calib' and opt.rootfile is None:
            parser.error('No input root file defined! (This has to contain ALL bias histograms)')

        if sys.argv[1] == 'calib':
            rootfile = opt.rootfile
        elif sys.argv[1] == 'all':
            rootfile = biasoutfileName

        ## open the file containing the bias histograms
        rfile = openTFile(rootfile)
        channels = getAllChannels(rfile)
        masses   = getAllMasses(rfile)
        print masses
        print channels

        ## make the bias summary plot
        for chan in channels:
            c_bias_summary = TCanvas('c_bias_summary_'+chan,'c_bias_summary_'+chan,400,400)
            binning = [160.5,162.5,165,168,171,174,177,180,183,186]
            h_bias = TH1F('h_bias_summary_'+chan,'h_bias_summary_'+chan,len(binning)-1,array('d',binning))
        
            for mass in masses:
                ROOT.gStyle.SetOptFit(1)
                ROOT.gStyle.SetOptStat(0)
                ROOT.gStyle.SetOptTitle(0)

                c_hist_fit = TCanvas('c_hist_fit'+chan,'c_hist_fit_'+chan,400,400)

                hist = getHist(rfile, 'bias_histo_'+chan+'_'+mass)
                hist.SetTitle('m_{gen} = '+str(float(mass)/10.)+' GeV, '+chan+' channel')
                hist.SetMarkerStyle(20)
                hist.SetMarkerColor(1)
                hist.SetLineColor(1)


                ## FIXME: CMS Preliminary is not working...
                l = ROOT.TLatex()
                l.SetNDC()
                l.SetTextAlign(22)
                anchorCMS = [0.15,0.04]
                l.SetTextSize(0.9*l.GetTextSize())
                l.DrawLatex(ROOT.gPad.GetLeftMargin()+anchorCMS[0],1-ROOT.gPad.GetTopMargin()-anchorCMS[1],'CMS Preliminary')

                gfunc = TF1('f1','gaus',-20.,20)
                gfunc.SetLineWidth(3)
                gfunc.SetLineColor(4)
                gfunc.SetParameters(50.,0.,5.)
                ## p0: height, p1: mean, p2: width, 
                hist.Fit('f1')
                mean = gfunc.GetParameter(1)
                width = gfunc.GetParameter(2)
                hist.Draw('e1')
                gfunc.Draw("same")

                ## plot a vertical line at zero
                vline = ROOT.TGraph()
                vline.SetPoint(0,0,10000)
                vline.SetPoint(1,0,-100)
                vline.SetLineWidth(2)
                vline.SetLineColor(2)
                vline.SetLineStyle(2)
                vline.Draw('L')



                leg = ROOT.TLegend(0.55,0.6,0.89,0.75)
                leg.SetFillColor(0)
                leg.SetLineColor(0)
                leg.SetHeader('m_{gen} = '+str(float(mass)/10.)+' GeV, '+chan+' channel')
                leg.AddEntry(hist,'MC toy experiment','p')
                leg.AddEntry(gfunc,'Gaussian fit','l')
                leg.Draw()
                
                c_hist_fit.Print('c_bias_fitted_'+chan+'_'+mass+'.pdf')
                c_hist_fit.Print('c_bias_fitted_'+chan+'_'+mass+'.png')

                
                m = float(mass)/10.
                ## h_bias.SetBinContent(massbindict[mass],hist.GetMean())
                ## h_bias.SetBinError(massbindict[mass],hist.GetMeanError())
                h_bias.SetBinContent(massbindict[mass],mean)
                h_bias.SetBinError(massbindict[mass],width)

            ## write gbias summary to a file
            outfile = ROOT.TFile().Open('bias_summary_'+chan+'.root','RECREATE')
            h_bias.Write()
            outfile.Close()

            h_bias.Draw()
            
            
    ## third step
            
    if sys.argv[1] == 'all' or sys.argv[1] == 'fit':
        for chan in channels:
            c_bias_fitted = TCanvas('c_bias_fitted_'+chan,'c_bias_fitted_'+chan,400,400)

            if sys.argv[1] == 'all':
                rootfile = 'bias_summary_'+chan+'.root'
            else:
                rootfile = opt.rootfile
            rfile = openTFile(rootfile)
            h_bias = getHist(rfile,'h_bias_summary_'+chan)
            h_bias.GetXaxis().SetTitle('m_{gen} [GeV]')
            h_bias.GetYaxis().SetTitle('m_{fit toys} - m_{gen} [GeV]')
            h_bias.SetMarkerStyle(20)

            print '******************************************* fitting'


            ROOT.gStyle.SetOptTitle(0)
            ROOT.gStyle.SetOptStat(0)
            ROOT.gStyle.SetOptFit(1)

            lfunc = TF1('lf','[0]+[1]*x',165,180)
            lfunc.SetLineWidth(3)
            lfunc.SetLineColor(8)
            lfunc.SetParameters(0.,0.)
            h_bias.Fit('lf')
            a = lfunc.GetParameter(0)
            b = lfunc.GetParameter(1)
            h_bias.SetMaximum(5)
            h_bias.SetMinimum(-5)
            h_bias.Draw('e1')
            lfunc.Draw('lsame')


            l = ROOT.TLatex()
            l.SetNDC()
            l.SetTextAlign(22)
            anchorCMS = [0.15,-0.04]
            l.SetTextSize(0.9*l.GetTextSize())
            l.DrawLatex(ROOT.gPad.GetLeftMargin()+anchorCMS[0],1-ROOT.gPad.GetTopMargin()-anchorCMS[1],'CMS Preliminary')
            
            ## r = ROOT.TLatex()
            ## r.SetNDC()
            ## r.SetTextAlign(22)
            ## anchorCMS = [0.15,-0.04]
            ## r.SetTextSize(0.9*l.GetTextSize())
            ## r.DrawLatex(ROOT.gPad.GetLeftMargin()+0.5,1-ROOT.gPad.GetTopMargin()-0.1,'%.2f' % a+' + %.2f' % b+'m_{gen}' )
            

            ## plot a line at zero
            line = ROOT.TGraph()
            line.SetPoint(0,-200,0)
            line.SetPoint(1,1500,0)
            line.SetLineWidth(2)
            line.SetLineColor(2)
            line.SetLineStyle(2)
            line.Draw('L')

            c_bias_fitted.Print('c_bias_fitted_'+chan+'.pdf')
            c_bias_fitted.Print('c_bias_fitted_'+chan+'.png')

            print'*********************************************'
            print 'Calibration bias fit result:'
            print 'liner function: '+str(a)+' + '+str(b)+' * mtop'


            ## calculate the statistical error on the top mass
            ## get the average width of the bias fits

            errorbars = [h_bias.GetBinError(i) for i in range(1,len(masses)) if h_bias.GetBinError(i) > 0.]
            print errorbars
            calib_error = sum(errorbars)/float(len(errorbars))
            print 'average error on bias: +-'+str(calib_error)
            

            workspace = getWorkspace(inputfile,'w')

            ## workspace.Print()

            mtop = workspace.var('mtop')
            mtop.setConstant(kFALSE)
            mtop.setRange(140,200)
            lxy = workspace.var('lxy')
            lxy.setConstant(kFALSE)
            lxy.setRange(0.,5.)

            workspace.var(''+chan+'alpha1').setConstant(kTRUE)
            workspace.var(''+chan+'alpha2').setConstant(kTRUE) 
            workspace.var(''+chan+'alpha3').setConstant(kTRUE) 
            workspace.var(''+chan+'alpha4').setConstant(kTRUE) 
            workspace.var(''+chan+'beta1').setConstant(kTRUE) 
            workspace.var(''+chan+'beta2').setConstant(kTRUE)  
            workspace.var(''+chan+'thr').setConstant(kTRUE)    
            workspace.var(''+chan+'wid').setConstant(kTRUE)    
            workspace.var(''+chan+'wid_bkg').setConstant(kTRUE)
            workspace.var(''+chan+'sigfrac').setConstant(kTRUE)

            c_data_fit = TCanvas('c_data_fit_'+chan,'c_data_fit_'+chan,400,400)

            ## now, retrieve the data from the workspace
            data = workspace.data(chan+'data')
            data.Print("v")
            
            pa = RooRealVar('pa','pa',a)
            pa.setConstant(kTRUE)
            pb = RooRealVar('pb','pb',b+1)
            pb.setConstant(kTRUE)

            mtopcalib = RooFormulaVar('mtopcalib','mtopcalib','@0+@1*@2',RooArgList(pa,pb,mtop))
            getattr(workspace,'import')(mtopcalib)
            newmodel = workspace.factory("EDIT::new"+chan+"model("+chan+"model,mtop=mtopcalib)")
            ## getattr(workspace,'import')(newmodel)
            

            ROOT.gStyle.SetOptStat(0)
            ROOT.gStyle.SetOptFit(1)

            newframe = lxy.frame()
            workspace.pdf('new'+chan+'model').fitTo(data,RooCmdArg(RooFit.FitOptions(RooFit.Save(kTRUE),RooFit.SumW2Error(kTRUE))))

            fit_res = mtop.getVal()
            fit_error = mtop.getError()
            stat_error = mtop.getError() * calib_error
            
            print '--------------------------'
            print 'Fit Results:'
            print '--------------------------'
            print 'mtop: ',fit_res
            print 'fit error: ',fit_error
            print 'calibration error: ',calib_error
            print 'statistical error: ',stat_error
            print '--------------------------'


            data.plotOn(newframe)
            workspace.pdf(chan+'model').plotOn(newframe)
            workspace.pdf('new'+chan+'model').plotOn(newframe)
            newframe.Draw()

            ## print the top mass on the plot
            pave = TPaveText(0.3,0.6,0.9,0.8,"NDC")
            pave.SetBorderSize(0);
            pave.SetFillStyle(0);
            pave.SetTextAlign(12);
            pave.SetTextFont(42);
            topmass = 'm_{Top} = '+str("%.2f" % fit_res)+' #pm '+str("%.2f" % stat_error)+' (stat.) GeV'
            pave.AddText(topmass);
            pave.Draw();


        c_data_fit.Print('c_data_fit_'+chan+'.pdf')
        c_data_fit.Print('c_data_fit_'+chan+'.png')
        
        











if __name__ == '__main__':
    main()


    ## possibilities:
    ##
    ## * only calibration step => FIRST root file => needs the workspace
    ## * only calculating the calibration function => SECOND root file => needs a FIRST root file 
    ## * only the fit to data => needs SECOND root file & workspace
    ##
    ##
    ##
    ##
    ##
    ##
    ##
    ##
    ##
    ##
    ##
    ##
    ##












