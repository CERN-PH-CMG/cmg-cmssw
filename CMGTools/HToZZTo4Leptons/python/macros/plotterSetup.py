#Load all plotter scripts
import sys
from CMGTools.HToZZTo4Leptons.macros.plotters.StackPlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.MergedPlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.HZZFakeRatePlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.HZZSignalParamPlotter import *
from CMGTools.HToZZTo4Leptons.macros.plotters.HZZBkgParamPlotter import *
from CMGTools.HToZZTo4Leptons.tools.fullPath import getFullPath




ROOT.gROOT.ProcessLine(".x "+getFullPath("python/macros/tdrstyle.C"))


class PlotterSetup(object):
    def __init__(self):
        self.data=dict()
        self.data['7TeV'] = dict()
        self.data['8TeV']=dict()
        self.data['lumi']={'7TeV':5050., '8TeV': 19790.}
        self.data['lumiErr']={'7TeV':0.022, '8TeV': 0.044}

        self.data['ZZTo4mu']={'8TeV':0.07691,'7TeV':0.06609}
        self.data['ZZTo4e']={'8TeV':0.07691,'7TeV':0.06609}
        self.data['ZZTo4tau']={'8TeV':0.07691,'7TeV':0.06609}
        self.data['ZZTo2e2mu']={'8TeV':0.1767,'7TeV':0.152}
        self.data['ZZTo2e2tau']={'8TeV':0.1767,'7TeV':0.152}
        self.data['ZZTo2mu2tau']={'8TeV':0.1767,'7TeV':0.152}
        self.data['GluGluToZZTo4L']={'8TeV':0.0048,'7TeV':0.00174}
        self.data['GluGluToZZTo2L2L']={'8TeV':0.01203, '7TeV':0.00348}
        self.data['higgsXS']  = {'7TeV':HiggsTheory(getFullPath('data/YR_7TeV.json')), '8TeV':HiggsTheory(getFullPath('data/YR_8TeV.json'))}
        self.data['finalState']={'4mu':'&&abs(H_Z1_leg1_PdgId)==13 && abs(H_Z2_leg1_PdgId)==13','4e':'&&abs(H_Z1_leg1_PdgId)==11 && abs(H_Z2_leg1_PdgId)==11','2e2mu':'&&abs(H_Z1_leg1_PdgId)!=abs(H_Z2_leg1_PdgId)'}
        self.data['higgsProduction']={'GluGluToH':'GGH','VBF':'VBF','ZH':'ZH','WH':'WH','TTbarH':'TTH'}
        self.data['filterEff']={'GluGluToH':1,'VBF':1,'ZH':0.028,'WH':0.0105,'TTbarH':0.0296}


    def findFile(self,directory,prefixes):
        files=[]
        for filename in os.listdir(directory):
            name,extention=filename.split('.')
            if extention.find('root') != -1:
                found=True
                for prefix in prefixes:
                    if not (name.find(prefix) != -1):
                        found=False
                if found:
                    files.append(name)
        if len(files)>1:
            print 'WARNING : Found more than one file with prefix ',prefixes
            print 'in >',files,'Using the first'


        if len(files)>0:    
            return files[0]
        else:
            
            return None
            
############################################################################
###########################################################################
############################PLOTTERS FROM NTUPLES###########################

    def getQQToZZPlotter(self,period):
        zzPlotters=[]

        slist=['ZZTo4e','ZZTo4mu','ZZTo4tau','ZZTo2e2mu','ZZTo2e2tau','ZZTo2mu2tau']
        
        for zzSample in slist:
            filename = self.findFile(period,[zzSample])
            if filename is None:
                return None
            
            zzPlotters.append(TreePlotter(period+'/'+filename+".root","FourLeptonTreeProducer/tree"))
            zzPlotters[-1].name=zzSample+period
            zzPlotters[-1].setupFromFile(period+"/"+filename+".pck")
            zzPlotters[-1].addCorrectionFactor('puWeight','eventWeight',0.0,'lnN')
            zzPlotters[-1].addCorrectionFactor('sigma',self.data[zzSample][period],0.0,'lnN')
            zzPlotters[-1].addCorrectionFactor('lumi'+period,self.data['lumi'][period],self.data['lumiErr'][period],'lnN')
            zzPlotters[-1].addCorrectionFactor('efficiencyWeight','H_eff',0.0,'lnN')

        zzPlotter = MergedPlotter(zzPlotters)
        zzPlotter.setFillProperties(1001,ROOT.kAzure-9)
        zzPlotter.name='QQToZZ'
        return {'plotter':zzPlotter,'deps':zzPlotters}

    def getGluGluToZZPlotter(self,period):
        zzPlotters=[]

        slist=['GluGluToZZTo4L','GluGluToZZTo2L2L']
        
        for zzSample in slist:
            filename = self.findFile(period,[zzSample])
            if filename is None:
                return None
            
            zzPlotters.append(TreePlotter(period+'/'+filename+".root","FourLeptonTreeProducer/tree"))
            zzPlotters[-1].name=zzSample+period
            zzPlotters[-1].setupFromFile(period+"/"+filename+".pck")
            zzPlotters[-1].addCorrectionFactor('puWeight','eventWeight',0.0,'lnN')
            zzPlotters[-1].addCorrectionFactor('sigma',self.data[zzSample][period],0.0,'lnN')
            zzPlotters[-1].addCorrectionFactor('lumi'+period,self.data['lumi'][period],self.data['lumiErr'][period],'lnN')
            zzPlotters[-1].addCorrectionFactor('efficiencyWeight','H_eff',0.0,'lnN')

        zzPlotter = MergedPlotter(zzPlotters)
        zzPlotter.setFillProperties(1001,ROOT.kAzure-9)
        zzPlotter.name='QQToZZ'
        return {'plotter':zzPlotter,'deps':zzPlotters}


    def getHiggsPlotter(self,productions,period,mass):
    #Now the Higgs
    #Get XS and BR from YR
          higgsPlotters=[]
          xs = self.data['higgsXS'][period].getInterp(mass)
          for production in productions:
            filename = self.findFile(period,[production,str(mass)])
            if filename is None:
                print 'WARNING:Cannot find signal file for production mode:',production,'ignoring'
                continue
            
            higgsPlotters.append(TreePlotter(period+"/"+filename+".root","FourLeptonTreeProducer/tree",'1'))
            higgsPlotters[-1].setupFromFile(period+"/"+filename+".pck")
            higgsPlotters[-1].name=production+period
            higgsPlotters[-1].production=production
            higgsPlotters[-1].mass=mass
            higgsPlotters[-1].width=xs['width']
            higgsPlotters[-1].addCorrectionFactor('lumi_'+period,self.data['lumi'][period],self.data['lumiErr'][period],'lnN')
            higgsPlotters[-1].addCorrectionFactor('puWeight','eventWeight',0.0,'lnN')
            higgsPlotters[-1].addCorrectionFactor('efficiencyWeight','H_eff',0.0,'lnN')
            higgsPlotters[-1].addCorrectionFactor('filterEff',self.data['filterEff'][production],0.0,'lnN')
    

            #correction factor for crappy 7 TeV samples
            if mass not in [124,125,126] and period =='7TeV':
                higgsPlotters[-1].addCorrectionFactor('kin',0.5+0.5*ROOT.TMath.Erf((mass - 80.85)/50.42),0.0,'lnN')
        

            #Higgs cross section, BR and uncertainties
            print 'Higgs cross section for '+production+':'+str(xs[self.data['higgsProduction'][production]]['sigma']  )
            higgsPlotters[-1].addCorrectionFactor('sigma',xs[self.data['higgsProduction'][production]]['sigma'],0.0,'lnN')
            higgsPlotters[-1].addCorrectionFactor('BRHiggs_hzz4l',xs['H4F']['llllt'],0.0,'lnN')


          higgsPlotter = MergedPlotter(higgsPlotters)
          higgsPlotter.setFillProperties(1001,ROOT.kAzure-9)
          return {'plotter':higgsPlotter,'deps':higgsPlotters}





    def getFakePlotter(self,period,controlRegion,factorizationRegion = None ,factorization=1):

        plotter = HZZFakeRatePlotter(period+"/DATA.root","FourLeptonTreeProducer/tree",'1')
        plotter.name='FAKES'
        plotter.setCR(controlRegion)
        plotter.setOSLS(1.2,1.08,1.02,0.99)
        plotter.setFakeRateVar('HLoose_fakeRate',period)
        if factorizationRegion is not None:
            plotter.setFactorizedRegion(factorizationRegion)
            plotter.applyFactorization(factorization)
        plotter.setFillProperties(1001,ROOT.kRed)

        return {'plotter':plotter,'deps':[plotter]}


    def getDataPlotter(self,period):
        plotter = TreePlotter(period+"/DATA.root","FourLeptonTreeProducer/tree","")
        return {'plotter':plotter,'deps':[plotter]}
    


#################################################################################################
#################################################################################################
###########################PARAMETRIC PLOTTERS###################################################



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






plotterSetup = PlotterSetup()
