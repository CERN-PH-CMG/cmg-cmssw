#CHANGED
from CMGTools.HToZZTo4Leptons.macros.plotters.StackPlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.MergedPlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.HZZFakeRatePlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.HZZSignalParamPlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.HZZBkgParamPlotter import *
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath




ROOT.gROOT.ProcessLine(".x "+getFullPath("python/macros/tdrstyle.C"))


class HZZSetup(object):
    def __init__(self):
        self.data=dict()
        self.data['7TeV'] = dict()
        self.data['8TeV']=dict()
#        self.data['lumi']={'7TeV':5050., '8TeV': 12210}
        self.data['lumi']={'7TeV':5050., '8TeV': 19630.}
        self.data['lumiErr']={'7TeV':0.022, '8TeV': 0.044}
        self.data['ZZ4mu']={'8TeV':0.07691,'7TeV':0.024887}
        self.data['ZZ4e']={'8TeV':0.07691,'7TeV':0.024993}
        self.data['ZZ4tau']={'8TeV':0.07691,'7TeV':0.027223}
        self.data['ZZ2e2mu']={'8TeV':0.1767,'7TeV':0.050066}
        self.data['ZZ2e2tau']={'8TeV':0.1767,'7TeV':0.051979}
        self.data['ZZ2mu2tau']={'8TeV':0.1767,'7TeV':0.051729}
        self.data['GGZZ4L']={'8TeV':0.0048,'7TeV':0.00174}
        self.data['GGZZ2L2L']={'8TeV':0.01203, '7TeV':0.00348}
        self.data['theory']  = {'7TeV':HiggsTheory(getFullPath('data/YR_7TeV.json')), '8TeV':HiggsTheory(getFullPath('data/YR_7TeV.json'))}




############################################################################
###########################################################################
###############################HELPERS######################################


    def getHiggsPlotter(self,finalstate,production,period,mass,category = 'inc'):
    #Now the Higgs    
          xs = self.data['theory'][period].getInterp(mass)



          if production in ['ggH','qqH']:
              higgsPlotter = TreePlotter("All_"+period+"/"+production+str(mass)+".root","FourLeptonTreeProducer/tree",'1')
              higgsPlotter.setupFromFile("All_"+period+"/"+production+str(mass)+".pck")
              print 'Using VBF sample'
          else:
              higgsPlotter=TreePlotter("All_"+period+"/VH"+str(mass)+".root","FourLeptonTreeProducer/tree",'1')
              higgsPlotter.setupFromFile("All_"+period+"/VH"+str(mass)+".pck",finalstate)
    
          higgsPlotter.name=production+period
          higgsPlotter.production=production
          higgsPlotter.mass=mass
          higgsPlotter.width=xs['width']

          higgsPlotter.addCorrectionFactor('lumi_'+period,self.data['lumi'][period],self.data['lumiErr'][period],'lnN')
          higgsPlotter.addCorrectionFactor('eventWeight','eventWeight',0.0,'lnN')
          higgsPlotter.addCorrectionFactor('eff','H_eff',0.0,'lnN')
    

          if mass not in [124,125,126] and period =='7TeV':
              higgsPlotter.addCorrectionFactor('kin',0.5+0.5*ROOT.TMath.Erf((mass - 80.85)/50.42),0.0,'lnN')
        
          pname = {'ggH':'GGH','qqH':'VBF','ZH':'ZH','WH':'WH','ttH':'ttH'}

    #Higgs cross section, BR and uncertainties
          higgsPlotter.addCorrectionFactor('sigma',xs[pname[production]]['sigma'],0.0,'lnN')

          if finalstate in ['MuMu','EleEle']:
              higgsPlotter.addCorrectionFactor('BRHiggs_hzz4l',xs['H4F']['eeee'],0.02,'lnN')
          elif finalstate in ['MuEle']:
              higgsPlotter.addCorrectionFactor('BRHiggs_hzz4l',xs['H4F']['eemumu'],0.02,'lnN')



          higgsPlotter.addCorrectionFactor('pdf_hzz4l_accept',1,0.02,'lnN')
          higgsPlotter.setFillProperties(1001,ROOT.kBlue)
          scale=''
          pdf=''
          if production =='ggH':
              scale='QCDscale_ggH'
              pdf='pdf_gg'

          elif production =='ttH':
              scale='QCDscale_ttH'
              pdf='pdf_gg'
              higgsPlotter.addCorrectionFactor('ttHSelect','genHProcess==121||genHProcess==122',0.0,'lnN')


          elif production =='qqH':
              scale='QCDscale_qqH'
              pdf='pdf_qqbar'
          elif production in['WH']:
              scale='QCDscale_VH'
              pdf='pdf_qqbar'
              higgsPlotter.addCorrectionFactor('WHSelect','genHProcess==24',0.0,'lnN')
          elif production in['ZH']:
              scale='QCDscale_VH'
              pdf='pdf_qqbar'
              higgsPlotter.addCorrectionFactor('ZHSelect','genHProcess==26',0.0,'lnN')


          higgsPlotter.addCorrectionFactor(pdf,1.0,(xs[pname[production]]['pdfUp']-xs[pname[production]]['pdfDown'])/200.,'lnN')
          if (production in ['ggH']) and category =='vbf':
              higgsPlotter.addCorrectionFactor('QCDscale_ggH2j',1.0,0.20,'lnN')

          elif (production in ['qqH']) and category =='vbf':   
              higgsPlotter.addCorrectionFactor('QCDscale_qqH',1.0,0.10,'lnN')
          else:    
              higgsPlotter.addCorrectionFactor(scale,1.0,(xs[pname[production]]['scaleUp']-xs[pname[production]]['scaleDown'])/200.,'lnN')

          if finalstate=='MuMu' and period =='7TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.02,'lnN')
          if finalstate=='MuMu' and period =='8TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.04,'lnN')
          if finalstate=='MuEle' and period =='7TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.01,'lnN')
                  higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.056,'lnN')
          if finalstate=='MuEle' and period =='8TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.02,'lnN')
                  higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.043,'lnN')
          if finalstate=='EleEle' and period =='7TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.11,'lnN')
          if finalstate=='EleEle' and period =='8TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.086,'lnN')


          return higgsPlotter


    def getHiggsParamPlotter(self,finalstate,production,period,mass,category = 'inc'):
    #Now the Higgs    
          xs = self.data['theory'][period].getInterp(mass)
          higgsPlotter =HZZSignalParamPlotter(finalstate,production,period,category)
          higgsPlotter.setupFromFile()
          higgsPlotter.name=production+period
          higgsPlotter.production=production
          higgsPlotter.period=period
          higgsPlotter.setMH(mass)
          higgsPlotter.addCorrectionFactor('lumi_'+period,self.data['lumi'][period],self.data['lumiErr'][period],'lnN')
          higgsPlotter.addCorrectionFactor('BRHiggs_hzz4l',1.0,0.02,'lnN')
          higgsPlotter.addCorrectionFactor('pdf_hzz4l_accept',1,0.02,'lnN')
          higgsPlotter.setFillProperties(1001,ROOT.kBlue)
          scale=''
          pdf=''
          if production =='ggH':
              scale='QCDscale_ggH'
              pdf='pdf_gg'
          elif production =='ttH':
              scale='QCDscale_ttH'
              pdf='pdf_gg'
          elif production =='qqH':
              scale='QCDscale_qqH'
              pdf='pdf_qqbar'
          elif production in['WH','ZH']:
              scale='QCDscale_VH'
              pdf='pdf_qqbar'

          pname = {'ggH':'GGH','qqH':'VBF','ZH':'ZH','WH':'WH','ttH':'TTH'}

          higgsPlotter.addCorrectionFactor(pdf,1.0,(xs[pname[production]]['pdfUp']-xs[pname[production]]['pdfDown'])/200.,'lnN')

          if (production in ['ggH']) and (category =='vbf' or category =='vbftag'):
              higgsPlotter.addCorrectionFactor('QCDscale_ggH2j',1.0,0.30,'lnN')
          elif (production in ['qqH']) and (category =='vbf' or  category=='vbftag'):   
              higgsPlotter.addCorrectionFactor('QCDscale_qqH',1.0,0.10,'lnN')

          if (production in ['ggH']) and category =='novbfhigh':
              higgsPlotter.addCorrectionFactor('QCDscale_ggHboost',1.0,0.15,'lnN')

          if (production in ['qqH']) and category =='novbhigh':   
              higgsPlotter.addCorrectionFactor('QCDscale_qqH',1.0,0.05,'lnN')


          higgsPlotter.addCorrectionFactor(scale,1.0,(xs[pname[production]]['scaleUp']-xs[pname[production]]['scaleDown'])/200.,'lnN')


          if finalstate=='MuMu' and period =='7TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.02,'lnN')
          if finalstate=='MuMu' and period =='8TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.04,'lnN')
          if finalstate=='MuEle' and period =='7TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.01,'lnN')
                  higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.056,'lnN')
          if finalstate=='MuEle' and period =='8TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.02,'lnN')
                  higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.043,'lnN')
          if finalstate=='EleEle' and period =='7TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.11,'lnN')
          if finalstate=='EleEle' and period =='8TeV':
                  higgsPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.086,'lnN')


          return higgsPlotter



    def getZZParamPlotter(self,finalstate,production,period,category = 'inc'):

        zzPlotter = HZZBkgParamPlotter(finalstate,production,period,category)
        zzPlotter.setupFromFile()
        zzPlotter.production=production
        zzPlotter.period=period
        zzPlotter.addCorrectionFactor('lumi_'+period,1,self.data['lumiErr'][period],'lnN')#lumi has been used in the param
        if production=='QQZZ':
            zzPlotter.addCorrectionFactor('pdf_qqbar',1.0,0.035,'lnN')
            zzPlotter.addCorrectionFactor('QCDscale_VV',1.0,0.0284,'lnN')
        else:    
            zzPlotter.addCorrectionFactor('pdf_gg',1.0,0.0708,'lnN')
            zzPlotter.addCorrectionFactor('QCDscale_ggVV',1.0,0.2431,'lnN')

        if (category=='vbftag' or category=='vbf'):
            zzPlotter.addCorrectionFactor('hzz4l_ZZ_VBF'+finalstate+production+period,1.0,0.50,'lnN')
            

        if finalstate=='MuMu' and period =='7TeV':
            zzPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.02,'lnN')
        if finalstate=='MuMu' and period =='8TeV':
            zzPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.04,'lnN')
        if finalstate=='MuEle' and period =='7TeV':
            zzPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.01,'lnN')
            zzPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.056,'lnN')
        if finalstate=='MuEle' and period =='8TeV':
            zzPlotter.addCorrectionFactor('CMS_eff_m',1.0,0.02,'lnN')
            zzPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.043,'lnN')
        if finalstate=='EleEle' and period =='7TeV':
            zzPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.11,'lnN')
        if finalstate=='EleEle' and period =='8TeV':
            zzPlotter.addCorrectionFactor('CMS_eff_e',1.0,0.086,'lnN')
        zzPlotter.setFillProperties(1001,ROOT.kOrange-3)
        zzPlotter.name=production
        return zzPlotter


    def getFakeParamPlotter(self,finalstate,production,period,category = 'inc'):
        fakePlotter = HZZBkgParamPlotter(finalstate,production,period,category)
        fakePlotter.setupFromFile()
        if (category=='vbftag' or category=='vbf'):
            fakePlotter.addCorrectionFactor('CMS'+'_'+finalstate+'_'+period+category+'_fakeRateVBF',1.0,0.50,'lnN')

        fakePlotter.addCorrectionFactor('CMS'+'_'+finalstate+'_'+period+category+'_fakeRate',1.0,[0.6,1.4],'lnN2')
        
        return fakePlotter

    def getZZPlotters(self,finalstate,production,period):
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
            zzPlotters[-1].addCorrectionFactor('sigma',self.data[zzSample][period],0.0,'lnN')
            zzPlotters[-1].addCorrectionFactor('lumi_'+period,self.data['lumi'][period],self.data['lumiErr'][period],'lnN')
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


    def getFakePlotter(self,finalstate,period,controlRegion,factorizationRegion = None ,factorization=1):

        plotter = HZZFakeRatePlotter("All_"+period+"/DATA.root","FourLeptonTreeProducer/tree",'1')
        plotter.name='FAKES'
        plotter.setCR(controlRegion)
        plotter.setOSLS(1.2,1.08,1.02,0.99)
        plotter.setFakeRateVar('HLoose_fakeRate',finalstate+period)
        if factorizationRegion is not None:
            plotter.setFactorizedRegion(factorizationRegion)
            plotter.applyFactorization(factorization)
        plotter.setFillProperties(1001,ROOT.kRed)

        return plotter




hzzSetup = HZZSetup()
