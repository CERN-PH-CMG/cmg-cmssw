from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.MergedPlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.HZZFakeRatePlotter import *
from CMGTools.HToZZToLeptons.macros.param.SMCrossSections import *


def getHiggsPlotter(setup,finalstate,production,period,mass,category = 'inc'):
    #Now the Higgs    
    xs = setup[period]['HXS'].get(mass)

    if production == 'VBF' and category == 'VBF': 
        higgsPlotter = TreePlotter("sandbox/All_"+period+"/VBF"+str(mass)+".root","FourLeptonTreeProducer/tree",'1')
        higgsPlotter.setupFromFile("sandbox/All_"+period+"/VBF"+str(mass)+".pck")
        print 'Using VBF sample'
    else:
        higgsPlotter=TreePlotter("sandbox/All_"+period+"/GGH"+str(mass)+".root","FourLeptonTreeProducer/tree",'1')
        higgsPlotter.setupFromFile("sandbox/All_"+period+"/GGH"+str(mass)+".pck")
    
    higgsPlotter.name=production+period
    higgsPlotter.production=production
    higgsPlotter.mass=mass
    higgsPlotter.width=xs['width']

    higgsPlotter.addCorrectionFactor('lumi_'+period,setup[period]['lumi'],setup[period]['lumiErr'],'lnN')
    higgsPlotter.addCorrectionFactor('eventWeight','eventWeight',0.0,'lnN')
    higgsPlotter.addCorrectionFactor('eff','H_eff',0.0,'lnN')
    
    #Add kinematic factor for 7 TeV samples
    if mass not in [124,125,126] and period =='7TeV':
        higgsPlotter.addCorrectionFactor('kin',0.5+0.5*ROOT.TMath.Erf((mass - 80.85)/50.42),0.0,'lnN')
        

    #Higgs cross section, BR and uncertainties
    higgsPlotter.addCorrectionFactor('sigma',xs[production]['sigma'],0.0,'lnN')

    if finalstate=='MuMu' or finalstate=='EleEle':
        higgsPlotter.addCorrectionFactor('BRHiggs_hzz4l',xs['H4F']['eeee'],0.02,'lnN')
    elif finalstate=='MuEle':
        higgsPlotter.addCorrectionFactor('BRHiggs_hzz4l',xs['H4F']['eemumu'],0.02,'lnN')
    else:
        higgsPlotter.addCorrectionFactor('BRHiggs_hzz4l',xs['H4F']['llllt'],0.02,'lnN')

    higgsPlotter.addCorrectionFactor('pdf_hzz4l_accept',1,0.02,'lnN')

    higgsPlotter.setFillProperties(1001,ROOT.kBlue)
    scale=''
    pdf=''
    if production =='GGH':
        scale='QCDscale_ggH'
        pdf='pdf_gg'
    if production =='TTH':
        scale='QCDscale_ttH'
        pdf='pdf_gg'
    if production =='VBF':
        scale='QCDscale_qqH'
        pdf='pdf_qqbar'
    if production in['WH','ZH']:
        scale='QCDscale_VH'
        pdf='pdf_qqbar'


    higgsPlotter.addCorrectionFactor(pdf,1.0,(xs[production]['pdfUp']-xs[production]['pdfDown'])/200.,'lnN')
    if (production in ['GGH']) and category =='vbf':
        higgsPlotter.addCorrectionFactor('QCDscale_ggH2j',1.0,0.20,'lnN')

    elif (production in ['VBF']) and category =='vbf':   
        higgsPlotter.addCorrectionFactor('QCDscale_qqH',1.0,0.10,'lnN')
    else:    
        higgsPlotter.addCorrectionFactor(scale,1.0,(xs[production]['scaleUp']-xs[production]['scaleDown'])/200.,'lnN')


    if finalstate in ['MuMu','MuEle','All']:    
        higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.01,'lnN')
    if finalstate in ['MuEle','All']:    
        higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.06,'lnN')
    if finalstate in ['EleEle','All']:    
        higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.11,'lnN')

    return higgsPlotter


def getZZPlotters(setup,sigma,finalstate,production,period):
    zzPlotters=[]

    list=[]
    if production =='QQZZ':
        slist=['ZZ4e','ZZ4mu','ZZ4tau','ZZ2e2mu','ZZ2e2tau','ZZ2mu2tau']
    else:    
        slist=['GGZZ4L','GGZZ2L2L']

    for zzSample in slist:
        zzPlotters.append(TreePlotter("All"+"_"+period+"/"+zzSample+".root","FourLeptonTreeProducer/tree"))
        zzPlotters[-1].name=zzSample+period
        zzPlotters[-1].setupFromFile("All_"+period+"/"+zzSample+".pck")
        zzPlotters[-1].addCorrectionFactor('eventWeight','eventWeight',0.0,'lnN')
        zzPlotters[-1].addCorrectionFactor('sigma',sigma[zzSample][period],0.0,'lnN')
        zzPlotters[-1].addCorrectionFactor('lumi_'+period,setup[period]['lumi'],setup[period]['lumiErr'],'lnN')
        zzPlotters[-1].addCorrectionFactor('H_eff','H_eff',0.0,'lnN')

        if production=='QQZZ':
            zzPlotters[-1].addCorrectionFactor('pdf_qqbar',1.0,0.035,'lnN')
            zzPlotters[-1].addCorrectionFactor('QCDscale_VV',1.0,0.0284,'lnN')
        else:    
            zzPlotters[-1].addCorrectionFactor('pdf_gg',1.0,0.0708,'lnN')
            zzPlotters[-1].addCorrectionFactor('QCDscale_ggVV',1.0,0.2431,'lnN')


        if finalstate in ['MuMu','MuEle','All']:    
            zzPlotters[-1].addCorrectionFactor('CMS_eff_m',1.0,0.02,'lnN')
        if finalstate in ['EleEle','MuEle','All']:    
            zzPlotters[-1].addCorrectionFactor('CMS_eff_e',1.0,0.06,'lnN')

    zzPlotter = MergedPlotter(zzPlotters)
    zzPlotter.setFillProperties(1001,ROOT.kOrange-3)
    zzPlotter.name=production
    return zzPlotter,zzPlotters


def getFakePlotter(controlRegion,finalstate,period):

    plotter = HZZFakeRatePlotter("All_"+period+"/DATA.root","FourLeptonTreeProducer/tree",'1')
    plotter.name='FAKES'
    plotter.setCR(controlRegion)
    plotter.setOSLS(1.2,1.08,1.02,0.99)
    plotter.setFakeRateVar('HLoose_fakeRate',finalstate+period)
    plotter.setFillProperties(1001,ROOT.kRed)
    return plotter

###############################################################################
###############################################################################

ROOT.gROOT.ProcessLine(".x tdrstyle.C")

###Define the plotter setup###################
setup=dict()
setup['7TeV']=dict()
setup['8TeV']=dict()
setup['7TeV']['lumi']=5050.
setup['8TeV']['lumi']=12210.
setup['7TeV']['lumiErr']=0.022
setup['8TeV']['lumiErr']=0.044

