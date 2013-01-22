#CHANGED
import ROOT
import json


class PDFFactory(object):
    def __init__(self,workspace,analysis='hzz4l'):
        self.w = workspace
        self.analysis=analysis
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")
        
    def makeCB(self,name,var,center = 125,channel = 'MuMu',suffix=''):
        '''

        SINGLE SIDED CRYSTAL BALL

        '''
        self.w.factory('m0['+str(center)+','+str(center-100)+','+str(center+100)+']')
        self.w.factory('sigma[10,0,100]')
        self.w.factory('alpha[10,0,100]')
        self.w.factory('n[1,0,100]')
        self.w.factory('CMS_scale_m[1.0,0,2]')
        self.w.factory('CMS_res_m[0.0,-2,2]')
        self.w.factory('CMS_scale_e[1.0,0,2]')
        self.w.factory('CMS_res_e[0.0,-2,2]')
        #make the formulas for scale and resolution



        if channel == 'MuMu':
            expr = "@0*@1"
            self.w.factory("expr::m0_param('"+expr+"',m0,CMS_scale_m)")
            expr = "@0*(1+@1)"
            self.w.factory("expr::sigma_param('"+expr+"',sigma+,"+"CMS_res_m)")
        if channel == 'EleEle':
            expr = "@0*@1"
            self.w.factory("expr::m0_param('"+expr+"',m0,CMS_scale_e)")
            expr = "@0*(1+@1)"
            self.w.factory("expr::sigma+_param('"+expr+"',sigma,CMS_res_e)")
        if channel == 'MuEle':
            expr = "@0*@1*@2"
            self.w.factory("expr::+m0_param('"+expr+"',m0,CMS_scale_e,CMS_scale_m)")
            expr = "@0*(1+@1+@2)"
            self.w.factory("expr::sigma_param('"+expr+"',sigma,CMS_res_e,CMS_res_m)")


        self.w.factory('RooCBShape::'+name+'('+var+','+m0+','+sigma+','+alpha+','+n+')')
        self.params=[m0,sigma,alpha,n]

    def makeDCB(self,name,var,center = 125,channel = 'MuMu',suffix=''):
        '''
        DOUBLE SIDED CRYSTAL BALL
        '''



        
        self.w.factory('m0[0.01,-50,50]')
        self.w.factory('sigma[10,0.01,100]')
        self.w.factory('alphaL[1,0.01,100]')
        self.w.factory('nL[2,0.01,100]')
        self.w.factory('alphaR[5,0.01,100]')
        self.w.factory('nR[20]')
        self.w.factory('CMS_scale_m[1.0,0.0,2]')
        self.w.factory('CMS_res_m[0.0,-2,2]')
        self.w.factory('CMS_scale_e[1.0,0,2]')
        self.w.factory('CMS_res_e[0.0,-2,2]')


        #make the formulas for scale and resolution



        if channel == 'MuMu':
            expr = "@0*@1+"+str(center)
            self.w.factory("expr::m0_param('"+expr+"',m0,CMS_scale_m)")
            expr = "@0*(1+@1)"
            self.w.factory("expr::sigma_param('"+expr+"',sigma,CMS_res_m)")
        if channel == 'EleEle':
            expr = "@0*@1+"+str(center)
            self.w.factory("expr::m0_param('"+expr+"',m0,CMS_scale_e)")
            expr = "@0*(1+@1)"
            self.w.factory("expr::sigma_param('"+expr+"',sigma,CMS_res_e)")
        if channel == 'MuEle':
            expr = "@0*@1*@2+"+str(center)
            self.w.factory("expr::m0_param('"+expr+"',m0,CMS_scale_e,CMS_scale_m)")
            expr = "@0*(1+@1+@2)"
            self.w.factory("expr::sigma_param('"+expr+"',sigma,CMS_res_e,CMS_res_m)")

        pdf    = ROOT.RooDoubleCB(name,name,self.w.var(var),self.w.function('m0_param'),self.w.function('sigma_param'),self.w.var('alphaL'),self.w.var('nL'),self.w.var('alphaR'),self.w.var('nR'))
        getattr(self.w,'importClassCode')(ROOT.RooDoubleCB.Class(),1)
        getattr(self.w,'import')(pdf)


        self.params=['m0','sigma','alphaL','nL','alphaR','nR']



    def makeLandau(self,name,var):
        '''

        Landau

        '''
        self.w.factory('mean[150,50,400]')
        self.w.factory('sigma[20,0,800]')
        self.w.factory('RooLandau::'+name+'('+var+',mean,sigma)')
        self.params=['mean','sigma']




    def makeBW(self,name,var):
        '''

        Special Breit Wigner

        '''
        self.w.factory('MH[125,100,1000]')
        self.w.factory('width[1]')
        
        pdf    = ROOT.RooRelBWUFParam(name,name,self.w.var(var),self.w.var('MH'),self.w.var('width'))
        getattr(self.w,'importClassCode')(ROOT.RooRelBWUFParam.Class(),1)
        getattr(self.w,'import')(pdf)

    def makeBWHighMass(self,name,var):
        '''

        Special Breit Wigner High Mass

        '''
        self.w.factory('MH[125,100,1000]')
        self.w.factory('width[100,1,500]')
        
        pdf    = ROOT.RooRelBWHighMass(name,name,self.w.var(var),self.w.var('MH'),self.w.var('width'))
        getattr(self.w,'importClassCode')(ROOT.RooRelBWUFParam.Class(),1)
        getattr(self.w,'import')(pdf)


    def makeQQZZ(self,name,var):
        '''

        Empirical QQ -> ZZ 

        '''
        params=[]

        for i in range(0,14):
            params.append('a'+str(i))
                          
        self.w.factory(params[0]+'[110,0,500]')                  
        self.w.factory(params[1]+'[15,0,100]')                  
        self.w.factory(params[2]+'[110,0,500]')                  
        self.w.factory(params[3]+'[0.05,0,10]')                  
        self.w.factory(params[4]+'[185,0,500]')                  
        self.w.factory(params[5]+'[11,0,100]')                  
        self.w.factory(params[6]+'[35,0,1000]')                  
        self.w.factory(params[7]+'[0.1,0,10]')                  
        self.w.factory(params[8]+'[63,0,100]')                  
        self.w.factory(params[9]+'[0.07,0,10]')                  
        self.w.factory(params[10]+'[98,0,500]')                  
        self.w.factory(params[11]+'[-5,-30,0]')                  
        self.w.factory(params[12]+'[1300,0,10000]')                  
        self.w.factory(params[13]+'[0.08,0,10]')                  
        

        pdf=ROOT.RooqqZZPdf_v2(name,name,self.w.var(var),self.w.var(params[0]),self.w.var(params[1]),\
                               self.w.var(params[2]),self.w.var(params[3]),self.w.var(params[4]),self.w.var(params[5])\
                               ,self.w.var(params[6]),self.w.var(params[7]),self.w.var(params[8]),self.w.var(params[9])\
                               ,self.w.var(params[10]),self.w.var(params[11]),self.w.var(params[12]),self.w.var(params[13]))

        getattr(self.w,'importClassCode')(ROOT.RooqqZZPdf_v2.Class(),1)
        getattr(self.w,'import')(pdf)

        self.params=params



    def makeGGZZ(self,name,var):
        '''

        Empirical gg -> ZZ 

        '''
        params=[]

        for i in range(0,10):
            params.append('a'+str(i))
                          
        self.w.factory(params[0]+'[127,0,500]')                  
        self.w.factory(params[1]+'[39,0,300]')                  
        self.w.factory(params[2]+'[130,50,500]')                  
        self.w.factory(params[3]+'[0.05,0,1]')                  
        self.w.factory(params[4]+'[185,100,220]')                  
        self.w.factory(params[5]+'[15,0,50]')                  
        self.w.factory(params[6]+'[30,0,100]')                  
        self.w.factory(params[7]+'[0.3,0,2]')                  
        self.w.factory(params[8]+'[60,0,500]')                  
        self.w.factory(params[9]+'[0.07,-1,1]')                  

        pdf=ROOT.RooggZZPdf_v2(name,"qqzzpdf",self.w.var(var),self.w.var(params[0]),self.w.var(params[1]),\
                               self.w.var(params[2]),self.w.var(params[3]),self.w.var(params[4]),self.w.var(params[5])\
                               ,self.w.var(params[6]),self.w.var(params[7]),self.w.var(params[8]),self.w.var(params[9]))
        
        getattr(self.w,'importClassCode')(ROOT.RooggZZPdf_v2.Class(),1)
        getattr(self.w,'import')(pdf)

        self.params=params


    def makeTsallis(self,name,var,isFunction = False):
        self.w.factory('m[1,0.5,1000]')
        self.w.factory('n[2,0.0001,100]')
        self.w.factory('n2[1,0.0001,2]')

        self.w.factory('bb[0.0148,0.000,1000]')
        self.w.factory('bb2[0.00253,0.0001,1000]')
        self.w.factory('fexp[0.0202,0.0001,1000]')
        self.w.factory('T[0.23622,0.000,1000]')

        if isFunction:
            v=self.w.function(var)
        else:
            v=self.w.var(var)

        tsallis=ROOT.RooTsallis(name,name,v,\
                            self.w.var('m'),self.w.var('n'),\
                            self.w.var('n2'),self.w.var('bb'),\
                            self.w.var('bb2'),self.w.var('fexp'),\
                            self.w.var('T'))

        getattr(self.w,'importClassCode')(ROOT.RooTsallis.Class(),1)
        getattr(self.w,'import')(tsallis)

        self.params=['m','n','n2','bb','bb2','fexp','T']

    def makeTemplateFromFile(self,name,vars,dir,hname,shifts =[]):
        varList=ROOT.RooArgList()
        varSet=ROOT.RooArgSet()
        for v in vars:
            varList.add(self.w.var(v))
            varSet.add(self.w.var(v))
            
        f=ROOT.TFile('template_'+dir+'.root')
        print 'reading histo',hname
        h=f.Get(hname)
        hist = ROOT.RooDataHist(name+'Hist','',varList,h)
        pdf = ROOT.RooHistPdf(name+'0','',varSet,hist)
        pdfList=ROOT.RooArgList(pdf)
        coeffList=ROOT.RooArgList()
        storage = []
        for shift in shifts: 
            print 'reading', hname+'_'+shift+'_Down'
            
            hD=f.Get(hname+'_'+shift+'_Down')
            if  not (hD == None):
                histD = ROOT.RooDataHist(name+shift+'HistD','',varList,hD)
                pdfD = ROOT.RooHistPdf(name+'D','',varSet,histD)
                hU=f.Get(hname+'_'+shift+'_Up')
                histU = ROOT.RooDataHist(name+shift+'HistU','',varList,hU)
                pdfU = ROOT.RooHistPdf(name+'U','',varSet,histU)
                
                pdfList.add(pdfD)
                pdfList.add(pdfU)
                storage.append(pdfD)
                storage.append(pdfU)
                storage.append(histD)
                storage.append(histU)
                
                self.w.factory(shift+'[-3,3]')
                coeffList.add(self.w.var(shift))
            else:
                print 'ERROR OR WARNING -> Could not find',hname+'_'+shift+'_Down' 
        #if there were no systematics make a dummy
        if coeffList.getSize()==0:
            self.w.factory('dummy[0]')
            coeffList.add(self.w.var('dummy'))
            pdfList.add(pdf)
            pdfList.add(pdf)

        if len(vars)>1:
            morphedHistPdf = ROOT.FastVerticalInterpHistPdf2D(name,'',self.w.var(vars[0]),self.w.var(vars[1]),True,pdfList,coeffList,1.0,1)
        else:
            morphedHistPdf = ROOT.FastVerticalInterpHistPdf(name,'',self.w.var(vars[0]),pdfList,coeffList,1.0,1)
                
        getattr(self.w,'import')(morphedHistPdf)

        f.Close()

    def makeTemplateFromHisto(self,w,name,vars,hname):
        varList=ROOT.RooArgList()
        varSet=ROOT.RooArgSet()
        for v in vars:
            varList.add(self.w.var(v))
            varSet.add(self.w.var(v))
            
        hist = ROOT.RooDataHist(name+'Hist','',varList,hname)
        pdf = ROOT.RooHistPdf(name,'',varSet,hist)
        getattr(self.w,'import')(pdf)
        


    def makeConditional(self,name,name1,name2,x):
        conditional = ROOT.RooProdPdf(name,'',ROOT.RooArgSet(self.w.pdf(name1)),ROOT.RooFit.Conditional(ROOT.RooArgSet(self.w.pdf(name2)),ROOT.RooArgSet(self.w.var(x))))
        getattr(self.w,'import')(conditional)

    def makeMultiConditional(self,name,names,conds):
        if len(names)==2:
            conditional = ROOT.RooProdPdf(name,'',ROOT.RooArgSet(self.w.pdf(names[0])),ROOT.RooFit.Conditional(ROOT.RooArgSet(self.w.pdf(names[1])),ROOT.RooArgSet(self.w.var(conds[0]))))
        elif len(names)==3:
            conditional = ROOT.RooProdPdf(name,'',ROOT.RooArgSet(self.w.pdf(names[0])),ROOT.RooFit.Conditional(ROOT.RooArgSet(self.w.pdf(names[1])),ROOT.RooArgSet(self.w.var(conds[0]))),ROOT.RooFit.Conditional(ROOT.RooArgSet(self.w.pdf(names[2])),ROOT.RooArgSet(self.w.var(conds[1]))))
        elif len(names)==4:
            conditional = ROOT.RooProdPdf(name,'',ROOT.RooArgSet(self.w.pdf(names[0])),ROOT.RooFit.Conditional(ROOT.RooArgSet(self.w.pdf(names[1])),ROOT.RooArgSet(self.w.var(conds[0]))),ROOT.RooFit.Conditional(ROOT.RooArgSet(self.w.pdf(names[2])),ROOT.RooArgSet(self.w.var(conds[1]))),ROOT.RooFit.Conditional(ROOT.RooArgSet(self.w.pdf(names[3])),ROOT.RooArgSet(self.w.var(conds[2]))))
        getattr(self.w,'import')(conditional)



    def makeFastProduct(self,name,name1,name2):
        conditional = ROOT.RooVerticalInterpUncorrProd2D(name,'',ROOT.RooArgSet(self.w.pdf(name1),self.w.pdf(name2)))
        getattr(self.w,'importClassCode')(ROOT.RooVerticalInterpUncorrProd2D.Class(),1)
        getattr(self.w,'import')(conditional)

    def makeFastCond(self,name,names,conds):
        conditional = ROOT.RooProdPdf(name,'',ROOT.RooArgSet(self.w.pdf(names[0])),ROOT.RooFit.Conditional(ROOT.RooArgSet(self.w.pdf(names[1])),ROOT.RooArgSet(self.w.var(conds[0]),self.w.var(conds[1]))))
        getattr(self.w,'import')(conditional)



    def makeProduct(self,name,name1,name2):
        self.w.factory('PROD::'+name+'('+name2+','+name1+')')



 
    def makeConvolution(self,name,var,name1,name2):
        convolved = ROOT.RooFFTConvPdf(name,'',self.w.var(var),self.w.pdf(name1),self.w.pdf(name2),2)
        convolved.setBufferFraction(0.2)
        getattr(self.w,'import')(convolved)
        

        
    

    def loadPtPreset(self,preset):    
        f=open('PtParams.json')
        d=json.load(f)
        if preset in d:
            self.w.var('m').setVal(float(d[preset]['m']))
            self.w.var('n').setVal(float(d[preset]['n']))
            self.w.var('fexp').setVal(float(d[preset]['fexp']))
            self.w.var('bb').setVal(float(d[preset]['bb']))
            self.w.var('T').setVal(float(d[preset]['T']))
            self.w.var('n2').setVal(float(d[preset]['n2']))
            self.w.var('bb2').setVal(float(d[preset]['bb2']))
            f.close()

            return True
        else:
            return False


