import ROOT
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
import json


class DataCardMaker:
    def __init__(self,finalstate,category,period,luminosity=1.0):
        self.physics="WV"
        self.finalstate=finalstate
        self.category=category
        self.period=period
        self.contributions=[]
        self.systematics=[]

        self.tag=self.physics+"_"+finalstate+"_"+category+"_"+period
        self.rootFile = ROOT.TFile(self.tag+".root","RECREATE")
        self.rootFile.cd()
        self.w=ROOT.RooWorkspace("w","w")
        self.w.factory(self.physics+"_"+period+"_lumi["+str(luminosity)+"]")
        if period=='8TeV':
            self.sqrt_s=8000.0
        if period=='13TeV':
            self.sqrt_s=13000.0


    def addSystematic(self,name,kind,values,addPar = ""):
        self.systematics.append({'name':name,'kind':kind,'values':values })


    def add2DSignalModel(self,name,ID,events,jsonFile,boson="W"):

        self.w.factory("MH[2000]")
        self.w.var("MH").setConstant(1)
        
        self.w.factory("MW[80.385]")
        self.w.var("MW").setConstant(1)

        self.w.factory("MZ[91.1876]")
        self.w.var("MZ").setConstant(1)

        self.w.factory("CMS_jetMassScale[0,-1,1]")
        self.w.factory("CMS_jetMassResolution[0,-1,1]")
        self.w.factory("CMS_scale_"+self.physics+"[0,-1,1]")
        self.w.factory("CMS_resolution_"+self.physics+"[0,-1,1]")

        
        mjj='_'.join(['mjj',self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        
        f=open(jsonFile)
        info=json.load(f)
        scaleVar="_".join(["mean",name,self.tag])
        self.w.factory("expr::{name}('{mboson}*({param})*(1+{jet_mass_syst})',{mboson},MH,{jet_mass_syst})".format(name=scaleVar,mboson="M"+boson,param=info['scale'],jet_mass_syst="CMS_jetMassScale"))


        sigmaVar="_".join(["sigma",name,self.tag])
        self.w.factory("expr::{name}('{mboson}*({param})*(1+{jet_mass_syst})',{mboson},MH,{jet_mass_syst})".format(name=sigmaVar,mboson="M"+boson,param=info['sigma'],jet_mass_syst="CMS_jetMassResolution"))


        alpha1Var="_".join(["alpha1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=alpha1Var,param=info['alpha1']))

        alpha2Var="_".join(["alpha2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=alpha2Var,param=info['alpha2']))

        n1Var="_".join(["n1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=n1Var,param=info['n1']))

        n2Var="_".join(["n2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=n2Var,param=info['n2']))

        SCALEVar="_".join(["MEAN",name,self.tag])
        self.w.factory("expr::{name}('MH*({param})*(1+{vv_syst})',MH,{vv_syst})".format(name=SCALEVar,param=info['SCALE'],vv_syst="CMS_scale_"+self.physics))

        SIGMAVar="_".join(["SIGMA",name,self.tag])
        self.w.factory("expr::{name}('MH*({param})*(1+{vv_syst})',MH,{vv_syst})".format(name=SIGMAVar,param=info['SIGMA'],vv_syst="CMS_resolution_"+self.physics))

        ALPHA1Var="_".join(["ALPHA1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=ALPHA1Var,param=info['ALPHA1']))

        ALPHA2Var="_".join(["ALPHA2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=ALPHA2Var,param=info['ALPHA2']))

        N1Var="_".join(["N1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=N1Var,param=info['N1']))

        N2Var="_".join(["N2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=N2Var,param=info['N2']))        


        jetMassName="_".join(["jetMass",name,self.tag])
        jetMass = ROOT.RooDoubleCB(jetMassName,jetMassName,self.w.var(mjj),self.w.function(scaleVar),self.w.function(sigmaVar),self.w.function(alpha1Var),self.w.function(n1Var),self.w.function(alpha2Var),self.w.function(n2Var))
        getattr(self.w,'import')(jetMass,ROOT.RooFit.Rename(jetMassName))

        vvMassName="_".join(["vvMass",name,self.tag])
        vvMass = ROOT.RooDoubleCB(vvMassName,vvMassName,self.w.var(MVV),self.w.function(SCALEVar),self.w.function(SIGMAVar),self.w.function(ALPHA1Var),self.w.function(N1Var),self.w.function(ALPHA2Var),self.w.function(N2Var))
        getattr(self.w,'import')(vvMass,ROOT.RooFit.Rename(vvMassName))

        pdfName="_".join([name,self.tag])
        self.w.factory("PROD::"+pdfName+"("+jetMassName+","+vvMassName+")")


        #OK now the normalization!
        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("expr::{name}('({param})*{lumi}',MH,{lumi})".format(name=pdfNorm,param=info['yield'],lumi=self.physics+"_"+self.period+"_lumi"))       
        f.close()

        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})



    def add1DSignalModel(self,name,ID,events,jsonFile,boson="W"):

        self.w.factory("MH[2000]")
        self.w.var("MH").setConstant(1)
        

        self.w.factory("CMS_scale_"+self.physics+"[0,-1,1]")
        self.w.factory("CMS_resolution_"+self.physics+"[0,-1,1]")

        
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        
        f=open(jsonFile)
        info=json.load(f)

        SCALEVar="_".join(["MEAN",name,self.tag])
        self.w.factory("expr::{name}('MH*({param})*(1+{vv_syst})',MH,{vv_syst})".format(name=SCALEVar,param=info['SCALE'],vv_syst="CMS_scale_"+self.physics))

        SIGMAVar="_".join(["SIGMA",name,self.tag])
        self.w.factory("expr::{name}('MH*({param})*(1+{vv_syst})',MH,{vv_syst})".format(name=SIGMAVar,param=info['SIGMA'],vv_syst="CMS_resolution_"+self.physics))

        ALPHA1Var="_".join(["ALPHA1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=ALPHA1Var,param=info['ALPHA1']))

        ALPHA2Var="_".join(["ALPHA2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=ALPHA2Var,param=info['ALPHA2']))

        N1Var="_".join(["N1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=N1Var,param=info['N1']))

        N2Var="_".join(["N2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=N2Var,param=info['N2']))        



        pdfName="_".join([name,self.tag])
        vvMass = ROOT.RooDoubleCB(pdfName,pdfName,self.w.var(MVV),self.w.function(SCALEVar),self.w.function(SIGMAVar),self.w.function(ALPHA1Var),self.w.function(N1Var),self.w.function(ALPHA2Var),self.w.function(N2Var))
        getattr(self.w,'import')(vvMass,ROOT.RooFit.Rename(pdfName))
        #OK now the normalization!
        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("expr::{name}('({param})*{lumi}',MH,{lumi})".format(name=pdfNorm,param=info['yield'],lumi=self.physics+"_"+self.period+"_lumi"))       
        f.close()
        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})



    def addMVVSignalShape(self,name,jsonFile):
        self.w.factory("MH[2000]")
        self.w.var("MH").setConstant(1)
       
        self.w.factory("CMS_scale_"+self.physics+"[0,-1,1]")
        self.w.factory("CMS_resolution_"+self.physics+"[0,-1,1]")

        
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")

        
        f=open(jsonFile)
        info=json.load(f)

        SCALEVar="_".join(["MEAN",name,self.tag])
        self.w.factory("expr::{name}('MH*({param})*(1+{vv_syst})',MH,{vv_syst})".format(name=SCALEVar,param=info['SCALE'],vv_syst="CMS_scale_"+self.physics))

        SIGMAVar="_".join(["SIGMA",name,self.tag])
        self.w.factory("expr::{name}('MH*({param})*(1+{vv_syst})',MH,{vv_syst})".format(name=SIGMAVar,param=info['SIGMA'],vv_syst="CMS_resolution_"+self.physics))

        ALPHA1Var="_".join(["ALPHA1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=ALPHA1Var,param=info['ALPHA1']))

        ALPHA2Var="_".join(["ALPHA2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=ALPHA2Var,param=info['ALPHA2']))

        N1Var="_".join(["N1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=N1Var,param=info['N1']))

        N2Var="_".join(["N2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=N2Var,param=info['N2']))        

        pdfName="_".join([name,self.tag])
        vvMass = ROOT.RooDoubleCB(pdfName,pdfName,self.w.var(MVV),self.w.function(SCALEVar),self.w.function(SIGMAVar),self.w.function(ALPHA1Var),self.w.function(N1Var),self.w.function(ALPHA2Var),self.w.function(N2Var))
        getattr(self.w,'import')(vvMass,ROOT.RooFit.Rename(pdfName))
        f.close()

    def addMJJSignalShape(self,name,jsonFile,boson="W"):
        self.w.factory("MH[2000]")
        self.w.var("MH").setConstant(1)
        
        self.w.factory("MW[80.385]")
        self.w.var("MW").setConstant(1)

        self.w.factory("MZ[91.1876]")
        self.w.var("MZ").setConstant(1)

        self.w.factory("CMS_jetMassScale[0,-1,1]")
        self.w.factory("CMS_jetMassResolution[0,-1,1]")
        self.w.factory("CMS_scale_"+self.physics+"[0,-1,1]")
        self.w.factory("CMS_resolution_"+self.physics+"[0,-1,1]")
        
        mjj='_'.join(['mjj',self.tag])
        self.w.factory(mjj+"[0,10000]")

        
        f=open(jsonFile)
        info=json.load(f)
        scaleVar="_".join(["mean",name,self.tag])
        self.w.factory("expr::{name}('{mboson}*({param})*(1+{jet_mass_syst})',{mboson},MH,{jet_mass_syst})".format(name=scaleVar,mboson="M"+boson,param=info['scale'],jet_mass_syst="CMS_jetMassScale"))


        sigmaVar="_".join(["sigma",name,self.tag])
        self.w.factory("expr::{name}('{mboson}*({param})*(1+{jet_mass_syst})',{mboson},MH,{jet_mass_syst})".format(name=sigmaVar,mboson="M"+boson,param=info['sigma'],jet_mass_syst="CMS_jetMassResolution"))


        alpha1Var="_".join(["alpha1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=alpha1Var,param=info['alpha1']))

        alpha2Var="_".join(["alpha2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=alpha2Var,param=info['alpha2']))

        n1Var="_".join(["n1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=n1Var,param=info['n1']))

        n2Var="_".join(["n2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=n2Var,param=info['n2']))


        pdfName="_".join([name,self.tag])
        jetMass = ROOT.RooDoubleCB(pdfName,pdfName,self.w.var(mjj),self.w.function(scaleVar),self.w.function(sigmaVar),self.w.function(alpha1Var),self.w.function(n1Var),self.w.function(alpha2Var),self.w.function(n2Var))
        getattr(self.w,'import')(jetMass,ROOT.RooFit.Rename(pdfName))
        f.close()


    def addMVVBackgroundShape(self,name,logTerm=False):
        
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")

        p0="_".join(["bkg_p0",name,self.tag])
        self.w.factory("{name}[33,0,100]".format(name=p0))

        p1="_".join(["bkg_p1",name,self.tag])
        self.w.factory("{name}[0.5,0,10]".format(name=p1))


        p2="_".join(["bkg_p2",name,self.tag])
        
        if logTerm:
            self.w.factory("{name}[0.001,0,1000]".format(name=p2))
        else:    
            self.w.factory("{name}[0]".format(name=p2))

        pdfName="_".join([name,self.tag])
        qcd = ROOT.RooQCDPdf(pdfName,pdfName,self.w.var(MVV),self.w.var(p0),self.w.var(p1),self.w.var(p2))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename(pdfName))


    def addMJJBackgroundShape(self,name):

        
        mjj='_'.join(['mjj',self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        bkgScale_a="_".join(["bkgScale_a",name,self.tag])
        self.w.factory("{name}[50,0,100]".format(name=bkgScale_a))

        bkgScale_b="_".join(["bkgScale_b",name,self.tag])
        self.w.factory("{name}[-1250,-10000,0]".format(name=bkgScale_b))

        bkgScale="_".join(["bkgScale",name,self.tag])
        self.w.factory("expr::{name}('{a}+{b}*{MVV}/{sqrt_s}',{a},{b},{MVV})".format(name=bkgScale,a=bkgScale_a,b=bkgScale_b,MVV=MVV,sqrt_s=self.sqrt_s))

        bkgOffset="_".join(["bkgOffset",name,self.tag])
        self.w.factory("{name}[160,0,2000]".format(name=bkgOffset))


        bkgAlpha="_".join(["bkgAlpha",name,self.tag])
        self.w.factory("{name}[0.005,0,10]".format(name=bkgAlpha))

        bkgBetaSlope="_".join(["bkgBetaSlope",name,self.tag])
        self.w.factory("{name}[2.5,0,100]".format(name=bkgBetaSlope))

        bkgBeta="_".join(["bkgBeta",name,self.tag])
        self.w.factory("expr::{name}('{a}+{b}*{MVV}/{sqrt_s}',{a},{b},{MVV})".format(name=bkgBeta,a=bkgAlpha,b=bkgBetaSlope,MVV=MVV,sqrt_s=self.sqrt_s))


        bkgGamma="_".join(["bkgGamma",name,self.tag])
        self.w.factory("{name}[0.1,0,1]".format(name=bkgGamma))


        jetMassName="_".join([name,self.tag])
        softDrop = ROOT.RooFatJetFallingPdf(jetMassName,jetMassName,self.w.var(mjj),self.w.function(bkgScale),self.w.var(bkgOffset),self.w.var(bkgAlpha),self.w.function(bkgBeta),self.w.var(bkgGamma))
        getattr(self.w,'import')(softDrop,ROOT.RooFit.Rename(jetMassName))



    def addMJJBackgroundModelBernstein(self,name):
        
        mjj='_'.join(['mjj',self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")



        bkgScale_a="_".join(["bkgScale_a",name,self.tag])
        self.w.factory("{name}[0.1,0,100]".format(name=bkgScale_a))

        bkgScale_b0="_".join(["bkgScale_b0",name,self.tag])
        self.w.factory("{name}[0.8,0,100]".format(name=bkgScale_b0))

        bkgScale_b1="_".join(["bkgScale_b1",name,self.tag])
        self.w.factory("{name}[-8e-5,-0.1,0]".format(name=bkgScale_b1))

        bkgScale_b="_".join(["bkgScale_b",name,self.tag])
        self.w.factory("expr::{name}('{a}+{b}*{MVV}',{a},{b},{MVV})".format(name=bkgScale_b,a=bkgScale_b0,b=bkgScale_b1,MVV=MVV))

        bkgScale_c="_".join(["bkgScale_c",name,self.tag])
        self.w.factory("{name}[0.15,0,1]".format(name=bkgScale_c))

        bkgScale_d0="_".join(["bkgScale_d0",name,self.tag])
        self.w.factory("{name}[0.15,0,1]".format(name=bkgScale_d0))
        bkgScale_d1="_".join(["bkgScale_d1",name,self.tag])

        self.w.factory("{name}[1000,0,10000]".format(name=bkgScale_d1))

        bkgScale_d2="_".join(["bkgScale_d2",name,self.tag])
        self.w.factory("{name}[400,0,10000]".format(name=bkgScale_d2))

        bkgScale_d="_".join(["bkgScale_d",name,self.tag])
        self.w.factory("expr::{name}('{a}*(0.5+0.5*TMath::Erf(({MVV}-{b})/{c}))',{a},{b},{c},{MVV})".format(name=bkgScale_d,a=bkgScale_d0,b=bkgScale_d1,c=bkgScale_d2,MVV=MVV))



        jetMassName="_".join([name,self.tag])

        cList = ROOT.RooArgList()
        cList.add(self.w.var(bkgScale_a))
        cList.add(self.w.function(bkgScale_b))
        cList.add(self.w.var(bkgScale_c))
        cList.add(self.w.function(bkgScale_d))


        softDrop = ROOT.RooBernsteinFast(4)(jetMassName,jetMassName,self.w.var(mjj),cList)
        getattr(self.w,'import')(softDrop,ROOT.RooFit.Rename(jetMassName))




    def conditionalProduct(self,name,pdf1,var,pdf2):
        pdfName="_".join([name,self.tag])
        pdfName1="_".join([pdf1,self.tag])
        pdfName2="_".join([pdf2,self.tag])
        varName='_'.join([var,self.tag])
        self.w.factory("PROD::{name}({name1}|{x},{name2})".format(name=pdfName,name1=pdfName1,x=varName,name2=pdfName2))


    def product(self,name,pdf1,pdf2):
        pdfName="_".join([name,self.tag])
        pdfName1="_".join([pdf1,self.tag])
        pdfName2="_".join([pdf2,self.tag])
        self.w.factory("PROD::{name}({name1},{name2})".format(name=pdfName,name1=pdfName1,name2=pdfName2))


    def addSignalContribution(self,name,ID,events,jsonFile):
        f=open(jsonFile)
        info=json.load(f)

        pdfName="_".join([name,self.tag])
        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("expr::{name}('({param})*{lumi}',MH,{lumi})".format(name=pdfNorm,param=info['yield'],lumi=self.physics+"_"+self.period+"_lumi"))       
        f.close()
        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})



    def addBackgroundContribution(self,name,ID,events,mini=0,maxi=1e+9):
        pdfName="_".join([name,self.tag])
        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("{name}[{val},{mini},{maxi}]".format(name=pdfNorm,val=events,mini=mini,maxi=maxi))       
        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})





    def add2DBackgroundModel(self,name,ID,events,logTerm=False):

        
        mjj='_'.join(['mjj',self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")



        bkgScale_a="_".join(["bkgScale_a",name,self.tag])
        self.w.factory("{name}[50,0,100]".format(name=bkgScale_a))

        bkgScale_b="_".join(["bkgScale_b",name,self.tag])
        self.w.factory("{name}[-1250,-10000,0]".format(name=bkgScale_b))

        bkgScale="_".join(["bkgScale",name,self.tag])
        self.w.factory("expr::{name}('{a}+{b}*{MVV}/{sqrt_s}',{a},{b},{MVV})".format(name=bkgScale,a=bkgScale_a,b=bkgScale_b,MVV=MVV,sqrt_s=self.sqrt_s))

        bkgOffset="_".join(["bkgOffset",name,self.tag])
        self.w.factory("{name}[160,0,2000]".format(name=bkgOffset))


        bkgAlpha="_".join(["bkgAlpha",name,self.tag])
        self.w.factory("{name}[0.005,0,10]".format(name=bkgAlpha))

        bkgBetaSlope="_".join(["bkgBetaSlope",name,self.tag])
        self.w.factory("{name}[2.5,0,100]".format(name=bkgBetaSlope))

        bkgBeta="_".join(["bkgBeta",name,self.tag])
        self.w.factory("expr::{name}('{a}+{b}*{MVV}/{sqrt_s}',{a},{b},{MVV})".format(name=bkgBeta,a=bkgAlpha,b=bkgBetaSlope,MVV=MVV,sqrt_s=self.sqrt_s))


        bkgGamma="_".join(["bkgGamma",name,self.tag])
        self.w.factory("{name}[0.1,0,1]".format(name=bkgGamma))


        jetMassName="_".join(["jetMass",name,self.tag])
        softDrop = ROOT.RooFatJetFallingPdf(jetMassName,jetMassName,self.w.var(mjj),self.w.function(bkgScale),self.w.var(bkgOffset),self.w.var(bkgAlpha),self.w.function(bkgBeta),self.w.var(bkgGamma))
        getattr(self.w,'import')(softDrop,ROOT.RooFit.Rename(jetMassName))


        p0="_".join(["bkg_p0",name,self.tag])
        self.w.factory("{name}[30,0,100]".format(name=p0))

        p1="_".join(["bkg_p1",name,self.tag])
        self.w.factory("{name}[0.5,0,10]".format(name=p1))


        p2="_".join(["bkg_p2",name,self.tag])
        if logTerm:
            self.w.factory("{name}[0.001,0,1000]".format(name=p2))
        else:    
            self.w.factory("{name}[0]".format(name=p2))


        vvMassName="_".join(["vvMass",name,self.tag])
        qcd = ROOT.RooQCDPdf(vvMassName,vvMassName,self.w.var(MVV),self.w.var(p0),self.w.var(p1),self.w.var(p2))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename(vvMassName))

        pdfName="_".join([name,self.tag])
        self.w.factory("PROD::{name}({name1}|{MVV},{name2})".format(name=pdfName,name1=jetMassName,MVV=MVV,name2=vvMassName))


        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("{name}[0.0001,1e+9]".format(name=pdfNorm))

        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})




    def add1DBackgroundModel(self,name,ID,events,logTerm=False):

        
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        p0="_".join(["bkg_p0",name,self.tag])
        self.w.factory("{name}[30,0,100]".format(name=p0))

        p1="_".join(["bkg_p1",name,self.tag])
        self.w.factory("{name}[0.5,0,10]".format(name=p1))


        p2="_".join(["bkg_p2",name,self.tag])
        
        if logTerm:
            self.w.factory("{name}[0.001,0,1000]".format(name=p2))
        else:    
            self.w.factory("{name}[0]".format(name=p2))

        pdfName="_".join([name,self.tag])
        qcd = ROOT.RooQCDPdf(pdfName,pdfName,self.w.var(MVV),self.w.var(p0),self.w.var(p1),self.w.var(p2))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename(pdfName))
        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("{name}[0.0001,1e+9]".format(name=pdfNorm))
        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})



    def add2DBackgroundModelBernstein(self,name,ID,events,logTerm=False):
        
        mjj='_'.join(['mjj',self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")



        bkgScale_a="_".join(["bkgScale_a",name,self.tag])
        self.w.factory("{name}[0.1,0,100]".format(name=bkgScale_a))

        bkgScale_b0="_".join(["bkgScale_b0",name,self.tag])
        self.w.factory("{name}[0.8,0,100]".format(name=bkgScale_b0))

        bkgScale_b1="_".join(["bkgScale_b1",name,self.tag])
        self.w.factory("{name}[-8e-5,-0.1,0]".format(name=bkgScale_b1))

        bkgScale_b="_".join(["bkgScale_b",name,self.tag])
        self.w.factory("expr::{name}('{a}+{b}*{MVV}',{a},{b},{MVV})".format(name=bkgScale_b,a=bkgScale_b0,b=bkgScale_b1,MVV=MVV))

        bkgScale_c="_".join(["bkgScale_c",name,self.tag])
        self.w.factory("{name}[0.15,0,1]".format(name=bkgScale_c))

        bkgScale_d0="_".join(["bkgScale_d0",name,self.tag])
        self.w.factory("{name}[0.15,0,1]".format(name=bkgScale_d0))
        bkgScale_d1="_".join(["bkgScale_d1",name,self.tag])

        self.w.factory("{name}[1000,0,10000]".format(name=bkgScale_d1))

        bkgScale_d2="_".join(["bkgScale_d2",name,self.tag])
        self.w.factory("{name}[400,0,10000]".format(name=bkgScale_d2))

        bkgScale_d="_".join(["bkgScale_d",name,self.tag])
        self.w.factory("expr::{name}('{a}*(0.5+0.5*TMath::Erf(({MVV}-{b})/{c}))',{a},{b},{c},{MVV})".format(name=bkgScale_d,a=bkgScale_d0,b=bkgScale_d1,c=bkgScale_d2,MVV=MVV))



        jetMassName="_".join(["jetMass",name,self.tag])

        cList = ROOT.RooArgList()
        cList.add(self.w.var(bkgScale_a))
        cList.add(self.w.function(bkgScale_b))
        cList.add(self.w.var(bkgScale_c))
        cList.add(self.w.function(bkgScale_d))


        softDrop = ROOT.RooBernsteinFast(4)(jetMassName,jetMassName,self.w.var(mjj),cList)
        getattr(self.w,'import')(softDrop,ROOT.RooFit.Rename(jetMassName))


        p0="_".join(["bkg_p0",name,self.tag])
        self.w.factory("{name}[33,0,100]".format(name=p0))

        p1="_".join(["bkg_p1",name,self.tag])
        self.w.factory("{name}[0.5,0,10]".format(name=p1))


        p2="_".join(["bkg_p2",name,self.tag])
        if logTerm:
            self.w.factory("{name}[0.001,0,1000]".format(name=p2))
        else:    
            self.w.factory("{name}[0]".format(name=p2))

        vvMassName="_".join(["vvMass",name,self.tag])
        qcd = ROOT.RooQCDPdf(vvMassName,vvMassName,self.w.var(MVV),self.w.var(p0),self.w.var(p1),self.w.var(p2))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename(vvMassName))

        pdfName="_".join([name,self.tag])
        self.w.factory("PROD::{name}({name1}|{MVV},{name2})".format(name=pdfName,name1=jetMassName,MVV=MVV,name2=vvMassName))


        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("{name}[0.0001,1e+9]".format(name=pdfNorm))

        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})
        
    
        

    def makeCard(self):

        f = open(self.tag+'.txt','w')
        f.write('imax 1\n')
        f.write('jmax {n}\n'.format(n=len(self.contributions)-1))
        f.write('kmax *\n')
        f.write('-------------------------\n')
        for c in self.contributions:
            f.write('shapes {name} {channel} {file}.root w:{pdf}\n'.format(name=c['name'],channel=self.tag,file=self.tag,pdf=c['pdf']))
        f.write('shapes {name} {channel} {file}.root w:{name}\n'.format(name="data_obs",channel=self.tag,file=self.tag))
        f.write('-------------------------\n')
        f.write('bin '+self.tag+'\n')
        f.write('observation  -1\n')
        f.write('-------------------------\n')
        f.write('bin\t') 

        for shape in self.contributions:
            f.write(self.tag+'\t')
        f.write('\n')

        #Sort the shapes by ID 
 
        shapes = sorted(self.contributions,key=lambda x: x['ID'])
        #print names
        f.write('process\t')
        for shape in shapes:
            f.write(shape['name']+'\t')
        f.write('\n')

        #Print ID
        f.write('process\t')
        for shape in shapes:
            f.write(str(shape['ID'])+'\t')
        f.write('\n')

        #print rates
        f.write('rate\t')
        for shape in shapes:
            f.write(str(shape['yield'])+'\t')
        f.write('\n')


        for syst in self.systematics:
            if syst['kind'] == 'param':
                f.write(syst['name']+'\t'+'param\t' +str(syst['values'][0])+'\t'+str(syst['values'][1])+'\n')
            elif syst['kind'] == 'lnN': 
                f.write(syst['name']+'\t'+ 'lnN\t' )
                for shape in shapes:
                    has=False
                    for name,v in syst['values'].iteritems():
                        if shape['name']==name:
                            f.write(str(v)+'\t' )
                            has=True
                            break;
                    if not has:
                            f.write('-\t' )
                f.write('\n' )
                            
                        
        f.close()


        self.rootFile.cd()
        self.w.Write()
        self.rootFile.Close()
            
    
        

    def importBinnedData(self,histogram,poi,name = "data_obs"):
        cList = ROOT.RooArgList()
        for i,po in enumerate(poi):
            p='_'.join([po,self.tag])

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
        
