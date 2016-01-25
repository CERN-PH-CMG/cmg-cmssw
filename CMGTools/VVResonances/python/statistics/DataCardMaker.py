import ROOT
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
import json


class DataCardMaker:
    def __init__(self,finalstate,category,period,luminosity=1.0,physics="LJ"):
        self.physics=physics
        self.finalstate=finalstate
        self.category=category
        self.period=period
        self.contributions=[]
        self.systematics=[]

        self.tag=self.physics+"_"+finalstate+"_"+category+"_"+period
        self.rootFile = ROOT.TFile(self.tag+".root","RECREATE")
        self.rootFile.cd()
        self.w=ROOT.RooWorkspace("w","w")
        self.luminosity=luminosity
        self.w.factory(self.physics+"_"+period+"_lumi["+str(luminosity)+"]")
        if period=='8TeV':
            self.sqrt_s=8000.0
        if period=='13TeV':
            self.sqrt_s=13000.0


    def addSystematic(self,name,kind,values,addPar = ""):
        self.systematics.append({'name':name,'kind':kind,'values':values })




    def addMVVSignalShape(self,name,jsonFile,scale ={},resolution={}):
        self.w.factory("MH[2000]")
        self.w.var("MH").setConstant(1)
       
        scaleStr='0'
        resolutionStr='0'

        scaleSysts=[]
        resolutionSysts=[]
        for syst,factor in scale.iteritems():
            self.w.factory(syst+"[0,-1,1]")
            scaleStr=scaleStr+"+{factor}*{syst}".format(factor=factor,syst=syst)
            scaleSysts.append(syst)
        for syst,factor in resolution.iteritems():
            self.w.factory(syst+"[0,-1,1]")
            resolutionStr=resolutionStr+"+{factor}*{syst}".format(factor=factor,syst=syst)
            resolutionSysts.append(syst)


        
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")

        
        f=open(jsonFile)
        info=json.load(f)

        SCALEVar="_".join(["MEAN",name,self.tag])
        self.w.factory("expr::{name}('MH*({param})*(1+{vv_syst})',MH,{vv_systs})".format(name=SCALEVar,param=info['SCALE'],vv_syst=scaleStr,vv_systs=','.join(scaleSysts)))

        SIGMAVar="_".join(["SIGMA",name,self.tag])
        self.w.factory("expr::{name}('MH*({param})*(1+{vv_syst})',MH,{vv_systs})".format(name=SIGMAVar,param=info['SIGMA'],vv_syst=resolutionStr,vv_systs=','.join(resolutionSysts)))

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

    def addMJJSignalShape(self,name,jsonFile,boson="W",varName="mjj",scale={},resolution={}):
        self.w.factory("MH[2000]")
        self.w.var("MH").setConstant(1)
        
        self.w.factory("MW[80.385]")
        self.w.var("MW").setConstant(1)

        self.w.factory("MZ[91.1876]")
        self.w.var("MZ").setConstant(1)
        
        mjj='_'.join([varName,self.tag])
        self.w.factory(mjj+"[0,10000]")


        scaleStr='0'
        resolutionStr='0'

        scaleSysts=[]
        resolutionSysts=[]
        for syst,factor in scale.iteritems():
            self.w.factory(syst+"[0,-1,1]")
            scaleStr=scaleStr+"+{factor}*{syst}".format(factor=factor,syst=syst)
            scaleSysts.append(syst)
        for syst,factor in resolution.iteritems():
            self.w.factory(syst+"[0,-1,1]")
            resolutionStr=resolutionStr+"+{factor}*{syst}".format(factor=factor,syst=syst)
            resolutionSysts.append(syst)
        
        f=open(jsonFile)
        info=json.load(f)
        scaleVar="_".join(["mean",name,self.tag])
        self.w.factory("expr::{name}('{mboson}*({param})*(1+{jet_mass_syst})',{mboson},MH,{jet_mass_systs})".format(name=scaleVar,mboson="M"+boson,param=info['scale'],jet_mass_syst=scaleStr,jet_mass_systs=','.join(scaleSysts)))


        sigmaVar="_".join(["sigma",name,self.tag])
        self.w.factory("expr::{name}('{mboson}*({param})*(1+{jet_mass_syst})',{mboson},MH,{jet_mass_systs})".format(name=sigmaVar,mboson="M"+boson,param=info['sigma'],jet_mass_syst=resolutionStr,jet_mass_systs=','.join(resolutionSysts)))


        alpha1Var="_".join(["alpha1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=alpha1Var,param=info['alpha1']))

        alpha2Var="_".join(["alpha2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=alpha2Var,param=info['alpha2']))

        n1Var="_".join(["n1",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=n1Var,param=info['n1']))

        n2Var="_".join(["n2",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=n2Var,param=info['n2']))

        slopeVar="_".join(["slope",name,self.tag])
        self.w.factory("expr::{name}('MH*0+{param}',MH)".format(name=slopeVar,param=info['slope']))

        fVar="_".join(["f",name,self.tag])
        self.w.factory("expr::{name}('TMath::Min(MH*0+{param},1.0)',MH)".format(name=fVar,param=info['f']))

        pdfName="_".join([name+"CB",self.tag])
        jetMass = ROOT.RooDoubleCB(pdfName,pdfName,self.w.var(mjj),self.w.function(scaleVar),self.w.function(sigmaVar),self.w.function(alpha1Var),self.w.function(n1Var),self.w.function(alpha2Var),self.w.function(n2Var))

        getattr(self.w,'import')(jetMass,ROOT.RooFit.Rename(pdfName))
        expoName="_".join([name+"Expo",self.tag])
        self.w.factory("RooExponential::{name}({var},{slope})".format(name=expoName,var=mjj,slope=slopeVar))

        sumName="_".join([name,self.tag])
        self.w.factory("SUM::{name}({f}*{name1},{name2})".format(name=sumName,f=fVar,name1=pdfName,name2=expoName))
        f.close()


    def addMJJTopShape(self,name,jsonFile,varName="mjj",scale={},resolution={}):

        boson="W"
        #same as signal but instead of MH use MVV!
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")

        self.w.factory("MH[2000]")
        self.w.var("MH").setConstant(1)
        
        self.w.factory("MW[80.385]")
        self.w.var("MW").setConstant(1)
        
        mjj='_'.join([varName,self.tag])
        self.w.factory(mjj+"[0,10000]")


        scaleStr='0'
        resolutionStr='0'

        scaleSysts=[]
        resolutionSysts=[]
        for syst,factor in scale.iteritems():
            self.w.factory(syst+"[0,-1,1]")
            scaleStr=scaleStr+"+{factor}*{syst}".format(factor=factor,syst=syst)
            scaleSysts.append(syst)
        for syst,factor in resolution.iteritems():
            self.w.factory(syst+"[0,-1,1]")
            resolutionStr=resolutionStr+"+{factor}*{syst}".format(factor=factor,syst=syst)
            resolutionSysts.append(syst)
        
        f=open(jsonFile)
        info=json.load(f)

        scaleVar="_".join(["mean",name,self.tag])
        self.w.factory("expr::{name}('{mboson}*({param})*(1+{jet_mass_syst})',{mboson},{MH},{jet_mass_systs})".format(name=scaleVar,mboson="M"+boson,param=info['scale'].replace("MH",MVV),jet_mass_syst=scaleStr,jet_mass_systs=','.join(scaleSysts),MH=MVV))


        sigmaVar="_".join(["sigma",name,self.tag])
        self.w.factory("expr::{name}('{mboson}*({param})*(1+{jet_mass_syst})',{mboson},{MH},{jet_mass_systs})".format(name=sigmaVar,mboson="M"+boson,param=info['sigma'].replace("MH",MVV),jet_mass_syst=resolutionStr,jet_mass_systs=','.join(resolutionSysts),MH=MVV))


        alpha1Var="_".join(["alpha1",name,self.tag])
        self.w.factory("expr::{name}('{MH}*0+{param}',{MH})".format(name=alpha1Var,param=info['alpha1'].replace("MH",MVV),MH=MVV))

        alpha2Var="_".join(["alpha2",name,self.tag])
        self.w.factory("expr::{name}('{MH}*0+{param}',{MH})".format(name=alpha2Var,param=info['alpha2'].replace("MH",MVV),MH=MVV))

        n1Var="_".join(["n1",name,self.tag])
        self.w.factory("expr::{name}('{MH}*0+{param}',{MH})".format(name=n1Var,param=info['n1'].replace("MH",MVV),MH=MVV))

        n2Var="_".join(["n2",name,self.tag])
        self.w.factory("expr::{name}('{MH}*0+{param}',{MH})".format(name=n2Var,param=info['n2'].replace("MH",MVV),MH=MVV))

        slopeVar="_".join(["slope",name,self.tag])
        self.w.factory("expr::{name}('{MH}*0+{param}',{MH})".format(name=slopeVar,param=info['slope'].replace("MH",MVV),MH=MVV))

        fVar="_".join(["f",name,self.tag])
        self.w.factory("expr::{name}('TMath::Min({MH}*0+{param},1.0)',{MH})".format(name=fVar,param=info['f'].replace("MH",MVV),MH=MVV))

        pdfName="_".join([name+"CB",self.tag])
        jetMass = ROOT.RooDoubleCB(pdfName,pdfName,self.w.var(mjj),self.w.function(scaleVar),self.w.function(sigmaVar),self.w.function(alpha1Var),self.w.function(n1Var),self.w.function(alpha2Var),self.w.function(n2Var))

        getattr(self.w,'import')(jetMass,ROOT.RooFit.Rename(pdfName))
        expoName="_".join([name+"Expo",self.tag])
        self.w.factory("RooExponential::{name}({var},{slope})".format(name=expoName,var=mjj,slope=slopeVar))

        sumName="_".join([name,self.tag])
        self.w.factory("SUM::{name}({f}*{name1},{name2})".format(name=sumName,f=fVar,name1=pdfName,name2=expoName))
        f.close()



    def addMVVBackgroundShape(self,name,logTerm=False,newTag="",preconstrains={}):
        
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        if newTag !="":
            tag=newTag
        else:
            tag=name+"_"+self.tag



        p0="_".join(["p0",tag])
        if "p0" in preconstrains.keys():
            val = preconstrains['p0']['val']
            err = preconstrains['p0']['err']
            self.addSystematic(p0,"param",[val,err])
        else:
            val = 15.0
        self.w.factory("{name}[{val},10,60]".format(name=p0,val=val))

        p1="_".join(["p1",tag])
        if "p1" in preconstrains.keys():
            val = preconstrains['p1']['val']
            err = preconstrains['p1']['err']
            self.addSystematic(p1,"param",[val,err])
        else:
            val = 0.001
        self.w.factory("{name}[{val},0,5]".format(name=p1,val=val))


        p2="_".join(["p2",tag])
        if "p2" in preconstrains.keys():
            val = preconstrains['p2']['val']
            err = preconstrains['p2']['err']
            self.addSystematic(p2,"param",[val,err])
        else:
            val = 0.001


        
        if logTerm:
            self.w.factory("{name}[{val},0,1000]".format(name=p2,val=val))
        else:    
            self.w.factory("{name}[0]".format(name=p2))

        pdfName="_".join([name,self.tag])
        qcd = ROOT.RooQCDPdf(pdfName,pdfName,self.w.var(MVV),self.w.var(p0),self.w.var(p1),self.w.var(p2))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename(pdfName))




    def addMVVBackgroundShapeErfExp(self,name,logTerm=False,newTag="",preconstrains={}):
        
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        if newTag !="":
            tag=newTag
        else:
            tag=name+"_"+self.tag



        p0="_".join(["p0",tag])
        if "p0" in preconstrains.keys():
            val = preconstrains['p0']['val']
            err = preconstrains['p0']['err']
            self.addSystematic(p0,"param",[val,err])
        else:
            val = -3e-3
        self.w.factory("{name}[{val},-0.1,0]".format(name=p0,val=val))

        p1="_".join(["p1",tag])
        if "p1" in preconstrains.keys():
            val = preconstrains['p1']['val']
            err = preconstrains['p1']['err']
            self.addSystematic(p1,"param",[val,err])
        else:
            val = 700
        self.w.factory("{name}[{val},10,5000]".format(name=p1,val=val))


        p2="_".join(["p2",tag])
        if "p2" in preconstrains.keys():
            val = preconstrains['p2']['val']
            err = preconstrains['p2']['err']
            self.addSystematic(p2,"param",[val,err])
        else:
            val = 2000
        self.w.factory("{name}[{val},0,10000]".format(name=p2,val=val))


        pdfName="_".join([name,self.tag])

        erfexp = ROOT.RooErfExpPdf(pdfName,pdfName,self.w.var(MVV),self.w.var(p0),self.w.var(p1),self.w.var(p2))       
        getattr(self.w,'import')(erfexp,ROOT.RooFit.Rename(pdfName))



    def addMVVBackgroundShapePow(self,name,newTag="",preconstrains={}):
        
        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        if newTag !="":
            tag=newTag
        else:
            tag=name+"_"+self.tag

        p0="_".join(["p0",tag])
        if "p0" in preconstrains.keys():
            val = preconstrains['p0']['val']
            err = preconstrains['p0']['err']
            self.addSystematic(p0,"param",[val,err])
        else:
            val = -4
        self.w.factory("{name}[{val},-100,0]".format(name=p0,val=val))

        pdfName="_".join([name,self.tag])
        qcd = ROOT.RooPower(pdfName,pdfName,self.w.var(MVV),self.w.var(p0))
        getattr(self.w,'import')(qcd,ROOT.RooFit.Rename(pdfName))



    def addHistoShapeFromFile(self,name,filename,histoname, observables,newTag=""):
        
        varset=ROOT.RooArgSet()
        varlist=ROOT.RooArgList()
        for var in observables:
            MVV='_'.join([var,self.tag])
            self.w.factory(MVV+"[0,10000]")
            varset.add(self.w.var(MVV))
            varlist.add(self.w.var(MVV))

        if newTag !="":
            tag=newTag
        else:
            tag=name+"_"+self.tag


        FR=ROOT.TFile(filename)
        histo=FR.Get(histoname)

        histName="_".join([name+"HIST",self.tag])
        roohist = ROOT.RooDataHist(histName,histName,varlist,histo)
       
        pdfName="_".join([name,self.tag])
        pdf=ROOT.RooHistPdf(pdfName,pdfName,varset,roohist)

        getattr(self.w,'import')(roohist,ROOT.RooFit.Rename(histName))
        getattr(self.w,'import')(pdf,ROOT.RooFit.Rename(pdfName))





    def addMJJBackgroundShapeBifur5P(self,name,varName='mjj',newTag="",preconstrains={}):
        
        mjj='_'.join([varName,self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        if newTag !="":
            tag=newTag
        else:
            tag=name+"_"+self.tag
        


        bkg_a0="_".join(["a0",tag])
        if "a0" in preconstrains.keys():
            val = preconstrains['a0']['val']
            err = preconstrains['a0']['err']
            self.addSystematic(bkg_a0,"param",[val,err])
        else:
            val = -35.661
          

        self.w.factory("{name}[{val},-55,15]".format(name=bkg_a0,val=val))




        bkg_a1="_".join(["a1",tag])
        if "a1" in preconstrains.keys():
            val = preconstrains['a1']['val']
            err = preconstrains['a1']['err']
            self.addSystematic(bkg_a1,"param",[val,err])
        else:
            val = 15.882

        self.w.factory("{name}[{val},10,35]".format(name=bkg_a1,val=val))


        bkg_a="_".join(["a",tag])
        self.w.factory("expr::{name}('{a}+{b}*log({MVV})',{a},{b},{MVV})".format(name=bkg_a,a=bkg_a0,b=bkg_a1,MVV=MVV))


        bkg_b0="_".join(["b0",tag])
        if "b0" in preconstrains.keys():
            val = preconstrains['b0']['val']
            err = preconstrains['b0']['err']
            self.addSystematic(bkg_b0,"param",[val,err])
        else:
            val = 19.5438

        self.w.factory("{name}[{val},10,30]".format(name=bkg_b0,val=val))


        bkg_b1="_".join(["b1",tag])
        if "b1" in preconstrains.keys():
            val = preconstrains['b1']['val']
            err = preconstrains['b1']['err']
            self.addSystematic(bkg_b1,"param",[val,err])
        else:
            val = 0.0101


        self.w.factory("{name}[{val},0,0.05]".format(name=bkg_b1,val=val))

        bkg_b="_".join(["b",tag])
        self.w.factory("expr::{name}('{a}+{b}*({MVV})',{a},{b},{MVV})".format(name=bkg_b,a=bkg_b0,b=bkg_b1,MVV=MVV))


        bkg_c="_".join(["c",tag])
        if "c" in preconstrains.keys():
            val = preconstrains['c']['val']
            err = preconstrains['c']['err']
            self.addSystematic(bkg_c,"param",[val,err])
        else:
            val = 62.6
        self.w.factory("{name}[{val},30,100]".format(name=bkg_c,val=val))



        jetMassName="_".join([name,self.tag])
        self.w.factory("RooBifurGauss::{name}({var},{a},{b},{c})".format(name=jetMassName,var=mjj,a=bkg_a,b=bkg_b,c=bkg_c))



    def addMJJBackgroundShapeBifur4P(self,name,varName='mjj',newTag="",preconstrains={}):
        
        mjj='_'.join([varName,self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        if newTag !="":
            tag=newTag
        else:
            tag=name+"_"+self.tag
        


        bkg_a="_".join(["a",tag])
        if "a" in preconstrains.keys():
            val = preconstrains['a']['val']
            err = preconstrains['a']['err']
            self.addSystematic(bkg_a,"param",[val,err])
        else:
            val = 77.4019         

        self.w.factory("{name}[{val},50,100]".format(name=bkg_a,val=val))


        bkg_b="_".join(["b",tag])
        if "b" in preconstrains.keys():
            val = preconstrains['b']['val']
            err = preconstrains['b']['err']
            self.addSystematic(bkg_b,"param",[val,err])
        else:
            val = 33.2044

        self.w.factory("{name}[{val},10,500]".format(name=bkg_b,val=val))


        bkg_c0="_".join(["c0",tag])
        if "c0" in preconstrains.keys():
            val = preconstrains['c0']['val']
            err = preconstrains['c0']['err']
            self.addSystematic(bkg_c0,"param",[val,err])
        else:
            val = -223.782
        self.w.factory("{name}[{val},-800,0]".format(name=bkg_c0,val=val))


        bkg_c1="_".join(["c1",tag])
        if "c1" in preconstrains.keys():
            val = preconstrains['c1']['val']
            err = preconstrains['c1']['err']
            self.addSystematic(bkg_c1,"param",[val,err])
        else:
            val = 33.2044
        self.w.factory("{name}[{val},10,100]".format(name=bkg_c1,val=val))

        bkg_c="_".join(["c",tag])
        self.w.factory("expr::{name}('{a}+{b}*log({MVV})',{a},{b},{MVV})".format(name=bkg_c,a=bkg_c0,b=bkg_c1,MVV=MVV))


        jetMassName="_".join([name,self.tag])
        self.w.factory("RooBifurGauss::{name}({var},{a},{b},{c})".format(name=jetMassName,var=mjj,a=bkg_a,b=bkg_b,c=bkg_c))



    def addMJJBackgroundShapeErfExp(self,name,varName='mjj',newTag="",preconstrains={}):
        
        mjj='_'.join([varName,self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        if newTag !="":
            tag=newTag
        else:
            tag=name+"_"+self.tag
        


        bkg_a0="_".join(["a0",tag])
        if "a0" in preconstrains.keys():
            val = preconstrains['a0']['val']
            err = preconstrains['a0']['err']
            self.addSystematic(bkg_a0,"param",[val,err])
        else:
            val = -0.0688
          

        self.w.factory("{name}[{val},-1,1]".format(name=bkg_a0,val=val))




        bkg_a1="_".join(["a1",tag])
        if "a1" in preconstrains.keys():
            val = preconstrains['a1']['val']
            err = preconstrains['a1']['err']
            self.addSystematic(bkg_a1,"param",[val,err])
        else:
            val = 7.609e-3

        self.w.factory("{name}[{val},0,0.01]".format(name=bkg_a1,val=val))


        bkg_a="_".join(["a",tag])
        self.w.factory("expr::{name}('{a}+{b}*log({MVV})',{a},{b},{MVV})".format(name=bkg_a,a=bkg_a0,b=bkg_a1,MVV=MVV))


        bkg_b0="_".join(["b0",tag])
        if "b0" in preconstrains.keys():
            val = preconstrains['b0']['val']
            err = preconstrains['b0']['err']
            self.addSystematic(bkg_b0,"param",[val,err])
        else:
            val = 74.58

        self.w.factory("{name}[{val},10,300]".format(name=bkg_b0,val=val))


        bkg_b1="_".join(["b1",tag])
        if "b1" in preconstrains.keys():
            val = preconstrains['b1']['val']
            err = preconstrains['b1']['err']
            self.addSystematic(bkg_b1,"param",[val,err])
        else:
            val = -0.0080


        self.w.factory("{name}[{val},-0.05,0]".format(name=bkg_b1,val=val))

        bkg_b="_".join(["b",tag])
        self.w.factory("expr::{name}('{a}+{b}*({MVV})',{a},{b},{MVV})".format(name=bkg_b,a=bkg_b0,b=bkg_b1,MVV=MVV))


        bkg_c="_".join(["c",tag])
        if "c" in preconstrains.keys():
            val = preconstrains['c']['val']
            err = preconstrains['c']['err']
            self.addSystematic(bkg_c,"param",[val,err])
        else:
            val = 40.9351
        self.w.factory("{name}[{val},10,200]".format(name=bkg_c,val=val))



        jetMassName="_".join([name,self.tag])

        erfexp = ROOT.RooErfExpPdf(jetMassName,jetMassName,self.w.var(mjj),self.w.function(bkg_a),self.w.function(bkg_b),self.w.var(bkg_c))
        
        getattr(self.w,'import')(erfexp,ROOT.RooFit.Rename(jetMassName))



    def addMJJBackgroundShapeExpo2P(self,name,varName='mjj',newTag="",preconstrains={}):
        
        mjj='_'.join([varName,self.tag])
        self.w.factory(mjj+"[0,10000]")

        MVV='_'.join(['MVV',self.tag])
        self.w.factory(MVV+"[0,10000]")


        if newTag !="":
            tag=newTag
        else:
            tag=name+"_"+self.tag


        bkgSlope_a="_".join(["a",tag])
        if "a" in preconstrains.keys():
            val = preconstrains['a']['val']
            err = preconstrains['a']['err']
            self.addSystematic(bkgSlope_a,"param",[val,err])
        else:
            val = -0.146816
        self.w.factory("{name}[{val},-1,0]".format(name=bkgSlope_a,val=val))


        bkgSlope_b="_".join(["b",tag])
        if "b" in preconstrains.keys():
            val = preconstrains['b']['val']
            err = preconstrains['b']['err']
            self.addSystematic(bkgSlope_b,"param",[val,err])
        else:
            val = 0.0171642
        self.w.factory("{name}[{val},0,0.1]".format(name=bkgSlope_b,val=val))


        bkgSlope="_".join(["bkgSlope",tag])
        self.w.factory("expr::{name}('{a}+{b}*log({MVV})',{a},{b},{MVV})".format(name=bkgSlope,a=bkgSlope_a,b=bkgSlope_b,MVV=MVV))

        jetMassName="_".join([name,self.tag])
        self.w.factory("RooExponential::{name}({var},{alpha})".format(name=jetMassName,var=mjj,alpha=bkgSlope))


    def conditionalProduct(self,name,pdf1,var,pdf2):
        pdfName="_".join([name,self.tag])
        pdfName1="_".join([pdf1,self.tag])
        pdfName2="_".join([pdf2,self.tag])
        varName='_'.join([var,self.tag])
        self.w.factory("PROD::{name}({name1}|{x},{name2})".format(name=pdfName,name1=pdfName1,x=varName,name2=pdfName2))

    def conditionalDoubleProduct(self,name,pdf1,pdf2,var,pdf3):
        pdfName="_".join([name,self.tag])
        pdfName1="_".join([pdf1,self.tag])
        pdfName2="_".join([pdf2,self.tag])
        pdfName3="_".join([pdf3,self.tag])
        varName='_'.join([var,self.tag])
        self.w.factory("PROD::{name}({name1}|{x},{name2}|{x},{name3})".format(name=pdfName,name1=pdfName1,x=varName,name2=pdfName2,name3=pdfName3))


    def product(self,name,pdf1,pdf2):
        pdfName="_".join([name,self.tag])
        pdfName1="_".join([pdf1,self.tag])
        pdfName2="_".join([pdf2,self.tag])
        self.w.factory("PROD::{name}({name1},{name2})".format(name=pdfName,name1=pdfName1,name2=pdfName2))


    def addParamContribution(self,name,ID,jsonFile):
        f=open(jsonFile)
        info=json.load(f)

        pdfName="_".join([name,self.tag])
        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("expr::{name}('({param})*{lumi}',MH,{lumi})".format(name=pdfNorm,param=info['yield'],lumi=self.physics+"_"+self.period+"_lumi"))       
        f.close()
        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':1.0})



    def addFloatingContribution(self,name,ID,events,mini=0,maxi=1e+9,constant=False):
        pdfName="_".join([name,self.tag])
        pdfNorm="_".join([name,self.tag,"norm"])
        self.w.factory("{name}[{val},{mini},{maxi}]".format(name=pdfNorm,val=events,mini=mini,maxi=maxi))       
        if constant:
            self.w.var(pdfNorm).setConstant(1)
        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':1.0})


    def addConstrainedContribution(self,name,ID,events,nuisance,uncertainty):
        pdfName="_".join([name,self.tag])
        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})
        self.addSystematic(nuisance,"lnN",{name:1+uncertainty})

    def addConstrainedContributionFromFile(self,name,ID,filename,nuisance,uncertainty):
        pdfName="_".join([name,self.tag])

        f=ROOT.TFile(filename)
        histogram=f.Get("histo")
        events=histogram.Integral()*self.luminosity
        self.contributions.append({'name':name,'pdf':pdfName,'ID':ID,'yield':events})
        self.addSystematic(nuisance,"lnN",{name:1+uncertainty})

    def addFixedContributionFromFile(self,name,ID,filename):
        pdfName="_".join([name,self.tag])
        f=ROOT.TFile(filename)
        histogram=f.Get("histo")
        events=histogram.Integral()*self.luminosity
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


        #Now systematics
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
            
    
        

    def importBinnedData(self,filename,poi,name = "data_obs"):
        f=ROOT.TFile(filename)
        histogram=f.Get("histo")

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
        
