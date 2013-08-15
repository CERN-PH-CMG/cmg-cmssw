import ROOT
import os
import sys
from array import array
import pickle
import copy
from array import array
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

class PlotterBase(object):

    def __init__(self):
        self.fillstyle=1001
        self.linestyle=1
        self.linecolor=1
        self.linewidth=2
        self.fillcolor=ROOT.kOrange-3
        self.markerstyle=20
        self.corrFactors=[]

    def addCorrectionFactor(self,name,value,error,model):
        corr=dict()
        corr['value']=value
        corr['error']=error
        corr['name']=name
        corr['model']=model
        self.corrFactors.append(corr)

    def setLineProperties(self,linestyle,linecolor,linewidth):
        self.linestyle=linestyle
        self.linecolor=linecolor
        self.linewidth=linewidth 

    def setFillProperties(self,fillstyle,fillcolor):
        self.fillstyle=fillstyle
        self.fillcolor=fillcolor

    def setMarkerProperties(self,markerstyle):
        self.markerstyle=markerstyle
        


    def makeDataSet(self,vars,cuts,mini = 0,maxi = 1.0e+10,miny=0,maxy=1e+10,name = "data_obs",entries=-1):
        return None    

    def makeDataSetFromWorkspace(self,w,var,cuts,name = "data_obs",entries=-1):
        return None    

    def drawTH2(self,var,cuts,lumi,binsx,minx,maxx,binsy,miny,maxy,titlex = "",unitsx = "",titley="",unitsy="", drawStyle = "COLZ"):
        return None

    def drawTH3(self,var,cuts,lumi,binsx,minx,maxx,binsy,miny,maxy,binsz,minz,maxz,titlex = "",unitsx = "",titley="",unitsy="", drawStyle = "COLZ"):
        return None

    def drawTH1(self,var,cuts,lumi,bins,min,max,titlex = "",units = "",drawStyle = "HIST"):
        return None


    def makeDataHist(self,var,cuts,binsx,minx,maxx,binsy=100,miny=0,maxy=1e+10,binsz=100,minz=-1e-9,maxz=1e+9,name='data',norm=0):
        variables = var.split(':')
        observables=[]
        params=[]
        
        if len(variables) ==1:
            h = self.drawTH1(var,cuts,'1',binsx,minx,maxx)
            rooVar = ROOT.RooRealVar(var,var,minx,maxx)
            dh = ROOT.RooDataHist(name,'',ROOT.RooArgList(rooVar),h)
            observables=[rooVar]
        elif len(variables) ==2:    
            h = self.drawTH2(var,cuts,'1',binsx,minx,maxx,binsy,miny,maxy)
            rooVar1 = ROOT.RooRealVar(variables[1],variables[1],minx,maxx)
            rooVar2 = ROOT.RooRealVar(variables[0],variables[0],miny,maxy)
            dh = ROOT.RooDataHist(name,'',ROOT.RooArgList(rooVar1,rooVar2),h)
            observables=[rooVar1,rooVar2]
        else:    
            h = self.drawTH3(var,cuts,'1',binsx,minx,maxx,binsy,miny,maxy,binsz,minz,maxz)
            rooVar1 = ROOT.RooRealVar(variables[2],variables[1],minx,maxx)
            rooVar2 = ROOT.RooRealVar(variables[1],variables[0],miny,maxy)
            rooVar3 = ROOT.RooRealVar(variables[0],variables[0],minz,maxz)
            dh = ROOT.RooDataHist(name,'',ROOT.RooArgList(rooVar1,rooVar2,rooVar3),h)
            observables=[rooVar1,rooVar2,rooVar3]


        return dh,observables

    def makeTemplateHist(self,var,cuts,binsx,minx,maxx,binsy=100,miny=0,maxy=1e+10,norm=0):
        variables = var.split(':')
        if len(variables) ==1:
            h = self.drawTH1(var,cuts,'1',binsx,minx,maxx)
        else:    
            h = self.drawTH2(var,cuts,'1',binsx,minx,maxx,binsy,miny,maxy)
            h.Smooth()
            if norm>0:
                print 'Normalizing assuming you need to play a trick for the MELA PDF. Ask Michalis if you are not doing ZZ'
                for binx in range(1,h.GetNbinsX()+1):
                    sumy=h.ProjectionY('px',binx,binx).Integral()
                    if sumy>0:
                        for biny in range(1,h.GetNbinsY()+1):
                            h.SetBinContent(binx,biny,h.GetBinContent(binx,biny)/sumy)
                            h.SetBinError(binx,biny,h.GetBinError(binx,biny)/sumy)
                        
        return h


    def makeTemplateHistBinned(self,var,cuts,binningx,binningy,norm=0):
        variables = var.split(':')
        if len(variables) ==1:
            h = self.drawTH1Binned(var,cuts,'1',binningx)
        else:    
            h = self.drawTH2Binned(var,cuts,'1',binningx,binningy)
            h.Smooth()
            if norm>0:
                print 'Normalizing assuming you need to play a trick for the MELA PDF. Ask Michalis if you are not doing ZZ'
                for binx in range(1,h.GetNbinsX()+1):
                    sumy=h.ProjectionY('px',binx,binx).Integral()
                    if sumy>0:
                        for biny in range(1,h.GetNbinsY()+1):
                            h.SetBinContent(binx,biny,h.GetBinContent(binx,biny)/sumy)
                            h.SetBinError(binx,biny,h.GetBinError(binx,biny)/sumy)
                        
        return h


    def makeLandauPdf(self,var,cuts,mini = 0,maxi = 1.0e+10,name='pdf'):
        """
        Make Landau PDF. 
        -----------------
        As input you can give var or vary:varx . If you give two dimensions
        it will use varx. cuts defines the preselection in the Plotter 
        before fitting the distribution.mini,maxi define the boundaries
        for the fit. Returns two parameters, the mean and sigma of Landau.
        """
        data,observables = self.makeDataHist(var,cuts,500,mini,maxi,20,-1e+10,1e+10,"tmpData")

        landauMean  = ROOT.RooRealVar('CMS_landauMean',"",mini,maxi)
        landauSigma = ROOT.RooRealVar('CMS_landauSigma',"",0,maxi-mini)
        observable=observables[0]
        
        pdf=ROOT.RooLandau(name,"landaupdf",observable,landauMean,landauSigma)
        #Run a fit on the control region to define the variables
        pdf.fitTo(data,ROOT.RooFit.SumW2Error(1))

        params=[]
        for p in [landauMean,landauSigma]:
            params.append({'name':p.GetName(),'mean':p.getVal(),'sigma':p.getError(),'var':p})

        self.makeValidationPlot(observable,pdf,data)

        return pdf,params,observables,[]


    def makeQQZZPdf(self,var,cuts,mini = 0,maxi = 1.0e+10,name='pdf'):
        """
        Make empirical QQ -> ZZ PDF
        -----------------
        As input you can give var or vary:varx . If you give two dimensions
        it will use varx. cuts defines the preselection in the Plotter 
        before fitting the distribution.mini,maxi define the boundaries
        for the fit. Returns 14 parameters!
        """


        data,observables = self.makeDataHist(var,cuts,1000,mini,maxi,20,-1e+10,1e+10,"tmpData")


        a0  = ROOT.RooRealVar('CMS_a0',"",110,0,500)
        a1  = ROOT.RooRealVar('CMS_a1',"",15,0,100)
        a2  = ROOT.RooRealVar('CMS_a2',"",110,0,500)
        a3  = ROOT.RooRealVar('CMS_a3',"",0.05,0,10)
        a4  = ROOT.RooRealVar('CMS_a4',"",185,0,500)
        a5  = ROOT.RooRealVar('CMS_a5',"",11,0,100)
        a6  = ROOT.RooRealVar('CMS_a6',"",35,0,maxi)
        a7  = ROOT.RooRealVar('CMS_a7',"",0.1,0,10)
        a8  = ROOT.RooRealVar('CMS_a8',"",63,0,100)
        a9  = ROOT.RooRealVar('CMS_a9',"",0.07,0,10)
        a10  = ROOT.RooRealVar('CMS_a10',"",98.,0,500)
        a11  = ROOT.RooRealVar('CMS_a11',"",-5.,-30,0)
        a12  = ROOT.RooRealVar('CMS_a12',"",1300,0,10000)
        a13  = ROOT.RooRealVar('CMS_a13',"",0.08,0,10)

        pdf=ROOT.RooqqZZPdf_v2(name,"qqzzpdf",observable,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)
        #Run a fit on the control region to define the variables
        pdf.fitTo(data,ROOT.RooFit.SumW2Error(1))

        params=[]
        for p in [a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13]:
            params.append({'name':p.GetName(),'mean':p.getVal(),'sigma':p.getError(),'var':p})

        importPackage=[]    
        self.makeValidationPlot(observable,pdf,data)

        return pdf,params,observables,importPackage


    def makeGGZZPdf(self,var,cuts,mini = 0,maxi = 1.0e+10,name='pdf'):
        """
        Make empirical GG -> ZZ PDF
        -----------------
        As input you can give var or vary:varx . If you give two dimensions
        it will use varx. cuts defines the preselection in the Plotter 
        before fitting the distribution.mini,maxi define the boundaries
        for the fit. Returns 10 parameters!
        """

        data,observables = self.makeDataHist(var,cuts,1000,mini,maxi,20,-1e+10,1e+10,"tmpData")

        a0  = ROOT.RooRealVar('CMS_a0',"",127,100,500)
        a1  = ROOT.RooRealVar('CMS_a1',"",39,0,200.)
        a2  = ROOT.RooRealVar('CMS_a2',"",130,50,500)
        a3  = ROOT.RooRealVar('CMS_a3',"",0.1,0,1)
        a4  = ROOT.RooRealVar('CMS_a4',"",185,100,220)
        a5  = ROOT.RooRealVar('CMS_a5',"",15,0,50)
        a6  = ROOT.RooRealVar('CMS_a6',"",30,0,100)
        a7  = ROOT.RooRealVar('CMS_a7',"",0.30,0,2)
        a8  = ROOT.RooRealVar('CMS_a8',"",60,0,500)
        a9  = ROOT.RooRealVar('CMS_a9',"",0.07,-1,1)

        pdf=ROOT.RooggZZPdf_v2(name,"ggzzpdf",observable,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)
        #Run a fit on the control region to define the variables
        pdf.fitTo(data,ROOT.RooFit.SumW2Error(1))
        
        params=[]
        for p in [a0,a1,a2,a3,a4,a5,a6,a7,a8,a9]:
            params.append({'name':p.GetName(),'mean':p.getVal(),'sigma':p.getError(),'var':p})
        self.makeValidationPlot(observable,pdf,data)
        
        return pdf,params,observables,[]

    def makeBWPdf(self,var,mini = 0,maxi = 1.0e+10,name='pdf',mass=125,width=0.1):
        """
        Make predefined Breit Wigner PDF
        -----------------
        As input you can give var or vary:varx . If you give two dimensions
        it will use varx. mini,maxi define the boundaries
        for the observable. mass is the mean and width is the width of the distribution.
        The default names of the parameters are synced with Higgs combination package (MH=mass)
        """
        observables=[]                  
        variables = var.split(':')
        if len(variables)==1:
            observables.append(ROOT.RooRealVar(variables[0],variables[0],mini,maxi))                    
        else:
            observables.append(ROOT.RooRealVar(variables[1],variables[1],mini,maxi))                    
            observables.append(ROOT.RooRealVar(variables[0],variables[0],0,1e+10))
                  
        observable = observables[0]
                  
        mH     = ROOT.RooRealVar("MH","",mass)
        width  = ROOT.RooRealVar("width","",width)
        pdf=ROOT.RooBreitWigner(name,"BW",observable,mH,width);
                
        params=[]  
        for p in [mH,width]:
          params.append({'name':p.GetName(),'mean':p.getVal(),'sigma':p.getError(),'var':p})                    
                              
        self.makeValidationPlot(observable,pdf,data)


        return pdf,params,observables



    def makeBWCBallPdf(self,var,cuts,mini = 0,maxi = 1.0e+10,name='pdf',mass=125,width=0.1,channel='MuMu'):
        """
        Make Convolution of Breit Wigner and Crystal ball
        -----------------
        As input you can give var or vary:varx . If you give two dimensions
        it will use varx. mini,maxi define the boundaries
        for the observable. mass is the mean and width is the width of the distribution.
        The default names of the parameters are synced with Higgs combination package (MH=mass)
        For the energy scale and resolution of the crystal ball two nuisances are created 
        for each to account for the effect of muons and electrons in the scale and sigma 
        i.e for four lepton analysis.
        If the width is < 100 MeV it just returns the crystalball shifted to the mass
        """

        data,observables = self.makeDataSet(var,cuts,mini,maxi,0,1e+10,"tmpData",500000)

#        data,observables = self.makeDataHist(var,cuts,1000,mini,maxi,20,0,1e+10,"tmpData")
        observable=observables[0]

        
        #Make Breit Wigner
        mH     = ROOT.RooRealVar("MH","",mass)
        width  = ROOT.RooRealVar("width","",width)
        pdf=ROOT.RooBreitWigner("HiggsBW","BW",observable,mH,width);


        #Make crystal ball
        m0     = ROOT.RooRealVar("CMS_CB_MeanFit","",0.0,-20.,20.)
        sigma  = ROOT.RooRealVar("CMS_CB_SigmaFit","",1.,0.01,100.)
        alpha  = ROOT.RooRealVar("CMS_CB_Alpha","",3,-10,10.)
        n      = ROOT.RooRealVar("CMS_CB_N","",2,-10.,10)

        scale_mu      = ROOT.RooRealVar("CMS_scale_m","",1.0,0,2.)
        scale_ele     = ROOT.RooRealVar("CMS_scale_e","",1.0,0,2.)
        res_mu        = ROOT.RooRealVar("CMS_res_m","",0.0,-1,1.)
        res_ele       = ROOT.RooRealVar("CMS_res_e","",0.0,-1,1.)

        #Define the energy scale variables and the formula vars
        if channel =='MuMu':
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1", ROOT.RooArgList(m0, scale_mu))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1)", ROOT.RooArgList(sigma, res_mu))
        elif channel =='MuEle':
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1*@2", ROOT.RooArgList(m0, scale_mu,scale_ele))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1+@2)", ROOT.RooArgList(sigma, res_mu,res_ele))

        else:
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1", ROOT.RooArgList(m0, scale_ele))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1)", ROOT.RooArgList(sigma, res_ele))




        cballname=name+'Resolution'
        cball = ROOT.RooCBShape(cballname,'',observable,meanCB,sigmaCB,alpha,n)
        convolved = ROOT.RooFFTConvPdf(name,'',observable,pdf,cball,2)
#        convolved.setBufferFraction(0.0)

        #Set the scale and resolution systematics constant before fit
        scale_mu.setConstant(ROOT.kTRUE)
        scale_ele.setConstant(ROOT.kTRUE)
        res_mu.setConstant(ROOT.kTRUE)
        res_ele.setConstant(ROOT.kTRUE)
       
#        convolved.fitTo(data,ROOT.RooFit.SumW2Error(0))
        convolved.fitTo(data)

        scale_mu.setConstant(ROOT.kFALSE)
        scale_ele.setConstant(ROOT.kFALSE)
        res_mu.setConstant(ROOT.kFALSE)
        res_ele.setConstant(ROOT.kFALSE)

        params=[]
        for p in [mH,width,m0,sigma,alpha,n,scale_mu,scale_ele,res_mu,res_ele]:
            params.append({'name':p.GetName(),'mean':p.getVal(),'sigma':p.getError(),'var':p})


            
#        if width<0.1:        
        self.makeValidationPlot(observable,convolved,data,channel+str(mass))
        importPackage=[convolved,pdf,cball,meanCB,sigmaCB]    
#            return cball,params,observables,importPackage
#        else:
#            self.makeValidationPlot(observable,convolved,data)
#       importPackage=[cball,pdf,meanCB,sigmaCB]    
#            return convolved,params,observables,importPackage
        return convolved,params,observables,importPackage



    def makeBWDoubleCBallPdf(self,var,cuts,mini = 0,maxi = 1.0e+10,name='pdf',mass=125,width=0.1,channel='MuMu'):
        """
        Make Convolution of Breit Wigner and Crystal ball
        -----------------
        As input you can give var or vary:varx . If you give two dimensions
        it will use varx. mini,maxi define the boundaries
        for the observable. mass is the mean and width is the width of the distribution.
        The default names of the parameters are synced with Higgs combination package (MH=mass)
        For the energy scale and resolution of the crystal ball two nuisances are created 
        for each to account for the effect of muons and electrons in the scale and sigma 
        i.e for four lepton analysis.
        If the width is < 100 MeV it just returns the crystalball shifted to the mass
        """

        data,observables = self.makeDataSet(var,cuts,mini,maxi,0,1e+10,"tmpData",500000)

#        data,observables = self.makeDataHist(var,cuts,1000,mini,maxi,20,0,1e+10,"tmpData")
        observable=observables[0]

        
        #Make Breit Wigner
        mH     = ROOT.RooRealVar("MH","",mass)
        width  = ROOT.RooRealVar("width","",width)
        pdf=ROOT.RooBreitWigner("HiggsBW","BW",observable,mH,width);


        #Make crystal ball
        m0     = ROOT.RooRealVar("CMS_CB_MeanFit","",0,-50,50)
        sigma  = ROOT.RooRealVar("CMS_CB_SigmaFit","",1.,0.01,100.)
        alpha  = ROOT.RooRealVar("CMS_CB_Alpha","",1.,0.01,100.)
        n      = ROOT.RooRealVar("CMS_CB_N","",2,0.01,100.)
        alpha2  = ROOT.RooRealVar("CMS_CB_Alpha2","",1.48,0.01,100.)
        n2      = ROOT.RooRealVar("CMS_CB_N2","",20,0.01,100)


        scale_mu      = ROOT.RooRealVar("CMS_scale_m","",1.0,0,2.)
        scale_ele     = ROOT.RooRealVar("CMS_scale_e","",1.0,0,2.)
        res_mu        = ROOT.RooRealVar("CMS_res_m","",0.0,-1,1.)
        res_ele       = ROOT.RooRealVar("CMS_res_e","",0.0,-1,1.)

        #Define the energy scale variables and the formula vars
        if channel =='MuMu':
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1", ROOT.RooArgList(m0, scale_mu))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1)", ROOT.RooArgList(sigma, res_mu))
        elif channel =='MuEle':
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1*@2", ROOT.RooArgList(m0, scale_mu,scale_ele))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1+@2)", ROOT.RooArgList(sigma, res_mu,res_ele))

        else:
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1", ROOT.RooArgList(m0, scale_ele))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1)", ROOT.RooArgList(sigma, res_ele))





        cball = ROOT.RooDoubleCB(name+'Res','r',observable,meanCB,sigmaCB,alpha,n,alpha2,n2)

        convolved = ROOT.RooFFTConvPdf(name,'',observable,pdf,cball,2)
        convolved.setBufferFraction(0.2)

        #Set the scale and resolution systematics constant before fit
        scale_mu.setConstant(ROOT.kTRUE)
        scale_ele.setConstant(ROOT.kTRUE)
        res_mu.setConstant(ROOT.kTRUE)
        res_ele.setConstant(ROOT.kTRUE)
       
        convolved.fitTo(data,ROOT.RooFit.SumW2Error(0))
#        cball.fitTo(data)

        scale_mu.setConstant(ROOT.kFALSE)
        scale_ele.setConstant(ROOT.kFALSE)
        res_mu.setConstant(ROOT.kFALSE)
        res_ele.setConstant(ROOT.kFALSE)

        params=[]
        for p in [mH,width,m0,sigma,alpha,n,alpha2,n2,scale_mu,scale_ele,res_mu,res_ele]:
            params.append({'name':p.GetName(),'mean':p.getVal(),'sigma':p.getError(),'var':p})
            
        self.makeValidationPlot(observable,convolved,data,channel+str(mass))
        importPackage=[convolved,pdf,cball,meanCB,sigmaCB]    
        return convolved,params,observables,importPackage
        



    def makeHZZBWCBallPdf(self,var,cuts,mini = 0,maxi = 1.0e+10,name='pdf',mass=125,widthh=0.1,channel='MuMu'):
        """
        Make Convolution of Breit Wigner and Crystal ball
        using the modified BW for H->ZZ taking into acoount 
        the partonic fractions
        -----------------
        As input you can give var or vary:varx . If you give two dimensions
        it will use varx. mini,maxi define the boundaries
        for the observable. mass is the mean and width is the width of the distribution.
        The default names of the parameters are synced with Higgs combination package (MH=mass)
        For the energy scale and resolution of the crystal ball two nuisances are created 
        for each to account for the effect of muons and electrons in the scale and sigma 
        i.e for four lepton analysis.
        If the width is < 100 MeV it just returns the crystalball shifted to the mass
        """

        data,observables = self.makeDataSet(var,cuts,mini,maxi,-1e+10,1e+10,"tmpData",100000)
        observable=observables[0]

        #Make Breit Wigner
        mH     = ROOT.RooRealVar("MH","",mass)
        width  = ROOT.RooRealVar("width","",1.0)
        pdf    = ROOT.RooRelBWUFParam("HiggsBW", "signalBW",observable,mH,width)

        #Make crystal ball
        m0     = ROOT.RooRealVar("CMS_CB_MeanFit","",0.0,-10.,10.)
        sigma  = ROOT.RooRealVar("CMS_CB_SigmaFit","",1.,0.,10.)
        alpha  = ROOT.RooRealVar("CMS_CB_Alpha","",1,0,2.)
        n      = ROOT.RooRealVar("CMS_CB_N","",5,0.,50.)

        scale_mu      = ROOT.RooRealVar("CMS_scale_m","",1.0,0,2.)
        scale_ele     = ROOT.RooRealVar("CMS_scale_e","",1.0,0,2.)
        res_mu        = ROOT.RooRealVar("CMS_res_m","",0.0,-1,1.)
        res_ele       = ROOT.RooRealVar("CMS_res_e","",0.0,-1,1.)

        #Define the energy scale variables and the formula vars
        if channel =='MuMu':
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1", ROOT.RooArgList(m0, scale_mu))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1)", ROOT.RooArgList(sigma, res_mu))
        elif channel =='MuEle':
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1*@2", ROOT.RooArgList(m0, scale_mu,scale_ele))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1+@2)", ROOT.RooArgList(sigma, res_mu,res_ele))

        else:
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1", ROOT.RooArgList(m0, scale_ele))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1)", ROOT.RooArgList(sigma, res_ele))




        cballname=name+'Resolution'
        cball = ROOT.RooCBShape(cballname,'',observable,meanCB,sigmaCB,alpha,n)
        convolved = ROOT.RooFFTConvPdf(name,'',observable,pdf,cball,2)
        convolved.setBufferFraction(0.2)


        #Set the scale and resolution systematics constant before fit
        scale_mu.setConstant(ROOT.kTRUE)
        scale_ele.setConstant(ROOT.kTRUE)
        res_mu.setConstant(ROOT.kTRUE)
        res_ele.setConstant(ROOT.kTRUE)


       
        convolved.fitTo(data)

        scale_mu.setConstant(ROOT.kFALSE)
        scale_ele.setConstant(ROOT.kFALSE)
        res_mu.setConstant(ROOT.kFALSE)
        res_ele.setConstant(ROOT.kFALSE)

        params=[]
        for p in [mH,width,m0,sigma,alpha,n,scale_mu,scale_ele,res_mu,res_ele]:
            params.append({'name':p.GetName(),'mean':p.getVal(),'sigma':p.getError(),'var':p})


            
        self.makeValidationPlot(observable,convolved,data,channel+str(mass))
        importPackage=[pdf,cball,cball1,cball2,meanCB,sigmaCB]    
        return convolved,params,observables,importPackage



    def makeHZZBWDoubleCBallPdf(self,var,cuts,mini = 0,maxi = 1.0e+10,name='pdf',mass=125,widthh=0.1,channel='MuMu'):
        """
        Make Convolution of Breit Wigner and Crystal ball
        using the modified BW for H->ZZ taking into acoount 
        the partonic fractions
        -----------------
        As input you can give var or vary:varx . If you give two dimensions
        it will use varx. mini,maxi define the boundaries
        for the observable. mass is the mean and width is the width of the distribution.
        The default names of the parameters are synced with Higgs combination package (MH=mass)
        For the energy scale and resolution of the crystal ball two nuisances are created 
        for each to account for the effect of muons and electrons in the scale and sigma 
        i.e for four lepton analysis.
        If the width is < 100 MeV it just returns the crystalball shifted to the mass
        """

        data,observables = self.makeDataSet(var,cuts,mini,maxi,-1e+10,1e+10,"tmpData",100000)
        observable=observables[0]

        
        #Make Breit Wigner
        mH     = ROOT.RooRealVar("MH","",mass)
        width  = ROOT.RooRealVar("width","",1.0)
        pdf    = ROOT.RooRelBWUFParam("HiggsBW", "signalBW",observable,mH,width)

        #Make crystal ball
        m0     = ROOT.RooRealVar("CMS_CB_MeanFit","",0,-50,50)
        sigma  = ROOT.RooRealVar("CMS_CB_SigmaFit","",1.,0.01,100.)
        alpha  = ROOT.RooRealVar("CMS_CB_Alpha","",1.,0.01,100.)
        n      = ROOT.RooRealVar("CMS_CB_N","",2,0.01,100.)
        alpha2  = ROOT.RooRealVar("CMS_CB_Alpha2","",1.48,0.01,100.)
        n2      = ROOT.RooRealVar("CMS_CB_N2","",20,0.01,100)


        scale_mu      = ROOT.RooRealVar("CMS_scale_m","",1.0,0,2.)
        scale_ele     = ROOT.RooRealVar("CMS_scale_e","",1.0,0,2.)
        res_mu        = ROOT.RooRealVar("CMS_res_m","",0.0,-1,1.)
        res_ele       = ROOT.RooRealVar("CMS_res_e","",0.0,-1,1.)

        #Define the energy scale variables and the formula vars
        if channel =='MuMu':
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1", ROOT.RooArgList(m0, scale_mu))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1)", ROOT.RooArgList(sigma, res_mu))
        elif channel =='MuEle':
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1*@2", ROOT.RooArgList(m0, scale_mu,scale_ele))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1+@2)", ROOT.RooArgList(sigma, res_mu,res_ele))

        else:
            meanCB = ROOT.RooFormulaVar('CMS_CB_Mean',"@0*@1", ROOT.RooArgList(m0, scale_ele))
            sigmaCB = ROOT.RooFormulaVar('CMS_CB_Sigma',"@0*(1+@1)", ROOT.RooArgList(sigma, res_ele))

        cballname=name+'Resolution'
        cball = ROOT.RooDCBShape(cballname,'',observable,meanCB,sigmaCB,alpha,n,alpha2,n2)
        convolved = ROOT.RooFFTConvPdf(name,'',observable,pdf,cball,2)
        convolved.setBufferFraction(0.2)


        #Set the scale and resolution systematics constant before fit
        scale_mu.setConstant(ROOT.kTRUE)
        scale_ele.setConstant(ROOT.kTRUE)
        res_mu.setConstant(ROOT.kTRUE)
        res_ele.setConstant(ROOT.kTRUE)


       
        convolved.fitTo(data)

        scale_mu.setConstant(ROOT.kFALSE)
        scale_ele.setConstant(ROOT.kFALSE)
        res_mu.setConstant(ROOT.kFALSE)
        res_ele.setConstant(ROOT.kFALSE)

        params=[]
        for p in [mH,width,m0,sigma,alpha,n,alpha2,n2,scale_mu,scale_ele,res_mu,res_ele]:
            params.append({'name':p.GetName(),'mean':p.getVal(),'sigma':p.getError(),'var':p})


            
        self.makeValidationPlot(observable,convolved,data,channel+str(mass))
        importPackage=[pdf,cball,meanCB,sigmaCB]    
        return convolved,params,observables,importPackage

    def drawSmoothedTH1(self,var,cuts,bins,mini,maxi,binsy=20,miny=0,maxy=1e+10):
        data,observables = self.makeDataSet(var,cuts,mini,maxi,miny,maxy,"tmpData")
        if len(observables)==1:
            pdf=ROOT.RooNDKeysPdf('pdf',"keyspdf",ROOT.RooArgList(observables[0]), data, "am",1,3)
            h=pdf.createHistogram("observable",bins)
        else:    
            pdf=ROOT.RooNDKeysPdf('pdf',"keyspdf",ROOT.RooArgList(observables[0],observables[1]), data, "am",1,3)
            h=pdf.createHistogram("observable1,observable2",bins,binsy)

        return h


