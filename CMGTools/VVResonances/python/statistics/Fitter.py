import ROOT


class Fitter(object):
    def __init__(self,poi = ['x']):
        self.cache=ROOT.TFile("cache.root","RECREATE")
        self.cache.cd()

        self.w=ROOT.RooWorkspace("w","w")
        self.dimensions = len(poi)
        self.poi=poi
        for v in poi:
            self.w.factory(v+"[0,1]")


    def factory(self,expr):
        self.w.factory(expr)

    def function(self,name,function,dependents):
        self.w.factory("expr::"+name+"('"+function+"',"+','.join(dependents)+")")


    def bernstein(self,name = 'model',poi='x',order=1):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

        cList = ROOT.RooArgList()
        for i in range(0,order):
            self.w.factory("c_"+str(i)+"[0,100]")
            cList.add(self.w.var("c_"+str(i)))
        bernsteinPDF = ROOT.RooBernsteinFast(order)(name,name,self.w.var(poi),cList)
        getattr(self.w,'import')(bernsteinPDF,ROOT.RooFit.Rename(name))


    def bernsteinPlusGaus(self,name = 'model',poi='x',order=1):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
        self.w.factory("RooGaussian::"+name+"G(x,c_0[-10,-15,15],c_1[3,0,10])")


        cList = ROOT.RooArgList()
        for i in range(3,order):
            self.w.factory("c_"+str(i)+"[0.1,0,100]")
            cList.add(self.w.var("c_"+str(i)))
        bernsteinPDF = ROOT.RooBernsteinFast(order)(name+"B",name,self.w.var(poi),cList)
        getattr(self.w,'import')(bernsteinPDF,ROOT.RooFit.Rename(name+"B"))

        self.w.factory("SUM::"+name+"(c_2[0.5,0,1]*"+name+"G,"+name+"B)")



    def expo(self,name = 'model',poi='x'):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
        self.w.factory("RooExponential::"+name+"(x,c_0[-1,-1000,0])")

    def bifur(self,name = 'model',poi='x'):
        self.w.factory("RooBifurGauss::"+name+"("+poi+",c_0[0,300],c_1[0,1000],c_2[0,1000])")






    def doubleCB(self,name = 'model',poi='x'):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

        self.w.factory("c_0[0.1,-100,200]")
        self.w.factory("c_1[5,0,40]")
        self.w.factory("c_2[100]")
        self.w.factory("c_3[100]")
        self.w.factory("c_4[2,1,20]")
        self.w.factory("c_5[2,0,20]")

        doubleCB = ROOT.RooDoubleCB(name,name,self.w.var(poi),self.w.var("c_0"),self.w.var("c_1"),self.w.var("c_2"),self.w.var("c_3"),self.w.var("c_4"),self.w.var("c_5"))
        getattr(self.w,'import')(doubleCB,ROOT.RooFit.Rename(name))



    def softDrop(self,name = 'model',poi='x'):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

        self.w.factory("c_0[-1000,-10000,-0.001]")
        self.w.factory("c_1[100,30,1000]")
        self.w.factory("c_2[1,0,100]")
        self.w.factory("c_3[1,0,100]")
        self.w.factory("c_4[1,0,100]")

        softDrop = ROOT.RooFatJetFallingPdf(name,name,self.w.var(poi),self.w.var("c_0"),self.w.var("c_1"),self.w.var("c_2"),self.w.var("c_3"),self.w.var("c_4"))
        getattr(self.w,'import')(softDrop,ROOT.RooFit.Rename(name))

#        self.w.factory("RooGaussian:modelG(x,c_3[0.0001,-5000,65],c_4[20,0,100])")
#        self.w.factory("SUM::"+name+"(c_5[0.5,0,1]*modelG,modelF)")




    def jetResonance(self,name = 'model',poi='x'):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
        self.w.factory("mean[80,50,100]")
        self.w.factory("sigma[10,0,100]")
        self.w.factory("alpha1[3,1,6]")
        self.w.factory("n1[10,0,100]")
        self.w.factory("alpha2[3,1,6]")
        self.w.factory("n2[10,0,100]")
        peak = ROOT.RooDoubleCB(name+'S','modelS',self.w.var('x'),self.w.var('mean'),self.w.var('sigma'),self.w.var('alpha1'),self.w.var('n1'),self.w.var('alpha2'),self.w.var('n2'))
        getattr(self.w,'import')(peak,ROOT.RooFit.Rename(name+'S'))

        self.w.factory("RooExponential::"+name+"B(x,slope[-1,-10,0])")
        
        self.w.factory("SUM::"+name+"(NS[1,0,1000000]*"+name+"S,NB[1,0,1000000]*"+name+"B)")



    def signal2D(self,name = 'model',boson='W'):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
        self.w.factory("MW[80.385]")
        self.w.factory("MZ[91.1876]")
        self.w.factory("MH[1000]")

        self.w.factory("n[1,0.7,1.3]")
        self.w.factory("scale[1,0,2]")
        self.w.factory("expr::meanNorm('scale*M"+boson+"',scale,M"+boson+")")

        self.w.factory("sigma[0.1,0,1]")
        self.w.factory("expr::sigmaNorm('sigma*M"+boson+"',sigma,M"+boson+")")
        self.w.factory("alpha1[1]")
        self.w.factory("n1[10,0,100]")
        self.w.factory("alpha2[1]")
        self.w.factory("n2[10,0,100]")
        
        peak_jj = ROOT.RooDoubleCB(name+'jj','modelS',self.w.var('m'),self.w.function('meanNorm'),self.w.function('sigmaNorm'),self.w.var('alpha1'),self.w.var('n1'),self.w.var('alpha2'),self.w.var('n2'))


        self.w.factory("SCALE[1,0,2]")
        self.w.factory("expr::MEANNORM('SCALE*MH',SCALE,MH)")

        self.w.factory("SIGMA[0.1,0,1]")
        self.w.factory("expr::SIGMANORM('SIGMA*MH',SIGMA,MH)")

        self.w.factory("ALPHA1[1]")
        self.w.factory("N1[10,0,100]")
        self.w.factory("ALPHA2[1]")
        self.w.factory("N2[10,0,100]")
        peak_vv = ROOT.RooDoubleCB(name+'VV','modelS',self.w.var('M'),self.w.function('MEANNORM'),self.w.function('SIGMANORM'),self.w.var('ALPHA1'),self.w.var('N1'),self.w.var('ALPHA2'),self.w.var('N2'))



        getattr(self.w,'import')(peak_jj,ROOT.RooFit.Rename(name+'jj'))
        getattr(self.w,'import')(peak_vv,ROOT.RooFit.Rename(name+'VV'))
      
        self.w.factory("PROD::"+name+"("+name+"jj,"+name+"VV)")



    def doublePol(self,name = 'model',poi='x'):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

        self.w.factory("c_0[0,-1,1]")
        self.w.factory("c_1[0,-100,100]")
        self.w.factory("c_2[0,-1000,1000]")
        self.w.factory("c_3[0]")

        softDrop = ROOT.RooDoublePolPdf(name,name,self.w.var(poi),self.w.var("c_0"),self.w.var("c_1"),self.w.var("c_2"),self.w.var("c_3"))
        getattr(self.w,'import')(softDrop,ROOT.RooFit.Rename(name))



    def softDrop2D(self):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

        self.w.factory("M[1000,20000]")
        self.w.factory("m[3,203]")


        self.w.factory("scale_a[50,0,100]")
        self.w.factory("scale_b[-1250,-10000,0]")
        self.w.factory("expr::scale('scale_a+scale_b*M/13000.0',scale_a,scale_b,M)")

        self.w.factory("offset[160,0,2000]")

        self.w.factory("alpha[0.005,0,10]")

        self.w.factory("beta_f[2.5,0,100]")
        self.w.factory("expr::beta('-alpha+beta_f*M/13000.0',alpha,beta_f,M)")


        self.w.factory("gamma[0.1,0,1]")


        softDrop = ROOT.RooFatJetFallingPdf("modelJJ","",self.w.var("m"),self.w.function("scale"),self.w.var("offset"),self.w.var("alpha"),self.w.function("beta"),self.w.var("gamma"))
        getattr(self.w,'import')(softDrop,ROOT.RooFit.Rename("modelJJ"))

        self.w.factory("p0[1,0,100]")
        self.w.factory("p1[2,0,100]")
        self.w.factory("p2[0]")

        qcd = ROOT.RooQCDPdf("modelQ","",self.w.var("M"),self.w.var("p0"),self.w.var("p1"),self.w.var("p2"))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename("modelQ"))

        self.w.factory("PROD::model(modelJJ|M,modelQ)")



    def bifurTimesQCD(self):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

        self.w.factory("M[1000,200000]")
        self.w.factory("m[25,165]")



        self.w.factory("alpha_0[-35.661,-40,-30]")
        self.w.factory("alpha_1[15.882,10,35]")
        self.w.factory("expr::alpha('alpha_0+alpha_1*log(M)',alpha_0,alpha_1,M)")

        self.w.factory("beta_0[19.5,0,35]")
        self.w.factory("beta_1[0.0101766,0,0.05]")
        self.w.factory("expr::beta('beta_0+beta_1*M',beta_0,beta_1,M)")       

        self.w.factory("gamma[62.6,40,80]")

        self.w.factory("RooBifurGauss::modelJJ(m,alpha,beta,gamma)")


        self.w.factory("p0[33,0,100]")
        self.w.factory("p1[0.5,0,10]")
        self.w.factory("p2[0.001,0,10]")

        qcd = ROOT.RooQCDPdf("modelQ","",self.w.var("M"),self.w.var("p0"),self.w.var("p1"),self.w.var("p2"))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename("modelQ"))
        self.w.factory("PROD::model(modelJJ|M,modelQ)")

    def expoTimesQCD(self):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

        self.w.factory("M[1000,200000]")
        self.w.factory("m[25,165]")



        self.w.factory("alpha_0[-35.661,-40,-30]")
        self.w.factory("alpha_1[15.882,10,35]")
        self.w.factory("expr::alpha('alpha_0+alpha_1*(M)',alpha_0,alpha_1,M)")

        self.w.factory("RooExponential::modelJJ(m,alpha)")


        self.w.factory("p0[33,0,100]")
        self.w.factory("p1[0.5,0,10]")
        self.w.factory("p2[0.001,0,10]")

        qcd = ROOT.RooQCDPdf("modelQ","",self.w.var("M"),self.w.var("p0"),self.w.var("p1"),self.w.var("p2"))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename("modelQ"))
        self.w.factory("PROD::model(modelJJ|M,modelQ)")




    def backgroundFast(self):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")

        self.w.factory("M[1000,20000]")
        self.w.factory("m[25,175]")



        self.w.factory("alpha_0[7,0,100]")
        self.w.factory("alpha_1[0,-0.1,0.1]")
        self.w.factory("expr::alpha('alpha_0+alpha_1*M',alpha_0,alpha_1,M)")

        self.w.factory("beta_0[6,0,100]")
        self.w.factory("beta_1[0,-0.1,0.1]")
        self.w.factory("expr::beta('beta_0+beta_1*M',beta_0,beta_1,M)")       

        self.w.factory("gamma_0[1,0,100]")
        self.w.factory("gamma_1[0,-0.1,0.1]")
        self.w.factory("expr::gamma('gamma_0+gamma_1*M',gamma_0,gamma_1,M)")       
        self.w.factory("delta[1.4,0,1000]")


        cList = ROOT.RooArgList()
        cList.add(self.w.function("alpha"))
        cList.add(self.w.function("beta"))
        cList.add(self.w.function("gamma"))
        cList.add(self.w.var("delta"))


        softDrop = ROOT.RooBernsteinFast(4)("modelJJ","modelJJ",self.w.var('m'),cList)
        getattr(self.w,'import')(softDrop,ROOT.RooFit.Rename("modelJJ"))
        self.w.factory("p0[20,0,100]")
        self.w.factory("p1[0.5,0,100]")
        self.w.factory("p2[0.0001,0,10]")

        qcd = ROOT.RooQCDPdf("modelQ","",self.w.var("M"),self.w.var("p0"),self.w.var("p1"),self.w.var("p2"))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename("modelQ"))

        self.w.factory("PROD::model(modelJJ|M,modelQ)")


    def qcd(self):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
        self.w.factory("M[1000,10000]")
        self.w.factory("m[10,200]")
        self.w.factory("p0[0,100]")
#        self.w.factory("expr::MSHIFT('M/13000.0',M)")
        self.w.factory("p1[0,100]")
        self.w.factory("p2[0]")
        qcd = ROOT.RooQCDPdf("model","",self.w.var("M"),self.w.var("p0"),self.w.var("p1"),self.w.var("p2"))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename("model"))
#        self.w.factory("RooExponential::modelS(MSHIFT,p0)")
#        self.w.factory("N[2000,0,1000000000]")
#        qcdE = ROOT.RooExtendPdf("model","",self.w.pdf("modelS"),self.w.var("N"))
#        getattr(self.w,'import')(qcdE,ROOT.RooFit.Rename("model"))


    def qcdINT(self,poi='x'):
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
        self.w.factory("p0[0,100]")
        self.w.factory("p1[0,100]")
        qcd = ROOT.RooDiBosonQCDPdf(13)("model","",self.w.var(poi),self.w.var("p0"),self.w.var("p1"))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename("model"))



    def erfpowPlusGaus(self,name = 'model',poi='x'):      
        self.w.factory("RooGaussian:modelG(x,c_0[0.0001,-5000,40],c_1[20,0,100])")
        self.w.factory("c_2[-2,-20,0]")
        self.w.factory("expr::xdisp('x-c_2',x,c_2)")
        self.w.factory("c_3[-1,-20,0]")
        self.w.factory("c_4[30,-1000,1000]")
        self.w.factory("c_5[11,-10000,10000]")      


        erfexp = ROOT.RooErfPowPdf(name+"F",name,self.w.function('xdisp'),self.w.var("c_3"),self.w.var("c_4"),self.w.var("c_5"))
        getattr(self.w,'import')(erfexp,ROOT.RooFit.Rename(name))
        self.w.factory("SUM::"+name+"(c_6[0.5,0.,1]*modelG,modelF)")



    def erfpow(self,name = 'model',poi='x'):      
        self.w.factory("c_0[0]")
        self.w.factory("c_1[0]")
        self.w.factory("c_6[0]")
        self.w.factory("c_2[-2,-200,20]")
        self.w.factory("expr::xdisp('x-c_2',x,c_2)")
        self.w.factory("c_3[-1,-20,0]")
        self.w.factory("c_4[30,-1000,1000]")
        self.w.factory("c_5[11,-10000,10000]")      
        erfexp = ROOT.RooErfPowPdf(name,name,self.w.function('xdisp'),self.w.var("c_3"),self.w.var("c_4"),self.w.var("c_5"))
        getattr(self.w,'import')(erfexp,ROOT.RooFit.Rename(name))



    def erfpowPlusGausLOG(self,name = 'model',poi='x'):      
        self.w.factory("RooGaussian:modelG(x,c_0[0.0001,-5000,40],c_1[20,0,100])")
        self.w.factory("c_2[0,-20,20]")
        self.w.factory("expr::xdisp('x-c_2',x,c_2)")
        self.w.factory("c_3[2,0,20]")
        self.w.factory("c_4[30,-100,100]")
        self.w.factory("c_5[11,-1000,1000]")      


        erfexp = ROOT.RooErfPowPdf(name+"F",name,self.w.function('xdisp'),self.w.var("c_3"),self.w.var("c_4"),self.w.var("c_5"))
        getattr(self.w,'import')(erfexp,ROOT.RooFit.Rename(name))
        self.w.factory("SUM::"+name+"(c_6[0.5,0,1]*modelG,modelF)")




        
    def importBinnedData(self,histogram,poi = ["x"],name = "data"):
        cList = ROOT.RooArgList()
        for i,p in enumerate(poi):
            cList.add(self.w.var(p))
            if i==0:
                axis=histogram.GetXaxis()
            elif i==1:
                axis=histogram.GetYaxis()
            elif i==2:
                axis=histogram.GetZaxis()
            else:
                print 'Asking for more than 3 D . ROOT doesnt support that, use unbinned data instead'
                return
            mini=axis.GetXmin()
            maxi=axis.GetXmax()
            bins=axis.GetNbins()
            self.w.var(p).setMin(mini)
            self.w.var(p).setMax(maxi)
            self.w.var(p).setBins(bins)
        dataHist=ROOT.RooDataHist(name,name,cList,histogram)
        getattr(self.w,'import')(dataHist,ROOT.RooFit.Rename(name))



    def importUnbinnedData(self,tree,name,variables,poi,accept):
        cList = ROOT.RooArgSet()
        for i,p in enumerate(poi):
            cList.add(self.w.var(p))
        
        data=ROOT.RooDataSet(name,name,cList)

        for event in tree:
            if not accept(event):
                continue
            for i,p in enumerate(poi):
                val =  getattr(event,variables[i])
                cList.find(p).setVal(val[0])

            data.add(cList)


        getattr(self.w,'import')(data,ROOT.RooFit.Rename(name))
            



    def fit(self,model = "model",data="data",options=[]):
        if len(options)==0:
            self.w.pdf(model).fitTo(self.w.data("data"))
        if len(options)==1:
            self.w.pdf(model).fitTo(self.w.data("data"),options[0])
        if len(options)==2:
            self.w.pdf(model).fitTo(self.w.data("data"),options[0],options[1])
        if len(options)==3:
            self.w.pdf(model).fitTo(self.w.data("data"),options[0],options[1],options[2])
        if len(options)==4:
            self.w.pdf(model).fitTo(self.w.data("data"),options[0],options[1],options[2],options[3])


    def fetch(self,var):
        return (self.w.var(var).getVal(), self.w.var(var).getError())

    def projection(self,model = "model",data="data",poi="x",filename="fit.root"):
        
        frame=self.w.var(poi).frame()
        self.w.data(data).plotOn(frame)
        self.w.pdf(model).plotOn(frame)
        c=ROOT.TCanvas("c","c")
        c.cd()
        frame.Draw()
        c.SaveAs(filename)
        
        
        
                                

        
        
