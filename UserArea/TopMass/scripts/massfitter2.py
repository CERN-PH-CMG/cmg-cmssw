#!/usr/bin/env python

##
## This script is used to perform the pseudo-experiments on the workspace obtained
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
from math import sqrt,pow
import ROOT
from ROOT import *

def drawCMSHeader() :
    l = ROOT.TPaveText(0.1,0.95,0.9,0.99,'brNDC')
    l.SetTextAlign(12)
    l.SetBorderSize(0)
    l.SetFillStyle(0)
    l.AddText('CMS preliminary')
    l.Draw()
    

def openTFile(path, option='r'):
    print 'Opening '+path+'...'
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


def getAllKeys(f,dir = "", match=''):
    f.cd(dir)
    return [ key.GetName() for key in gDirectory.GetListOfKeys() if str(key.GetName()).find(match)>=0 ]


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

class toyResult():
    def __init__(self):
        self.variable = 'lxy'
        self.channel = ''
        self.mass = ''
        self.histograms = {}
        #self.biasHistos = []
        self.biasHistos = {}
        self.massHistos = []
        self.bias = {}
        self.biasError = {}
        self.toyavgHisto = None
        self.toydiffHisto = None
        self.integral_templ = 0.
        # self.integral_toys

    def doCalibrationStep(self,workspace,nToys,nSamples,fittype,floatSignal,outdir):
        ## get the model for the fit, the signal and the background

        #check inputs
        if(nSamples<=0):
            data = workspace.data(self.channel+'data')
            nSamples=data.sumEntries()
        print 'Generating %d toys of %d events for %s'%(nToys,nSamples,self.channel)
            
        #pdfs and parameterizations (signal systs are hardcoded as masses)
        sigfrac = workspace.var(self.channel+'sigfrac')
        sigfrac.setConstant(kTRUE)
        mod_sig  = workspace.pdf(self.channel+'lxy_1725')
        if self.mass not in masses:
            syst = self.mass
            self.mass = '1725'
            if syst.find('dy')<0  :
                mod_sig  = workspace.pdf(self.channel+self.variable+'_'+syst+'syst') # mulxy_mepsyst
            else :
                kfactor=1.0
                chfactor=0.24
                if self.channel == 'emu' : chfactor=0.04
                if syst.find('dyup')>=0   : kfactor = 1.0+0.13*chfactor
                if syst.find('dydown')>=0 : kfactor = 1.0-0.13*chfactor
                sigfrac.setVal( sigfrac.getVal()*kfactor )
        else:
            mod_sig  = workspace.pdf(self.channel+self.variable+'_'+self.mass)
        mod_bkg  = workspace.pdf(self.channel+self.variable+'_bckg')
        mod_gen = RooAddPdf('s+b_model_'+self.channel+'_'+self.mass,'s+b_model_'+self.channel+'_'+self.mass,RooArgList(mod_sig, mod_bkg),RooArgList(sigfrac))
        getattr(workspace,'import')(mod_gen)
        
        mod_25q  = workspace.function(self.channel+'25qInv')
        mod_50q  = workspace.function(self.channel+'50qInv')
        mod_mean = workspace.function(self.channel+'meanInv')
        
        #for the fit will let mtop float
        workspace.var('mtop').setConstant(kFALSE)
        workspace.var('mtop').setRange(140,200)

        #save default values
        nullParams = workspace.allVars().snapshot()
        nullParams.Print()
        workspace.saveSnapshot("default",nullParams,kTRUE)

        print 'Model and parameterizations retrieved from workspace. Signal fraction is %f'%(sigfrac.getVal())
        print 'Mean: toy model %3.4f  signal %3.4f  background %3.4f'%(mod_gen.mean(workspace.var(self.variable)).getVal(),
                                                                       mod_sig.mean(workspace.var(self.variable)).getVal(),
                                                                       mod_bkg.mean(workspace.var(self.variable)).getVal())
        
        ## control plot for the generation step
        c_toy_templ = TCanvas('c_toy_templ_'+self.channel+'_'+self.mass,'c_toy_templ_'+self.channel+'_'+self.mass,600,600)
        toy_templ_frame = workspace.var(self.variable).frame()
        mod_gen.plotOn(toy_templ_frame,RooFit.LineStyle(1),RooFit.LineWidth(1))
        mod_gen.plotOn(toy_templ_frame,RooFit.Components(self.channel+self.variable+'_bckg'),RooFit.LineStyle(2),RooFit.LineWidth(1))
        toy_templ_frame.Draw()
        c_toy_templ.Print(outdir+'/c_toy_templ_'+self.channel+'_'+self.mass+'.png')
        
        #configure for pseudo-experiments
        mtop=workspace.var('mtop')
        observable=workspace.var(self.variable)
        modelConfig=None
        if(fittype.find('plr')>=0) :
            modelConfig=RooStats.ModelConfig("mc",workspace)
            modelConfig.SetPdf(mod_fit)
            modelConfig.SetParametersOfInterest(RooArgSet(mtop))
            modelConfig.SetObservables(RooArgSet(observable))
            #modelConfig.SetNuisanceParameters(RooArgSet(sigfrac))
        resultsSummary={}
        resultsSummary["fit"]=[]
        resultsSummary["mean"]=[]
        resultsSummary["q25"]=[]
        resultsSummary["q50"]=[]
        xq=array('d',[0.25,0.5,0.75,1])
        yq=array('d',[0,   0,  0,   0])

        self.toyavgHisto  = ROOT.TH1F('toyavg_histo_'+self.channel+'_'+self.mass, ';average from toy [cm];entries/bin',  1000,  0., 2.5)
        self.toydiffHisto = ROOT.TH1F('toydiff_histo_'+self.channel+'_'+self.mass, ';#Delta m_{top} (mean-quantile);entries/bin',  1000,  -2.5, 2.5)
        
        #run pseudo-experiments
        for toy in xrange(nToys):

            #load the defaults
            workspace.loadSnapshot("default")
            workspace.var('mtop').setVal(float(self.mass)/10) 

            #generate (binned or not)
            toyData=None
            if fittype=='plr' or fittype=='ll':
                toyData=mod_gen.generateBinned(RooArgSet(observable),nSamples)
            else:
                toyData=mod_gen.generate(RooArgSet(observable),nSamples)

            # momenta of the distribution
            binnedData=toyData.createHistogram("tmp",observable)
            obsMean=binnedData.GetMean()
            self.toyavgHisto.Fill(obsMean)
            print '[Toy #%d] generated mean: RooDataHist %f  TH1 %f'%(toy+1,toyData.mean(workspace.var(self.variable)),obsMean)

            # control plots for this toy experiment
            # c_toy_exp = TCanvas('c_toy_exp_'+str(toy)+'_'+self.channel+'_'+self.mass,'c_toy_exp_'+str(toy)+'_'+self.channel+'_'+self.mass,600,600)
            # toy_templ_frame = workspace.var(self.variable).frame()
            # toyData.plotOn(toy_templ_frame)
            # mod_gen.plotOn(toy_templ_frame,RooFit.LineStyle(1),RooFit.LineWidth(1))
            # mod_sig.plotOn(toy_templ_frame,RooFit.LineStyle(1),RooFit.LineColor(8),RooFit.LineWidth(1))
            # toy_templ_frame.Draw()
            # c_toy_exp.Print('c_toy_exp_'+str(toy)+'_'+self.channel+'_'+self.mass+'.png')
            # c_toy = TCanvas('c_toy','c_toy',500,500)
            # binnedData.Draw()
            # c_toy.Update()
            # c_toy.Print('toy_distr_'+self.channel+'_'+self.mass+'_'+str(toy)+'.pdf')
            # c_toy.Print('toy_distr_'+self.channel+'_'+self.mass+'_'+str(toy)+'.png')

            observable.setVal(obsMean)
            mtop_mean       = mod_mean.getVal()
            observable.setVal(obsMean+binnedData.GetMeanError())
            mtop_mean_eHigh = mod_mean.getVal()-mtop_mean
            observable.setVal(obsMean-binnedData.GetMeanError())
            mtop_mean_eLow  = mtop_mean-mod_mean.getVal()
            mtop_mean_eStat = 0.5*(fabs(mtop_mean_eLow)+fabs(mtop_mean_eHigh))
            mtop_mean_pull  = (mtop_mean-float(self.mass)/10.)/mtop_mean_eStat
            resultsSummary["mean"].append([mtop_mean,mtop_mean_eStat,mtop_mean_pull,mtop_mean_eHigh,mtop_mean_eLow])
            
            binnedData.GetQuantiles(4,yq,xq)
            observable.setVal(yq[0])
            mtop_25q=mod_25q.getVal()
            resultsSummary["q25"].append([mtop_25q,0,0,0,0])

            observable.setVal(yq[1])
            mtop_50q=mod_50q.getVal()
            resultsSummary["q50"].append([mtop_50q,0,0,0,0])
            self.toydiffHisto.Fill(mtop_mean-mtop_50q)
                              
            #fit
            #if(fittype.find('plr')>=0):
            #    pl=RooStats.ProfileLikelihoodCalculator(toyData,modelConfig)
            #    pl.SetConfidenceLevel(0.68)
            #    interval = pl.GetInterval();
            #    res   = modelConfig.GetParametersOfInterest().first().getVal()
            #    eLow  = res-interval.LowerLimit(mtop)
            #    eHigh = interval.UpperLimit(mtop)-res
            #else:
            #    mod_fit.fitTo(toyData)
            #    res   = mtop.getVal()
            #    eLow  = mtop.getErrorLo()
            #    eHigh = mtop.getErrorHi()
            #eStat = 0.5*(fabs(eLow)+fabs(eHigh))
            #pull  = (res-float(self.mass)/10.)/eStat
            #resultsSummary["fit"].append([res,eStat,pull,eHigh,eLow])

            #remove generated histogram from memory
            binnedData.Delete()

        print 'Preparing toy experiments\' report'
        for method in resultsSummary:
            print method
            biasH, pullH = self.makeControlPlots(method,resultsSummary[method],outdir)
            self.biasHistos[method] = biasH

        
    def makeControlPlots(self,method,resultsSummary,outdir):

        #define and fill the histograms
        fitResH  = ROOT.TH1F(method+'res_'+self.channel+'_'+self.mass,        ';Fitted m_{top} [GeV];Toys / (1 GeV)',                    50,  150,   200)
        biasH    = ROOT.TH1F(method+'bias_histo_'+self.channel+'_'+self.mass, ';Fitted m_{top}-Generated m_{top} [GeV];Toys / 0.5 GeV',  100,  -15., 15)
        asymErrH = ROOT.TH1F(method+'asymerr_'+self.channel+'_'+self.mass,    ';#sigma_{m_{top}} [GeV];Toys / (0.25 GeV)',               100,  -12.375, 12.625)
        errH     = ROOT.TH1F(method+'err_'+self.channel+'_'+self.mass,        ';#sigma_{m_{top}} [GeV];Toys / (0.5 GeV)',                25,  0,     12.5)
        pullH    = ROOT.TH1F(method+'pull_'+self.channel+'_'+self.mass,       ';Pull;Toys / (0.05)',                                     50,  -2.45, 2.55)

        for r in resultsSummary:
            fitResH.Fill(r[0])
            biasH.Fill(r[0]-float(self.mass)/10.)                
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
        c_mtop.Print(outdir+'/c_mtop_'+method+'_'+self.channel+'_'+self.mass+'.pdf')
        c_mtop.Print(outdir+'/c_mtop_'+method+'_'+self.channel+'_'+self.mass+'.png')

        c_bias = TCanvas('c_bias_'+self.channel+'_'+self.mass,'c_bias_'+self.channel+'_'+self.mass,400,400)
        gafunc = TF1('gaus','gaus',-20.,20)
        gafunc.SetLineWidth(3)
        gafunc.SetLineColor(2)
        gafunc.SetParameters(50.,0.,5.)
        ## p0: height, p1: mean, p2: width, 
        biasH.Fit('gaus','Q')
        mean = gafunc.GetParameter(1)
        width = gafunc.GetParError(1)
        biasH.Draw()
        gafunc.Draw("same")
        c_bias.Print(outdir+'/c_bias_'+method+'_'+self.channel+'_'+self.mass+'.pdf')
        c_bias.Print(outdir+'/c_bias_'+method+'_'+self.channel+'_'+self.mass+'.png')

        self.bias[method] = mean
        self.biasError[method] = width

        return biasH,pullH


## FIXME: read masses from command line 
masses = [ '1615','1635','1665','1695','1725','1755','1785','1815','1845' ] 
#masses = ['1695','1725'] 
## FIXME: read channels from command line
channels = ['mu']
systematics = ['mepdown','mepup','q2down','q2up']
#systematics = []
#systematics = ['q2down']
#systematics = ['q2down','q2up']
#systematics = ['mepdown','mepup']

def main():
    
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-i', '--inputfile'  ,    dest='inputfile'  , help='Name of the local input file (This has to be a workspace).' , default=None)
    parser.add_option('-r', '--rootfile'   ,    dest='rootfile'   , help='Name of the root file.'                                     , default=None)
    parser.add_option('-c', '--channel'    ,    dest='channel'    , help='If only one channel is processed.'                          , default=None)
    parser.add_option('-f', '--fit'        ,    dest='fittype'    , help='Choose fit type ll,ull,plr,uplr'                            , default='ll')
    parser.add_option('-S', '--floatsignal',    dest='floatsignal', help='Signal is left to float in the fit'                         , default=False,  action="store_true")
    parser.add_option('-m', '--mass'       ,    dest='mass'       , help='If only one mass is processed.'                             , default=None)
    parser.add_option('-n', '--ntoys'      ,    dest='ntoys'      , help='Number of toy experiments.'                                 , default=25,     type = int)
    parser.add_option('-s', '--samplesize' ,    dest='samplesize' , help='Number of events per toy experiment.'                       , default=0,      type = int)
    parser.add_option('-b', '--batch'      ,    dest='batch'      , help='Use this flag to run root in batch mode.'                   , default=False,  action='store_true')
    parser.add_option('-o', '--outputdir'  ,    dest='outputdir'  , help='Name of the local output directory.'                        , default='.')

    (opt, args) = parser.parse_args()

    print '''
                          __ _ _   _            
                         / _(_) | | |           
 _ __ ___   __ _ ___ ___| |_ _| |_| |_ ___ _ __ 
| '_ ` _ \ / _` / __/ __|  _| | __| __/ _ \ '__|
| | | | | | (_| \__ \__ \ | | | |_| ||  __/ |   
|_| |_| |_|\__,_|___/___/_| |_|\__|\__\___|_|   
                                                '''

    #configure the run
    if sys.argv[1] not in ['all', 'toys', 'calib', 'fit', 'syst']:
        print 'Need to specify a valid operation! Possibilities are: \'all\' \'toys\', \'calib\', \'fit\' '
        sys.exit(2)
    runMode=sys.argv[1]
    print 'Chosen operation: %s'%runMode
    if opt.inputfile is None:
        parser.error('No input file defined! (input workspace needed)')
    inputfile = opt.inputfile
    print 'Input file %s'%inputfile
    if opt.channel is not None:
        channels = [opt.channel]
    if opt.mass is not None:
        masses = [opt.mass]
    else:
        masses = [ '1615','1635','1665','1695','1725','1755','1785','1815','1845' ]
    if runMode == 'all' and len(masses) == 1:
        print 'It\'s not possible to run \'all\' for only one mass point!'
        sys.exit(2)
    if opt.batch:
        sys.argv.append( '-b' )
        ROOT.gROOT.SetBatch()
 
    #
    # SYSTEMATICS EVALUATION
    #
    if runMode == 'syst':
        systSummaryFileName = 'systSummary_'+opt.channel+'.txt'
        ## if a second argument is passed, use this one to just make one systematic estimation
        ## the statistical uncertainty of on the nominal should be covered by the GetMeanError()
        # print sys.argv[2]
        # if not sys.argv[2].startswith('-'):
        #     print 'entered'
        #     systSummaryFileName.replace('.txt','_'+sys.argv[2]+'.txt')
        #     systematics = [sys.argv[2]]

        workspace = getWorkspace(inputfile,'w')
        workspace.Print() 
        # systname = 'mulxy_mepsyst'
        # get the nominal mass bias
        nominal = toyResult()
        nominal.variable = 'lxy'
        nominal.channel = opt.channel
        nominal.mass = '1725'
        nominal.doCalibrationStep(workspace, opt.ntoys, opt.samplesize, opt.fittype,opt.floatsignal,opt.outputdir)

        allSystematics = {}
        for systematic in systematics:
            syst = toyResult()
            syst.variable = 'lxy'
            syst.channel = opt.channel
            syst.mass = systematic
            syst.doCalibrationStep(workspace, opt.ntoys, opt.samplesize, opt.fittype,opt.floatsignal,opt.outputdir)
            allSystematics[systematic] = syst

        ## 
        outfile = open(systSummaryFileName,'w')
        outfile.write('number of toy experiment:\t'+str(opt.ntoys)+'\n')
        outfile.write('samplesize per toy experiment:\t'+str(opt.samplesize)+'\n')
        outfile.write('-'*100+'\n')
        outfile.write('mean method: \n')
        outfile.write('-'*100+'\n')
        outfile.write('nominal bias:\t'+str(nominal.bias['mean'])+' \t+- '+str(nominal.biasError['mean'])+'\n')
        for syst in allSystematics:
            outfile.write(syst+' bias:\t'+str(allSystematics[syst].bias['mean'])+' \t+- '+str(allSystematics[syst].biasError['mean'])+'\n')
        outfile.write('-'*100+'\n')
        for syst in allSystematics:
            outfile.write(syst+' uncertainty:\t'+str(nominal.bias['mean']-allSystematics[syst].bias['mean'])+' \t+- '+str(sqrt(pow(nominal.biasError['mean'],2)+pow(allSystematics[syst].biasError['mean'],2)))+'\n')

        outfile.write('-'*100+'\n')
        outfile.write('q50 method: \n')
        outfile.write('-'*100+'\n')
        outfile.write('nominal bias:\t'+str(nominal.bias['q50'])+' \t+- '+str(nominal.biasError['q50'])+'\n')
        for syst in allSystematics:
            outfile.write(syst+' bias:\t'+str(allSystematics[syst].bias['q50'])+' \t+- '+str(allSystematics[syst].biasError['q50'])+'\n')
        outfile.write('-'*100+'\n')
        for syst in allSystematics:
            outfile.write(syst+' uncertainty:\t'+str(nominal.bias['q50']-allSystematics[syst].bias['q50'])+' \t+- '+str(sqrt(pow(nominal.biasError['q50'],2)+pow(allSystematics[syst].biasError['q50'],2)))+'\n')

        outfile.write('-'*100+'\n')
        outfile.write('q25 method: \n')
        outfile.write('-'*100+'\n')
        outfile.write('nominal bias:\t'+str(nominal.bias['q25'])+' \t+- '+str(nominal.biasError['q25'])+'\n')
        for syst in allSystematics:
            outfile.write(syst+' bias:\t'+str(allSystematics[syst].bias['q25'])+' \t+- '+str(allSystematics[syst].biasError['q25'])+'\n')
        outfile.write('-'*100+'\n')
        for syst in allSystematics:
            outfile.write(syst+' uncertainty:\t'+str(nominal.bias['q25']-allSystematics[syst].bias['q25'])+' \t+- '+str(sqrt(pow(nominal.biasError['q25'],2)+pow(allSystematics[syst].biasError['q25'],2)))+'\n')

        outfile.write('number of toy experiment:\t'+str(opt.ntoys)+'\n')
        outfile.write('samplesize per toy experiment:\t'+str(opt.samplesize)+'\n')
        outfile.write('-'*100+'\n')
        outfile.write('nominal bias:\t'+str(nominal.bias['fit'])+' \t+- '+str(nominal.biasError['fit'])+'\n')
        for syst in allSystematics:
            outfile.write(syst+' bias:\t'+str(allSystematics[syst].bias['fit'])+' \t+- '+str(allSystematics[syst].biasError['fit'])+'\n')
        outfile.write('-'*100+'\n')
        for syst in allSystematics:
            outfile.write(syst+' uncertainty:\t'+str(abs(nominal.bias['fit']-allSystematics[syst].bias['fit']))+' \t+- '+str(sqrt(pow(nominal.biasError['fit'],2)+pow(allSystematics[syst].biasError['fit'],2)))+'\n')

        
        print 'Systematic uncertainties:'
        print 'nominal bias [fit]: '+str(nominal.bias['fit'])+' +- '+str(nominal.biasError['fit'])
        print 'nominal bias [mean]: '+str(nominal.bias['mean'])+' +- '+str(nominal.biasError['mean'])
        print 'nominal bias [q25]: '+str(nominal.bias['q25'])+' +- '+str(nominal.biasError['q25'])
        print 'nominal bias [q50]: '+str(nominal.bias['q50'])+' +- '+str(nominal.biasError['q50'])
        for syst in allSystematics:
            print syst+' bias [fit]: '+str(allSystematics[syst].bias['fit'])+' +- '+str(allSystematics[syst].biasError['fit'])
            print syst+' bias [mean]: '+str(allSystematics[syst].bias['mean'])+' +- '+str(allSystematics[syst].biasError['mean'])
            print syst+' bias [q25]: '+str(allSystematics[syst].bias['q25'])+' +- '+str(allSystematics[syst].biasError['q25'])
            print syst+' bias [q50]: '+str(allSystematics[syst].bias['q50'])+' +- '+str(allSystematics[syst].biasError['q50'])



    #
    # TOY EXPERIMENTS
    #
    if runMode == 'all' or runMode == 'toys':

        ## get the workspace form the root file
        workspace = getWorkspace(inputfile,'w')

        ## prepare a container for the toyResults
        ## a dict sorted according to channel and mass
        allFitResults = {}

        ## loop over all channels and mass points
        for chan in channels:
            for mass in masses:
                thisResult          = toyResult()
                thisResult.variable = 'lxy'
                thisResult.channel  = chan
                thisResult.mass     = mass
                print 'Running %s calibration for %s events with m=%s'%(thisResult.variable,thisResult.channel,thisResult.mass)
                thisResult.doCalibrationStep(workspace,opt.ntoys,opt.samplesize,opt.fittype,opt.floatsignal,opt.outputdir)
                tag = thisResult.channel + '_' + thisResult.mass
                allFitResults[tag] = thisResult

        ## save the bias hitograms in a root file
        if len(channels) == 1 and len(masses) == 1:
            biasoutfileName = opt.outputdir+'/bias_hists_'+channels[0]+'_'+masses[0]+'.root'
        elif len(channels) == 1:
            biasoutfileName = opt.outputdir+'/bias_hists_'+channels[0]+'.root'
        elif len(masses) == 1:
            biasoutfileName = opt.outputdir+'/bias_hists_'+masses[0]+'.root'
        else:
            biasoutfileName = opt.outputdir+'/bias_hists.root'
        biasoutfile = ROOT.TFile().Open(biasoutfileName,'RECREATE')
        for tag in allFitResults:
            allFitResults[tag].biasHistos['fit'].Write()
            allFitResults[tag].biasHistos['mean'].Write()
            allFitResults[tag].biasHistos['q25'].Write()
            allFitResults[tag].biasHistos['q50'].Write()
            allFitResults[tag].toyavgHisto.Write()
            allFitResults[tag].toydiffHisto.Write()
        biasoutfile.Close()
        print 'Results can be found @ %s'%biasoutfileName

    #
    # CALIBRATION STEP
    #
    if runMode == 'all' or runMode == 'calib':
        if runMode == 'calib' and opt.rootfile is None and opt.outputdir is None:
            parser.error('No input root file defined! (This has to contain ALL bias histograms)')

        if runMode == 'calib':
            rootfile = opt.rootfile
        elif runMode == 'all':
            rootfile = biasoutfileName

        if rootfile == None:
            print '[Warning] hadding all bias files found @ %s'%opt.outputdir
            rootfile='%s/bias_hists.root'%(opt.outputdir)
            os.system('hadd -f %s %s/bias_hists_*'%(rootfile,opt.outputdir))


        ## open the file containing the bias histograms
        rfile = openTFile(rootfile)
        channels = getAllChannels(rfile)
        masses   = getAllMasses(rfile)
        masses=sorted(masses)
        print 'Calibrating from %s masses %s for %s events'%(rootfile,masses,channels)

        all_h_toyavg = []
        all_h_bias = []
        for chan in channels:
            c_toy_avg = TCanvas('c_toy_avg_'+chan,'c_toy_avg_'+chan,400,400)
            #h_toy_avg = TH1F('h_toy_avg_'+chan,'h_bias_summary_'+chan,len(binning)-1,array('d',binning))
            h_toy_avg = ROOT.TGraphAsymmErrors()
            h_toy_avg.SetName('h_toy_avg_'+chan)
            for mass in masses:
                massRanHi=1.5
                massRanLo=1.5
                if mass=='1615'                : massRanHi=1
                if mass=='1615' or mass=='1635': massRanLo=1
                hist = getHist(rfile, 'toyavg_histo_'+chan+'_'+mass)
                np=h_toy_avg.GetN()
                h_toy_avg.SetPoint(np,float(mass)/10,hist.GetMean())
                h_toy_avg.SetPointError(np,massRanLo,massRanHi,hist.GetMeanError(),hist.GetMeanError())
                #h_toy_avg.SetBinContent(massbindict[mass],hist.GetMean())
                #h_toy_avg.SetBinError(massbindict[mass],hist.GetMeanError())
            #h_toy_avg.SetMinimum(0.)
            #h_toy_avg.SetMaximum(2.)
            all_h_toyavg.append(h_toy_avg)

            
        ## make the bias summary plot, one per method
        #for method in ['fit', 'mean', 'q50', 'q25']:
        for method in [ 'mean', 'q50', 'q25']:

            for chan in channels:
                c_bias_summary = TCanvas('c_bias_summary_'+method+'_'+chan,'c_bias_summary_'+chan,400,400)
                #h_bias = TH1F('h_bias_summary_'+method+'_'+chan,'h_bias_summary_'+method+'_'+chan,len(binning)-1,array('d',binning))
                h_bias = ROOT.TGraphAsymmErrors()
                h_bias.SetName('h_bias_summary_'+method+'_'+chan)
                for mass in masses:
                    ROOT.gStyle.SetOptFit(1)
                    ROOT.gStyle.SetOptStat(0)
                    ROOT.gStyle.SetOptTitle(0)

                    c_hist_fit = TCanvas('c_hist_fit'+method+'_'+chan,'c_hist_fit_'+method+'_'+chan,400,400)
                    hist = getHist(rfile, method+'bias_histo_'+chan+'_'+mass)
                    hist.SetTitle('m_{gen} = '+str(float(mass)/10.)+' GeV, '+chan+' channel')
                    hist.SetMarkerStyle(20) 
                    hist.SetMarkerColor(1)
                    hist.SetLineColor(1)

                    drawCMSHeader()

                    gfunc = TF1('f1','gaus',-20.,20)
                    gfunc.SetLineWidth(3)
                    gfunc.SetLineColor(4)
                    gfunc.SetParameters(50.,0.,5.)
                    ## p0: height, p1: mean, p2: width, 
                    hist.Fit('f1','Q')
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
                    leg.SetHeader('m_{gen} = '+str(float(mass)/10.)+' GeV, '+chan+' channel ('+method+')')
                    leg.AddEntry(hist,'MC toy experiment','p')
                    leg.AddEntry(gfunc,'Gaussian fit','l')
                    leg.Draw()
                
                    c_hist_fit.Print(opt.outputdir+'/c_bias_fitted_'+method+'_'+chan+'_'+mass+'.pdf')
                    c_hist_fit.Print(opt.outputdir+'/c_bias_fitted_'+method+'_'+chan+'_'+mass+'.png')

                
                    massRanHi=1.5
                    massRanLo=1.5
                    if mass=='1615'                : massRanHi=1
                    if mass=='1615' or mass=='1635': massRanLo=1
                    np=h_bias.GetN()
                    h_bias.SetPoint(np,float(mass)/10,mean)
                    h_bias.SetPointError(np,massRanLo,massRanHi,width,width)
                    #h_bias.SetBinContent(massbindict[mass],mean)
                    #h_bias.SetBinError(massbindict[mass],width)

                all_h_bias.append(h_bias)

        ## write a bias summary to a file
        outfile = ROOT.TFile().Open(opt.outputdir+'/bias_summary.root','RECREATE')
        for h_bias in all_h_bias:
            h_bias.Write()
        for h_toy_avg in all_h_toyavg:
            h_toy_avg.Write()
        outfile.Close()

    #        
    # FIT STEP
    #        
    if runMode == 'all' or runMode == 'fit':

        #open bias summary file
        if runMode == 'all' :
            rootfile = 'bias_summary.root'
        if opt.rootfile is None:
            rootfile = '%s/bias_summary.root'%(opt.outputdir)
        else:
            rootfile = opt.rootfile
        rfile = openTFile(rootfile)
        keys=getAllKeys(rfile,'','bias_summary')
        methods =  set([k.split('_')[3] for k in keys])
        if opt.channel is None:
            channels = set([k.split('_')[4] for k in keys])
        print 'Fitting from %s for %s events with %s methods'%(rootfile,channels,methods)

        #common style configurations
        ROOT.gStyle.SetOptTitle(0)
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetOptFit(1)

        #get the workspace
        workspace = getWorkspace(inputfile,'w')

        fOutName=opt.outputdir+'/fitResults.txt'
        fOut = open(fOutName, 'w')
        
        # now loop and finalize the fit
        for chan in channels:

            fOut.write('\n******************** %s events *******************\n'%chan)

            for method in methods:

                #get the bias summary and project with statistics up and down
                h_bias = getHist(rfile,'h_bias_summary_'+method+'_'+chan)
                h_stat_up = h_bias.Clone('h_bias_summary_'+method+'_stat_up')
                h_stat_down = h_bias.Clone('h_bias_summary_'+method+'_stat_down')
                for i in xrange(0,h_bias.GetN()):
                    x,y = ROOT.Double(0),ROOT.Double(0)
                    h_bias.GetPoint(i,x,y)
                    exHi=h_bias.GetErrorXhigh(i)
                    exLo=h_bias.GetErrorXlow(i)
                    eyHi=h_bias.GetErrorYhigh(i)
                    eyLo=h_bias.GetErrorYlow(i)
                    h_stat_up.SetPoint(i,x,y+eyHi)
                    h_stat_down.SetPoint(i,x,y-eyLo)
                    
                #for i in xrange(1,h_bias.GetNbinsX()+1):
                #    h_stat_up.SetBinContent(i, h_stat_up.GetBinContent(i)+h_stat_up.GetBinError(i))
                #    h_stat_down.SetBinContent(i, h_stat_down.GetBinContent(i)-h_stat_down.GetBinError(i))
                
                #prepare a canvas outpout 
                c_bias_method = TCanvas('c_bias_'+method+'_'+chan,'c_bias_'+method+'_'+chan,600,600)
                c_bias_method.cd()


                #fit a straight line to the calibration bias and its stat envelope
                lfunc = TF1('lf_'+method,'[0]+[1]*x',165,180)
                lfunc.SetLineWidth(3)
                lfunc.SetLineColor(8)
                lfunc.SetParameters(0.,0.)
                h_bias.Fit(lfunc,'Q')
                a = lfunc.GetParameter(0)
                b = lfunc.GetParameter(1)
                #h_bias.Draw('e1')
                h_bias.Draw('ap')
                h_bias.GetYaxis().SetRangeUser(-10,10)
                h_bias.GetXaxis().SetTitle('m_{gen} [GeV]')
                h_bias.GetYaxis().SetTitle('m_{fit} - m_{gen} [GeV]')
                h_bias.SetMarkerStyle(20)
                lfunc.Draw('lsame')

                lfunc_up = lfunc.Clone('lf_'+method+'_up')
                lfunc_up.SetLineWidth(2)
                lfunc_up.SetLineColor(4)
                lfunc_up.SetLineStyle(2)
                lfunc_up.SetParameters(0.,0.)
                h_stat_up.Fit(lfunc_up,'Q')
                a_up = lfunc_up.GetParameter(0)
                b_up = lfunc_up.GetParameter(1)
                lfunc_up.Draw('lsame')

                lfunc_down = lfunc.Clone('lf_'+method+'_down')
                lfunc_down.SetLineWidth(2)
                lfunc_down.SetLineColor(4)
                lfunc_down.SetLineStyle(2)
                lfunc_down.SetParameters(0.,0.)
                h_stat_down.Fit(lfunc_down,'Q')
                a_down = lfunc_down.GetParameter(0)
                b_down = lfunc_down.GetParameter(1)
                lfunc_down.Draw('lsame')
                
                line = ROOT.TGraph()
                line.SetPoint(0,-200,0)
                line.SetPoint(1,1500,0)
                line.SetLineWidth(2)
                line.SetLineColor(2)
                line.SetLineStyle(2)
                line.Draw('L')

                drawCMSHeader()

                c_bias_method.Modified()
                c_bias_method.Update()
                c_bias_method.SaveAs( opt.outputdir + '/' + c_bias_method.GetName() + '.pdf')
                c_bias_method.SaveAs( opt.outputdir + '/' + c_bias_method.GetName() + '.png')
                c_bias_method.SaveAs( opt.outputdir + '/' + c_bias_method.GetName() + '.C')

                #average bias error
                errorbars = [h_bias.GetErrorY(i) for i in range(1,len(masses)) if h_bias.GetErrorY(i) > 0.]
                #errorbars = [h_bias.GetBinError(i) for i in range(1,len(masses)) if h_bias.GetBinError(i) > 0.]
                calib_error = sum(errorbars)/float(len(errorbars))
                
                #get data from workspace and re-configure parameters
                data = workspace.data(chan+'data')
                #data.Print("v")
                mtop = workspace.var('mtop')
                mtop.setConstant(kFALSE)
                mtop.setRange(140,200)
                lxy = workspace.var('lxy')
                lxy.setConstant(kFALSE)
                lxy.setRange(0.,5.)

                #import the calibrated curve and its variations
                pa = RooRealVar('pa_'+method,'pa_'+method,a)
                pb = RooRealVar('pb_'+method,'pb_'+method,b)
                getattr(workspace,'import')(pa)
                getattr(workspace,'import')(pb)
                pb.setConstant(kTRUE)
                pa.setConstant(kTRUE)
                
                pa_up = RooRealVar('pa_'+method+'_up','pa_'+method+'_up',a_up)
                pb_up = RooRealVar('pb_'+method+'_up','pb_'+method+'_up',b_up)
                getattr(workspace,'import')(pa_up)
                getattr(workspace,'import')(pb_up)
                pb_up.setConstant(kTRUE)
                pa_up.setConstant(kTRUE)
                
                pa_down = RooRealVar('pa_'+method+'_down','pa_'+method+'_down',a_down)
                pb_down = RooRealVar('pb_'+method+'_down','pb_'+method+'_down',b_down)
                getattr(workspace,'import')(pa_down)
                getattr(workspace,'import')(pb_down)
                pb_down.setConstant(kTRUE)
                pa_down.setConstant(kTRUE)

                #print some debug
                fOut.write( '\n' )
                fOut.write( '[Calibration bias result for %s method]\n'%method )
                fOut.write( ' Nominal:%f+%f*mtop Stat_up:%f+%f*mtop Stat_down: %f+%f*mtop \n'%(a,b,a_up,b_up,a_down,b_down) )
                fOut.write( ' Average error on bias: %s\n'%str(calib_error) )
                fOut.write( ' Statistical uncertainty @ 172.5 GeV \n' )
                fOut.write( ' Stat. up: %f    Stat. down: %f \n'%(
                    (172.5-pa.getVal())/(1+ pb.getVal())-(172.5-pa_up.getVal())/(1+pb_up.getVal()),
                    (172.5-pa.getVal())/(1+ pb.getVal())-(172.5-pa_down.getVal())/(1+pb_down.getVal())
                    )
                            )
                
                            
                # fit method specific
                if method=='fit' :
                    workspace.var(''+chan+'alpha1').setConstant(kTRUE)
                    workspace.var(''+chan+'alpha2').setConstant(kTRUE) 
                    workspace.var(''+chan+'alpha3').setConstant(kTRUE) 
                    workspace.var(''+chan+'alpha4').setConstant(kTRUE) 
                    workspace.var(''+chan+'beta1').setConstant(kTRUE) 
                    workspace.var(''+chan+'beta2').setConstant(kTRUE)  
                    workspace.var(''+chan+'thr').setConstant(kTRUE)    
                    #            workspace.var(''+chan+'thr_bkg').setConstant(kTRUE)    
                    workspace.var(''+chan+'wid').setConstant(kTRUE)    
                    workspace.var(''+chan+'wid_bkg').setConstant(kTRUE)
                    workspace.var(''+chan+'sigfrac').setConstant(kTRUE)
 
                    #introduce a calibration to the fit model
                    mtopcalib = RooFormulaVar('mtopcalib','mtopcalib','@0+@1*@2',RooArgList(pa,pb,mtop))
                    getattr(workspace,'import')(mtopcalib)
                    newmodel = workspace.factory("EDIT::new"+chan+"model("+chan+"model,mtop=mtopcalib)")
                    getattr(workspace,'import')(newmodel)

                    #fit and show the result
                    c_data = TCanvas('c_data_'+method+'_'+chan,'c_data_'+method+'_'+chan,400,400)
                    newframe = lxy.frame()
                    workspace.pdf('new'+chan+'model').fitTo(data,RooCmdArg(RooFit.FitOptions(RooFit.Save(kTRUE),RooFit.SumW2Error(kTRUE))))
                    fit_res = mtop.getVal()
                    fit_error = mtop.getError()
                    stat_error = mtop.getError()
                    data.plotOn(newframe)
                    workspace.pdf(chan+'model').plotOn(newframe)
                    #workspace.pdf('new'+chan+'model').plotOn(newframe)
                    newframe.Draw()

                    drawCMSHeader()
            
                    pave = TPaveText(0.3,0.6,0.9,0.8,"NDC")
                    pave.SetBorderSize(0);
                    pave.SetFillStyle(0);
                    pave.SetTextAlign(12);
                    pave.SetTextFont(42);
                    topmass = 'm_{Top} = '+str("%.2f" % fit_res)+' #pm '+str("%.2f" % stat_error)+' (stat.) GeV'
                    pave.AddText(topmass);
                    pave.Draw();

                    c_data.Print(c_data.GetName()+'.pdf')
                    c_data.Print(c_data.GetName()++'.png')

            
                    fOut.write( '--------------------------\n')
                    fOut.write( 'Fit Results:\n')
                    fOut.write( '--------------------------\n')
                    fOut.write( 'mtop: %f +/- %f (stat) \n'%(fit_res,fit_error))
                    fOut.write( 'calibration error: %f \n',calib_error)
                    fOut.write( 'statistical error: %f \n',stat_error)
                    fOut.write( 'product error: %f\n',stat_error*calib_error)
                    fOut.write( 'f_{signal}: %f +/- %f (stat) \n'%(workspace.var(''+chan+'sigfrac').getVal(),workspace.var(''+chan+'sigfrac').getError()))
                    fOut.write( '--------------------------\n')

                    #Pedro: why do you need a fit with the uncalibrated model? is it a cross check?
                    #workspace.pdf(chan+'model').fitTo(data,RooCmdArg(RooFit.FitOptions(RooFit.Save(kTRUE),RooFit.SumW2Error(kTRUE))))
                    #print '****',mtop.getVal()

                else :

                    observable=workspace.var('lxy')
                    binnedData=data.createHistogram("tmp",observable)
                    xq=array('d',[0.25,0.5,0.75,1])
                    yq=array('d',[0,   0,  0,   0])
                    binnedData.GetQuantiles(4,yq,xq)
            
                    invFormula=workspace.function(chan+'meanInv')
                    obsVal=binnedData.GetMean()
                    obsValErr=binnedData.GetMeanError()
                    if method=='q25' :
                        invFormula=workspace.function(chan+'25qInv')
                        obsVal=yq[0]
                        obsValErr=0.637*obsValErr
                    if method=='q50' :
                        invFormula=workspace.function(chan+'50qInv')
                        obsVal=yq[1]
                        obsValErr=0.637*obsValErr
                    observable.setVal(obsVal)

                    mtop_fit_raw        = invFormula.getVal()

                    #Pedro: why?
                    #mtop_fit_calib      = mtop_fit_raw-(pa.getVal() + pb.getVal()*mtop_fit_raw)
                    #mtop_fit_calib_up   = mtop_fit_raw-(pa_up.getVal() + pb_up.getVal()*mtop_fit_raw)
                    #mtop_fit_calib_down = mtop_fit_raw-(pa_down.getVal() + pb_down.getVal()*mtop_fit_raw)


                    mtop_fit_calib      = (mtop_fit_raw-pa.getVal())/(1 + pb.getVal())
                    mtop_fit_calib_up   = (mtop_fit_raw-pa_up.getVal())/(1 + pb_up.getVal())
                    mtop_fit_calib_down = (mtop_fit_raw-pa_down.getVal())/(1 + pb_down.getVal())
                    stat_err_up         = mtop_fit_calib-mtop_fit_calib_up
                    stat_err_down       = mtop_fit_calib-mtop_fit_calib_down

                    fOut.write(' data observable: %f +/- %f\n'%(obsVal,obsValErr))
                    fOut.write(' mtop(raw): %f \n'%mtop_fit_raw)
                    fOut.write(' mtop(calib): %f+%f-%f \n'%(mtop_fit_calib,stat_err_up,stat_err_down))

                    binnedData.Delete()
                    

                    #get calibration data
                    #binnedData=data.createHistogram("tmp",observable)
                    #observable.setVal(binnedData.GetMean())
                
                    #res=mod_mean.getVal()
                    #observable.setVal(binnedData.GetMean()+binnedData.GetMeanError())
                    #eHigh=mod_mean.getVal()-res
                    #observable.setVal(binnedData.GetMean()-binnedData.GetMeanError())
                    #eLow=res-mod_mean.getVal()
                    #eStat=0.5*(fabs(eLow)+fabs(eHigh))

                    #print res, eHigh, eLow, eStat
                    #xq=array('d',[0.25,0.5,0.75,1])
                    #yq=array('d',[0,   0,  0,   0])
                    #binnedData.GetQuantiles(4,yq,xq)
                    #observable.setVal(yq[0])
                    #res=mod_25q.getVal()
                    #print res
                    #observable.setVal(yq[1])
                    #res=mod_50q.getVal()
                    #print res

                    #print '**************************************'
                    #print 'Calibration error:'
                    #observable.setVal(1.109)
                    #print 'mean@1725: ',res-mod_mean.getVal()
                    #observable_q50.setVal(0.835)
                    #print 'median@1725: ',res_q50-mod_50q.getVal()

            fOut.write('\n')

        fOut.close()
        print 'Summary of results can be found @ %s'%fOutName
        os.system('cat %s'%fOutName)


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












